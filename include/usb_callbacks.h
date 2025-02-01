#ifndef USB_CALLBACKS_H
#define USB_CALLBACKS_H

#include <stdint.h>
#include <stdbool.h>

#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"
#include "status_led.h"

#ifdef __cplusplus
extern "C" {
#endif

// Blink intervals
#define BLINK_NOT_MOUNTED 1000
#define BLINK_MOUNTED 500
#define BLINK_SUSPENDED 250

#define REPORT_INPUT 1
#define REPORT_FEATURE 2


extern uint32_t blink_interval_ms;
extern volatile int scroll_delta;

/// Standard HID Boot Protocol Mouse Report.
typedef struct TU_ATTR_PACKED
{
//   uint8_t resolution_multiplier; /**< buttons mask for currently pressed buttons in the mouse. */
  int8_t  wheel;   /**< Current delta wheel movement on the mouse. */
} hid_wheel_report_t;

void usb_init(void);
void usb_update(void);

// Device callbacks
void tud_mount_cb(void);
void tud_umount_cb(void);
void tud_suspend_cb(bool remote_wakeup_en);
void tud_resume_cb(void);

// HID functions
void hid_task(void);
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint8_t len);
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen);
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize);

// LED blinking task
void led_blinking_task(void);


#ifdef __cplusplus
}
#endif

#endif // USB_CALLBACKS_H
