#include "ws2812.pio.h"
#include "common.h"

namespace led{
    enum status{
        ok,
        not_ok
    };

    class LED{
        public:
            LED();
            void setStatus(status status);
            void update();
        private:
            status led_status;
            PIO pio;
            uint8_t sm;
            float freq = 800000;

            void setLED(int r, int g, int b);

    };
}