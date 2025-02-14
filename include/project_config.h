#ifndef __PROJECT_CONFIG_H__
#define __PROJECT_CONFIG_H__

/* Define your project pins etc here */

#define STATUS_LED_PIN 12
#define NUMBER_OF_LEDS 18

#define I2C_INST i2c0
#define SDA 8
#define SCL 9

#define I2C_EEPROM_FREQUENC 1000 * 100
#define I2C_INST_EEPROM i2c1
#define SDA_EEPROM 11
#define SCL_EEPROM 12
#define EEPROM_ADDRS 12
#endif // __PROJECT_CONFIG_H__