#include <actuator/actuator_interface.h>
#include <types/actuator_status.h>

ActuatorStatus ActuatorInterface::GetStatus() {
  int raw_status;

  struct stat st;
  if (file_system_->fstat(device_id_, &st) == -1 ||
      st.st_size != sizeof(int) * 2) {
    return ActuatorStatus::kBad;
  }

  auto bytes_read = file_system_->pread(device_id, &raw_status, sizeof(int),
                                        kActuatorStatusPos);

  if (bytes_read != static_cast<ssize_t>(sizeof(int))) {
    return ActuatorStatus::kBad;
  }

  if (raw_status == 1) {
    return ActuatorStatus::kFine;
  }
  return ActuatorStatus::kBad;
}
