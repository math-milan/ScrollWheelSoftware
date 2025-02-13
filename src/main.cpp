#include "common.h"
#include "status_led.h"
#include "usb_callbacks.h"
#include "PositionSensor.h"


// uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;
USBData usb_data;
led::LED *statusLED;
int main(void){
    stdio_init_all();
    usb_init(4);

    statusLED = new led::LED();
    sensors::PositionSensor position_sensor = sensors::PositionSensor();

    statusLED->setStatus(led::ok);
    statusLED->update();



    while(1){   
        position_sensor.acquire_data();
        usb_data.scroll_delta = position_sensor.getDelta();

        usb_update();
        sleep_ms(5);
    }
};
