#include <actuator/drive_controller.h>
#include <types/motor_action.h>

#include <variant>

void DriveController::SetOperation(ActuatorOperation operation) {
  auto operation_value = std::get<DriveOperation>(operation);

  ActuatorStatus status;
  switch (operation_value) {
    case DriveOperation::kBackward:
      status = Drive(MotorAction::kBackward, MotorAction::kBackward);
      break;

    case DriveOperation::kForward:
      status = Drive(MotorAction::kForward, MotorAction::kForward);
      break;

    case DriveOperation::kLeft:
      status = Drive(MotorAction::kBackward, MotorAction::kForward);
      break;

    case DriveOperation::kRight:
      status = Drive(MotorAction::kForward, MotorAction::kBackward);
      break;

    case DriveOperation::kStop:
      status = Drive(MotorAction::kStop, MotorAction::kStop);
      break;

    default:
      status = ActuatorStatus::kBad;
  }

  if (status == ActuatorStatus::kBad) {
    observer_->Notify(Event::kHWFault);
  }
}
ActuatorStatus DriveController::Drive(MotorAction left, MotorAction right) {
  ActuatorStatus left_status =
      interfaces_[0].hardware_interface->SetAction(left);

  ActuatorStatus right_status =
      interfaces_[1].hardware_interface->SetAction(right);

  ActuatorStatus motor_status = ActuatorStatus::kFine;
  if (left_status == ActuatorStatus::kBad ||
      right_status == ActuatorStatus::kBad) {
    motor_status = ActuatorStatus::kBad;
  }

  return motor_status;
}
