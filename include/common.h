#include "pico/stdlib.h"
#include "stdio.h"
// #include "hardware/adc.h"
// #include "hardware/spi.h"
#include "hardware/i2c.h"
// #include "hardware/dma.h"
#include "hardware/pio.h"
// #include "hardware/interp.h"
// #include "hardware/timer.h"
// #include "hardware/watchdog.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "math.h"
#include "stdbool.h"

#define STATUS_LED_PIN 17 // 12

#define I2C_INST i2c0
#define SDA 8
#define SCL 9