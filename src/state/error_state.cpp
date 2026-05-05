#include <state/error_state.h>

#include <iostream>

std::unique_ptr<RvcState> ErrorState::Handle(Event event) {
  if (event == Event::kTimerExpired) {
    observer_->Notify(Event::kSystemShutdown);
  }

  return nullptr;
}

void ErrorState::Enter() {
  EmergencyStop();
  DisplayFatalError();

  timer_ = kDisplayErrorDuration;
}

void ErrorState::Exit() { WriteErrorLog(); }

void ErrorState::EmergencyStop() {
  drive_controller_->SetOperation(DriveOperation::kStop);
  cleaner_controller_->SetOperation(CleanerAction::kOff);

  std::cout << "[System] Emergency Stop: All actuators powered down."
            << std::endl;
}

void ErrorState::DisplayFatalError() {
  std::cout << "****************************************" << std::endl;
  std::cout << "*          FATAL SYSTEM ERROR          *" << std::endl;
  std::cout << "*       RECOVERY IS NOT POSSIBLE       *" << std::endl;
  std::cout << "*    SYSTEM WILL SHUTDOWN SHORTLY      *" << std::endl;
  std::cout << "****************************************" << std::endl;
}

void ErrorState::WriteErrorLog() {
  std::cout << "[LOG] Writing fatal error log to persistent storage..."
            << std::endl;
  std::cout << "[LOG] Error Code: 0xDEADBEEF" << std::endl;
  std::cout << "[LOG] State: ErrorState transition complete." << std::endl;
}
