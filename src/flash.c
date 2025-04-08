#include "flash.h"

bool storage_get_usb_data(storage_usb_t *storage){
    const uint8_t length = 2;
    uint8_t buffer[length + 1];

    // if (at24c02c_readSequential(I2C_INST_EEPROM, EEPROM_ADDRS, USB_HID_MODE, buffer, length) != length){
    //     return false;
    // }

    storage->usb_hid_desc_id = buffer[0];
    storage->usb_hid_resolution_multiplier = buffer[1];

    return true;
}

bool storage_get_led_data(storage_led_t *storage_led)
{
    uint8_t dst[5];

    // if (at24c02c_readSequential(I2C_INST_EEPROM, EEPROM_ADDRS, LED_MODE, dst, 3) != 3){
    //     return false;
    // }

    storage_led->mode = dst[0];
    storage_led->speed = dst[1];
    storage_led->length = dst[2];

    if (storage_led->length == PICO_ERROR_GENERIC){
        storage_led->length = 0;
        return false;
    }

    if (storage_led->length == 0){
        storage_led->length = 0;
        return true;
    }

    storage_led->buffer = (uint8_t *)malloc(sizeof(uint8_t) * storage_led->length);
 
    if (storage_led->buffer == NULL){
        storage_led->length = 0;
        return false;
    }

    // return at24c02c_readSequential(I2C_INST_EEPROM, EEPROM_ADDRS, LED_BUFFER_LENGTH + 1, storage_led->buffer, storage_led->length) == storage_led->length;
}

bool storage_write_usb_data(storage_usb_t *storage_usb){
    bool sucssess;

    // sucssess = at24c02c_writeByte(I2C_INST_EEPROM, EEPROM_ADDRS, USB_HID_MODE, storage_usb->usb_hid_desc_id);
    // sucssess = at24c02c_writeByte(I2C_INST_EEPROM, EEPROM_ADDRS, USB_RESOLUTION_MULT, storage_usb->usb_hid_resolution_multiplier);

    return sucssess; 
}

bool storage_write_led_data(storage_led_t * storage_led){
    const uint8_t buffer_offset = 3;
    bool sucssess;
    uint8_t *src = (uint8_t *)malloc(sizeof(uint8_t) * (3 + storage_led->length));

    if (src == NULL){
        return false;
    }

    src[0] = storage_led->mode;
    src[1] = storage_led->speed;
    src[2] = storage_led->length;

    for (uint8_t i = 0; i < storage_led->length; i++)
    {
        src[buffer_offset + i] = storage_led->buffer[i];
    }

    uint8_t length;
    uint8_t index = 0;

    do { /// @todo this is still shit
        length = storage_led->length < (buffer_offset + index) % 8 ? storage_led->length - index : (buffer_offset + index) % 8;

        // sucssess = at24c02c_writePage(I2C_INST_EEPROM, EEPROM_ADDRS, index, storage_led->buffer, length);

        index += length;
    } while (index < storage_led->length);
    
    return sucssess; 
}

bool storage_check_check_value(){
    int16_t value =  0;//at24c02c_readRandom(I2C_INST_EEPROM, EEPROM_ADDRS, STATUS_FIELD);

    if (value == PICO_ERROR_GENERIC){
        return false;
    }

    return value == EEPROM_CHECK_VALUE;
}

bool storage_write_check_value(){
    return false;
    // return at24c02c_writeByte(I2C_INST_EEPROM, EEPROM_ADDRS, STATUS_FIELD, EEPROM_CHECK_VALUE);
}

bool storage_write_defualt_config(){
    bool sucssess;
    
    sucssess = storage_write_check_value();
    
    storage_usb_t usb_data = {
        .usb_hid_desc_id = 0,
        .usb_hid_resolution_multiplier = 15
    };

    sucssess = storage_write_usb_data(&usb_data);

    storage_led_t led_data = {
        .mode = 0,
        .speed = 0,
        .length = 3,
        .buffer = (uint8_t *)malloc(sizeof(uint8_t) * 3)
    };

    led_data.buffer[0] = 100;   // R
    led_data.buffer[1] = 0;     // G
    led_data.buffer[2] = 0;     // B

    sucssess = storage_write_led_data(&led_data);

    free(led_data.buffer);

    return sucssess;
}

bool storage_init(){
    i2c_init(I2C_INST_EEPROM, 1000 * 100);
    gpio_set_function(SDA_EEPROM, GPIO_FUNC_I2C);
    gipi_set_function(SCL_EEPROM, GPIO_FUNC_I2C);

    if (!storage_check_check_value()){
        storage_write_defualt_config();
    }


    return false;
}
