
1. Function description

    This project demonstrates BLE transparent transmission with a 0.96 inch SSD1306 OLED.
    Phone-to-board data is forwarded to USART1 and displayed on OLED.
    USART1 received data is forwarded back to the BLE client and displayed on OLED.

2. Operating environment
    Software development environment: KEIL MDK-ARM V5.30.0.0 or newer
    Hardware environment: N32WB031_STB + 0.96 inch I2C OLED (SSD1306 128x64)

3. Instructions

    System configuration:
        1. Clock source: HSI=64M, AHB=64M, APB1=32M, APB2=64M
        2. LED1: PB0 (power / run)
        3. LED2: PA6 (BLE connection status)
        4. Bluetooth device name: N32WB031_OLED
        5. BLE data bridge USART: USART1 PB6(TX), PB7(RX), 115200-8-N-1
        6. OLED software I2C: PB8(SCL), PB9(SDA), address 0x3C / 0x78

    Usage:
        1. Open the project in MDK-ARM
        2. Build and download to the board
        3. Connect OLED VCC/GND/SDA/SCL
        4. Open a BLE debugging app such as nRF Connect or LightBlue
        5. Search and connect to N32WB031_OLED
        6. Enable notification and send data to verify BLE + OLED + UART interaction

4. Matters needing attention
    1. PB6 and PB7 are kept as the convenient board USB serial bridge in this project.
    2. OLED is changed to software I2C on PB8 and PB9.
    3. This project is based on the official BLE stack example and is intended as a teaching-friendly copy.
