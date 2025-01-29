#include "as5600.h"

namespace encoder{
    
    AS5600::AS5600(){
        this->dst = new uint8_t[DST_BUFFER_LEN];
        this->init();
    };
    
    AS5600::~AS5600(){
        delete[] dst;
    }

    bool AS5600::get_config(config *Config)
    {
        if (!writeInstruction(Register_Address::STATUS) || !read(2)){
            return false;
        }

        Config->sf = (encoder::SlowFilter)(dst[0] & 0x03);
        Config->fth = (encoder::FastFilterThreshold)(dst[0] & 0x0C);
        Config->watch_dog = (bool)(dst[0] & 0x20);

        Config->pm = (encoder::PowerMode)(dst[1] & 0x03);
        Config->hyst = (encoder::Hysteresis)(dst[1] & 0x0C);
        Config->outs = (encoder::OutputStage)(dst[1] & 0x30);
        Config->pwmf = (encoder::PWMFrequency)(dst[1] & 0xC0);

        return true;
    }

    bool AS5600::get_settigs_string(char *str)
    {
        config cfg;

        if (!get_config(&cfg)){
            return false;
        }

        snprintf(str, 256,  
        "Power Mode: %d\n"
        "Hysteresis: %d\n"
        "Output Stage: %d\n"
        "PWM Frequency: %d\n"
        "Slow Filter: %d\n"
        "Fast Filter Threshold: %d\n"
        "Watchdog: %s\n",
        cfg.pm,
        cfg.hyst,
        cfg.outs,
        cfg.pwmf,
        cfg.sf,
        cfg.fth,
        cfg.watch_dog ? "ON" : "OFF"
        );

        return true;
    }

    int16_t AS5600::getRawAngel()
    {
        if (!writeInstruction(Register_Address::RAW_ANGLE) || !read(2)){
            return -1;
        }

        return get_12_bits();
    }

    int16_t AS5600::getAngel(){
        if (!writeInstruction(Register_Address::ANGLE) || !read(2)){
            return -1;
        }

        return get_12_bits();
    }
    /// @brief Initilizes i2c on the rp2040
    void AS5600::init() {
        i2c_init(I2C_INST, 100*1000); // 100kHz Baudrate 
        
        gpio_set_function(SDA, GPIO_FUNC_I2C);
        gpio_set_function(SCL, GPIO_FUNC_I2C);
    }
    /// @brief Writes an Register Address Word to the AS5600
    bool AS5600::writeInstruction(Register_Address address)
    {
        uint8_t reg = static_cast<uint8_t>(address);
        return (i2c_write_blocking(I2C_INST, AS5600_ADDRESS, &reg, 1, true) == 1); // if the 1 byte is written return true
    }
    /// @brief Reads the specified number of bytes
    bool AS5600::read(int len)
    {
        if (len >= DST_BUFFER_LEN){
            return false;
        }
        return (i2c_read_blocking(I2C_INST, AS5600_ADDRESS, this->dst, len, true) == len);
    }
    /// @brief Extracts a 12 bit number from the buffer according to the Register Description off the AS5600
    /// @return 12 bit number
    int16_t AS5600::get_12_bits()
    {
        return ((dst[0] & 0x0F) << 8) | dst[1];
    };
}