#include "common.h"
#include "lib/librp2040/src/AT24C01C/at24c01c.h"


// 
#define LED_LENGTH_ADDR 0



/// @brief Returns the Length of the data stored for the leds
/// @return PICO_GENERIC_ERROR if the device dose not responde
int16_t getLedDataLength();
bool getLedData(uint8_t *buffer, int len);
bool writeData(uint8_t *buffer, int len);