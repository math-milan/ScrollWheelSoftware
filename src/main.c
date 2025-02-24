#include "common.h"
#include "led_ring.h"
#include "rotary_encoder.h"

int main(void){
    led_ring_t led_ring;

    stdio_init_all();
    led_ring_init(&led_ring);

    led_ring_set_all_rgb(&led_ring, 250, 250, 0);
    led_ring_update(&led_ring);


    return 0;
}