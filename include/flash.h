#include "common.h"
#include "at24c02c.h"

#ifndef FLASH_H
#define FLASH_H

enum storage_Address{
    USB_HID = 0
};

typedef struct {
    uint8_t usb_hid_desc_id;
    uint8_t usb_hid_resolution_multiplier;
} storage_usb_t;

/*
# Memory Addressing
Address Use
0x00    usb hid desc id
0x01    usb hid resolution multiplier     
*/

bool storage_get_usb_data(storage_usb_t *storage_usb);

#endif