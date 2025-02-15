#include "PositionSensor.h"

namespace sensors{

    int16_t PositionSensor::map(int16_t input){
        return input;
        // return (float)((input) * ((float)resolution / 4096.0f)); // 12 bit resolution;
    }

    PositionSensor::PositionSensor() : as5600(I2C_INST) {
        current = 0;
        last = 0;
        sleep_ms(50);
        for (int i = 0; i < 2; i++)
        {
            acquire_data();
            sleep_ms(10);
        }
    }

    PositionSensor::~PositionSensor(){
        // delete as5600;
    }

    bool PositionSensor::acquire_data(){
        int16_t new_angel = as5600.getAngel();
        if (new_angel == -1){
            return false;
        }
        last = current;
        current = map(new_angel);
        return true;
    }

    int16_t PositionSensor::getDelta(){
        int delta = current - last;
        if (delta > resolution / 2) {
            delta -= resolution;
        } else if (delta < - resolution / 2) {
            delta += resolution;
        }
        return delta;
        // return current; 
    }
}