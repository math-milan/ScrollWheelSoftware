#include "storage.h"
#include "lib/librp2040/src/AT24C01C/at24c01c.h"


int16_t getLedDataLength()
{
    i2c_flash::at24c01c flash = i2c_flash::at24c01c(I2C_INST_EEPROM, EEPROM_ADDRS);
    uint8_t dst = 0;
    if (flash.readRandom(LED_LENGTH_ADDR, &dst) == PICO_ERROR_GENERIC){
        return PICO_ERROR_GENERIC;
    }
    return (int16_t)dst;
}

bool getLedData(uint8_t *buffer, int len)
{
    i2c_flash::at24c01c flash = i2c_flash::at24c01c(I2C_INST_EEPROM, EEPROM_ADDRS);
    bool success = flash.readSequential(LED_LENGTH_ADDR + 1, buffer, len) == len;
    return success;
}

bool writeData(uint8_t *buffer, int len){
    i2c_flash::at24c01c flash = i2c_flash::at24c01c(I2C_INST_EEPROM, EEPROM_ADDRS);
    flash.writeByte(LED_LENGTH_ADDR, (uint8_t)len);

    for (int i = 0; i < len; i++)
    {
        if (flash.writeByte(LED_LENGTH_ADDR + 1 + i, buffer[i]) == false){
            return false;
        }
    }
    return true;
}