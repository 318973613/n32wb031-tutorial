1. Function description
    1. This project is a minimal blank template for N32WB031-STB.
    2. PB0 and PA6 are configured as push-pull outputs.
    3. After reset, the board runs a short startup animation.
    4. Then the LEDs enter an alternating heartbeat pattern.

2. Use environment
    Software development environment: KEIL MDK-ARM V5.30.0.0 or newer
    Hardware environment: N32WB031_STB

3. Instructions for use
    System Configuration:
        1. SystemClock: HSI 64MHz
        2. GPIO: PB0 and PA6 output LED pattern
    Instructions:
        1. Open the project in MDK-ARM
        2. Build and download to the board
        3. Reset or power-cycle the board
        4. LED1 and LED2 should show startup and heartbeat animation

4. Matters needing attention
    1. This template only keeps the minimum files required to start from scratch.
    2. Add new peripheral drivers in the MDK project as your application grows.
