#ifndef USB_MANAGER_H
#define USB_MANAGER_H

#include <stdint.h>

typedef struct {
    uint8_t hid_desc_id;
    uint8_t hid_resolution_multipliere;

    uint8_t vendor_data_in[8];
    uint8_t vendor_data_out[8];

} usb_manager_t;

usb_manager_t manager;

usb_manager_t* get_usb_manager(void);
#endif // USB_MANAGER_H
