#include "led_ring.h"

void led_ring_init(led_ring_t *instance)
{
    ws2812b_init(&instance->ws2812b_inst, STATUS_LED_PIN, NUMBER_OF_LEDS);
}

void led_ring_update(led_ring_t *instance)
{
    ws2812b_output_data(&instance->ws2812b_inst, instance->led_buffer, NUMBER_OF_LEDS);
}

void led_ring_set_all_rgb(led_ring_t *instance, uint8_t r, uint8_t g, uint8_t b){
    uint32_t value = ws2812b_convert_RGB(r, g, b);
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++)
    {
        instance->led_buffer[i] = value;
    }    
}