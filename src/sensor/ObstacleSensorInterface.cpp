#include <sensor/ObstacleSensorInterface.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int ObstacleSensorInterface::readSensor(){
    std::ifstream file("obstacle.txt");
    std::string line;

    if (file.is_open() && std::getline(file, line)) {
        try {
            std::stringstream ss(line);
            std::string token;
            int currentColumn = 0;
            while (ss >> token){
                if (currentColumn == this->sensorId){
                    try{
                        int obstacleLevel = std::stoi(token);
                        return obstacleLevel;
                    }catch(const std::exception& e){
                        return -1;                        //에러
                    }
                }
                currentColumn++;
            }
        

            return -1; 
        }
        catch (const std::exception& e) { 
            return -1; 
        }
    
    }else {
        //에러
         return -1; 
    }
}