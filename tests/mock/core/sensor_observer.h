#include <gmock/gmock.h>
#include <core/sensor_observer.h>

class MockSensorObserver : public SensorObserver {
    public:
        MOCK_METHOD(Event, notify, (), ()); 
        MOCK_METHOD(void, set_observer, (), ());
}
