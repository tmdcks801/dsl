#ifndef MOCK_RVC_STATE_H
#define MOCK_RVC_STATE_H

#include <gmock/gmock.h>
#include <state/rvc_state.h>

class MockRvcState : public RvcState {
 public:
  using RvcState::RvcState;

  MOCK_METHOD(std::unique_ptr<RvcState>, Handle, (Event event), (override));
  MOCK_METHOD(void, Enter, (), (override));
  MOCK_METHOD(void, Exit, (), (override));
  MOCK_METHOD(void, Tick, (Seconds tick_interval), (override));
};

#endif
