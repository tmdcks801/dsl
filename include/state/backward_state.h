#ifndef BACKWARD_STATE_H
#define BACKWARD_STATE_H

#include <state/rvc_state.h>
#include <types/event.h>

class BackwardState : public RvcState {
 public:
	 using RvcState::RvcState;
  virtual std::unique_ptr<RvcState> Handle(Event event) override;
  virtual void Enter() override;
  virtual void Exit() override;

  ~BackwardState() override = default;

  private:
  static constexpr Seconds kBackwardDuration = 3s;
};
#endif
