#include <state/rvc_state.h>

void RvcState::Tick(Seconds tick_interval) {
  timer_ -= tick_interval;
  if (timer_ <= 0s) {
    observer_->Notify(Event::kTimerExpired);
  }
}
