#include "ws2812b.h"
#include "common.h"

typedef struct {
    ws2812b_inst_t ws2812b_inst;
    uint32_t led_buffer[NUMBER_OF_LEDS];
} led_ring_t;

void led_ring_init(led_ring_t *instance);
void led_ring_update(led_ring_t *instance);
void led_ring_set_all_rgb(led_ring_t *instance, uint8_t r, uint8_t g, uint8_t b);