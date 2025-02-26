#include "rotary_encoder.h"

void encoder_init(rotary_encoder_t *instance){
    i2c_init(I2C_INST, 100000);  // Example for 100kHz
    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C);

    instance->pos_value_current = as5600_getAngel(I2C_INST);
}

bool encoder_acquire_data(rotary_encoder_t *instance){
    int new_reading = as5600_getAngel(I2C_INST);

    if (new_reading == PICO_ERROR_GENERIC){
        return false;
    }

    instance->pos_value_last = instance->pos_value_current;
    instance->pos_value_current = new_reading;
    
    return true;
}

bool encoder_get_delta(rotary_encoder_t *instance, int *delta){
    bool success = encoder_acquire_data(instance);

    *delta = instance->pos_value_current - instance->pos_value_last;
    int resolution = 0xFFF;

    if (*delta > resolution / 2) {
        *delta -= resolution;
    } else if (*delta < -resolution / 2) {
        *delta += resolution;
    }

    return success;
}


bool encoder_peek_delta(rotary_encoder_t *instance)
{
    int new_reading = as5600_getAngel(I2C_INST);
    return new_reading == instance->pos_value_current;
}