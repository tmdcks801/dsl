#ifndef OBSTACLE_SENSOR_CONTROLLER_H
#define OBSTACLE_SENSOR_CONTROLLER_H

#include <sensor/sensor_controller.h>

#include <array>
#include <limits>

class ObstacleSensorController : public SensorController {
 public:
  explicit ObstacleSensorController(
      Observer* observer,
      std::unique_ptr<InterfaceEntry<SensorInterface>[]> interfaces,
      const std::string &file_path_prefix)
      : SensorController(observer, std::move(interfaces),
                         kMaxSensor,
                         file_path_prefix, sensor_events_) {}

 private:
  [[nodiscard]] virtual bool CheckValue(int raw_data) const noexcept override {
    return raw_data < kObstacleThreshold;
  }

  static constexpr int kObstacleThreshold = 0x3FFF'FFFF;
  static constexpr std::size_t kMaxSensor = 3;

  static constexpr std::array<Event, kMaxSensor> sensor_events_ = {
      Event::kFrontObstacle,
      Event::kRightObstacle,
      Event::kLeftObstacle,
  };

};

#endif
