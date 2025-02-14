#include "common.h"
#include "lib/librp2040/src/as5600/as5600.h"

namespace sensors{
    class PositionSensor{
        private:
            int resolution = 4000;

            as5600::AS5600 as5600;

            int current, last;
            
            int16_t map(int16_t input);
        public:
            PositionSensor();
            ~PositionSensor();

            bool acquire_data();
            int16_t getDelta();
    };
}
