#include "common.h"
#include "status_led.h"
#include "usb_callbacks.h"
#include "PositionSensor.h"


// uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;
USBData usb_data;
led::LED *statusLED;
int main(void){
    stdio_init_all();
    usb_init(0x00);

    i2c_init(i2c0, 100000);  // Example for 100kHz
    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C);


    statusLED = new led::LED();

    statusLED->setStatus(led::ok);
    statusLED->update();

    sensors::PositionSensor position_sensor = sensors::PositionSensor();


    while(1){   
        position_sensor.acquire_data();
        usb_data.scroll_delta = position_sensor.getDelta();

        usb_update();
        sleep_ms(5);
    }
};
