#include <state/avoidance_state.h>
#include <state/backward_state.h>
#include <state/forward_state.h>

// [삭제] 이전 회전 방향 결정 helper 구현: std::unique_ptr<RvcState> AvoidanceState::DetermineAndStartTurn().
// [변경] 기존 좌/우 장애물 누적 후 단일 판단하던 구현은 phase 기반 회피 절차로 대체했다.

std::unique_ptr<RvcState> AvoidanceState::Handle(Event event) {
  // [변경] 좌측 장애물 이벤트는 기존 전 구간 누적 대신 좌측 확인 단계에서만 반영한다.
  if (phase_ == Phase::kCheckingLeft && event == Event::kLeftObstacle) {
    is_left_blocked_ = true;
    return nullptr;
  }

  // [추가] 우측 스캔 단계에서만 전방 센서 이벤트를 우측 장애물로 해석한다.
  if (phase_ == Phase::kScanningRight && event == Event::kFrontObstacle) {
    is_right_blocked_ = true;
    return nullptr;
  }

  if (event == Event::kTimerExpired) {
    switch (phase_) {
      case Phase::kCheckingLeft:
        // [변경] 좌측이 비어 있으면 기존 우회전 대신 좌회전 회피를 수행한다.
        if (!is_left_blocked_) {
          StartTurn(DriveOperation::kLeft, Phase::kTurningLeftToAvoid);
          return nullptr;
        }

        // [변경] 좌측이 막혀 있으면 우회전 회피가 아니라 우측 탐색 회전을 수행한다.
        StartTurn(DriveOperation::kRight, Phase::kTurningRightToScan);
        return nullptr;

      case Phase::kTurningLeftToAvoid:
        // [변경] 좌회전 회피가 끝나면 직진 주행으로 복귀한다.
        return std::make_unique<ForwardState>(observer_, drive_controller_,
                                              cleaner_controller_);

      case Phase::kTurningRightToScan:
        // [추가] 우측 탐색 자세가 되면 정지 후 1 tick 동안 전방 센서로 우측을 확인한다.
        phase_ = Phase::kScanningRight;
        is_right_blocked_ = false;
        drive_controller_->SetOperation(DriveOperation::kStop);
        timer_ = kScanDuration;
        return nullptr;

      case Phase::kScanningRight:
        // [추가] 우측이 비어 있으면 이미 우측을 바라보는 상태이므로 곧바로 직진 주행으로 전환한다.
        if (!is_right_blocked_) {
          return std::make_unique<ForwardState>(observer_, drive_controller_,
                                                cleaner_controller_);
        }

        // [추가] 우측도 막혀 있으면 원래 각도로 복귀한 뒤 후진한다.
        StartTurn(DriveOperation::kLeft, Phase::kReturningFromRightScan);
        return nullptr;

      case Phase::kReturningFromRightScan:
        // [추가] 원래 각도 복귀가 끝나면 전방 장애물에서 멀어지도록 후진 상태로 전환한다.
        return std::make_unique<BackwardState>(observer_, drive_controller_,
                                               cleaner_controller_);
    }
  }

  return RvcState::Handle(event);
}

// [변경] 진입 시 즉시 회전하지 않고 1 tick 동안 좌측 장애물 여부를 확인한다.
void AvoidanceState::Enter() {
  phase_ = Phase::kCheckingLeft;
  is_left_blocked_ = false;
  is_right_blocked_ = false;
  timer_ = kScanDuration;
}

void AvoidanceState::Exit() {
  drive_controller_->SetOperation(DriveOperation::kStop);
}

// [변경] StartTurn()은 회전 방향과 해당 회전 phase를 함께 설정한다.
void AvoidanceState::StartTurn(DriveOperation operation, Phase phase) {
  phase_ = phase;
  drive_controller_->SetOperation(operation);
  timer_ = kTurnDuration;
}
