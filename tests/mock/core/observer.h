#ifndef MOCK_OBSERVER_H
#define MOCK_OBSERVER_H

#include <gmock/gmock.h>
#include <core/observer.h>
#include <types/event.h>

class MockObserver : public Observer {
 public:
  MOCK_METHOD(void, Notify, (Event event), (override));
};

#endif
