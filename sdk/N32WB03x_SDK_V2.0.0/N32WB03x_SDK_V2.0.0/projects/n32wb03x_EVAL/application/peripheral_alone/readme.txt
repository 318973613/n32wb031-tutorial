1. Function description
    1. LED1 and LED2 light up upon startup
    2. Short press K1 to enter the sleep mode, long press K1 to enter the sleep mode and
     RTC wake-up function, and short press K1 to wake up the sleep
    3. Short press K2 to read the value of ADC CH1 (PB10) and output it to the serial port
    4. Timer TIM3 CH1 (PB4) CH2 (PB5) PWM with the same output frequency and different duty cycle
    5. The serial port runs the loopback application with 115200 8n1 parameters, and the input 
    string of less than 256 bytes will return to the output immediately

2. Operating environment
    Software development environment: KEIL MDK-ARM V5.30.0.0
    Hardware environment: Developed based on the development board N32WB031_STB

3. Instructions
    System configuration;
        1. Clock source: HSI=64M, AHB=64M, APB1=32M, APB2=64M
        2. Port configuration:
            PB4 selects CH1 output of TIM3
            PB5 selects CH2 output of TIM3
            PB6 selects TX as USART
            PB7 selects RX as USART
            PB10 is selected as CH1 of ADC
            Select LED1 for PB0
            Select LED2 for PA6
            Select K1 for PB1
            Select K2 for PB2
        3. The frequency of TIM3 is 40KHz, the duty cycle of CH1 is 75%, and the duty cycle of CH2 is 50%
        4. Serial port uses USART1, the baud rate is 115200, 8bit data bit, no check bit, 1bit stop bit, no flow control.
        5. ADC CH1 can measure from 0.15 to 0.95V

    usage method:
        1. Burn to development board after compilation
        2. Observe the waveform of PB4 and PB5 with an oscilloscope or logic analyzer
        3. Connect the serial port tool to USART1
        4. After the program runs, LED1 and LED2 on the board light up
        6. Send any string (less than 256 bytes) to the serial port to receive and return the same string
        6. Short press K1 to go to sleep, and press K1 or K2 to wake up
        7. Long press K1 to enter sleep, and start the wake-up function of RTC. After 10 seconds, MCU will wake up.
        8. Short press K2 to flip LED1, read the ADC value, and convert it to voltage (mA).

4. Precautions
    1. If there is no HSE crystal on the board, please define macro NO on the option C/C++page_ HSE_ ON_ BOORD: Enable direct
    writing of HSI to trim values. The default value is 0x4F.
        If the frequency is wrong, you can fine-tune it by yourself. Later, you will write the correct trim value to 
    flash in production, and users only need to read and write the register.
    2. Program cannot be burned during MCU sleep. Please reset and burn.