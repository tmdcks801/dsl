#include <state/forward_state.h>
#include <state/error_state.h>
#include <state/avoidance_state.h>

std::unique_ptr<RvcState> ForwardState::Handle(Event event) {

  if (event == Event::kHighDust) {
    cleaner_controller_->SetOperation(CleanerAction::kPowerUp);
    timer_ = Seconds(kTurboCleanDuration);
  }

  if (event == Event::kTimerExpired) {
    cleaner_controller_->SetOperation(CleanerAction::kOn);
    timer_ = Seconds::max();
  }

  if (event == Event::kFrontObstacle) {
    return std::make_unique<AvoidanceState>(observer_, drive_controller_,
                                           cleaner_controller_);
  }

  return RvcState::Handle(event);
}

void ForwardState::Enter() {
  cleaner_controller_->SetOperation(CleanerAction::kOn);
  drive_controller_->SetOperation(DriveOperation::kForward);
}

void ForwardState::Exit() {
  cleaner_controller_->SetOperation(CleanerAction::kOff);
  drive_controller_->SetOperation(DriveOperation::kStop);
}
