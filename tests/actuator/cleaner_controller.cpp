#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <actuator/cleaner_controller.h>
#include <core/observer.h>
#include <types/event.h>

#include <mock/core/observer.h>
#include <mock/actuator/actuator_interface.h>

#include <memory>
#include <vector>

using ::testing::_;
using ::testing::Eq;
using ::testing::Return;


class CleanerControllerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    auto mock_observer_ptr = std::make_unique<MockObserver>();
    mock_observer = mock_observer_ptr.get();

    auto mock_interface_ptr = std::make_unique<MockActuatorInterface>();
    mock_interface = mock_interface_ptr.get();

    auto interfaces = std::make_unique<InterfaceEntry<ActuatorInterface>[]>(1);
    interfaces[0].hardware_interface = std::move(mock_interface_ptr);
    interfaces[0].event = Event::kHWFault;

    std::vector<Event> events = {Event::kHWFault};

    controller = std::make_unique<CleanerController>(
        std::move(mock_observer_ptr), 
        std::move(interfaces), 
        1, 
        "mock_cleaner_dev", 
        events);
  }

  MockObserver* mock_observer;
  MockActuatorInterface* mock_interface;
  
  std::unique_ptr<CleanerController> controller;
};

//클리너 정상 작동
TEST_F(CleanerControllerTest, SetHappyPath) {
  ActuatorAction expected_action = static_cast<int>(CleanerAction::kOn);

  EXPECT_CALL(*mock_interface, SetAction(Eq(expected_action)))
      .WillOnce(Return(ActuatorStatus::kFine));

  EXPECT_CALL(*mock_observer, Notify(_)).Times(0);

  controller->SetOperation(CleanerAction::kOn);
}

//클리너 오작동
TEST_F(CleanerControllerTest, SetSadPath) {
  ActuatorAction expected_action = static_cast<int>(CleanerAction::kPowerUp);

  EXPECT_CALL(*mock_interface, SetAction(Eq(expected_action)))
      .WillOnce(Return(ActuatorStatus::kBad));

  EXPECT_CALL(*mock_observer, Notify(Eq(Event::kHWFault))).Times(1);

  controller->SetOperation(CleanerAction::kPowerUp);
}