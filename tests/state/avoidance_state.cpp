#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <state/avoidance_state.h>
#include <state/backward_state.h>
#include <state/error_state.h>
#include <state/forward_state.h>

#include <mock/actuator/cleaner_controller.h>
#include <mock/actuator/drive_controller.h>
#include <mock/core/observer.h>

using ::testing::_;
using ::testing::Mock;
using ::testing::VariantWith;

class AvoidanceStateTest : public ::testing::Test {
 protected:
  // [추가] 좌측/우측 확인 window가 40Hz 기준 1 tick인지 검증하기 위한 상수.
  static constexpr int kFrequency = 40;
  static constexpr Seconds kOneTick = 1.0s / kFrequency;
  static constexpr Seconds kShorterThanOneTick = kOneTick / 2;

  void SetUp() override {
    mock_observer = std::make_unique<MockObserver>();
    mock_drive_controller = std::make_shared<MockDriveController>();
    mock_cleaner_controller = std::make_shared<MockCleanerController>();

    state = std::make_unique<AvoidanceState>(
        mock_observer.get(), mock_drive_controller, mock_cleaner_controller);
  }

  std::unique_ptr<MockObserver> mock_observer;
  std::shared_ptr<MockDriveController> mock_drive_controller;
  std::shared_ptr<MockCleanerController> mock_cleaner_controller;
  std::unique_ptr<AvoidanceState> state;

  // [삭제] 이전 timer helper: void SimulateTicksUntilTimerExpires();
};

//Exit 실행 후, drive controller에 정지 명령 전달
TEST_F(AvoidanceStateTest, ExitAndStopDriving) {
  EXPECT_CALL(*mock_drive_controller,
              SetOperation(VariantWith<DriveOperation>(DriveOperation::kStop)))
      .Times(1);

  state->Exit();
}

// [변경] 좌측 장애물이 없으면 좌회전 후 ForwardState로 전환
TEST_F(AvoidanceStateTest, LeftClearStartsLeftTurnAndSwitchesToForward) {
  state->Enter();

  EXPECT_CALL(*mock_drive_controller,
              SetOperation(VariantWith<DriveOperation>(DriveOperation::kLeft)))
      .Times(1);

  EXPECT_EQ(state->Handle(Event::kTimerExpired), nullptr);

  auto next_state = state->Handle(Event::kTimerExpired);
  EXPECT_NE(next_state, nullptr);
  EXPECT_NE(dynamic_cast<ForwardState*>(next_state.get()), nullptr);
}

// [변경] 좌측 장애물이 있으면 우회전 회피가 아니라 우측 탐색 회전을 시작
TEST_F(AvoidanceStateTest, LeftBlockedStartsRightScanTurn) {
  state->Enter();
  EXPECT_EQ(state->Handle(Event::kLeftObstacle), nullptr);

  EXPECT_CALL(*mock_drive_controller,
              SetOperation(VariantWith<DriveOperation>(DriveOperation::kRight)))
      .Times(1);

  EXPECT_EQ(state->Handle(Event::kTimerExpired), nullptr);
}

// [추가] 좌측이 막히고 우측이 비어 있으면 우측 탐색 자세 그대로 ForwardState로 전환
TEST_F(AvoidanceStateTest, RightClearAfterRightScanSwitchesToForward) {
  state->Enter();
  EXPECT_EQ(state->Handle(Event::kLeftObstacle), nullptr);

  EXPECT_CALL(*mock_drive_controller,
              SetOperation(VariantWith<DriveOperation>(DriveOperation::kRight)))
      .Times(1);
  EXPECT_EQ(state->Handle(Event::kTimerExpired), nullptr);
  Mock::VerifyAndClearExpectations(mock_drive_controller.get());

  EXPECT_CALL(*mock_drive_controller,
              SetOperation(VariantWith<DriveOperation>(DriveOperation::kStop)))
      .Times(1);
  EXPECT_EQ(state->Handle(Event::kTimerExpired), nullptr);
  Mock::VerifyAndClearExpectations(mock_drive_controller.get());

  auto next_state = state->Handle(Event::kTimerExpired);

  EXPECT_NE(next_state, nullptr);
  EXPECT_NE(dynamic_cast<ForwardState*>(next_state.get()), nullptr);
}

// [변경] 좌측과 우측이 모두 막히면 즉시 후진하지 않고 원래 각도로 복귀한 뒤 BackwardState로 전환
TEST_F(AvoidanceStateTest, BothSidesBlockedReturnsThenSwitchesToBackward) {
  state->Enter();
  EXPECT_EQ(state->Handle(Event::kLeftObstacle), nullptr);

  EXPECT_CALL(*mock_drive_controller,
              SetOperation(VariantWith<DriveOperation>(DriveOperation::kRight)))
      .Times(1);
  EXPECT_EQ(state->Handle(Event::kTimerExpired), nullptr);
  Mock::VerifyAndClearExpectations(mock_drive_controller.get());

  EXPECT_CALL(*mock_drive_controller,
              SetOperation(VariantWith<DriveOperation>(DriveOperation::kStop)))
      .Times(1);
  EXPECT_EQ(state->Handle(Event::kTimerExpired), nullptr);
  EXPECT_EQ(state->Handle(Event::kFrontObstacle), nullptr);
  Mock::VerifyAndClearExpectations(mock_drive_controller.get());

  EXPECT_CALL(*mock_drive_controller,
              SetOperation(VariantWith<DriveOperation>(DriveOperation::kLeft)))
      .Times(1);
  EXPECT_EQ(state->Handle(Event::kTimerExpired), nullptr);
  Mock::VerifyAndClearExpectations(mock_drive_controller.get());

  auto next_state = state->Handle(Event::kTimerExpired);
  EXPECT_NE(next_state, nullptr);
  EXPECT_NE(dynamic_cast<BackwardState*>(next_state.get()), nullptr);
}

// [추가] 좌측 확인 window는 40Hz 기준 1 tick 동안 유지
TEST_F(AvoidanceStateTest, LeftCheckWindowLastsOneTick) {
  state->Enter();

  EXPECT_CALL(*mock_observer, Notify(Event::kTimerExpired)).Times(0);
  state->Tick(kShorterThanOneTick);
  Mock::VerifyAndClearExpectations(mock_observer.get());

  EXPECT_CALL(*mock_observer, Notify(Event::kTimerExpired)).Times(1);
  state->Tick(kOneTick);
}

//회전 중 타 이벤트 무시
TEST_F(AvoidanceStateTest, IgnoreUnrelatedEvents) {
  EXPECT_EQ(state->Handle(Event::kHighDust), nullptr);
  EXPECT_EQ(state->Handle(Event::kSystemShutdown), nullptr);
}

//회전 중 좌측 모터 오작동
TEST_F(AvoidanceStateTest, SadPathLMotorFail) {
  auto next_state = state->Handle(Event::kLMotorFault);

  EXPECT_NE(next_state, nullptr);
  EXPECT_NE(dynamic_cast<ErrorState*>(next_state.get()), nullptr);
}
