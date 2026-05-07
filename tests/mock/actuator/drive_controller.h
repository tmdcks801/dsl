#ifndef MOCK_DRIVE_CONTROLLER_H
#define MOCK_DRIVE_CONTROLLER_H

#include <gmock/gmock.h>
#include <actuator/drive_controller.h>
#include <mock/actuator/actuator_interface.h>

class MockDriveController : public DriveController {
 public:
  MockDriveController()
      : DriveController(nullptr, CreateMockInterfaces(), "DRIVE_T") {}
  MOCK_METHOD(void, SetOperation, (ActuatorOperation operation), (override));

  private:
  static std::unique_ptr<InterfaceEntry<ActuatorInterface>[]>
  CreateMockInterfaces() {
    auto interfaces = std::make_unique<InterfaceEntry<ActuatorInterface>[]>(2);

    interfaces[0].hardware_interface =
        std::make_unique<MockActuatorInterface>();

    interfaces[0].event = Event::kLMotorFault;

    interfaces[1].hardware_interface =
        std::make_unique<MockActuatorInterface>();

    interfaces[1].event = Event::kRMotorFault;

    return interfaces;
  }
};

#endif
