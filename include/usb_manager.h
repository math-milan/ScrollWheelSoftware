#ifndef USB_MANAGER_H
#define USB_MANAGER_H

#include <stdint.h>
#include <stdbool.h>


enum usb_bus_status{
    USB_MOUNTED,
    USB_NOT_MOUNTED,
    USB_SUSPENDED
};

typedef struct {
    int bus_status;
    bool remote_wakeup_en;

    uint8_t hid_desc_id;
    uint8_t hid_resolution_multipliere;

    uint8_t vendor_data_in[8];
    uint8_t vendor_data_out_buffer[8];
    bool vendor_data_out_ready_for_transfer;

    // usb desc 
    int usb_hid_desc_length;
    uint8_t *usb_hid_desc;

    bool (*ready_for_hid_report) (void);
    int (*as5600_get_delta) (void);
} usb_manager_t;

usb_manager_t* get_usb_manager(void);
#endif // USB_MANAGER_H
