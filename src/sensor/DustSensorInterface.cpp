#include <sensor/DustSensorInterface.h>
#include <iostream>
#include <fstream>
#include <string>
int DustSensorInterface::readSensor(){

    std::ifstream file("dust.txt");
    std::string line;

    if (file.is_open() && std::getline(file, line)) {
        try {
            int dustLevel = std::stoi(line); 
            return dustLevel;
        } 
        catch (const std::exception& e) {
            //에러
            return -1; 
        }
    }else {
         return -1; //에러
    }

}
