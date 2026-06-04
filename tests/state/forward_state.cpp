#include <gtest/gtest.h>
#include <state/avoidance_state.h>
#include <state/error_state.h>
#include <state/forward_state.h>

#include <mock/core/observer.h>
#include <mock/actuator/drive_controller.h>
#include <mock/actuator/cleaner_controller.h>

using ::testing::_;
using ::testing::VariantWith;

class ForwardStateTest : public ::testing::Test {
 protected:
  void SetUp() override {
    mock_observer = std::make_unique<MockObserver>();
    mock_drive_controller = std::make_shared<MockDriveController>();
    mock_cleaner_controller = std::make_shared<MockCleanerController>();

    state = std::make_unique<ForwardState>(
        mock_observer.get(), mock_drive_controller, mock_cleaner_controller);
  }

  std::unique_ptr<MockObserver> mock_observer;
  std::shared_ptr<MockDriveController> mock_drive_controller;
  std::shared_ptr<MockCleanerController> mock_cleaner_controller;
  std::unique_ptr<ForwardState> state;
};

//forward state 진입 시 모터 클리너 작동
TEST_F(ForwardStateTest, HappyPathHWActivate) {
  EXPECT_CALL(
      *mock_drive_controller,
      SetOperation(VariantWith<DriveOperation>(DriveOperation::kForward)))
      .Times(1);
  EXPECT_CALL(*mock_cleaner_controller,
              SetOperation(VariantWith<CleanerAction>(CleanerAction::kOn)))
      .Times(1);

  state->Enter();
}

//forward state 종료 시 모터 클리너 종료
TEST_F(ForwardStateTest, ExitStopsDriveAndCleaning) {
  EXPECT_CALL(*mock_drive_controller,
              SetOperation(VariantWith<DriveOperation>(DriveOperation::kStop)))
      .Times(1);
  EXPECT_CALL(*mock_cleaner_controller,
              SetOperation(VariantWith<CleanerAction>(CleanerAction::kOff)))
      .Times(1);

  state->Exit();
}

//avoidance state 전환
TEST_F(ForwardStateTest, HappyPathSwitch2Avoidance) {
  auto next_state = state->Handle(Event::kFrontObstacle);

  EXPECT_NE(next_state, nullptr);
  EXPECT_NE(dynamic_cast<AvoidanceState*>(next_state.get()), nullptr);
}

//강력 청소 수행
TEST_F(ForwardStateTest, HappyPathPowerCleaning) {
  EXPECT_CALL(*mock_cleaner_controller,
              SetOperation(VariantWith<CleanerAction>(CleanerAction::kPowerUp)))
      .Times(1);

  auto next_state = state->Handle(Event::kHighDust);
  EXPECT_EQ(next_state, nullptr);
}

//강력 청소 종료
TEST_F(ForwardStateTest, HapplyPathBack2Cleaning) {
  EXPECT_CALL(*mock_cleaner_controller,
              SetOperation(VariantWith<CleanerAction>(CleanerAction::kOn)))
      .Times(1);

  auto next_state = state->Handle(Event::kTimerExpired);
  EXPECT_EQ(next_state, nullptr);
}

//하드웨어 오작동
TEST_F(ForwardStateTest, SadPathLMotorFail) {
  auto next_state = state->Handle(Event::kLMotorFault);

  EXPECT_NE(next_state, nullptr);
  EXPECT_NE(dynamic_cast<ErrorState*>(next_state.get()), nullptr);
}
