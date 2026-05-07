#include <gtest/gtest.h>
#include <mock/core/observer.h>
#include <mock/file_system.h>
#include <mock/sensor/sensor_interface.h>
#include <sensor/obstacle_sensor_controller.h>

using ::testing::_;
using ::testing::Return;
using ::testing::AnyNumber;

class ObstacleSensorControllerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    mock_observer_res_ = std::make_unique<MockObserver>();
    mock_observer_ptr_ = mock_observer_res_.get();

    auto interfaces = std::make_unique<InterfaceEntry<SensorInterface>[]>(3);

    for (int i = 0; i < kSensorCount; ++i) {
      auto mock_file_system = std::make_unique<MockFileSystem>();
      EXPECT_CALL(*mock_file_system, close(MockSensorInterface::kFakeFd))
          .Times(AnyNumber());

      auto mock_interface = std::make_unique<MockSensorInterface>(std::move(mock_file_system));
      mock_sensors_[i] = mock_interface.get();

      interfaces[i].hardware_interface = std::move(mock_interface);
      interfaces[i].event = sensor_events_[i];
    }

    controller_ = std::make_unique<ObstacleSensorController>(
        mock_observer_ptr_,
        std::move(interfaces), kFakeFileName);
  }

  static constexpr int kSensorCount = 3;
  static constexpr std::string kFakeFileName = "TEST";
  static constexpr int kThreshold = std::numeric_limits<int>::max() / 2;
  static constexpr std::array<Event, kSensorCount> sensor_events_ = {
      Event::kFrontObstacle,
      Event::kRightObstacle,
      Event::kLeftObstacle,
  };

  std::unique_ptr<MockObserver> mock_observer_res_;
  MockObserver* mock_observer_ptr_;
  std::unique_ptr<ObstacleSensorController> controller_;
  MockSensorInterface* mock_sensors_[3];
};

// 전방 장애물 감지
TEST_F(ObstacleSensorControllerTest, FrontObstacleDetected) {
  EXPECT_CALL(*mock_sensors_[0], ReadSensor())
      .WillOnce(Return(kThreshold - 100));
  EXPECT_CALL(*mock_sensors_[1], ReadSensor())
      .WillOnce(Return(kThreshold + 100));
  EXPECT_CALL(*mock_sensors_[2], ReadSensor())
      .WillOnce(Return(kThreshold + 100));

  EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kFrontObstacle)).Times(1);

  controller_->CheckSensor();
}

// 다중 장애물 감지
TEST_F(ObstacleSensorControllerTest, FrontLeftObstalceDetected) {
  EXPECT_CALL(*mock_sensors_[0], ReadSensor())
      .WillOnce(Return(kThreshold - 100));
  EXPECT_CALL(*mock_sensors_[1], ReadSensor())
      .WillOnce(Return(kThreshold + 100));
  EXPECT_CALL(*mock_sensors_[2], ReadSensor())
      .WillOnce(Return(kThreshold - 100));

  EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kFrontObstacle)).Times(1);
  EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kLeftObstacle)).Times(1);

  controller_->CheckSensor();
}

// 장애물 없음
TEST_F(ObstacleSensorControllerTest, NothingDetected) {
  EXPECT_CALL(*mock_sensors_[0], ReadSensor())
      .WillOnce(Return(kThreshold + 100));
  EXPECT_CALL(*mock_sensors_[1], ReadSensor())
      .WillOnce(Return(kThreshold + 100));
  EXPECT_CALL(*mock_sensors_[2], ReadSensor())
      .WillOnce(Return(kThreshold + 100));

  EXPECT_CALL(*mock_observer_ptr_, Notify(_)).Times(0);

  controller_->CheckSensor();
}

// 장애물 문턱값 미만
TEST_F(ObstacleSensorControllerTest, JustBelowThreshold) {
  EXPECT_CALL(*mock_sensors_[0], ReadSensor())
      .WillOnce(Return(kThreshold - 1));
  EXPECT_CALL(*mock_sensors_[1], ReadSensor())
      .WillOnce(Return(kThreshold + 100));  
  EXPECT_CALL(*mock_sensors_[2], ReadSensor())
      .WillOnce(Return(kThreshold + 100));

  EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kFrontObstacle)).Times(1);

  controller_->CheckSensor();
}

// 장애물 문턱값 일치
TEST_F(ObstacleSensorControllerTest, ExactlyAtThreshold) {
  EXPECT_CALL(*mock_sensors_[0], ReadSensor())
      .WillOnce(Return(kThreshold));
  EXPECT_CALL(*mock_sensors_[1], ReadSensor())
      .WillOnce(Return(kThreshold + 100));
  EXPECT_CALL(*mock_sensors_[2], ReadSensor())
      .WillOnce(Return(kThreshold + 100));

  EXPECT_CALL(*mock_observer_ptr_, Notify(_)).Times(0);

  controller_->CheckSensor();
}

// 장애물 수치 최솟값
TEST_F(ObstacleSensorControllerTest, MinimumValue) {
  EXPECT_CALL(*mock_sensors_[0], ReadSensor()).WillOnce(Return(0));
  EXPECT_CALL(*mock_sensors_[1], ReadSensor())
      .WillOnce(Return(kThreshold + 100));
  EXPECT_CALL(*mock_sensors_[2], ReadSensor())
      .WillOnce(Return(kThreshold + 100));

  EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kFrontObstacle)).Times(1);

  controller_->CheckSensor();
}

// 장애물 수치 최댓값
TEST_F(ObstacleSensorControllerTest, MaximumValue) {
  EXPECT_CALL(*mock_sensors_[0], ReadSensor()).WillOnce(Return(std::numeric_limits<int>::max()));
  EXPECT_CALL(*mock_sensors_[1], ReadSensor())
      .WillOnce(Return(kThreshold + 100));
  EXPECT_CALL(*mock_sensors_[2], ReadSensor())
      .WillOnce(Return(kThreshold + 100));

  EXPECT_CALL(*mock_observer_ptr_, Notify(_)).Times(0);

  controller_->CheckSensor();
}

// 하드웨어 결함 : 센서 읽기 실패
TEST_F(ObstacleSensorControllerTest, SensorReadFail) {
  EXPECT_CALL(*mock_sensors_[0], ReadSensor())
      .WillOnce(Return(std::nullopt));

  EXPECT_CALL(*mock_sensors_[1], ReadSensor())
      .Times(0);

  EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kDataSizeFault)).Times(1);

  controller_->CheckSensor();
}

// 하드웨어 결함 : 잘못된 데이터 수신 (음수)
TEST_F(ObstacleSensorControllerTest, ReceiveWrongValue) {
  EXPECT_CALL(*mock_sensors_[0], ReadSensor()).WillOnce(Return(-1));
  EXPECT_CALL(*mock_sensors_[1], ReadSensor()).Times(0);
  EXPECT_CALL(*mock_sensors_[2], ReadSensor()).Times(0);

  EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kDataMinusFault)).Times(1);

   controller_->CheckSensor();
}
  // 하드웨어 결함 : 중간 센서 결함 시 로직 중단
TEST_F(ObstacleSensorControllerTest, MiddleSensorFail) {
  EXPECT_CALL(*mock_sensors_[0], ReadSensor())
      .WillOnce(Return(kThreshold + 100));
  EXPECT_CALL(*mock_sensors_[1], ReadSensor()).WillOnce(Return(std::nullopt));

  EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kFrontObstacle)).Times(0);
  EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kDataSizeFault)).Times(1);

  controller_->CheckSensor();
}
