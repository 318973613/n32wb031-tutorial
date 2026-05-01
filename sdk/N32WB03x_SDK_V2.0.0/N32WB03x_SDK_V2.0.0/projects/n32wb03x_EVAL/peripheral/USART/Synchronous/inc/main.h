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

#include "n32wb03x.h"

#define _USART1_SPI1_
//#define _USART2_SPI2_

#ifdef _USART1_SPI1_
#define USARTy              USART1
#define USARTy_CLK          RCC_APB2_PERIPH_USART1
#define USARTy_GPIO         GPIOB
#define USARTy_Clk_GPIO     GPIOA
#define USARTy_GPIO_CLK     RCC_APB2_PERIPH_GPIOB
#define USARTy_RxPin        GPIO_PIN_7
#define USARTy_TxPin        GPIO_PIN_6
#define USARTy_ClkPin       GPIO_PIN_6
#define USARTy_Rx_GPIO_AF   GPIO_AF4_USART1
#define USARTy_Tx_GPIO_AF   GPIO_AF4_USART1
#define USARTy_Clk_GPIO_AF  GPIO_AF3_USART1
#define USARTy_APBxClkCmd   RCC_EnableAPB2PeriphClk

#define SPIy                SPI1
#define SPIy_CLK            RCC_APB2_PERIPH_SPI1
#define SPIy_GPIO           GPIOA
#define SPIy_GPIO_CLK       RCC_APB2_PERIPH_GPIOA
#define SPIy_SCKPin         GPIO_PIN_1
#define SPIy_MISOPin        GPIO_PIN_3
#define SPIy_MOSIPin        GPIO_PIN_2
#define SPIy_SCK_GPIO_AF    GPIO_AF1_SPI1
#define SPIy_MISO_GPIO_AF   GPIO_AF1_SPI1
#define SPIy_MOSI_GPIO_AF   GPIO_AF1_SPI1
#define SPIy_APBxClkCmd     RCC_EnableAPB2PeriphClk
#endif

#ifdef _USART2_SPI2_
#define USARTy              USART2
#define USARTy_CLK          RCC_APB1_PERIPH_USART2
#define USARTy_GPIO         GPIOB
#define USARTy_Clk_GPIO     GPIOB
#define USARTy_GPIO_CLK     RCC_APB2_PERIPH_GPIOB
#define USARTy_RxPin        GPIO_PIN_5
#define USARTy_TxPin        GPIO_PIN_4
#define USARTy_ClkPin       GPIO_PIN_13
#define USARTy_Rx_GPIO_AF   GPIO_AF3_USART1
#define USARTy_Tx_GPIO_AF   GPIO_AF3_USART1
#define USARTy_Clk_GPIO_AF  GPIO_AF3_USART1
#define USARTy_APBxClkCmd   RCC_EnableAPB1PeriphClk

#define SPIy                SPI2
#define SPIy_CLK            RCC_APB2_PERIPH_SPI2
#define SPIy_GPIO           GPIOB
#define SPIy_GPIO_CLK       RCC_APB2_PERIPH_GPIOB
#define SPIy_SCKPin         GPIO_PIN_1
#define SPIy_MISOPin        GPIO_PIN_3
#define SPIy_MOSIPin        GPIO_PIN_2
#define SPIy_SCK_GPIO_AF    GPIO_AF2_SPI2
#define SPIy_MISO_GPIO_AF   GPIO_AF2_SPI2
#define SPIy_MOSI_GPIO_AF   GPIO_AF2_SPI2
#define SPIy_APBxClkCmd     RCC_EnableAPB2PeriphClk
#endif
 
 
typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
} TestStatus;

void RCC_Configuration(void);
void GPIO_Configuration(void);
void SPI_Configuration(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */
