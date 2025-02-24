#include "as5600.h"
#include "common.h"

typedef struct {
    int pos_value_current;
    int pos_value_last;
} rotary_encoder_t;

void encoder_init(rotary_encoder_t *instance);
bool encoder_acquire_data(rotary_encoder_t *instance);
int encoder_get_delta(rotary_encoder_t *instance);