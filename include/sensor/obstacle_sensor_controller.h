#ifndef OBSTACLE_SENSOR_CONTROLLER_H
#define OBSTACLE_SENSOR_CONTROLLER_H

#include <sensor/sensor_controller.h>

#include <array>
#include <limits>

class ObstacleSensorController : public SensorController {
 public:
  explicit ObstacleSensorController(std::unique_ptr<SensorObserver> observer,
                                    const std::string &file_path_prefix)
      : SensorController(kMaxSensor, std::move(observer),
                         std::make_unique<SensorEntry[]>(kMaxSensor),
                         file_path_prefix, sensor_events_) {}

 private:
  [[nodiscard]] bool CheckValue(int raw_data) const noexcept override {
    return raw_data < kObstacleThreshold;
  }

  static constexpr int kObstacleThreshold = std::numeric_limits<int>::max() / 2;
  static constexpr int kMaxSensor = 3;

  static constexpr std::array<Event, kMaxSensor> sensor_events_ = {
      Event::kFrontObstacle,
      Event::kRightObstacle,
      Event::kLeftObstacle,
  };

};

#endif
