#ifndef DUST_SENSOR_CONTROLLER_H
#define DUST_SENSOR_CONTROLLER_H

#include <sensor/sensor_controller.h>

#include <array>
#include <limits>

class DustSensorController : public SensorController {
 public:
  explicit DustSensorController(
      std::unique_ptr<Observer> observer,
      std::unique_ptr<InterfaceEntry<SensorInterface>[]> interfaces,
      const std::string &file_path_prefix)
      : SensorController(std::move(observer), std::move(interfaces), kMaxSensor,
                         file_path_prefix, sensor_events_) {}

 private:
  [[nodiscard]] bool CheckValue(int raw_data) const noexcept override {
    return raw_data > kDustThreshold;
  }

  static constexpr int kDustThreshold = std::numeric_limits<int>::max() / 2;
  static constexpr int kMaxSensor = 1;

  static constexpr std::array<Event, 1> sensor_events_ = {Event::kHighDust};
};

#endif
