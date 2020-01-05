#include <project_cfg.h>

const fpioa_cfg_t g_fpioa_cfg =
    {
        .version = PIN_CFG_VERSION,
        .functions_count = 6,
        .functions =
            {
                {LED_PIN, PIN_FUNCTION(LED_PIN)},
                {I2C0_SDA_PIN, FUNC_I2C0_SDA},
                {I2C0_SCLK_PIN, FUNC_I2C0_SCLK},
                {SIPEED_ST7789_DCX_PIN, static_cast<fpioa_function_t>(FUNC_GPIOHS0 + DCX_GPIONUM)},
                {SIPEED_ST7789_SS_PIN, static_cast<fpioa_function_t>(FUNC_SPI0_SS0 + SPI_SLAVE_SELECT)},
                {SIPEED_ST7789_SCLK_PIN, FUNC_SPI0_SCLK}}};

const pin_cfg_t g_pin_cfg =
    {
        .version = PIN_CFG_VERSION,
        .set_spi0_dvp_data = 1};