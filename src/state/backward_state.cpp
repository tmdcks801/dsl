#include <state/backward_state.h>
#include <state/avoidance_state.h>

std::unique_ptr<RvcState> BackwardState::Handle(Event event) {
  if (event == Event::kTimerExpired) {
    return std::unique_ptr<AvoidanceState>(observer_, drive_controller_, cleaner_controller_);
  }

    return nullptr;
}

void BackwardState::Enter() {
  drive_controller_->SetOperation(DriveOperation::kBackward);
}

void BackwardState::Exit() {
  drive_controller_->SetOperation(DriveOperation::kStop);
}
