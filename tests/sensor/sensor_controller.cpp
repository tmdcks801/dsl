#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <vector>
#include <span>

// Your actual headers
#include <hardware_controller.h>
#include <sensor/sensor_controller.h>
#include <sensor/sensor_interface.h>
#include <core/observer.h>
#include <types/event.h>

using ::testing::_;
using ::testing::Return;
using ::testing::AnyNumber;


class MockObserver : public Observer {
 public:
  MOCK_METHOD(void, Notify, (Event event), (override));
};

class MockSensorInterface : public SensorInterface {
 public:
  MockSensorInterface() : SensorInterface(0, nullptr) {}//+++

  MOCK_METHOD(std::optional<int>, ReadSensor, (), (noexcept, override));
};

class TestableSensorController : public SensorController {
 public:
  using SensorController::SensorController;

  // Mock the pure virtual method
  MOCK_METHOD(bool, CheckValue, (int raw_data), (const, noexcept, override));

  // Backdoor method to safely inject our mocked sensors into the protected array
  void InjectMockSensor(std::size_t index, std::unique_ptr<HardwareInterface> mock_sensor) {
    if (index < interface_count_) {
      interfaces_[index].hardware_interface = std::move(mock_sensor);
    }
  }
};

class SensorControllerTest : public ::testing::Test {
 protected:
  MockObserver* mock_observer_ptr_ = nullptr;
  std::unique_ptr<TestableSensorController> controller_;

  static constexpr std::size_t kTestSensorCount = 2;
  const std::vector<Event> test_events_{Event::kFrontObstacle, Event::kLeftObstacle};

  void SetUp() override {
    auto mock_observer = std::make_unique<MockObserver>();
    mock_observer_ptr_ = mock_observer.get(); 

    auto interfaces = std::make_unique<InterfaceEntry[]>(kTestSensorCount);

    EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kHWFault)).Times(AnyNumber());

    controller_ = std::make_unique<TestableSensorController>(
        std::move(mock_observer),
        std::move(interfaces),
        kTestSensorCount,
        "dummy_path_",
        test_events_);

    // Clear setup expectations so tests start fresh
    testing::Mock::VerifyAndClearExpectations(mock_observer_ptr_);
  }
};

TEST_F(SensorControllerTest, FailsWhenReadReturnsNullopt) {
  auto mock_sensor = std::make_unique<MockSensorInterface>();
  EXPECT_CALL(*mock_sensor, ReadSensor()).WillOnce(Return(std::nullopt));

  EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kHWFault)).Times(1);
  EXPECT_CALL(*controller_, CheckValue(_)).Times(0); 

  controller_->InjectMockSensor(0, std::move(mock_sensor));
  controller_->CheckSensor();
}

TEST_F(SensorControllerTest, FailsWhenReadReturnsNegative) {
  auto mock_sensor = std::make_unique<MockSensorInterface>();

  EXPECT_CALL(*mock_sensor, ReadSensor()).WillOnce(Return(-5));

  EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kHWFault)).Times(1);
  EXPECT_CALL(*controller_, CheckValue(_)).Times(0);

  controller_->InjectMockSensor(0, std::move(mock_sensor));
  controller_->CheckSensor();
}

TEST_F(SensorControllerTest, IgnoresInvalidValue) {
  auto mock_sensor = std::make_unique<MockSensorInterface>();
  EXPECT_CALL(*mock_sensor, ReadSensor()).WillOnce(Return(100));

  EXPECT_CALL(*controller_, CheckValue(100)).WillOnce(Return(false));
  EXPECT_CALL(*mock_observer_ptr_, Notify(_)).Times(0);

  controller_->InjectMockSensor(0, std::move(mock_sensor));
  controller_->CheckSensor();
}

TEST_F(SensorControllerTest, NotifiesEventOnSuccess) {
  auto mock_sensor = std::make_unique<MockSensorInterface>();
  EXPECT_CALL(*mock_sensor, ReadSensor()).WillOnce(Return(200));

  EXPECT_CALL(*controller_, CheckValue(200)).WillOnce(Return(true));

  EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kFrontObstacle)).Times(1);

  controller_->InjectMockSensor(0, std::move(mock_sensor));
  controller_->CheckSensor();
}

TEST_F(SensorControllerTest, CheckSensor_BreaksLoopOnFirstError) {

  auto mock_sensor_0 = std::make_unique<MockSensorInterface>();
  EXPECT_CALL(*mock_sensor_0, ReadSensor()).WillOnce(Return(100));
  EXPECT_CALL(*controller_, CheckValue(100)).WillOnce(Return(true));
  EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kNormalEvent1)).Times(1);

  auto mock_sensor_1 = std::make_unique<MockSensorInterface>();
  EXPECT_CALL(*mock_sensor_1, ReadSensor()).WillOnce(Return(std::nullopt));
  EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kHWFault)).Times(1);

  controller_->InjectMockSensor(0, std::move(mock_sensor_0));
  controller_->InjectMockSensor(1, std::move(mock_sensor_1));

  controller_->CheckSensor();
}
