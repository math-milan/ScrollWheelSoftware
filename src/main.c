#include "common.h"
// #include "led_ring.h"
// #include "rotary_encoder.h"
// #include "usb_hid.h"
// #include "flash.h"
#include "ws2812b.h"
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
    storage_init();

    ws2812b_inst_t led;
    ws2812b_init(&led, 6, 20);
    uint32_t buffer[30];

    for (int i = 0; i < 30; i++)
    {
        buffer[i] = ws2812b_convert_RGB(255, 0, 0);
    }
    
    ws2812b_output_data(&led, buffer, 20);

    usb_hid_init(ready_for_hid_report, as5600_get_delta);

    while (true){
        usb_hid_update();
        led_ring_update(&led_ring);
    }

    return 0;
}