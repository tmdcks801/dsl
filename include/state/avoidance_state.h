#ifndef AVOIDANCE_STATE_H
#define AVOIDANCE_STATE_H

#include <state/rvc_state.h>
#include <types/event.h>

// [삭제] 이전 TurnDirection 열거형은 AvoidanceState 내부 phase 흐름으로 대체한다.
// [삭제] 이전 내부 helper 선언: std::unique_ptr<RvcState> DetermineAndStartTurn();

class AvoidanceState : public RvcState {
 public:
  // [변경] 기존 즉시 회전 방향 판단 대신 상태 진입 후 phase별 회피 절차를 수행한다.
  using RvcState::RvcState;

  virtual std::unique_ptr<RvcState> Handle(Event event) override;
  virtual void Enter() override;
  virtual void Exit() override;

  ~AvoidanceState() override = default;

 private:
  // [추가] 좌측 확인, 회피 회전, 우측 탐색 회전, 우측 확인, 원위치 복귀 단계를 구분한다.
  enum class Phase {
    kCheckingLeft,
    kTurningLeftToAvoid,
    kTurningRightToScan,
    kScanningRight,
    kReturningFromRightScan
  };

  // [변경] 회전 시작 시 다음 phase와 타이머를 함께 설정한다.
  void StartTurn(DriveOperation operation, Phase phase);

  // [추가] 회전 시간은 3초, 좌측/우측 확인 window는 40Hz 기준 1 tick으로 정의한다.
  static constexpr Seconds kTurnDuration = 3s;
  static constexpr Seconds kScanDuration = 1.0s / 40;

  Phase phase_ = Phase::kCheckingLeft;
  bool is_left_blocked_ = false;
  bool is_right_blocked_ = false;
};
#endif
