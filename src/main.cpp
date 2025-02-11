#include "common.h"
#include "status_led.h"
#include "as5600.h"
#include "usb_callbacks.h"


// uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;
USBData usb_data;
led::LED *statusLED;
int main(void){
    stdio_init_all();
    usb_init(4);

    statusLED = new led::LED();
    encoder::Encoder encoder = encoder::Encoder();

    statusLED->setStatus(led::ok);
    statusLED->update();



    while(1){   
        encoder.acquire_data();
        usb_data.scroll_delta = encoder.getDelta();

        usb_update();
        sleep_ms(5);
    }
};
