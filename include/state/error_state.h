#ifndef ERROR_STATE_H
#define ERROR_STATE_H

#include <state/rvc_state.h>
#include <types/event.h>

class ErrorState : public RvcState {
 public:
  virtual std::unique_ptr<RvcState> Handle(Event event) override;
  virtual void Enter() override;
  virtual void Exit() override;

  private:
  virtual void EmergencyStop();
   virtual void DisplayFatalError();
  virtual void WriteErrorLog();

  static constexpr Seconds kDisplayErrorDuration = 5s;
}
#endif
