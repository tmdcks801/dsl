#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <state/avoidance_state.h>
#include <state/backward_state.h>
#include <state/forward_state.h>
#include <state/error_state.h>

#include <mock/core/observer.h>
#include <mock/actuator/drive_controller.h>
#include <mock/actuator/cleaner_controller.h>

using ::testing::_;
using ::testing::VariantWith;
using ::testing::Return;

class AvoidanceStateTest : public ::testing::Test {
 protected:
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

  void SimulateTicksUntilTimerExpires() {
      state->Handle(Event::kTimerExpired);
  }
};

//Exit 실행 후, drive controller에 정지 명령 전달
TEST_F(AvoidanceStateTest, ExitAndStopDriving) {
  EXPECT_CALL(*mock_drive_controller, 
              SetOperation(VariantWith<DriveOperation>(DriveOperation::kStop)))
      .Times(1);

  state->Exit();
}
//전방 장애물 후 좌우 장애물 없음 > 우회전
TEST_F(AvoidanceStateTest, HappyPathNoObstBothSide) {
  state->Enter();

  EXPECT_CALL(*mock_drive_controller, 
              SetOperation(VariantWith<DriveOperation>(DriveOperation::kRight)))
      .Times(1);
      
  auto next_state = state->Handle(Event::kTimerExpired);
  EXPECT_EQ(next_state, nullptr);

  next_state = state->Handle(Event::kTimerExpired);
  
  EXPECT_NE(next_state, nullptr);
  EXPECT_NE(dynamic_cast<ForwardState*>(next_state.get()), nullptr);
}

//전방 우측 장애물 > 좌회전
TEST_F(AvoidanceStateTest, HappyPathRightObst) {
  state->Enter();

  state->Handle(Event::kRightObstacle);

  EXPECT_CALL(*mock_drive_controller, 
              SetOperation(VariantWith<DriveOperation>(DriveOperation::kLeft)))
      .Times(1);
      
  auto next_state = state->Handle(Event::kTimerExpired);
  EXPECT_EQ(next_state, nullptr);

  next_state = state->Handle(Event::kTimerExpired);
  
  EXPECT_NE(next_state, nullptr);
  EXPECT_NE(dynamic_cast<ForwardState*>(next_state.get()), nullptr);
}

//전좌우 장애물 > 후진 state 변환
TEST_F(AvoidanceStateTest, HappyPathBothObst) {
  state->Enter();

  state->Handle(Event::kRightObstacle);
  state->Handle(Event::kLeftObstacle);

  EXPECT_CALL(*mock_drive_controller, SetOperation(_)).Times(0);

  auto next_state = state->Handle(Event::kTimerExpired);
  
  EXPECT_NE(next_state, nullptr);
  EXPECT_NE(dynamic_cast<BackwardState*>(next_state.get()), nullptr);
}

//전좌측 장애물 > 우회전
TEST_F(AvoidanceStateTest, HappyPathLeftObst) {
  state->Enter();

  state->Handle(Event::kLeftObstacle);

  EXPECT_CALL(*mock_drive_controller, 
              SetOperation(VariantWith<DriveOperation>(DriveOperation::kRight)))
      .Times(1);
      
  auto next_state = state->Handle(Event::kTimerExpired);
  EXPECT_EQ(next_state, nullptr);
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
