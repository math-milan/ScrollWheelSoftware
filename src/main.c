#include "common.h"
#include "led_ring.h"
#include "rotary_encoder.h"
#include "usb_hid.h"

led_ring_t led_ring;
rotary_encoder_t rotary_encoder;

bool ready_for_hid_report(void){
    return true;
    return encoder_peek_delta(&rotary_encoder);
}

int as5600_get_delta(void){ 
    int delta;

    rotary_encoder.status = encoder_get_delta(&rotary_encoder, &delta);

    return rotary_encoder.status ? delta : 0;
}

int main(void){

    stdio_init_all();
    led_ring_init(&led_ring);
    encoder_init(&rotary_encoder);

    usb_hid_init(ready_for_hid_report, as5600_get_delta);

    led_ring_set_all_rgb(&led_ring, 0, 0, 50);
    led_ring_update(&led_ring);


    while (true){
        usb_hid_update();
    }

    return 0;
}