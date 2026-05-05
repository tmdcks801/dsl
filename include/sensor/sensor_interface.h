#ifndef SENSOR_INTERFACE_H
#define SENSOR_INTERFACE_H

#include <hardware_interface.h>

#include <optional>

class SensorInterface : public HardwareInterface {
 public:
  using HardwareInterface::HardwareInterface;

  virtual ~SensorInterface() = default;

  [[nodiscard]] virtual std::optional<int> ReadSensor() noexcept;

 private:
  static constexpr int kSensorValuePos = 0;
};

#endif
