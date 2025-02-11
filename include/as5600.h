#include "common.h"

namespace encoder{
    #define AS5600_ADDRESS 0x36

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
        uint8_t pm : 2;
        uint8_t hyst : 2;
        uint8_t outs : 2;
        uint8_t pwmf : 2;
        uint8_t sf : 2;
        uint8_t fth : 3;
        bool wd : 1;

        config(uint16_t value)
        {
            pm = value & 0b11;
            hyst = (value >> 2) & 0b11;
            outs = (value >> 4) & 0b11;
            pwmf = (value >> 6) & 0b11;
            sf = (value >> 8) & 0b11;
            fth = (value >> 10) & 0b111;
            wd = (value >> 13) & 0b1;
        }

        // Convert struct to uint16_t
        uint16_t toUInt16() const
        {
            return (pm) |           // Bits 0-1
               (hyst << 2)  |       // Bits 2-3
               (outs << 4)  |       // Bits 4-5
               (pwmf << 6)  |       // Bits 6-7
               (sf << 8)    |       // Bits 8-9
               (fth << 10)  |       // Bits 10-12
               (wd << 13);          // Bit 13
        }    
    
    };
    
    class Buffer{
        private:
            static constexpr int ARRAY_SIZE = 5;
            int16_t *buffer;
            int head, count;
        public:
            Buffer();
            ~Buffer();

            void push(int16_t number);
            const int16_t& operator[](int index) const;
            int size();
    };    
    
    class AS5600{ 
        public:
            AS5600();
            ~AS5600();

            // bool get_config(config *Config);
            /// @brief 
            /// @param str The Char Buffer must be at least 256 Bytes Long
            /// @return False if no response from the device
            // bool get_settigs_string(char *str);
                
            int16_t getRawAngel();
            int16_t getAngel();

            config getConfig();
            bool setConfig(config cfg);
        private:
            bool error_flag;
            void init();
            void setErrorFlag();

            bool writeInstruction(Register_Address address);
            bool writeData(uint16_t data, int len);
            uint16_t read(int len);            
    };

    class Encoder{
        private:
            int resolution = 1000;

            AS5600 as5600;

            int current, last;
            
            int16_t map(int16_t input);
        public:
            Encoder();
            ~Encoder();

            bool acquire_data();
            int16_t getDelta();
    };

    
}
