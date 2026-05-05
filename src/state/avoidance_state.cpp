#include <state/avoidance_state.h>
#include <state/backward_state.h>
#include <state/forward_state.h>

std::unique_ptr<RvcState> AvoidanceState::Handle(Event event) {
  if (event == Event::kTimerExpired) {
    if (is_turning) {
      if (is_right_blocked_) {
        if (is_left_blocked_) {
          return std::unique_ptr<BackwardState>(observer_, drive_controller_,
                                                cleaner_controller_);
        }
        StartTurn(DriveOperation::kLeft)
        return nullptr;
      }
      StartTurn(DriveOperation::kRight);
      return nullptr;
    } else {
      return std::unique_ptr<ForwardState>(observer_, drive_controller_,
                                           cleaner_controller_);
    }
  }

  if (event == Event::kRightObstacle) {
    is_right_blocked_ = true;
  }

  if (event == Event::kLeftObstacle) {
    is_left_blocked_ = true;
  }

  return nullptr;
}

void AvoidanceState::Enter() { timer_ = 0; }

void AvoidanceState::Exit() {
  drive_controller_->SetOperation(DriveOperation::kStop);
}

void AvoidanceState::StartTurn(DriveOperation operation) {
  is_turning = true;
  drive_controller_->SetOperation(operation);
  timer_ = kTurnDuration;
}

//  < 1 틱 >
// F : 직진 상태 -> 회피 상태
// R : 우측 장애물 플래그
// L : 좌측 장애물 플래그

// TimerExpired

// < 2 틱 >
// ...

{
}
