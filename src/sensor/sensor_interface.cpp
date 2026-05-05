#include <sensor/sensor_interface.h>

[[nodiscard]] std::optional<int> SensorInterface::ReadSensor() noexcept {
  int raw_data;

  struct stat st;
  if (file_system_->fstat(device_id_, &st) == -1 || st.st_size != sizeof(int)) {
    return std::nullopt;
  }

  auto bytes_read =
      file_system_->pread(device_id_, &raw_data, sizeof(int), kSensorValuePos);

  if (bytes_read < static_cast<ssize_t>(sizeof(int))) {
    return std::nullopt;
  }

  return raw_data;
}
