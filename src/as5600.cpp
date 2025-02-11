#include "as5600.h"

namespace encoder{
    
    AS5600::AS5600(){
        this->init();
    };
    
    AS5600::~AS5600(){
    }


    int16_t AS5600::getRawAngel()
    {
        if (!writeInstruction(Register_Address::RAW_ANGLE)){
            setErrorFlag();
            return -1;
        }
        uint16_t data = read(2);
        
        return data & 0xFFF; // Returns 12 bits
    }

    int16_t AS5600::getAngel(){
        if (!writeInstruction(Register_Address::ANGLE)){
            setErrorFlag();
            return -1;
        }
        uint16_t data = read(2);
        
        return data & 0xFFF; // Returns 12 bits
    }

    config AS5600::getConfig(){
        if (!writeInstruction(Register_Address::CONF)){
            setErrorFlag();
            return (config)0xFFF;
        }
        uint16_t data = read(2);
        
        return (config)data;
    };

    bool AS5600::setConfig(config cfg){
        if (!writeInstruction(Register_Address::CONF)){
            setErrorFlag();
            return false;
        }
        writeData(cfg.toUInt16(), 2);
        return true;
    }
    /// @brief Initilizes i2c on the rp2040
    void AS5600::init() {
        error_flag = false;

        i2c_init(I2C_INST, 100*1000); // 100kHz Baudrate 
        
        gpio_set_function(SDA, GPIO_FUNC_I2C);
        gpio_set_function(SCL, GPIO_FUNC_I2C);
    }

    void AS5600::setErrorFlag(){
        error_flag = true;
    }
    /// @brief Writes an Register Address Word to the AS5600
    bool AS5600::writeInstruction(Register_Address address)
    {
        uint8_t reg = static_cast<uint8_t>(address);
        return (i2c_write_blocking(I2C_INST, AS5600_ADDRESS, &reg, 1, true) == 1); // if the 1 byte is written return true
    }

    bool AS5600::writeData(uint16_t data, int len){
        uint8_t src[2];
        if (len == 1){
            src[0] = data & 0xFF;
        }
        else{
            src[0] = data & 0xF00;
            src[1] = data & 0xFF;
        }
        if ( i2c_write_blocking(I2C_INST, AS5600_ADDRESS, src, len, true) == len ){
            setErrorFlag();
            return false;
        }
        return true;
    }
    /// @brief Reads the specified number of bytes
    uint16_t AS5600::read(int len)
    {
        int DST_BUFFER_LEN = 5;
        uint8_t dst[DST_BUFFER_LEN];
        if (len >= DST_BUFFER_LEN){
            return false;
        }
        if (i2c_read_blocking(I2C_INST, AS5600_ADDRESS, dst, len, true) != len){
            setErrorFlag();
            return 0xFFFF;
        }
        return (dst[0] << 8) | dst[1];
    }

    int16_t Encoder::map(int16_t input){

        return (float)((input) * ((float)resolution / 4096.0f)); // 12 bit resolution;
    }

    Encoder::Encoder(){
        as5600 = AS5600();
        sleep_ms(50);
        for (int i = 0; i < 2; i++)
        {
            acquire_data();
            sleep_ms(10);
        }
    }

    Encoder::~Encoder(){
        // delete as5600;
    }

    bool Encoder::acquire_data(){
        int16_t new_angel = as5600.getAngel();
        if (new_angel == -1){
            return false;
        }
        last = current;
        current = map(new_angel);
        return true;
    }

    int16_t Encoder::getDelta(){
        int delta = current - last;
        if (delta > resolution / 2) {
            delta -= resolution;
        } else if (delta < - resolution / 2) {
            delta += resolution;
        }
        return delta;
        // return current; 
    }

    Buffer::Buffer(){
        buffer = new int16_t[ARRAY_SIZE];
        head = 0; count = 0;
    }

    Buffer::~Buffer(){
        delete buffer;
    }

    void Buffer::push(int16_t number){
        buffer[head] = number;
        head++; count++;
        if (head >= ARRAY_SIZE){
            head = 0;
        }
    }

    int Buffer::size(){
        if (count >= ARRAY_SIZE){
            count = ARRAY_SIZE - 1;
        }
        return count;
    }

    const int16_t& Buffer::operator[](int index) const{
        int true_index = (head - index);
        if (true_index < 0 ){
            true_index += ARRAY_SIZE;
        }
        return this->buffer[true_index];
    }
}