#include "usb_hid.h"

/* A combination of interfaces must have a unique product id, since PC will save device driver after the first plug.
 * Same VID/PID with different interface e.g MSC (first), then CDC (later) will possibly cause system error on PC.
 *
 * Auto ProductID layout's Bitmap:
 *   [MSB]         HID | MSC | CDC          [LSB]
 */
#define _PID_MAP(itf, n)  ( (CFG_TUD_##itf) << (n) )
#define USB_PID           (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1) | _PID_MAP(HID, 2) | \
                           _PID_MAP(MIDI, 3) | _PID_MAP(VENDOR, 4) )

#define USB_VID   0xCafe
#define USB_BCD   0x0200

//--------------------------------------------------------------------+
// Device Descriptors
//--------------------------------------------------------------------+
tusb_desc_device_t const desc_device =
{
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = USB_BCD,
    .bDeviceClass       = 0x00,
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = USB_VID,
    .idProduct          = USB_PID,
    .bcdDevice          = 0x0100,

    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,

    .bNumConfigurations = 0x01
};

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
uint8_t const * tud_descriptor_device_cb(void)
{
  return (uint8_t const *) &desc_device;
}

// Invoked when received GET HID REPORT DESCRIPTOR
uint8_t const * tud_hid_descriptor_report_cb(uint8_t instance)
{
    usb_manager_t *manager = get_usb_manager();
  
    return manager->usb_hid_desc;
    // return desc_hid_report_1;
}

enum
{
  ITF_NUM_HID,
  ITF_NUM_TOTAL
};

#define  CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)

#define EPNUM_HID   0x81

uint8_t desc_configuration[] =
{
  // Config number, interface count, string index, total length, attribute, power in mA
  TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 500),

  // Interface number, string index, protocol, report descriptor len, EP In address, size & polling interval
  TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_NONE, 0, EPNUM_HID, CFG_TUD_HID_EP_BUFSIZE, 5) // the length her 0 is at 25 index
};

// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
{
    usb_manager_t *manager = get_usb_manager();

    desc_configuration[25] = manager->usb_hid_desc_length;
    return desc_configuration;
}

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+

// array of pointer to string descriptors
char const* string_desc_arr [] =
{
  (const char[]) { 0x09, 0x04 }, // 0: is supported language is English (0x0409)
  "KNOB",                        // 1: Manufacturer
  "RP2040",                      // 2: Product
  "123456",                      // 3: Serials, should use chip ID
};

static uint16_t _desc_str[32];

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
  (void) langid;

  uint8_t chr_count;

  if ( index == 0)
  {
    memcpy(&_desc_str[1], string_desc_arr[0], 2);
    chr_count = 1;
  }else
  {
    // Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
    // https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors

    if ( !(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0])) ) return NULL;

    const char* str = string_desc_arr[index];

    // Cap at max char
    chr_count = strlen(str);
    if ( chr_count > 31 ) chr_count = 31;

    // Convert ASCII string into UTF-16
    for(uint8_t i=0; i<chr_count; i++)
    {
      _desc_str[1+i] = str[i];
    }
  }

  // first byte is length (including header), second byte is string type
  _desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2*chr_count + 2);

  return _desc_str;
}

// Callbacks
// Invoked when device is mounted
void tud_mount_cb(void)
{
    usb_manager_t *manager = get_usb_manager();
    manager->bus_status = USB_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
    usb_manager_t *manager = get_usb_manager();
    manager->bus_status = USB_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
    usb_manager_t *manager = get_usb_manager();
    manager->remote_wakeup_en = remote_wakeup_en;
    manager->bus_status = USB_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
    usb_manager_t *manager = get_usb_manager();
    manager->bus_status = USB_MOUNTED;
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

static void send_hid_report(uint8_t report_id)
{
    // skip if hid is not ready yet
    if ( !tud_hid_ready() ) return;

    usb_manager_t *manager = get_usb_manager();
    
    switch(report_id)
    {
        case REPORT_FEATURE:{
            char report[] = {0, 0, 0, manager->as5600_get_delta(), 0};

            tud_hid_report(REPORT_FEATURE, (const void*)&report, sizeof(report));
        }
        break;
        case REPORT_VENDOR1:{
            tud_hid_report(REPORT_VENDOR1, manager->vendor_data_out_buffer, 8);
        }
        break;
    default: break;
  }
}

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
void hid_task(void)
{
    // Poll every 10ms
    const uint32_t interval_ms = 1;
    static uint32_t start_ms = 0;
    
    if ( board_millis() - start_ms < interval_ms) return; // not enough time
    usb_manager_t *manager = get_usb_manager();
    if ( !manager->ready_for_hid_report() ) return;         // Not ready for 
    start_ms += interval_ms;

    // Remote wakeup
    if ( tud_suspended())
    {
        // Wake up host if we are in suspend mode
        // and REMOTE_WAKEUP feature is enabled by host
        tud_remote_wakeup();
    }else
    {
        send_hid_report(REPORT_FEATURE);
        if (manager->vendor_data_out_ready_for_transfer){
            send_hid_report(REPORT_VENDOR1);
        }
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
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

    if (report_type == HID_REPORT_TYPE_FEATURE){
        // buffer[] = report_id;
        // buffer[0] = usb_data.Resolution_Multiplier;
        // statusLED->setLED(100, 0, 0);
        // return 1;
    }
    if (report_type == HID_REPORT_TYPE_INPUT){
        // statusLED->setLED(0, 0, 100);
    }

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
//   (void) instance;
//   if (report_id == 2){
//     // TODO Log what is send by the host Hardware 2.0 is needed.
//     // statusLED->setLED(0, 0, 100);
//   }
  // This might be a way to send data for users to set the color and animation of the rgb leds in Hardware 2.0
//   if (report_id == 3) // Vendor Report ID 3
//     {
//         if (bufsize == 3)
//         {
//             uint8_t r = buffer[0]; // Red
//             uint8_t g = buffer[1]; // Green
//             uint8_t b = buffer[2]; // Blue

//             // statusLED->setLED(r, g, b); // Update LED with received color
//             // watchdog_reboot(0, 0, 5); // After data is writen the device times out debuger is needed to find the issue ?
//         }
//     }
}

void usb_hid_desc_1_init(usb_manager_t *manager){
    manager->usb_hid_desc_length = desc_hid_report_1_size;
    manager->usb_hid_desc = (uint8_t*)malloc(sizeof(uint8_t) * desc_hid_report_1_size);

    memcpy(manager->usb_hid_desc, desc_hid_report_1, desc_hid_report_1_size);

    // Set Resolution Multiplier. 
    manager->usb_hid_desc[71] = 0x20;
}

void usb_hid_desc_init(usb_manager_t *manager){
    usb_hid_desc_1_init(manager);
}

void usb_hid_init(bool (*ready_for_hid_report) (void), int (*as5600_get_delta) (void)){
    usb_manager_t *manager = get_usb_manager();

    manager->ready_for_hid_report = ready_for_hid_report;
    manager->as5600_get_delta = as5600_get_delta;

    usb_hid_desc_init(manager);

    tusb_init();
}

void usb_hid_update(){
    tud_task(); // tinyusb device task
    hid_task(); // Send HID Reports
}