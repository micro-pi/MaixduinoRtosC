#ifndef PROJECT_CFG_H
#define PROJECT_CFG_H

#include <pin_cfg.h>

#define SIPEED_ST7789_DCX_PIN (38)
#define SIPEED_ST7789_SS_PIN (36)
#define SIPEED_ST7789_SCLK_PIN (39)

#define SIPEED_ST7789_DCX_GPIONUM (7)
#define SIPEED_ST7789_SS (3)

#define SPI_SLAVE_SELECT SIPEED_ST7789_SS
#define DCX_GPIONUM SIPEED_ST7789_DCX_GPIONUM

#define PIN_FUNCTION(PIN) (static_cast<fpioa_function_t>(FUNC_GPIOHS0 + PIN))
#define LED_PIN (14)

#define CORE_0 (0)
#define CORE_1 (1)

#define I2C0_SDA_PIN (31)
#define I2C0_SCLK_PIN (30)

#define CALIBRATION_SAMPLES (100)

#endif
