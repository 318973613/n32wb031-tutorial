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
 * @file app_usart.h
 * @author NSING Firmware Team
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#ifndef __APP_USART_H__
#define __APP_USART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32wb03x.h"

    
/* Public typedef -----------------------------------------------------------*/
/* Public define ------------------------------------------------------------*/
#define _USART1_COM_
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

#define USARTx_DAT_Base         (USART1_BASE + 0x04)
#define USARTx_Tx_DMA_Channel   DMA_CH1
#define USARTx_Tx_DMA_FLAG      DMA_FLAG_TC1
#define USARTx_Rx_DMA_Channel   DMA_CH2
#define USARTx_Rx_DMA_FLAG      DMA_FLAG_TC2
#define USARTx_Tx_DMA_REMAP     DMA_REMAP_USART1_TX
#define USARTx_Rx_DMA_REMAP     DMA_REMAP_USART1_RX
#define USARTx_IRQn             USART1_IRQn
#define USARTx_IRQHandler       USART1_IRQHandler
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

#define USARTx_DAT_Base         (USART2_BASE + 0x04)
#define USARTx_Tx_DMA_Channel   DMA_CH1
#define USARTx_Tx_DMA_FLAG      DMA_FLAG_TC1 
#define USARTx_Rx_DMA_Channel   DMA_CH2
#define USARTx_Rx_DMA_FLAG      DMA_FLAG_TC2
#define USARTx_Tx_DMA_REMAP     DMA_REMAP_USART2_TX
#define USARTx_Rx_DMA_REMAP     DMA_REMAP_USART2_RX
#define USARTx_IRQn             USART2_IRQn
#define USARTx_IRQHandler       USART2_IRQHandler
#endif


#define USART_RX_DMA_SIZE  256
#define USART_RX_FIFO_SIZE 1000
#define USART_TX_FIFO_SIZE 1000


/* Public macro -------------------------------------------------------------*/
/* Public variables ---------------------------------------------------------*/

/* Public function prototypes -----------------------------------------------*/

void app_usart_configuration(void);
uint8_t app_usart_rx_data_fifo_enter(const uint8_t *p_data, uint16_t len);
uint8_t usart_tx_dma_send(uint8_t *p_data, uint16_t len);
void app_usart_tx_data_blocking(uint8_t *p_data, uint16_t len);

uint8_t app_usart_tx_fifo_enter(const uint8_t *p_data, uint16_t len);
void usart_forward_to_ble_loop(void);
void app_usart_dma_enable(FunctionalState Cmd);
void app_usart_tx_process(void);
#ifdef __cplusplus
}
#endif

#endif /* __APP_USART_H__ */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
