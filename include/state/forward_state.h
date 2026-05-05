#ifndef FORWARD_STATE_H
#define FORWARD_STATE_H

#include <state/rvc_state.h>
#include <types/event.h>

class ForwardState : public RvcState {
 public:
  virtual std::unique_ptr<RvcState> Handle(Event event) override;
  virtual void Enter() override;
  virtual void exit() override;

 private:
  static constexpr Seconds kTurboCleanDuration = 3s;
};
#endif
