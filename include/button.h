#include "common.h"
#include "flash.h"
#include "led_ring.h"

/*
change resolution
change modus

change between simpel led modies. 
*/

typedef struct {
    int32_t start_time_last_push;
} button_manager_t;

void button_init(button_manager_t *manager);

void button_update(buttoin_manager_t *manager);