#include "status_led.h"

namespace led{

    LED::LED(){
        pio = pio0;
        uint offset = pio_add_program(pio, &ws2812_program);
        sm = pio_claim_unused_sm(pio, true);
        ws2812_program_init(pio, sm, offset, STATUS_LED_PIN, freq, false);
    };

    void LED::setStatus(status status){
        led_status = status;
    };

    void LED::update(){
        switch (led_status){
            case status::ok:
                setLED(0, 100, 0);
                break;
            case status::not_ok:
                setLED(100, 0, 0);
                break;
            default:
                setLED(0, 0, 0);
        }
    };

    void LED::setLED(int r, int g, int b){
        pio_sm_put_blocking(pio0, 0, (((uint32_t) (r) << 8) | ((uint32_t) (g) << 16) | (uint32_t)  (b)) << 8u);
    }

}