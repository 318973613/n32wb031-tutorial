
1. Function description

    This project demonstrates BLE transparent transmission based on the official RDTSS example.
    After the phone connects to the board, phone-to-board data is forwarded to USART1,
    and USART1 received data is forwarded back to the BLE client.

2. Operating environment
    Software development environment: KEIL MDK-ARM V5.30.0.0 or newer
    Hardware environment: N32WB031_STB

3. Instructions

    System configuration:
        1. Clock source: HSI=64M, AHB=64M, APB1=32M, APB2=64M
        2. LED1: PB0 (power / run)
        3. LED2: PA6 (BLE connection status)
        4. Bluetooth device name: N32WB031_RDTSS
        5. BLE data bridge USART: USART1 PB6(TX), PB7(RX), 115200-8-N-1

    Usage:
        1. Open the project in MDK-ARM
        2. Build and download to the board
        3. Open a BLE debugging app such as nRF Connect or LightBlue
        4. Search and connect to N32WB031_RDTSS
        5. Enable notification and send data to verify two-way transparent transmission

4. Matters needing attention
    1. The program may enter sleep mode, so SWD may need reset timing when re-downloading.
    2. This project is based on the official BLE stack example and is intended as a teaching-friendly copy.
