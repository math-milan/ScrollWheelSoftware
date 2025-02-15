#include "lib/librp2040/src/ws2812b/ws2812.h"
#include "storage.h"
#include "common.h"

namespace ledring{
enum LEDModus{
    STATIC = 0
};

class animation : protected ws2812{
    public:
        animation();
        ~animation();
        /* code */

        virtual void update();

        void setError();

    protected:
        uint8_t error_code;
        bool error;
        void error_update();
};


class LEDRing{
    #define STATIC_LENGTH 4

    /*
    The configuration data for the led ring is send by the user from the computer via Vendor Specific field in the HID config. This data length can be arbitrary.
    The Length is stored in a Length Register. Then the data can be read and passsed to the different animations.
    */

    public:
        LEDRing();
        ~LEDRing();

        void update();

    private:
        bool startup();
        
        animation *ledHandler;        
};


class Static : public animation{
    public:
        /// @brief A Static color with no changes.
        /// @param data [animation ID, R, G, B]
        /// @param len min of 4 bytes
        Static(uint8_t *data, int len);
        ~Static();

        void update() override;

    private:
        uint32_t color_setting;

};

}