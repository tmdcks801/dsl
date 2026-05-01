#ifndef SENSOR_INTERFACE_H
#define SENSOR_INTERFACE_H

enum class Event;

class SensorInterface{
    private:
        Event sensorEvent;
    protected:
        int sensorId;   //// 먼지 : 3, 장애물 좌 0, 앞 :1, 우  2
    public :
        SensorInterface(int sensorId, Event sensorEvent)
            : sensorId(sensorId), sensorEvent(sensorEvent) {}
        // 실제 생성 SensorInterface frontSensor = SensorInterface(1, FrontObtacle);
        virtual ~SensorInterface() = default;
        virtual int readSensor() =0;
        int getId(){
            return sensorId;
        }
};
#endif
