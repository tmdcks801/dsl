#include <gtest/gtest.h>
#include <state/avoidance_state.h>
#include <state/backward_state.h>
#include <state/error_state.h>

#include <mock/core/observer.h>
#include <mock/actuator/drive_controller.h>
#include <mock/actuator/cleaner_controller.h>

using ::testing::_;
using ::testing::VariantWith;

class BackwardStateTest : public ::testing::Test {
 protected:
  void SetUp() override {
    mock_observer = std::make_unique<MockObserver>();
    mock_drive_controller = std::make_shared<MockDriveController>();
    mock_cleaner_controller = std::make_shared<MockCleanerController>();

    state = std::make_unique<BackwardState>(
        mock_observer.get(), mock_drive_controller, mock_cleaner_controller);
  }

  std::unique_ptr<MockObserver> mock_observer;
  std::shared_ptr<MockDriveController> mock_drive_controller;
  std::shared_ptr<MockCleanerController> mock_cleaner_controller;
  std::unique_ptr<BackwardState> state;
};

//후진 정상 작동 체크
TEST_F(BackwardStateTest, HappyPathBackward) {
  EXPECT_CALL(
      *mock_drive_controller,
      SetOperation(VariantWith<DriveOperation>(DriveOperation::kBackward)))
      .Times(1);

  state->Enter();
}

//후진 후 정지 체크
TEST_F(BackwardStateTest, HapplyPathExitAndStop) {
  EXPECT_CALL(*mock_drive_controller,
              SetOperation(VariantWith<DriveOperation>(DriveOperation::kStop)))
      .Times(1);

  state->Exit();
}

//후진 후 avoidance state 전환 체크
TEST_F(BackwardStateTest, HappyPathBackToAvoid) {
  auto next_state = state->Handle(Event::kTimerExpired);

  EXPECT_NE(next_state, nullptr);
  EXPECT_NE(dynamic_cast<AvoidanceState*>(next_state.get()), nullptr);
}

//좌측 모터 오작동 체크
TEST_F(BackwardStateTest, SadPathLMotorFail) {
  auto next_state = state->Handle(Event::kLMotorFault);

  EXPECT_NE(next_state, nullptr);
  EXPECT_NE(dynamic_cast<ErrorState*>(next_state.get()), nullptr);
}

//후진 중 타 이벤트 무시
TEST_F(BackwardStateTest, IgnoreUnrelatedEvents) {
  EXPECT_EQ(state->Handle(Event::kHighDust), nullptr);
  EXPECT_EQ(state->Handle(Event::kRightObstacle), nullptr);
}