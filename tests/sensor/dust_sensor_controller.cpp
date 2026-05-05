#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <limits>
#include <optional>

// Headers based on your project structure
#include <sensor/dust_sensor_controller.h>
#include <sensor/sensor_interface.h>
#include <core/observer.h> 
#include <types/event.h>

#include <mock/core/observer.h>
#include <mock/sensor/sensor_interface.h>

using ::testing::_;
using ::testing::Return;


class TestableDustSensorController : public DustSensorController {
 public:
  using DustSensorController::DustSensorController;

  void InjectMockSensor(std::unique_ptr<SensorInterface> mock_sensor) {
    interfaces_[0].hardware_interface = std::move(mock_sensor);
  }
};


class DustSensorControllerTest : public ::testing::Test {
 protected:
  MockObserver* mock_observer_ptr_ = nullptr;
  std::unique_ptr<TestableDustSensorController> controller_;

  static constexpr int kThreshold = std::numeric_limits<int>::max() / 2;
  static constexpr std::size_t kTestSensorCount = 1;

  void SetUp() override {
    auto mock_observer = std::make_unique<MockObserver>();
    mock_observer_ptr_ = mock_observer.get();

    auto interfaces = std::make_unique<InterfaceEntry<SensorInterface>[]>(kTestSensorCount);

    EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kHWFault)).Times(testing::AnyNumber());

    controller_ = std::make_unique<TestableDustSensorController>(
        std::move(mock_observer), 
        std::move(interfaces),
        "dust_");
  }
};

// 읽은 값 == MAX
TEST_F(DustSensorControllerTest, NotifiesHighDustAtIntMax) {
  auto mock_sensor = std::make_unique<MockSensorInterface>();
  EXPECT_CALL(*mock_sensor, ReadSensor()).WillOnce(Return(std::numeric_limits<int>::max()));

  EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kHighDust)).Times(1);

  controller_->InjectMockSensor(std::move(mock_sensor));
  controller_->CheckSensor();
}

// 읽은 값 == 0
TEST_F(DustSensorControllerTest, DoesNotNotifyWhenAt0) {
  auto mock_sensor = std::make_unique<MockSensorInterface>();
  EXPECT_CALL(*mock_sensor, ReadSensor()).WillOnce(Return(0));

  EXPECT_CALL(*mock_observer_ptr_, Notify(_)).Times(0);

  controller_->InjectMockSensor(std::move(mock_sensor));
  controller_->CheckSensor();
}

// 0 < 읽은 값 < 임계값(이벤트X)
TEST_F(DustSensorControllerTest, DoesNotNotifyWhenLowerThanThreshold) {
  auto mock_sensor = std::make_unique<MockSensorInterface>();
  EXPECT_CALL(*mock_sensor, ReadSensor()).WillOnce(Return(kThreshold - 1));

  EXPECT_CALL(*mock_observer_ptr_, Notify(_)).Times(0);

  controller_->InjectMockSensor(std::move(mock_sensor));
  controller_->CheckSensor();
}

// 읽은 값 == 임계값(이벤트X)
TEST_F(DustSensorControllerTest, DoesNotNotifyWhenAtThreshold) {
  auto mock_sensor = std::make_unique<MockSensorInterface>();
  EXPECT_CALL(*mock_sensor, ReadSensor()).WillOnce(Return(kThreshold));

  EXPECT_CALL(*mock_observer_ptr_, Notify(_)).Times(0);

  controller_->InjectMockSensor(std::move(mock_sensor));
  controller_->CheckSensor();
}

// 읽은 값 > 임계값
TEST_F(DustSensorControllerTest, NotifiesHighDustWhenAboveThreshold) {
  auto mock_sensor = std::make_unique<MockSensorInterface>();
  EXPECT_CALL(*mock_sensor, ReadSensor()).WillOnce(Return(kThreshold + 1));

  EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kHighDust)).Times(1);

  controller_->InjectMockSensor(std::move(mock_sensor));
  controller_->CheckSensor();
}

// 하드웨어 에러
TEST_F(DustSensorControllerTest, NotifiesFaultOnReadFailure) {
  auto mock_sensor = std::make_unique<MockSensorInterface>();
  EXPECT_CALL(*mock_sensor, ReadSensor()).WillOnce(Return(std::nullopt));

  EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kHWFault)).Times(1);

  controller_->InjectMockSensor(std::move(mock_sensor));
  controller_->CheckSensor();
}

// 읽은 값 < 0
TEST_F(DustSensorControllerTest, NotifiesFaultOnNegativeValues) {
  auto mock_sensor = std::make_unique<MockSensorInterface>();
  EXPECT_CALL(*mock_sensor, ReadSensor()).WillOnce(Return(-500));

  EXPECT_CALL(*mock_observer_ptr_, Notify(Event::kHWFault)).Times(1);

  controller_->InjectMockSensor(std::move(mock_sensor));
  controller_->CheckSensor();
}
