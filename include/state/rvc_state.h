#ifndef RVC_STATE_H
#include <actuator/cleaner_controller.h>
#include <actuator/drive_controller.h>

#include <chrono>
#include <memory>

using Seconds = std::chrono::duration<double>;

class RvcState {
 public:
  RvcState(std::shared_ptr<Observer> observer,
           std::shared_ptr<DriveController> drive_controller,
           std::shared_ptr<CleanerController> cleaner_controller)
      : observer_(observer),
        drive_controller_(drive_controller),
        cleaner_controller_(cleaner_controller) {}

  virtual ~RvcState() = default;

  virtual std::unique_ptr<RvcState> Handle(Event event) = 0;
  virtual void Enter() = 0;
  virtual void Exit() = 0;
  virtual void Tick(Seconds tick_interval);

 protected:
  Seconds timer_;

  std::shared_ptr<Observer> observer_;
  std::shared_ptr<DriveController> drive_controller_;
  std::shared_ptr<CleanerController> cleaner_controller_;
};

#endif
