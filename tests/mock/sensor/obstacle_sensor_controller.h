#ifndef MOCK_OBSTACLE_SENSOR_CONTROLLER_H
#define MOCK_OBSTACLE_SENSOR_CONTROLLER_H

#include <gmock/gmock.h>
#include <mock/sensor/sensor_interface.h>
#include <sensor/obstacle_sensor_controller.h>

class MockObstacleSensorController : public ObstacleSensorController {
 public:
  MockObstacleSensorController()
      : ObstacleSensorController(nullptr, CreateMockInterfaces(),
                                 "OBSTACLE_T") {}

  MOCK_METHOD(void, CheckSensor, (), (const, override, noexcept));
  MOCK_METHOD(void, AddObserver, (Observer * observer), (override));

 private:
  static std::unique_ptr<InterfaceEntry<SensorInterface>[]>
  CreateMockInterfaces() {
    auto interfaces = std::make_unique<InterfaceEntry<SensorInterface>[]>(3);

    interfaces[0].hardware_interface = std::make_unique<MockSensorInterface>();

    interfaces[0].event = Event::kFrontObstacle;

    interfaces[1].hardware_interface = std::make_unique<MockSensorInterface>();

    interfaces[1].event = Event::kRightObstacle;

    interfaces[2].hardware_interface = std::make_unique<MockSensorInterface>();

    interfaces[2].event = Event::kLeftObstacle;

    return interfaces;
  }
};

#endif
