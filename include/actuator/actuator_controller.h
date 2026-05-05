#ifndef ACTUATOR_CONTROLLER_H
#define ACTUATOR_CONTROLLER_H

#include <actuator/actuator_interface.h>
#include <hardware_controller.h>
#include <types/cleaner_action.h>
#include <types/drive_operation.h>

#include <variant>

using ActuatorOperation = std::variant<CleanerAction, DriveOperation, int>;

class ActuatorController : public HardwareController<ActuatorInterface> {
 public:
  using HardwareController<ActuatorInterface>::HardwareController;

  virtual ~ActuatorController() = default;

  virtual void SetOperation(ActuatorOperation operation) = 0;
};

#endif
