#include <sensor/sensor_controller.h>
#include <sensor/sensor_interface.h>

void SensorController::CheckSensor() const noexcept {
  for (int i = 0; i < interface_count_; ++i) {
    const auto& [sensor_interface, event] = interfaces_[i];

    if (sensor_interface == nullptr) {
      break;
    }

    auto raw_data = sensor_interface->ReadSensor();

    if (!raw_data.has_value()){
      observer_->Notify(Event::kDataSizeFault);
      break;
    }


    if (*raw_data < 0) {
      observer_->Notify(Event::kDataMinusFault);
      break;
    }

    if (CheckValue(*raw_data)) {
      observer_->Notify(event);
    }
  }
}
