// Notify
/*
정상적으로 상태 전이?
상태 전이 전후로 Enter, Exit?
특수 로직 - kShutDown 확인
*/

#include <core/rvc_system.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mock/actuator/cleaner_controller.h>
#include <mock/actuator/drive_controller.h>
#include <mock/sensor/dust_sensor_controller.h>
#include <mock/sensor/obstacle_sensor_controller.h>
#include <mock/state/rvc_state.h>

using ::testing::_;
using ::testing::ByMove;
using ::testing::Return;

class RvcSystemTest : public ::testing::Test {
 protected:
  void SetUp() override {
    auto mock_dust_unique = std::make_unique<MockDustSensorController>();
    auto mock_obstacle_unique =
        std::make_unique<MockObstacleSensorController>();

    mock_drive_ = std::make_shared<MockDriveController>();
    mock_cleaner_ = std::make_shared<MockCleanerController>();

    auto mock_state_unique = std::make_unique<MockRvcState>(
        system_.get(), mock_drive_, mock_cleaner_);

    mock_dust_ = mock_dust_unique.get();
    mock_obstacle_ = mock_obstacle_unique.get();
    mock_state_ = mock_state_unique.get();

    EXPECT_CALL(*mock_dust_, AddObserver(_)).Times(1);
    EXPECT_CALL(*mock_obstacle_, AddObserver(_)).Times(1);
    EXPECT_CALL(*mock_state_, Enter()).Times(1);

    system_ = std::make_unique<RvcSystem>(
        std::move(mock_dust_unique), std::move(mock_obstacle_unique),
        mock_drive_, mock_cleaner_, kTickInterval,
        std::move(mock_state_unique));
  }

  static constexpr Seconds kTickInterval = 0.1s;

  MockDustSensorController* mock_dust_;
  MockObstacleSensorController* mock_obstacle_;

  MockRvcState* mock_state_;

  std::shared_ptr<MockDriveController> mock_drive_;
  std::shared_ptr<MockCleanerController> mock_cleaner_;

  std::unique_ptr<RvcSystem> system_;
};

// Tick 정상 동작 (센서 확인 -> State Tick)
TEST_F(RvcSystemTest, TickCallsSensorsAndState) {
  EXPECT_CALL(*mock_dust_, CheckSensor()).Times(1);
  EXPECT_CALL(*mock_obstacle_, CheckSensor()).Times(1);
  EXPECT_CALL(*mock_state_, Tick(kTickInterval)).Times(1);

  system_->Tick();
}

// 다중 Tick
TEST_F(RvcSystemTest, MultipleTickCalls) {
  int iterations = 5;

  EXPECT_CALL(*mock_dust_, CheckSensor()).Times(iterations);
  EXPECT_CALL(*mock_obstacle_, CheckSensor()).Times(iterations);
  EXPECT_CALL(*mock_state_, Tick(kTickInterval)).Times(iterations);

  for (int i = 0; i < iterations; ++i) {
    system_->Tick();
  }
}

// State 유지
TEST_F(RvcSystemTest, NoStateTransitionWhenHandleReturnsNull) {
  EXPECT_CALL(*mock_state_, Handle(Event::kLeftObstacle))
      .WillOnce(Return(ByMove(nullptr)));

  EXPECT_CALL(*mock_state_, Exit()).Times(0);
  EXPECT_CALL(*mock_state_, Enter()).Times(0);

  system_->Notify(Event::kLeftObstacle);
}

// State 전이
TEST_F(RvcSystemTest, NotifyTriggerStateTransition) {
  auto next_state_unique =
      std::make_unique<MockRvcState>(system_.get(), mock_drive_, mock_cleaner_);
  MockRvcState* next_state = next_state_unique.get();

  EXPECT_CALL(*mock_state_, Handle(Event::kFrontObstacle))
      .WillOnce(Return(ByMove(std::move(next_state_unique))));

  EXPECT_CALL(*mock_state_, Exit()).Times(1);
  EXPECT_CALL(*next_state, Enter()).Times(1);

  system_->Notify(Event::kFrontObstacle);
}

// 시스템 강제 종료
TEST_F(RvcSystemTest, NotifyThrowsExceptionOnShutdown) {
  EXPECT_CALL(*mock_state_, Handle(_)).Times(0);
  EXPECT_CALL(*mock_state_, Exit()).Times(0);

  EXPECT_THROW({ system_->Notify(Event::kSystemShutdown);

  }, std::runtime_error);
}
