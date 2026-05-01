/**
*     Copyright (c) 2025, NSING Technologies Inc.
* 
*     All rights reserved.
*
*     This software is the exclusive property of NSING Technologies Inc. (Hereinafter 
* referred to as NSING). This software, and the product of NSING described herein 
* (Hereinafter referred to as the Product) are owned by NSING under the laws and treaties
* of the People's Republic of China and other applicable jurisdictions worldwide.
*
*     NSING does not grant any license under its patents, copyrights, trademarks, or other 
* intellectual property rights. Names and brands of third party may be mentioned or referred 
* thereto (if any) for identification purposes only.
*
*     NSING reserves the right to make changes, corrections, enhancements, modifications, and 
* improvements to this software at any time without notice. Please contact NSING and obtain 
* the latest version of this software before placing orders.

*     Although NSING has attempted to provide accurate and reliable information, NSING assumes 
* no responsibility for the accuracy and reliability of this software.
* 
*     It is the responsibility of the user of this software to properly design, program, and test 
* the functionality and safety of any application made of this information and any resulting product. 
* In no event shall NSING be liable for any direct, indirect, incidental, special,exemplary, or 
* consequential damages arising in any way out of the use of this software or the Product.
*
*     NSING Products are neither intended nor warranted for usage in systems or equipment, any
* malfunction or failure of which may cause loss of human life, bodily injury or severe property 
* damage. Such applications are deemed, "Insecure Usage".
*
*     All Insecure Usage shall be made at user's risk. User shall indemnify NSING and hold NSING 
* harmless from and against all claims, costs, damages, and other liabilities, arising from or related 
* to any customer's Insecure Usage.

*     Any express or implied warranty with regard to this software or the Product, including,but not 
* limited to, the warranties of merchantability, fitness for a particular purpose and non-infringement
* are disclaimed to the fullest extent permitted by law.

*     Unless otherwise explicitly permitted by NSING, anyone may not duplicate, modify, transcribe
* or otherwise distribute this software for any purposes, in whole or in part.
*
*     NSING products and technologies shall not be used for or incorporated into any products or systems
* whose manufacture, use, or sale is prohibited under any applicable domestic or foreign laws or regulations. 
* User shall comply with any applicable export control laws and regulations promulgated and administered by 
* the governments of any countries asserting jurisdiction over the parties or transactions.
**/


/**
 * @file main.c
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#include <stdio.h>
#include "main.h"
/** @addtogroup N32WB03X_StdPeriph_Examples
 * @{
 */
extern void system_delay_n_10us(uint32_t value);


/**
 * @brief  Configures the GPIO ports.
 */
void GPIO_Configuration(void)
{
	GPIO_InitType GPIO_InitStructure;
    GPIO_InitStruct(&GPIO_InitStructure);
	GPIO_InitStructure.Pin 				= GPIO_PIN_4;
	GPIO_InitStructure.GPIO_Mode 		= GPIO_MODE_AF_PP;
	GPIO_InitStructure.GPIO_Pull 		= GPIO_NO_PULL;
	GPIO_InitStructure.GPIO_Alternate 	= GPIO_AF2;
	GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
}

/**
 * @brief  IRgen module send data in NEC protocol
 * @param   
 * @return  
 * @note    
 */
void irgen_module_send_NEC(uint16_t address, uint16_t command)
{
	// Set carrier frequency 38k. 64M/38k = 1684, 1684*1/3 = 561,1684*2/3 = 1122.6
	IRC->FREQ_CARRIER_ON = 561; 	 
	IRC->FREQ_CARRIER_OFF = 1122;	
	
	// Set "logic 0" parameters
	IRC->LOGIC_ZERO_TIME = 0x1515; // mark and space duration = 1 clock cycle
	
	// Set "logic 1" parameters
	IRC->LOGIC_ONE_TIME = 0x153f;  // same here
	
	IRC->REPEAT_TIME = 4180;  //110ms*38k = 4180
	
	IRC->CTRL |= IR_CTRL_ENABLE_Msk;
	
	hw_irgen_insert_paint_message(HW_IRGEN_FIFO_CODE,HW_IRGEN_PAINT_MARK,342);  // 9ms*38K = 342
	hw_irgen_insert_paint_message(HW_IRGEN_FIFO_CODE,HW_IRGEN_PAINT_SPACE,171);	// 4.5ms*38K = 171	
    
    hw_irgen_insert_digital_message(HW_IRGEN_FIFO_CODE, 16, address); //((~address)<<8|address)
    
	hw_irgen_insert_digital_message(HW_IRGEN_FIFO_CODE, 16, ((~command)<<8|command));
	hw_irgen_insert_paint_message(HW_IRGEN_FIFO_CODE,HW_IRGEN_PAINT_MARK,21); // 560us*38k = 21
	
	hw_irgen_insert_paint_message(HW_IRGEN_FIFO_REPEAT,HW_IRGEN_PAINT_MARK,342); // 9ms*38K = 342
	hw_irgen_insert_paint_message(HW_IRGEN_FIFO_REPEAT,HW_IRGEN_PAINT_SPACE,85); // 2.25ms*38k = 85.5
	hw_irgen_insert_paint_message(HW_IRGEN_FIFO_REPEAT,HW_IRGEN_PAINT_MARK,21);	 // 560us*38k = 21.28	
    
    hw_irgen_start();
}

/**
 * @brief  IRgen module send data in RC5 protocol
 * @param   
 * @return  
 * @note    
 */
void irgen_module_send_RC5(uint16_t address, uint16_t command)
{
    static uint8_t toggle_bit = 0;
    uint16_t rc5_format;
	// Set carrier frequency 36k. 64M/36k = 1778, 1778*1/3 = 592,842*2/3 =1185
	IRC->FREQ_CARRIER_ON = 592; 	 
	IRC->FREQ_CARRIER_OFF = 1185;	
	
	// Set "logic 0" parameters  889us*36k = 32
	IRC->LOGIC_ZERO_TIME = 0x2020; // mark and space duration = 1 clock cycle
	
	// Set "logic 1" parameters 889us*36k = 32
	IRC->LOGIC_ONE_TIME = 0x2020;  // same here
	
	IRC->REPEAT_TIME = 4000;  // 111ms*36k = 4000
	
	IRC->CTRL |= IR_CTRL_ENABLE_Msk;
    IRC->CTRL |= IR_CTRL_LOGIC_ONE_FORMAT_Msk;   // logic 1 is space to mark 
    IRC->CTRL &= ~IR_CTRL_LOGIC_ZERO_FORMAT_Msk; // logic 0 is mark to space   
    
    // '1' '1' as start bit 
    hw_irgen_insert_paint_message(HW_IRGEN_FIFO_REPEAT,HW_IRGEN_PAINT_SPACE,32);	// 889us*36k = 32
	hw_irgen_insert_paint_message(HW_IRGEN_FIFO_REPEAT,HW_IRGEN_PAINT_MARK,32);  // 889us*36k = 32
	hw_irgen_insert_paint_message(HW_IRGEN_FIFO_REPEAT,HW_IRGEN_PAINT_SPACE,32);	// 889us*36k = 32
	hw_irgen_insert_paint_message(HW_IRGEN_FIFO_REPEAT,HW_IRGEN_PAINT_MARK,32);  // 889us*36k *2 = 32
	
    // Toggle Bit
    toggle_bit++;
    if(toggle_bit&0x01)
    {
        //'1'
        hw_irgen_insert_paint_message(HW_IRGEN_FIFO_REPEAT,HW_IRGEN_PAINT_SPACE,32);	// 889us*36k = 32	        
        hw_irgen_insert_paint_message(HW_IRGEN_FIFO_REPEAT,HW_IRGEN_PAINT_MARK,32);	// 889us*36k = 32	
    }else
    {
        //'0'
        hw_irgen_insert_paint_message(HW_IRGEN_FIFO_REPEAT,HW_IRGEN_PAINT_MARK,32);	// 889us*36k = 32	
        hw_irgen_insert_paint_message(HW_IRGEN_FIFO_REPEAT,HW_IRGEN_PAINT_SPACE,32);	// 889us*36k = 32	
    }
    
    // address and data
    rc5_format = hw_irgen_reverse_bit_order(address,5);
    hw_irgen_insert_digital_message(HW_IRGEN_FIFO_REPEAT, 5, rc5_format);

    rc5_format = hw_irgen_reverse_bit_order(command,6);
    hw_irgen_insert_digital_message(HW_IRGEN_FIFO_REPEAT, 6,rc5_format );
    
    hw_irgen_start();

}


/**
 * @brief  Main program.
 */
int main(void)
{
	uint16_t ADDRESS = 0x59A6;
	uint8_t COMMAND1 = 0x16;
	uint8_t COMMAND2 = 0x18;

    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);		
	RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_IRC, ENABLE);	
    
    GPIO_Configuration();
	
    #if 1
	irgen_module_send_NEC(ADDRESS,COMMAND1);
    system_delay_n_10us(100*500); // 500ms
    hw_irgen_stop();
    system_delay_n_10us(100);
    
    irgen_module_send_NEC(ADDRESS,COMMAND2);
    system_delay_n_10us(100*500); // 500ms
    hw_irgen_stop();
    system_delay_n_10us(100);
    #else
    irgen_module_send_RC5(ADDRESS,COMMAND1);
    system_delay_n_10us(100*500); // 500ms
    hw_irgen_stop();
    system_delay_n_10us(100);
    
    irgen_module_send_RC5(ADDRESS,COMMAND2);
    system_delay_n_10us(100*500); // 500ms
    hw_irgen_stop();
    system_delay_n_10us(100);
    #endif

    while (1)
    {
    }
}


#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param file pointer to the source file name
 * @param line assert_param error line source number
 */
void assert_failed(const uint8_t* expr, const uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}

#endif

/**
 * @}
 */

/**
 * @}
 */
