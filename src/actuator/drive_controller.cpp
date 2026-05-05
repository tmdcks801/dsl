#include <actuator/drive_controller.h>

#include <variant>

#include <types/motor_action.h>

void DriveController::SetOperation(ActuatorOperation operation) {

  auto operation_value = std::get<DriveOperation>(operation);



  switch (operation_value) {
    case DriveOperation::kBackward:
      interfaces_[0].hardware_interface->SetAction(MotorAction::kBackward);
      interfaces_[1].hardware_interface->SetAction(MotorAction::kBackward);
      break;

    case DriveOperation::kForward:
      interfaces_[0].hardware_interface->SetAction(MotorAction::kForward);
      interfaces_[1].hardware_interface->SetAction(MotorAction::kForward);
      break;

    case DriveOperation::kLeft:
      interfaces_[0].hardware_interface->SetAction(MotorAction::kBackward);
      interfaces_[1].hardware_interface->SetAction(MotorAction::kForward);
      break;

    case DriveOperation::kRight:
      interfaces_[0].hardware_interface->SetAction(MotorAction::kForward);
      interfaces_[1].hardware_interface->SetAction(MotorAction::kBackward);
      break;

    case DriveOperation::kStop:
      interfaces_[0].hardware_interface->SetAction(MotorAction::kStop);
      interfaces_[1].hardware_interface->SetAction(MotorAction::kStop);
      break;
  }
}
