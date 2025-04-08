#include "led_ring.h"


void led_animation_time_helper(led_ring_t *instance, int16_t period){
    if (instance->time_animation > period){
        instance->time_animation = 0;
    }
}

void led_animation_check_requierments(led_ring_t *instance){
    /// @todo add check if mode is not out off bound
    bool error;

    switch (instance->storage_led.mode){
        case LED_MODE_STATIC:{
            error = instance->storage_led.length == 3;
        }
        break;
        case LED_MODE_BLINKING:{
            error = instance->storage_led.length == 3;
        }
        break;
        case LED_MODE_RAINBOW:{
            error = false;
        }
        break;
        case LED_MODE_SPEED_INDICATOR:{
            error = false;
        }
        default:{
            error = false;
        }
        break;
    }
}

void led_animation_init(led_ring_t *instance){
    if (storage_read_led_data(&instance->storage_led)){
        /// @todo add defualt values to prevent crash
    }
    led_animation_check_requierments(instance);
}

void led_ring_init(led_ring_t *instance)
{
    ws2812b_init(&instance->ws2812b_inst, STATUS_LED_PIN, NUMBER_OF_LEDS);
    led_animation_init(instance);
}

void led_animation_static(led_ring_t *instance)
{
    if (instance->time_animation == 0){
        led_ring_set_all_rgb(instance, 0, 0, 50);
        instance->time_animation++;
    }
}

void led_animation_blinking(led_ring_t *instance){
    const int16_t period = 255;
    led_animation_time_helper(instance, period);        

    double scall = (sin((2 * M_PI * (double)instance->time_animation) / (double)period) + 1) / 2;

    int b = (int)((double)255 * scall);
    led_ring_set_all_rgb(instance, 0, 0, b);
}

void led_animation_rainbow(led_ring_t *instance){
    
}

void led_animation_speed_indicator(led_ring_t *instance){
    
}

void led_ring_update(led_ring_t *instance){ 
    if (time_us_64() - 10 * 1000 < instance->last_update_us){
        return;
    }
    animation_table[instance->storage_led.mode](instance);

    ws2812b_output_data(&instance->ws2812b_inst, instance->led_buffer, NUMBER_OF_LEDS);
}

void led_ring_set_all_rgb(led_ring_t *instance, uint8_t r, uint8_t g, uint8_t b){
    uint32_t value = ws2812b_convert_RGB(r, g, b);
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++)
    {
        instance->led_buffer[i] = value;
    }    
}