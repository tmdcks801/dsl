#include <state/error_state.h>
#include <state/rvc_state.h>
#include <iostream>

bool RvcState::isHardwareFault(Event event) {
  return (event & kFaultMask) == kFaultMask;
}

std::unique_ptr<RvcState> RvcState::Handle(Event event) {
  if (isHardwareFault(event)) {
    return std::make_unique<ErrorState>(observer_, drive_controller_,
                                        cleaner_controller_, event);
  }

  return nullptr;
}

void RvcState::Tick(Seconds tick_interval) {
  timer_ -= tick_interval;
  if (timer_ <= 0s) {
    observer_->Notify(Event::kTimerExpired);
  }
}
