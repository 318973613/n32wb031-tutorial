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
 * @file main.h
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include "n32wb03x.h"


    
typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
} Status;
#define GLOBAL_INT_DISABLE()         \
uint32_t ui32IntStatus = 0;          \
do{                                  \
    ui32IntStatus = __get_PRIMASK(); \
    __set_PRIMASK(1);                \
}while(0)

#define GLOBAL_INT_RESTORE()      \
do{                               \
    __set_PRIMASK(ui32IntStatus); \
}while(0)

#define _USART1_COM_  //don't use it with ADC test at the same time, PB7 as ADC CH4. 
//#define _USART2_COM_

#ifdef _USART1_COM_
#define USARTx              USART1
#define USARTx_CLK          RCC_APB2_PERIPH_USART1
#define USARTx_GPIO         GPIOB
#define USARTx_GPIO_CLK     RCC_APB2_PERIPH_GPIOB
#define USARTx_RxPin        GPIO_PIN_7
#define USARTx_TxPin        GPIO_PIN_6
#define USARTx_Rx_GPIO_AF   GPIO_AF4_USART1
#define USARTx_Tx_GPIO_AF   GPIO_AF4_USART1
#define GPIO_APBxClkCmd     RCC_EnableAPB2PeriphClk
#define USART_APBxClkCmd    RCC_EnableAPB2PeriphClk
#endif

#ifdef _USART2_COM_
#define USARTx              USART2
#define USARTx_CLK          RCC_APB1_PERIPH_USART2
#define USARTx_GPIO         GPIOB
#define USARTx_GPIO_CLK     RCC_APB2_PERIPH_GPIOB
#define USARTx_RxPin        GPIO_PIN_5
#define USARTx_TxPin        GPIO_PIN_4
#define USARTx_Rx_GPIO_AF   GPIO_AF3_USART2
#define USARTx_Tx_GPIO_AF   GPIO_AF3_USART2
#define GPIO_APBxClkCmd     RCC_EnableAPB2PeriphClk
#define USART_APBxClkCmd    RCC_EnableAPB1PeriphClk
#endif


#define LED1_PORT GPIOB
#define LED1_PIN  GPIO_PIN_0
#define LED2_PORT GPIOA
#define LED2_PIN  GPIO_PIN_6

#define KEY1_INPUT_PORT        GPIOB
#define KEY1_INPUT_PIN         GPIO_PIN_1
#define KEY1_INPUT_EXTI_LINE   EXTI_LINE1
#define KEY1_INPUT_PORT_SOURCE GPIOB_PORT_SOURCE
#define KEY1_INPUT_PIN_SOURCE  GPIO_PIN_SOURCE1
#define KEY1_INPUT_IRQn        EXTI0_1_IRQn

#define KEY2_INPUT_PORT        GPIOB
#define KEY2_INPUT_PIN         GPIO_PIN_2
#define KEY2_INPUT_EXTI_LINE   EXTI_LINE2
#define KEY2_INPUT_PORT_SOURCE GPIOB_PORT_SOURCE
#define KEY2_INPUT_PIN_SOURCE  GPIO_PIN_SOURCE2
#define KEY2_INPUT_IRQn        EXTI2_3_IRQn



void LedBlink(GPIO_Module* GPIOx, uint16_t Pin);
void LedOn(GPIO_Module* GPIOx, uint16_t Pin);
void LedOff(GPIO_Module* GPIOx, uint16_t Pin);
uint16_t ADC_GetData_blocking(uint32_t ADC_Channel);
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
