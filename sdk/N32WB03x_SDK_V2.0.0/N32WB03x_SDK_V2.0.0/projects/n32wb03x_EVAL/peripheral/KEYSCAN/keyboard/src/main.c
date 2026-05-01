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
void KEYSCAN_Configuration(uint8_t key_mask, uint8_t mode, uint8_t dts, uint8_t wts, bool int_en);
NVIC_InitType NVIC_InitStructure;
EXTI_InitType EXTI_InitStructure;
GPIO_InitType GPIO_InitStructure; 
KEYSCAN_InitType KEYSCAN_InitStructure;
uint16_t key_receive = 0;

   
/**
 * @brief  Main program.
 */
int main(void)
{
    log_init();
    log_info("\nThis is KEYSCAN demo in 44 key mode with 8 GPIO!\n");
    log_info("Connect each other PA0, PA1, PA2, PA3, PA6, PB8, PB9, PB10,\n");
    log_info("Or connect one of then to GND or VCC as one key pressed. \n");
    log_info("                                                                               VCC\n");
    log_info("PA0-----------------------------------------------------------KEY_29-|--KEY_37--|\n");
    log_info("PA1---KEY_1---------------------------------------------------KEY_30-|--KEY_38--|\n");
    log_info("PA2---KEY_2---KEY_3-------------------------------------------KEY_31-|--KEY_39--|\n");
    log_info("PA3---KEY_4---KEY_5---KEY_6-----------------------------------KEY_32-|--KEY_40--|\n");
    log_info("PA6---KEY_7---KEY_8---KEY_9---KEY_10--------------------------KEY_33-|--KEY_41--|\n");
    log_info("PB10--KEY_11--KEY_12--KEY_13--KEY_14--KEY_15------------------KEY_34-|--KEY_42--|\n");
    log_info("PB8---KEY_16--KEY_17--KEY_18--KEY_19--KEY_20--KEY_21----------KEY_35-|--KEY_43--|\n");
    log_info("PB9---KEY_22--KEY_23--KEY_24--KEY_25--KEY_26--KEY_27--KEY_28--KEY_36-|--KEY_44--|\n");
    log_info("                                                                    GND          \n");
    
    KEYSCAN_Configuration(KEY_44,MODE_FIXED_INTV, DTS_10MS, WTS_0MS, INT_EN);
    while (1)
    {
        if(key_receive)
        {
            log_info("Keyboard detect KEY_%d pressed.\n",key_receive);
            key_receive = 0;// clear last key value
        }
    }
}

/**
 * @brief  KEYSCAN Configuration
 * @param  @KEY_MASK     :   KEY_65, KEY_44, KEY_104
 * @param  @KEY_MODE     :   MODE_FIXED_INTV, MODE_SW_TRIG, MODE_PRESS_TRIG,
 * @param  @KEY_DTS      :   DTS_10MS, DTS_20MS, DTS_40MS, DTS_80MS, DTS_160MS, DTS_320MS, DTS_640MS, DTS_640MS_2
 * @param  @KEY_WTS      :   WTS_0MS, WTS_32MS, WTS_64MS, WTS_96MS, WTS_128MS, WTS_160MS, WTS_192MS, WTS_224MS
 * @param  @KEY_INT      :   INT_DIS, INT_EN
 * @return Return value
 * @note   Note
 */
void KEYSCAN_Configuration(uint8_t key_mask, uint8_t mode, uint8_t dts, uint8_t wts, bool int_en)
{
    /* KEYSCAN CLK EN */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, ENABLE);
    RCC->LSCTRL |= RCC_LSCTRL_KEYSCANEN;
     /* config GPIO */
    GPIO_InitStruct(&GPIO_InitStructure);
    if(key_mask == KEY_65)
    {
        GPIO_InitStructure.Pin =     GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;        
    }
    else if(key_mask == KEY_44)
    {
        GPIO_InitStructure.Pin =     GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6;
    }
    else
    {
        GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
    }
    
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_OD;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF5;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = 0; //deinit
    if(key_mask == KEY_65 ||key_mask == KEY_44)
    {
        GPIO_InitStructure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;        
    }
    else
    {
        GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    }
    
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_OD;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF5;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

    /* config KEYSCAN */
    KEYSCAN_InitStructure.Mask = key_mask;
    KEYSCAN_InitStructure.Mode = mode;
    KEYSCAN_InitStructure.Wts = wts;
    KEYSCAN_InitStructure.Dts = dts;
    KEYSCAN_InitStructure.Int_en = int_en;
    KEYSCAN_Init(&KEYSCAN_InitStructure);
    /* config intterrupt */
    if(int_en)
    {
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Line = EXTI_LINE13;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
        EXTI_InitPeripheral(&EXTI_InitStructure);
            
        
        NVIC_DisableIRQ(KEYSCAN_IRQn);
        /* Enable the KEYSCAN Interrupt */
        NVIC_InitStructure.NVIC_IRQChannel                   = KEYSCAN_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPriority           = 3;
        NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        
        NVIC_EnableIRQ(KEYSCAN_IRQn);
    }
    /* Enable KEYSCAN */
    KEYSCAN_Enable(ENABLE);
}


/**
 * @}
 */

/**
 * @}
 */
