
1. Function description

    Bluetooth BLE HID Report Host master example. 
    It can connect and bond the device with hidmouse slave example, then can recieve the report data from slave device.
    And it can reconect the slave device with whiitelist after disconnection or reboot.

2. Operating environment
    Software development environment: KEIL MDK-ARM V5.30.0.0
    Hardware environment: Developed based on the development board N32WB031_STB

3. Instructions

    System configuration;
        1. Clock source: HSI=64M, AHB=64M, APB1=32M, APB2=64M
        2. Port configuration:
            LED1: PB0
            LED2: PA6
            KEY1: PB1
            KEY2: PB2			
        3. Log print: DEMO board PB6(TX), baud rate: 115200, 8 data bits, 1 stop bit, no parity bit, no hardware flow control
        4. LED1 lights up when it is turned on, LED2 indicates the connection status, and lights up when Bluetooth is connected
        5. The counter test routine is the hid_mouse slave routine in the ble directory

    usage method:
        1. Burn to development board after compilation
        2. Power on development board and check the log.
        3. Connect the salve device.
		4. Press K1 on slave board, LED2 off in master board;
		5. Press K2 on slave board, LED2 on in master board;
		6. Press K1 on master board to erase all bond info.

4. Precautions
    Since the program will enter the sleep mode, the SWD will not be accessible. Press the RESET button to execute the program burning step within one second.