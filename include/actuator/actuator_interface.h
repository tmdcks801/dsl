#ifndef ACTUATOR_INTERFACE_H
#define ACTUATOR_INTERFACE_H

#include <hardware_interface.h>
#include <concepts>

enum class ActuatorStatus;
enum class CleanerAction;
enum class MotorAction;

class ActuatorInterface : public HardwareInterface {
 public:
  virtual ~ActuatorInterface() = default;

  virtual ActuatorStatus GetStatus();

  template <typename T>
  requires std::_Is_nothrow_convertible_v(T, int)
  ActuatorStatus SetAction(T action);

 private:
  static constexpr int kActuatorActionPos = 0;
  static constexpr int kActuatorStatusPos = sizeof(int);
};

using CleanerInterface = ActuatorInterface<CleanerAction>;
using MotorInterface = ActuatorInterface<MotorAction>

#endif
