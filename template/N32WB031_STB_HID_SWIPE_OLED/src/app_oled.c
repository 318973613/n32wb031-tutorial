#include <string.h>
#include "app_oled.h"

static uint8_t oled_buffer[8][128];
static char oled_ble_line[22] = "BLE:";
static char oled_uart_line[22] = "UART:";
static uint8_t oled_connected = 0U;

#define OLED_SCL_HIGH() GPIO_SetBits(OLED_GPIO, OLED_SCL_PIN)
#define OLED_SCL_LOW()  GPIO_ResetBits(OLED_GPIO, OLED_SCL_PIN)
#define OLED_SDA_HIGH() GPIO_SetBits(OLED_GPIO, OLED_SDA_PIN)
#define OLED_SDA_LOW()  GPIO_ResetBits(OLED_GPIO, OLED_SDA_PIN)

static const uint8_t font6x8[][6] =
{
    {0x00,0x00,0x00,0x00,0x00,0x00}, /* space */
    {0x3E,0x51,0x49,0x45,0x3E,0x00}, /* 0 */
    {0x00,0x42,0x7F,0x40,0x00,0x00}, /* 1 */
    {0x42,0x61,0x51,0x49,0x46,0x00}, /* 2 */
    {0x21,0x41,0x45,0x4B,0x31,0x00}, /* 3 */
    {0x18,0x14,0x12,0x7F,0x10,0x00}, /* 4 */
    {0x27,0x45,0x45,0x45,0x39,0x00}, /* 5 */
    {0x3C,0x4A,0x49,0x49,0x30,0x00}, /* 6 */
    {0x01,0x71,0x09,0x05,0x03,0x00}, /* 7 */
    {0x36,0x49,0x49,0x49,0x36,0x00}, /* 8 */
    {0x06,0x49,0x49,0x29,0x1E,0x00}, /* 9 */
    {0x7E,0x11,0x11,0x11,0x7E,0x00}, /* A */
    {0x7F,0x49,0x49,0x49,0x36,0x00}, /* B */
    {0x3E,0x41,0x41,0x41,0x22,0x00}, /* C */
    {0x7F,0x41,0x41,0x22,0x1C,0x00}, /* D */
    {0x7F,0x49,0x49,0x49,0x41,0x00}, /* E */
    {0x7F,0x09,0x09,0x09,0x01,0x00}, /* F */
    {0x3E,0x41,0x49,0x49,0x7A,0x00}, /* G */
    {0x7F,0x08,0x08,0x08,0x7F,0x00}, /* H */
    {0x00,0x41,0x7F,0x41,0x00,0x00}, /* I */
    {0x20,0x40,0x41,0x3F,0x01,0x00}, /* J */
    {0x7F,0x08,0x14,0x22,0x41,0x00}, /* K */
    {0x7F,0x40,0x40,0x40,0x40,0x00}, /* L */
    {0x7F,0x02,0x0C,0x02,0x7F,0x00}, /* M */
    {0x7F,0x04,0x08,0x10,0x7F,0x00}, /* N */
    {0x3E,0x41,0x41,0x41,0x3E,0x00}, /* O */
    {0x7F,0x09,0x09,0x09,0x06,0x00}, /* P */
    {0x3E,0x41,0x51,0x21,0x5E,0x00}, /* Q */
    {0x7F,0x09,0x19,0x29,0x46,0x00}, /* R */
    {0x46,0x49,0x49,0x49,0x31,0x00}, /* S */
    {0x01,0x01,0x7F,0x01,0x01,0x00}, /* T */
    {0x3F,0x40,0x40,0x40,0x3F,0x00}, /* U */
    {0x1F,0x20,0x40,0x20,0x1F,0x00}, /* V */
    {0x7F,0x20,0x18,0x20,0x7F,0x00}, /* W */
    {0x63,0x14,0x08,0x14,0x63,0x00}, /* X */
    {0x03,0x04,0x78,0x04,0x03,0x00}, /* Y */
    {0x61,0x51,0x49,0x45,0x43,0x00}, /* Z */
    {0x08,0x08,0x08,0x08,0x08,0x00}, /* - */
    {0x00,0x60,0x60,0x00,0x00,0x00}, /* . */
    {0x00,0x36,0x36,0x00,0x00,0x00}, /* : */
    {0x08,0x08,0x3E,0x08,0x08,0x00}, /* + */
    {0x00,0x00,0x7F,0x00,0x00,0x00}, /* I bar */
};

static uint8_t oled_font_index(char c)
{
    if (c == ' ')
        return 0;
    if (c >= '0' && c <= '9')
        return (uint8_t)(c - '0' + 1);
    if (c >= 'A' && c <= 'Z')
        return (uint8_t)(c - 'A' + 11);
    if (c >= 'a' && c <= 'z')
        return (uint8_t)(c - 'a' + 11);
    if (c == '-')
        return 37;
    if (c == '.')
        return 38;
    if (c == ':')
        return 39;
    if (c == '+')
        return 40;
    return 0;
}

static void oled_copy_text(char* dest, const char* prefix, const uint8_t* data, uint16_t len)
{
    uint8_t pos = 0U;
    uint8_t i = 0U;

    while (prefix[pos] != '\0' && pos < 20U)
    {
        dest[pos] = prefix[pos];
        pos++;
    }

    while (i < len && pos < 21U)
    {
        uint8_t ch = data[i++];
        if (ch < 32U || ch > 126U)
        {
            ch = '.';
        }
        if (ch >= 'a' && ch <= 'z')
        {
            ch = (uint8_t)(ch - 'a' + 'A');
        }
        dest[pos++] = (char)ch;
    }

    dest[pos] = '\0';
}

static void oled_render_dashboard(void)
{
    oled_show_status("BLE OLED DEMO",
                     oled_connected ? "STATE: CONNECTED" : "STATE: ADVERTISING",
                     oled_ble_line,
                     oled_uart_line);
}

static void oled_delay(uint32_t count)
{
    while (count-- > 0U)
    {
        __NOP();
    }
}

static void oled_i2c_delay(void)
{
    oled_delay(40U);
}

static void oled_gpio_init(void)
{
    GPIO_InitType gpio;

    RCC_EnableAPB2PeriphClk(OLED_GPIO_CLK, ENABLE);

    GPIO_InitStruct(&gpio);
    gpio.Pin = OLED_SCL_PIN | OLED_SDA_PIN;
    gpio.GPIO_Speed = GPIO_SPEED_HIGH;
    gpio.GPIO_Mode = GPIO_MODE_OUTPUT_OD;
    gpio.GPIO_Pull = GPIO_PULL_UP;
    gpio.GPIO_Alternate = GPIO_NO_AF;
    GPIO_InitPeripheral(OLED_GPIO, &gpio);

    OLED_SCL_HIGH();
    OLED_SDA_HIGH();
}

static void oled_i2c_start(void)
{
    OLED_SDA_HIGH();
    OLED_SCL_HIGH();
    oled_i2c_delay();
    OLED_SDA_LOW();
    oled_i2c_delay();
    OLED_SCL_LOW();
}

static void oled_i2c_stop(void)
{
    OLED_SCL_LOW();
    OLED_SDA_LOW();
    oled_i2c_delay();
    OLED_SCL_HIGH();
    oled_i2c_delay();
    OLED_SDA_HIGH();
    oled_i2c_delay();
}

static void oled_i2c_write_bit(uint8_t bit)
{
    if (bit != 0U)
    {
        OLED_SDA_HIGH();
    }
    else
    {
        OLED_SDA_LOW();
    }

    oled_i2c_delay();
    OLED_SCL_HIGH();
    oled_i2c_delay();
    OLED_SCL_LOW();
}

static void oled_i2c_write_byte_raw(uint8_t value)
{
    uint8_t i;

    for (i = 0U; i < 8U; i++)
    {
        oled_i2c_write_bit((uint8_t)((value & 0x80U) != 0U));
        value <<= 1;
    }

    OLED_SDA_HIGH();
    oled_i2c_delay();
    OLED_SCL_HIGH();
    oled_i2c_delay();
    OLED_SCL_LOW();
}

static void oled_write_byte(uint8_t control, uint8_t data)
{
    oled_i2c_start();
    oled_i2c_write_byte_raw(OLED_I2C_ADDR);
    oled_i2c_write_byte_raw(control);
    oled_i2c_write_byte_raw(data);
    oled_i2c_stop();
}

static void oled_write_cmd(uint8_t cmd)
{
    oled_write_byte(0x00, cmd);
}

static void oled_write_data(uint8_t data)
{
    oled_write_byte(0x40, data);
}

void oled_set_cursor(uint8_t page, uint8_t column)
{
    oled_write_cmd((uint8_t)(0xB0 + page));
    oled_write_cmd((uint8_t)(0x10 + ((column >> 4) & 0x0F)));
    oled_write_cmd((uint8_t)(column & 0x0F));
}

void oled_clear(void)
{
    memset(oled_buffer, 0, sizeof(oled_buffer));
    oled_refresh();
}

void oled_refresh(void)
{
    uint8_t page;
    uint8_t col;

    for (page = 0; page < 8U; page++)
    {
        oled_set_cursor(page, 0);
        for (col = 0; col < 128U; col++)
        {
            oled_write_data(oled_buffer[page][col]);
        }
    }
}

void oled_write_char(char c)
{
    (void)c;
}

void oled_write_string(uint8_t page, uint8_t column, const char* str)
{
    uint8_t idx;
    uint8_t char_index;

    if (page >= 8U || column >= 128U)
    {
        return;
    }

    while (*str != '\0' && column <= 122U)
    {
        char_index = oled_font_index(*str++);
        for (idx = 0; idx < 6U; idx++)
        {
            oled_buffer[page][column + idx] = font6x8[char_index][idx];
        }
        column += 6U;
    }
}

void oled_show_status(const char* line1, const char* line2, const char* line3, const char* line4)
{
    memset(oled_buffer, 0, sizeof(oled_buffer));
    oled_write_string(0, 0, line1);
    oled_write_string(2, 0, line2);
    oled_write_string(4, 0, line3);
    oled_write_string(6, 0, line4);
    oled_refresh();
}

void oled_init(void)
{
    oled_gpio_init();
    oled_delay(0x20000U);
    oled_write_cmd(0xAE);
    oled_write_cmd(0x20);
    oled_write_cmd(0x10);
    oled_write_cmd(0xB0);
    oled_write_cmd(0xC8);
    oled_write_cmd(0x00);
    oled_write_cmd(0x10);
    oled_write_cmd(0x40);
    oled_write_cmd(0x81);
    oled_write_cmd(0x7F);
    oled_write_cmd(0xA1);
    oled_write_cmd(0xA6);
    oled_write_cmd(0xA8);
    oled_write_cmd(0x3F);
    oled_write_cmd(0xA4);
    oled_write_cmd(0xD3);
    oled_write_cmd(0x00);
    oled_write_cmd(0xD5);
    oled_write_cmd(0xF0);
    oled_write_cmd(0xD9);
    oled_write_cmd(0x22);
    oled_write_cmd(0xDA);
    oled_write_cmd(0x12);
    oled_write_cmd(0xDB);
    oled_write_cmd(0x20);
    oled_write_cmd(0x8D);
    oled_write_cmd(0x14);
    oled_write_cmd(0xAF);
    oled_clear();
    oled_render_dashboard();
}

void oled_show_boot(void)
{
    oled_show_status("BLE OLED DEMO",
                     "STATE: BOOT",
                     "BLE: WAITING",
                     "UART: WAITING");
}

void oled_set_conn_state(uint8_t connected)
{
    oled_connected = connected;
    oled_render_dashboard();
}

void oled_set_ble_text(const uint8_t* data, uint16_t len)
{
    oled_copy_text(oled_ble_line, "BLE: ", data, len);
    oled_render_dashboard();
}

void oled_set_uart_text(const uint8_t* data, uint16_t len)
{
    oled_copy_text(oled_uart_line, "UART: ", data, len);
    oled_render_dashboard();
}
