#include <gmock/gmock.h>
#include <system/SensorObserver.h>

class MockSensorObserver : public SensorObserver {
    public:
        MOCK_METHOD(Event, notify, (), ());
        MOCK_METHOD(void, set_observer, (), ());
}