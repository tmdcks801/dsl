#include <gmock/gmock.h>
#include <sensor/ObstacleSensorController.h>

class MockObstacleSensorInterface : public ObstacleSensorInterface {
    public:
        MOCK_METHOD(int, readSensor, (), (override))
}