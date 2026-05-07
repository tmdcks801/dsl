#include <state/avoidance_state.h>
#include <state/backward_state.h>
#include <state/error_state.h>
#include <state/forward_state.h>

std::unique_ptr<RvcState> AvoidanceState::Handle(Event event) {
  if (event == Event::kTimerExpired) {
    if (!is_turning) {
      return DetermineAndStartTurn();
    }

    return std::make_unique<ForwardState>(observer_, drive_controller_,
                                          cleaner_controller_);
  }

  if (event == Event::kRightObstacle) {
    is_right_blocked_ = true;
  }

  if (event == Event::kLeftObstacle) {
    is_left_blocked_ = true;
  }

  return RvcState::Handle(event);
}

void AvoidanceState::Enter() { timer_ = 0s; }

void AvoidanceState::Exit() {
  drive_controller_->SetOperation(DriveOperation::kStop);
}

std::unique_ptr<RvcState> AvoidanceState::DetermineAndStartTurn() {
  if (is_right_blocked_) {
    if (is_left_blocked_) {
      return std::make_unique<BackwardState>(observer_, drive_controller_,
                                             cleaner_controller_);
    }
    StartTurn(DriveOperation::kLeft);
    return nullptr;
  }
  StartTurn(DriveOperation::kRight);
  return nullptr;
}

void AvoidanceState::StartTurn(DriveOperation operation) {
  is_turning = true;
  drive_controller_->SetOperation(operation);
  timer_ = kTurnDuration;
}
