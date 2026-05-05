#include <actuator/actuator_controller.h>

#include <concepts>

class DriveController : public ActuatorController {
 public:
  void SetOperation(ActuatorOperation operation) override;

};
