#include <sensor/sensor_controller.h>
#include <sensor/sensor_interface.h>

void SensorController::CheckSensor() const noexcept {
  for (int i = 0; i < interface_count_; ++i) {
    const auto& [sensor_interface, event] = interfaces_[i];

    if (sensor_interface == nullptr) {
      break;
    }

    auto raw_data = sensor_interface->ReadSensor();

    if (!raw_data.has_value() || *raw_data < 0) {
      observer_->Notify(kHWError);
      break;
    }

    if (CheckValue(*raw_data)) {
      observer_->Notify(event);
    }
  }
}
