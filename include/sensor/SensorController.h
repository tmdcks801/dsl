#ifndef SENSOR_CONTROLLER_H
#define SENSOR_CONTROLLER_H

class SensorObserver;

class SensorController{
    private: 
        SensorObserver * observer;
    public:
        virtual ~SensorController() = default;
        virtual void checkSensor() = 0;
};
#endif