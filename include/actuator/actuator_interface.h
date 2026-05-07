#ifndef ACTUATOR_INTERFACE_H
#define ACTUATOR_INTERFACE_H

#include <types/actuator_status.h>
#include <hardware_interface.h>
#include <variant>
#include <types/cleaner_action.h>
#include <types/motor_action.h>

using ActuatorAction = std::variant<CleanerAction, MotorAction, int>;

class ActuatorInterface : public HardwareInterface {
 public:
  using HardwareInterface::HardwareInterface;

  ~ActuatorInterface() override =  default;

  virtual ActuatorStatus GetStatus();

  virtual ActuatorStatus SetAction(ActuatorAction action);

  private:
  static constexpr int kActuatorStatusPos = 0;
  static constexpr int kActuatorActionPos = sizeof(int);
};


#endif
