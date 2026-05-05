#include  <actuator/actuator_controller.h>

class CleanerController : public ActuatorController {
 public:
  void SetOperation(ActuatorOperation operation) override;
};
