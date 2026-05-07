#include <state/avoidance_state.h>
#include <state/backward_state.h>
#include <state/error_state.h>

std::unique_ptr<RvcState> BackwardState::Handle(Event event) {
  if (event == Event::kTimerExpired) {
    return std::make_unique<AvoidanceState>(observer_, drive_controller_,
                                            cleaner_controller_);
  }

  return RvcState::Handle(event);
}

void BackwardState::Enter() {
  drive_controller_->SetOperation(DriveOperation::kBackward);
  timer_ = kBackwardDuration;
}

void BackwardState::Exit() {
  drive_controller_->SetOperation(DriveOperation::kStop);
}
