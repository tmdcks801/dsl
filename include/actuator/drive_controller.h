#ifndef DRIVE_CONTROLLER_H
#define DRIVE_CONTROLLER_H

#include <actuator/actuator_controller.h>
#include <array>

class DriveController : public ActuatorController {
 public:
  explicit DriveController(
      Observer* observer,
      std::unique_ptr<InterfaceEntry<ActuatorInterface>[]> interfaces,
      const std::string& file_path_prefix)
      : ActuatorController(observer, std::move(interfaces), kMaxActuator,
                           file_path_prefix, actuator_events_) {}


  void SetOperation(ActuatorOperation operation) override;

 private:
  static constexpr std::size_t kMaxActuator = 2;

  static constexpr std::array<Event, kMaxActuator> actuator_events_ = {
      Event::kLMotorFault, Event::kRMotorFault
  };

  void Drive(MotorAction left, MotorAction right);
};

#endif
