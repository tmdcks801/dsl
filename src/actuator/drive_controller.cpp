#include <actuator/drive_controller.h>
#include <types/motor_action.h>

#include <variant>

void DriveController::SetOperation(ActuatorOperation operation) {
  auto operation_value = std::get<DriveOperation>(operation);

  ActuatorStatus status;
  switch (operation_value) {
    case DriveOperation::kBackward:
      Drive(MotorAction::kBackward, MotorAction::kBackward);
      break;

    case DriveOperation::kForward:
      Drive(MotorAction::kForward, MotorAction::kForward);
      break;

    case DriveOperation::kLeft:
      Drive(MotorAction::kBackward, MotorAction::kForward);
      break;

    case DriveOperation::kRight:
      Drive(MotorAction::kForward, MotorAction::kBackward);
      break;

    case DriveOperation::kStop:
      Drive(MotorAction::kStop, MotorAction::kStop);
      break;
  }
}
void DriveController::Drive(MotorAction left, MotorAction right) {
  ActuatorStatus left_status =
      interfaces_[0].hardware_interface->SetAction(left);

  ActuatorStatus right_status =
      interfaces_[1].hardware_interface->SetAction(right);

  ActuatorStatus motor_status = ActuatorStatus::kFine;
  if (left_status == ActuatorStatus::kBad) {
    observer_->Notify(interfaces_[0].event);
  }

  if (right_status == ActuatorStatus::kBad) {
    observer_->Notify(interfaces_[1].event);
  }
}
