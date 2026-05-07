#ifndef MOCK_DUST_SENSOR_CONTROLLER_H
#define MOCK_DUST_SENSOR_CONTROLLER_H

#include <gmock/gmock.h>
#include <sensor/dust_sensor_controller.h>
#include <mock/sensor/sensor_interface.h>

class MockDustSensorController : public DustSensorController {
 public:
  MockDustSensorController()
      : DustSensorController(nullptr, CreateMockInterfaces(), "DUST_T") {}

  MOCK_METHOD(void, CheckSensor, (), (const, override, noexcept));
  MOCK_METHOD(void, AddObserver, (Observer * observer), (override));

  private:
  static std::unique_ptr<InterfaceEntry<SensorInterface>[]>
  CreateMockInterfaces() {
     auto interfaces = std::make_unique<InterfaceEntry<SensorInterface>[]>(1);

    interfaces[0].hardware_interface =
        std::make_unique<MockSensorInterface>();

    interfaces[0].event = Event::kHighDust;

    return interfaces;
  }
};

#endif
