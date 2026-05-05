#ifndef DRIVE_CONTROLLER_H
#define DRIVE_CONTROLLER_H

#include <actuator/actuator_controller.h>

class DriveController : public ActuatorController {
 public:
  using ActuatorController::ActuatorController;
  void SetOperation(ActuatorOperation operation) override;

 private:
  ActuatorStatus Drive(MotorAction left, MotorAction right);
};

#endif
