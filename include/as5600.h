#include "common.h"

namespace encoder{
    #define AS5600_ADDRESS 0x36
    #define DST_BUFFER_LEN 3


    enum Register_Address{
        ZMCO = 0x00,
        ZPOS = 0x01,
        MPOS = 0x03,
        MANG = 0x05,
        CONF = 0x07,
        RAW_ANGLE = 0x0C,
        ANGLE = 0x0E,
        STATUS = 0x0B,
        AGC = 0x1A,
        MAGNITUDE = 0x01C
    };

    enum PowerMode{
        NOM = 0,
        LPM1 = 1,
        LMP2 = 2,
        LMP3 = 3
    };

    enum Hysteresis{
        OFF = 0,
        LSB1 = 1,
        LSB2 = 2,
        LSB3 = 3
    };

    enum OutputStage{
        analog_full = 0,
        analog_reduced = 1,
        PWM = 2
    };

    enum PWMFrequency{
        F115Hz = 0,
        F230Hz = 1,
        F460Hz = 2,
        F920Hz = 3
    };

    enum SlowFilter{
        x16 = 0,
        x8 = 1,
        x4 = 2,
        x2 = 3
    };

    enum FastFilterThreshold{
        slow_filter_only = 0,
        LSBs6 = 1,
        LSBs7 = 2,
        LSBs9 = 3,
        LSBs18 = 4,
        LSBs21 = 5,
        LSBs24 = 6,
        LSBs10 = 7
    };

    struct config
    {
        PowerMode pm;
        Hysteresis hyst;
        OutputStage outs;
        PWMFrequency pwmf;
        SlowFilter sf;
        FastFilterThreshold fth;
        bool watch_dog;
    };
    
    
    class AS5600{ 
        public:
            AS5600();
            ~AS5600();

            bool get_config(config *Config);
            /// @brief 
            /// @param str The Char Buffer must be at least 256 Bytes Long
            /// @return False if no response from the device
            bool get_settigs_string(char *str);
                
            int16_t getRawAngel();
            int16_t getAngel();
        private:
            uint8_t *dst;
            void init();

            bool writeInstruction(Register_Address address);
            bool read(int len);
            int16_t get_12_bits();

            
    };
}
