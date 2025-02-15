#include "common.h"
#include "usb_callbacks.h"
#include "PositionSensor.h"
#include "LedRing.h"

// uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;
USBData usb_data;
int main(void){
    stdio_init_all();
    usb_init(0x00);

    i2c_init(i2c0, 100000);  // Example for 100kHz
    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C);

    i2c_init(i2c1, 100000);  // Example for 100kHz
    gpio_set_function(SDA_EEPROM, GPIO_FUNC_I2C);
    gpio_set_function(SCL_EEPROM, GPIO_FUNC_I2C);
    
    ledring::LEDRing leds = ledring::LEDRing();
    sensors::PositionSensor position_sensor = sensors::PositionSensor();

    while(1){
        position_sensor.acquire_data();
        usb_data.scroll_delta = position_sensor.getDelta();

        usb_update();
        // sleep_ms(5);
    }
};