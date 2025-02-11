#include "ws2812.pio.h"
#include "common.h"

#ifndef STATUS_LED_H
#define STATUS_LED_H

namespace led{
    enum status{
        not_mounted,
        mounted,
        suspended,
        ok,
        not_ok
    };

    class LED{
        public:
            LED();
            void setStatus(status status);
            void update();
            void setLED(int r, int g, int b);
        private:
            status led_status;
            PIO pio;
            uint8_t sm;
            float freq = 800000;

            

    };
}

#endif