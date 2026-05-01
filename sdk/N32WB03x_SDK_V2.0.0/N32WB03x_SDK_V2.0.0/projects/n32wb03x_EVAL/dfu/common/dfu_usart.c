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
 * @file dfu_usart.c
 * @author NSING Firmware Team
 * @version v1.0.4
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

/** @addtogroup 
 * @{
 */

 /* Includes ------------------------------------------------------------------*/
#include "dfu_usart.h"
#include "n32wb03x.h"
#include <stdio.h>
#include <string.h>
extern void system_delay_cycles(uint32_t i);

/**
 * @brief  config rcc of usart1
 * @param  
 * @return 
 * @note   
 */
static void rcc_configure_usart1(void)
{ 
    RCC->APB2PCLKEN |= RCC_APB2_PERIPH_GPIOA|RCC_APB2_PERIPH_GPIOB;
    RCC->APB2PCLKEN |= RCC_APB2_PERIPH_USART1;
}

/**
 * @brief  config gpio of usart1
 * @param  
 * @return 
 * @note   
 */
static void gpio_configure_usart1(uint8_t gpio)
{
    GPIO_InitType GPIO_InitStructure;
    
    GPIO_InitStruct(&GPIO_InitStructure);
    if(gpio == USART1_GPIO_PA45)
    {
        GPIO_InitStructure.Pin              =    GPIO_PIN_4;
        GPIO_InitStructure.GPIO_Mode        =    GPIO_MODE_AF_PP;
        GPIO_InitStructure.GPIO_Alternate   =    GPIO_AF3_USART1;
        GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
        GPIO_InitStructure.Pin              =    GPIO_PIN_5;
        GPIO_InitStructure.GPIO_Alternate   =    GPIO_AF3_USART1;
        GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure); 
        
    }
    else {
        
        GPIO_InitStructure.Pin              =    GPIO_PIN_4;
        GPIO_InitStructure.GPIO_Mode        =    GPIO_MODE_AF_PP;
        GPIO_InitStructure.GPIO_Alternate   =    GPIO_AF0_SWD;
        GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
        GPIO_InitStructure.Pin              =    GPIO_PIN_5;
        GPIO_InitStructure.GPIO_Alternate   =    GPIO_AF0_SWD;
        GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure); 
        
        GPIO_InitStructure.Pin              =    GPIO_PIN_6;
        GPIO_InitStructure.GPIO_Mode        =    GPIO_MODE_AF_PP;
        GPIO_InitStructure.GPIO_Alternate   =    GPIO_AF4_USART1;
        GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
        GPIO_InitStructure.Pin              =    GPIO_PIN_7;
        GPIO_InitStructure.GPIO_Alternate   =    GPIO_AF4_USART1;
        GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);           
    }        
    
    
}

/**
 * @brief  config usart1
 * @param  
 * @return 
 * @note   
 */
static void config_usart1(uint32_t baud)
{
    USART_InitType USART_InitStructure;
    
    USART_InitStructure.BaudRate                   =      baud;
    USART_InitStructure.WordLength                 =      USART_WL_8B;
    USART_InitStructure.StopBits                   =      USART_STPB_1;
    USART_InitStructure.Parity                     =      USART_PE_NO;
    USART_InitStructure.HardwareFlowControl        =      USART_HFCTRL_NONE;
    USART_InitStructure.Mode                       =      USART_MODE_RX | USART_MODE_TX;
    
    USART_Init(USART1, &USART_InitStructure);

}

/**
 * @brief  config usart1 for dfu 
 * @param  
 * @return 
 * @note   
 */

void dfu_usart1_poll_config(uint8_t gpio,uint32_t baud)
{
    
    rcc_configure_usart1();
    gpio_configure_usart1(gpio);
    dfu_usart1_disable();
    config_usart1(baud);
    dfu_usart1_enable();
}

/**
 * @brief  enable usart1
 * @param  
 * @return 
 * @note   
 */
void dfu_usart1_enable(void)
{
    USART_Enable(USART1, ENABLE);
}


/**
 * @brief  disable usart1
 * @param  
 * @return 
 * @note   
 */
void dfu_usart1_disable(void)
{
    USART_Enable(USART1, DISABLE);
}

/**
 * @brief  send data via usart1
 * @param  
 * @return 
 * @note   
 */
void dfu_usart1_send(uint8_t *p_data, uint32_t len)
{
    for(uint32_t i=0;i<len;i++)
    {
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXDE) != SET);
        USART_SendData(USART1, p_data[i]);
    }
}


/**
 * @brief  receive data from usart1
 * @param  
 * @return 
 * @note   
 */
uint8_t dfu_usart1_receive(uint8_t *p_data)
{
    if(USART_GetFlagStatus(USART1, USART_FLAG_RXDNE) != RESET)
    {
        *p_data = USART_ReceiveData(USART1);
        return 1;
    }
    return 0;
}

uint8_t dfu_usart1_poll_boot_byte(uint8_t byte, uint32_t timeout)
{  
    while(timeout--){
        if(USART_GetFlagStatus(USART1, USART_FLAG_RXDNE) != RESET)
        {
            if(USART_ReceiveData(USART1) == byte){
                system_delay_cycles(10);
                while(USART_GetFlagStatus(USART1, USART_FLAG_TXDE) != SET);
                USART_SendData(USART1, 0x79);              
                return 1;
            }
        }    
    }
    return 0;
}


/**
 * @brief  config nvic of usart1
 * @param  
 * @return 
 * @note   
 */
static void nvic_configure_usart1(void)
{
    NVIC_InitType NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel              =            USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority      =            0;
    NVIC_InitStructure.NVIC_IRQChannelCmd           =            ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

/**
 * @brief  enable interrupt of usart1
 * @param  
 * @return 
 * @note   
 */
static void config_interrupt_usart1(void)
{
    USART_ConfigInt(USART1, USART_INT_RXDNE, ENABLE);
}

/**
 * @brief  config usart1 and enable interrupt 
 * @param  
 * @return 
 * @note   
 */
void dfu_usart1_interrupt_config(void)
{
    nvic_configure_usart1();
    config_interrupt_usart1();
}



void dfu_usart1_default_config(void)
{   
    
    RCC->APB2PRST |= RCC_APB2_PERIPH_AFIO|RCC_APB2_PERIPH_GPIOA|RCC_APB2_PERIPH_GPIOB|RCC_APB2_PERIPH_USART1;
    RCC->APB2PRST &= ~(RCC_APB2_PERIPH_AFIO|RCC_APB2_PERIPH_GPIOA|RCC_APB2_PERIPH_GPIOB|RCC_APB2_PERIPH_USART1);

    RCC->APB2PCLKEN &= ~(RCC_APB2_PERIPH_AFIO|RCC_APB2_PERIPH_GPIOA|RCC_APB2_PERIPH_GPIOB|RCC_APB2_PERIPH_USART1);
}

