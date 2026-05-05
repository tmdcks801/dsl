#ifndef CLEANER_CONTROLLER_H
#define CLEANER_CONTROLLER_H

#include  <actuator/actuator_controller.h>

class CleanerController : public ActuatorController {
 public:
  using ActuatorController::ActuatorController;

  void SetOperation(ActuatorOperation operation) override;
};

#endif
