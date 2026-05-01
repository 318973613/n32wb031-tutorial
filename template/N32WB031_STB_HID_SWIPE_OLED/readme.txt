
1. Function description

    This project demonstrates BLE HID camera remote control with OLED status display.
    After the phone is connected, KEY1 sends volume up for camera shutter.
    KEY2 sends volume down as an auxiliary control.
    OLED shows advertising / connected state and the latest key action.

2. Operating environment
    Software development environment: KEIL MDK-ARM V5.30.0.0 or newer
    Hardware environment: N32WB031_STB + 0.96 inch SSD1306 OLED

3. Instructions

    System configuration:
        1. Clock source: HSI=64M, AHB=64M, APB1=32M, APB2=64M
        2. LED1: PB0
        3. LED2: PA6
        4. KEY1: PB1 -> camera shot (VOL+)
        5. KEY2: PB2 -> auxiliary key (VOL-)
        6. OLED software I2C: PB8(SCL), PB9(SDA)
        7. Bluetooth BLE name: N32WB031_CAMERA

    Usage:
        1. Build and download the project
        2. Power on the board and pair the phone with N32WB031_CAMERA
        3. After connection, press KEY1 or KEY2
        4. Open the phone camera and set volume key as shutter if needed
        5. Press KEY1 to test camera shutter and observe OLED text update

4. Precautions
    1. This project is based on the official BLE HID mouse example.
    2. Camera shutter effect depends on phone camera app support for volume-key shutter.
