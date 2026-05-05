#include <sensor/sensor_controller.h>
#include <sensor/sensor_interface.h>

void SensorController::CheckSensor() const noexcept {
  for (int i = 0; i < interface_count_; ++i) {
    const auto& [sensor_interface, event] = interfaces_[i];

    auto sensor = static_cast<SensorInterface*>(sensor_interface.get());

    if (!sensor) {
      break;
    }

    auto raw_data = sensor->ReadSensor();

    if (!raw_data || *raw_data < 0) {
      observer_->Notify(kHWError);
      break;
    }

    if (CheckValue(*raw_data)) {
      observer_->Notify(event);
    }
  }
}
