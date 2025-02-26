#include "flash.h"

bool storage_get_usb_data(storage_usb_t *storage){
    const uint8_t length = 2;
    uint8_t buffer[length + 1];

    if (at24c02c_readSequential(I2C_INST_EEPROM, EEPROM_ADDRS, USB_HID, buffer, length) != length){
        return false;
    }

    storage->usb_hid_desc_id = buffer[0];
    storage->usb_hid_resolution_multiplier = buffer[1];

    return true;
}