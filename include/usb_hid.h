#ifndef USB_HID_H
#define USB_HID_H

#include <stdint.h>
#include <stdbool.h>

#include "bsp/board.h"
#include "tusb.h"

#include "usb_hid_desc.h"
#include "usb_manager.h"
#include <stdlib.h>
#include "flash.h"

void usb_hid_init(bool (*ready_for_hid_report)(void), int (*as5600_get_delta)(void));
void usb_hid_update(void);

#endif