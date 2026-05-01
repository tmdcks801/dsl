#include <gmock/gmock.h>
#include <sensor/DustSensorController.h>

class MockDustSensorInterface : public MockDustSensorInterface {
    public:
        MOCK_METHOD(int, readSensor(), (), (override))
}