#include "common.h"
#include "status_led.h"
#include "as5600.h"
#include "usb_callbacks.h"


#define ENCODER_CLK 3
#define ENCODER_DT  10
#define ENCODER_SW  8

uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

volatile int scroll_delta = 0;
volatile bool last_clk_state;
int last_sw_state = 1;

void encoder_isr(uint gpio, uint32_t events) {
    bool clk_state = gpio_get(ENCODER_CLK);
    bool dt_state = gpio_get(ENCODER_DT);

    if (clk_state != last_clk_state) {
        if (dt_state != clk_state) {
            scroll_delta++;  // Clockwise rotation
        } else {
            scroll_delta--;  // Counter-clockwise rotation
        }
        last_clk_state = clk_state;
    }
}

int main(void){
    stdio_init_all();
    usb_init();
    
    gpio_init(16);
    gpio_set_dir(16, true);
    gpio_put(16, true);


    gpio_init(ENCODER_CLK);
    gpio_set_dir(ENCODER_CLK, GPIO_IN);
    gpio_pull_up(ENCODER_CLK);

    gpio_init(ENCODER_DT);
    gpio_set_dir(ENCODER_DT, GPIO_IN);
    gpio_pull_up(ENCODER_DT);

    gpio_init(ENCODER_SW);
    gpio_set_dir(ENCODER_SW, GPIO_IN);

    last_clk_state = gpio_get(ENCODER_CLK);

    gpio_set_irq_enabled_with_callback(ENCODER_CLK, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &encoder_isr);

    led::LED statusLED = led::LED();

    statusLED.setStatus(led::mounted);
    statusLED.update();

    char buffer[256];
    int last_intervall = blink_interval_ms;

    int last_event = time_us_64() / 1000;



    while(1){   
        if (last_event + 1000 > time_us_64() / 1000){
            scroll_delta = 1;
            last_event = time_us_64() / 1000;
        }
        usb_update();
        sleep_ms(20);
    }
};
