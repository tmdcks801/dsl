#ifndef AVOIDANCE_STATE_H
#define AVOIDANCE_STATE_H

#include <state/rvc_state.h>
#include <types/event.h>

enum class TurnDirection {
	kLeft,
	kRight,
	kNone
};

class AvoidanceState : public RvcState {
 public:
  using RvcState::RvcState;

  virtual std::unique_ptr<RvcState> Handle(Event event) override;
  virtual void Enter() override;
  virtual void Exit() override;

  ~AvoidanceState() override = default;

 private:
  std::unique_ptr<RvcState> DetermineAndStartTurn();
  void StartTurn(DriveOperation operation);

  static constexpr Seconds kTurnDuration = 3s;
  bool is_left_blocked_ = false;
  bool is_right_blocked_ = false;

  bool is_turning = false;
};
#endif
