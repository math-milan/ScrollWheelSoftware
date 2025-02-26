#include "usb_manager.h"

static usb_manager_t manager = {
    .bus_status = USB_NOT_MOUNTED,
    .remote_wakeup_en = false,
    .hid_desc_id = 0,
    .hid_resolution_multipliere = 0,
    .vendor_data_in = {0},
    .vendor_data_out_buffer = {0},
    .vendor_data_out_ready_for_transfer = false,
    .usb_hid_desc_length = 0,
    .usb_hid_desc = 0,
    .ready_for_hid_report = false
};

usb_manager_t* get_usb_manager(void){
    return &manager;
}