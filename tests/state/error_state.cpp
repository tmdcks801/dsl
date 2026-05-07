#include <gtest/gtest.h>
#include <state/error_state.h>

#include <mock/core/observer.h>
#include <mock/actuator/drive_controller.h>
#include <mock/actuator/cleaner_controller.h>

using ::testing::_;
using ::testing::VariantWith;

class ErrorStateTest : public ::testing::Test {
 protected:
  void SetUp() override {
    mock_observer = std::make_unique<MockObserver>();
    mock_drive_controller = std::make_shared<MockDriveController>();
    mock_cleaner_controller = std::make_shared<MockCleanerController>();

    state = std::make_unique<ErrorState>(
        mock_observer.get(), mock_drive_controller, mock_cleaner_controller,
        Event::kLMotorFault);
  }

  std::unique_ptr<MockObserver> mock_observer;
  std::shared_ptr<MockDriveController> mock_drive_controller;
  std::shared_ptr<MockCleanerController> mock_cleaner_controller;
  std::unique_ptr<ErrorState> state;
};

//Error state 진입 시 하드웨어 정지 명령
TEST_F(ErrorStateTest, HappyPathStopHW) {
  EXPECT_CALL(*mock_drive_controller,
              SetOperation(VariantWith<DriveOperation>(DriveOperation::kStop)))
      .Times(1);
  EXPECT_CALL(*mock_cleaner_controller,
              SetOperation(VariantWith<CleanerAction>(CleanerAction::kOff)))
      .Times(1);

  state->Enter();
}

//Error state 정상 종료
TEST_F(ErrorStateTest, HappyPathSafelyExit) {
  EXPECT_NO_THROW(state->Exit());
}

//5초 후 kSystemShutdown 명령
TEST_F(ErrorStateTest, HappyPath5secShutdown) {
  EXPECT_CALL(*mock_observer, Notify(Event::kSystemShutdown)).Times(1);

  auto next_state = state->Handle(Event::kTimerExpired);

  EXPECT_EQ(next_state, nullptr);
}

//Error state 중 타 이벤트 무시
TEST_F(ErrorStateTest, IgnoreUnrelatedEvents) {
  EXPECT_CALL(*mock_observer, Notify(_)).Times(0);

  EXPECT_EQ(state->Handle(Event::kFrontObstacle), nullptr);
  EXPECT_EQ(state->Handle(Event::kHighDust), nullptr);
  EXPECT_EQ(state->Handle(Event::kLMotorFault),
            nullptr);
}