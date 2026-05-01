#ifndef __APP_OLED_H__
#define __APP_OLED_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32wb03x.h"
#include "n32wb03x_gpio.h"
#include "n32wb03x_rcc.h"

#define OLED_GPIO                GPIOB
#define OLED_GPIO_CLK            RCC_APB2_PERIPH_GPIOB
#define OLED_SCL_PIN             GPIO_PIN_8
#define OLED_SDA_PIN             GPIO_PIN_9
#define OLED_I2C_ADDR            0x78

void oled_init(void);
void oled_clear(void);
void oled_refresh(void);
void oled_set_cursor(uint8_t page, uint8_t column);
void oled_write_char(char c);
void oled_write_string(uint8_t page, uint8_t column, const char* str);
void oled_show_status(const char* line1, const char* line2, const char* line3, const char* line4);
void oled_show_boot(void);
void oled_set_conn_state(uint8_t connected);
void oled_set_ble_text(const uint8_t* data, uint16_t len);
void oled_set_uart_text(const uint8_t* data, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* __APP_OLED_H__ */
