#include "LedRing.h"

ledring::LEDRing::LEDRing(){
    this->startup();
}

ledring::LEDRing::~LEDRing(){
    delete ledHandler;
}

void ledring::LEDRing::update(){
    ledHandler->update();
}

bool ledring::LEDRing::startup(){
    int length = getLedDataLength();
    if (length == PICO_ERROR_GENERIC || length == 0){
        ledHandler = new animation();
        ledHandler->setError();
        return false;
    }
    uint8_t *buffer = new uint8_t[length];

    bool success = getLedData(buffer, length);

    if (!success && length > 0){
        ledHandler = new animation();
        ledHandler->setError();
        delete buffer;
        return false;
    }

    success = true;

    switch (buffer[0]){
        case STATIC:
            ledHandler = new Static(buffer, length);
            break;
        default:
            ledHandler = new animation();
            ledHandler->setError();
            success = false;
            break;
    }
    delete buffer;
    return success;
}

ledring::animation::animation() : ws2812(STATUS_LED_PIN, NUMBER_OF_LEDS){
    error = false;
}

ledring::animation::~animation(){
    
}

void ledring::animation::update()
{
    if (error){
        error_update();
    }
}

void ledring::animation::setError()
{
    error = true;
}

void ledring::animation::error_update(){
    
}

ledring::Static::Static(uint8_t *data, int len) : animation()
{
    if (len < STATIC_LENGTH){
        error = true;
    }
    color_setting = ((uint32_t) (data[1]) << 8) | ((uint32_t) (data[2]) << 16) | (uint32_t)  (data[3]);

    setAllHEX(color_setting); // set all leds
    flip(); // push update to all leds
}

ledring::Static::~Static()
{
}

void ledring::Static::update()
{ // nothing to do. It dosen't make to update teh led's
    if (error){
        error_update();
    }
}