#ifndef RVC_STATE_H
#define RVC_STATE_H

#include <actuator/cleaner_controller.h>
#include <actuator/drive_controller.h>

#include <chrono>
#include <memory>

using Seconds = std::chrono::duration<double>;
using namespace std::literals::chrono_literals;

class RvcState {
 public:
  RvcState(Observer* observer,
           std::shared_ptr<DriveController> drive_controller,
           std::shared_ptr<CleanerController> cleaner_controller)
      : observer_(observer),
        drive_controller_(drive_controller),
        cleaner_controller_(cleaner_controller) {
  }

  virtual ~RvcState() = default;

  virtual std::unique_ptr<RvcState> Handle(Event event);
  virtual void Enter() = 0;
  virtual void Exit() = 0;
  virtual void Tick(Seconds tick_interval);

 protected:
  static bool isHardwareFault(Event event);

  static constexpr uint32_t kFaultMask = 0xFFFF'FF00;

  Seconds timer_;

  Observer* observer_;
  std::shared_ptr<DriveController> drive_controller_;
  std::shared_ptr<CleanerController> cleaner_controller_;
};

#endif
