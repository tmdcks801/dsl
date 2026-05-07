#ifndef MOCK_CLEANER_CONTROLLER_H
#define MOCK_CLEANER_CONTROLLER_H

#include <actuator/cleaner_controller.h>
#include <gmock/gmock.h>
#include <mock/actuator/actuator_interface.h>

class MockCleanerController : public CleanerController {
 public:
  MockCleanerController()
      : CleanerController(
            nullptr, CreateMockInterfaces(),
            "CLEANER_T") {}
  MOCK_METHOD(void, SetOperation, (ActuatorOperation operation), (override));

  private:
  static std::unique_ptr<InterfaceEntry<ActuatorInterface>[]>
      CreateMockInterfaces() {
     auto interfaces = std::make_unique<InterfaceEntry<ActuatorInterface>[]>(1);

     interfaces[0].hardware_interface =
         std::make_unique<MockActuatorInterface>();

     interfaces[0].event = Event::kCleanerFault;

     return interfaces;
  }
};

#endif
