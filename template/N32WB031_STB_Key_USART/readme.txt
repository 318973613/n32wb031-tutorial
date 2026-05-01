1. Function description
    1. This project demonstrates button input and USART log output on N32WB031-STB.
    2. KEY1(PB1) switches LED animation mode.
    3. KEY2(PB2) pauses/resumes animation and prints current status.
    4. USART1(PB6/PB7) outputs a command menu and accepts serial commands.

2. Use environment
    Software development environment: KEIL MDK-ARM V5.30.0.0 or newer
    Hardware environment: N32WB031_STB

3. Instructions for use
    System Configuration:
        1. SystemClock: HSI 64MHz
        2. LED1: PB0
        3. LED2: PA6
        4. KEY1: PB1
        5. KEY2: PB2
        6. USART1: PB6(TX), PB7(RX), 115200-8-N-1
    Instructions:
        1. Open the project in MDK-ARM
        2. Build and download to the board
        3. Open serial assistant and set 115200 baud
        4. Press KEY1 / KEY2 or send commands from serial terminal

4. Matters needing attention
    1. This project uses polling for button scan and serial command processing.
    2. If no serial output is seen, first confirm the PACK and USART wiring settings.
