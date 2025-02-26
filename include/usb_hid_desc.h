
#ifndef DESC_HID_H
#define DESC_HID_H
#include <stdint.h>


enum REPORT_TYPE{
    REPORT_FEATURE = 1,
    REPORT_RESOLUTION_MULTIPLIER = 2,
    REPORT_VENDOR1 = 3
};

extern const uint8_t desc_hid_report_1[];
extern const uint16_t desc_hid_report_1_size;
#endif