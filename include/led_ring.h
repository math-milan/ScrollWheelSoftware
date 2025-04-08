#include "ws2812b.h"
#include "common.h"
#include "flash.h"

typedef enum {
    LED_MODE_STATIC,
    LED_MODE_BLINKING,
    LED_MODE_RAINBOW,
    LED_MODE_SPEED_INDICATOR
} led_mode_t;

typedef struct {
    ws2812b_inst_t ws2812b_inst;
    uint32_t led_buffer[NUMBER_OF_LEDS];
    uint32_t last_update_us;
    uint16_t time_animation;
    // Settings for Led animations
    storage_led_t storage_led;
} led_ring_t;

typedef void (*led_animation_fn)(led_ring_t* instance);

void led_animation_check_requiermen(led_ring_t *instance);
void led_animation_init(led_ring_t *instance);
void led_ring_init(led_ring_t *instance);

void static led_animation_time_helper(led_ring_t* instance, int16_t period);

void led_animation_static(led_ring_t* instance);
void led_animation_blinking(led_ring_t* instance);
void led_animation_rainbow(led_ring_t* instance);
void led_animation_speed_indicator(led_ring_t* instance);

static const led_animation_fn animation_table[] = {
    led_animation_static,
    led_animation_blinking,
    led_animation_rainbow,
    led_animation_speed_indicator
};

void led_ring_update(led_ring_t *instance);
void led_ring_set_all_rgb(led_ring_t *instance, uint8_t r, uint8_t g, uint8_t b);

/*
Need for a status funktion of the led ring.
    Front 4 Leds a status update for not critical funktions.
    All led's for major issues
*/