#ifndef SENSOR_CONTROLLER_H
#define SENSOR_CONTROLLER_H

#include <hardware_controller.h>
#include <sensor/sensor_interface.h>
#include <types/event.h>

class SensorController : public HardwareController<SensorInterface> {
 public:
  using HardwareController<SensorInterface>::HardwareController;

  virtual ~SensorController() = default;

  void CheckSensor() const noexcept;

 protected:
  [[nodiscard]] virtual bool CheckValue(int raw_data) const noexcept = 0;
};

#endif
