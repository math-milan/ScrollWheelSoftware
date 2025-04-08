#include "common.h"
#include "stdlib.h"
// #include "at24c02c.h"

#ifndef FLASH_H
#define FLASH_H

#define EEPROM_CHECK_VALUE 0xFF

enum storage_Address{
    USB_HID_MODE        = 0x0,
    USB_RESOLUTION_MULT = 0x1,
    LED_MODE            = 0x8,
    LED_SPEED           = 0x9,
    LED_BUFFER_LENGTH   = 0xA,
    STATUS_FIELD        = 0x20
};

typedef struct {
    uint8_t usb_hid_desc_id;
    uint8_t usb_hid_resolution_multiplier;
} storage_usb_t;


typedef struct {
    uint8_t mode;
    uint8_t speed;
    uint8_t length;
    uint8_t *buffer;
} storage_led_t;



bool storage_read_usb_data(storage_usb_t *storage_usb);
/// @warning claims memory from heap that must be freed. If the length is 0 no memory was claimed
bool storage_read_led_data(storage_led_t *storage_led);

bool storage_write_usb_data(storage_usb_t *storage_usb);
bool storage_write_led_data(storage_led_t *storage_led);

bool storage_check_check_value();
bool storage_write_check_value();
bool storage_write_defualt_config();
bool storage_init();

#endif