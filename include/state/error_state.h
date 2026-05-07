#ifndef ERROR_STATE_H
#define ERROR_STATE_H

#include <state/rvc_state.h>
#include <types/event.h>

class ErrorState : public RvcState {
 public:
  explicit ErrorState(
      Observer *observer, std::shared_ptr<DriveController> drive_controller,
                      std::shared_ptr<CleanerController> cleaner_controller,
                      Event error_event)
      : RvcState(observer, drive_controller, cleaner_controller),
        error_event_(error_event) {}

  virtual std::unique_ptr<RvcState> Handle(Event event) override;
  virtual void Enter() override;
  virtual void Exit() override;

  ~ErrorState() override = default;

 private:
  virtual void EmergencyStop();
  virtual void DisplayFatalError();
  virtual void WriteErrorLog();

  static constexpr Seconds kDisplayErrorDuration = 5s;
  Event error_event_;
};

#endif
