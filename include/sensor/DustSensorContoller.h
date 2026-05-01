#ifndef DUST_SENSOR_CONTROLLER_H
#define DUST_SENSOR_CONTROLLER_H

#include <sensor/SensorController.h>
#include <sensor/DustSensorInterface.h>
#include <array>

class DustSensorContoller : public SensorController{
    private :
        const int DUST_THRESHOLD;
        std :: array<DustSensorInterface*,1> sensors;
    public :
        DustSensorContoller(DustSensorInterface *sensor, const int threshold) : DUST_THRESHOLD(threshold) {
            sensor[0] = sensor;
        }
        
        void checkSensor() override;
};

#endif