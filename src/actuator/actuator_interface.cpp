#include <actuator/actuator_interface.h>
#include <variant>

ActuatorStatus ActuatorInterface::GetStatus() {
  int raw_status;

  auto bytes_read = file_system_->pread(device_id_, &raw_status, sizeof(int),
                                        kActuatorStatusPos);

  if (bytes_read != sizeof(int)) {
    return ActuatorStatus::kBad;
  }

  return (raw_status == 1) ? ActuatorStatus::kFine : ActuatorStatus::kBad;
}

ActuatorStatus ActuatorInterface::SetAction(ActuatorAction action) {
  int raw_action = std::visit(
      [](auto&& arg) -> int { return static_cast<int>(arg); }, action);

  auto bytes_write = file_system_->pwrite(device_id_, &raw_action, sizeof(int),
                                          kActuatorActionPos);

  int raw_status = 1;
  ActuatorStatus actuator_status = ActuatorStatus::kFine;

  if (bytes_write != sizeof(int)) {
    raw_status = 0;
    actuator_status = ActuatorStatus::kBad;
  }

  file_system_->pwrite(device_id_, &raw_status, sizeof(int),
                                     kActuatorStatusPos);

  return actuator_status;
}
