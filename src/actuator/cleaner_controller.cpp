#include <actuator/actuator_interface.h>
#include <actuator/cleaner_controller.h>

#include <variant>

void CleanerController::SetOperation(ActuatorOperation operation) {
  auto operation_value = std::get<CleanerAction>(operation);
  interfaces_[0].hardware_interface->SetAction(
      static_cast<int>(operation_value));
}
