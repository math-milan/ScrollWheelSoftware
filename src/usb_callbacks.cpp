#include "usb_callbacks.h"

void usb_init(){
    tusb_init();
}

void usb_update(){
    tud_task(); // tinyusb device task
    hid_task();
}
//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

static void send_hid_report(uint8_t report_id)
{
  // skip if hid is not ready yet
  if ( !tud_hid_ready() ) return;

  switch(report_id)
  {
    case REPORT_INPUT:{
      char report[] = {0, 0, 0, (char)scroll_delta, 0};
    

      tud_hid_report(REPORT_INPUT, (const void*)&report, sizeof(report));

      if (scroll_delta != 0){
        scroll_delta = 0;
      }
    }
    break;
    case REPORT_FEATURE:{
      char buffer[] = {0x03};
      tud_hid_report(REPORT_FEATURE, (const void *)&buffer, sizeof(buffer));
      break;
    }
    default: break;
  }
}

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
void hid_task(void)
{
  // Poll every 10ms
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  if ( board_millis() - start_ms < interval_ms) return; // not enough time
  start_ms += interval_ms;

  uint32_t const btn = 0; //board_button_read();

  // Remote wakeup
  if ( tud_suspended() )
  {
    // Wake up host if we are in suspend mode
    // and REMOTE_WAKEUP feature is enabled by host
    tud_remote_wakeup();
  }else
  {
    // Send the 1st of report chain, the rest will be sent by tud_hid_report_complete_cb()
    send_hid_report(REPORT_INPUT);
  }
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint8_t len)
{

}

// Invoked when received GET_REPORT control requbuttonest
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  blink_interval_ms = BLINK_NOT_MOUNTED;
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  if (report_type == HID_REPORT_TYPE_FEATURE){
    buffer[0] = 0x03;
    return 1;
  }

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT)
  {
    // // Set keyboard LED e.g Capslock, Numlock etc...
    // if (report_id == REPORT_ID_KEYBOARD)
    // {
    //   // bufsize should be (at least) 1
    //   if ( bufsize < 1 ) return;

    //   uint8_t const kbd_leds = buffer[0];

    //   if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
    //   {
    //     // Capslock On: disable blink, turn led on
    //     blink_interval_ms = 0;
    //     // board_led_write(true);
    //   }else
    //   {
    //     // Caplocks Off: back to normal blink
    //     // board_led_write(false);
    //     // blink_interval_ms = BLINK_MOUNTED;
    //   }
    // }
  }
}