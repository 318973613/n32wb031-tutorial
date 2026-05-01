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
 * @file ns_log_lpuart.c
 * @author NSING Firmware Team
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

/** @addtogroup 
 * @{
 */

 /* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "n32wb03x.h"
#include "ns_log.h"
#if (NS_LOG_LPUART_ENABLE)
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define _LPUART1_COM_    (0)

#if (_LPUART1_COM_ == 0)
#define LPUARTx             LPUART1
#define LPUARTx_CLK         RCC_LPUART1CLK
#define LPUARTx_GPIO        GPIOB
#define LPUARTx_GPIO_CLK    RCC_APB2_PERIPH_GPIOB
#define LPUARTx_RxPin       GPIO_PIN_2
#define LPUARTx_TxPin       GPIO_PIN_1
#define LPUARTx_Rx_GPIO_AF  GPIO_AF4_LPUART1
#define LPUARTx_Tx_GPIO_AF  GPIO_AF4_LPUART1
#else
#define LPUARTx             LPUART1
#define LPUARTx_CLK         RCC_LPUART1CLK
#define LPUARTx_GPIO        GPIOB
#define LPUARTx_GPIO_CLK    RCC_APB2_PERIPH_GPIOB
#define LPUARTx_RxPin       GPIO_PIN_11
#define LPUARTx_TxPin       GPIO_PIN_12
#define LPUARTx_Rx_GPIO_AF  GPIO_AF2_LPUART1
#define LPUARTx_Tx_GPIO_AF  GPIO_AF2_LPUART1
#endif


/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
//extern void lpuart_init(void);
void RCC_Configuration(uint32_t LPUART_CLK_SRC);
void GPIO_Configuration(void);
/* Private functions ---------------------------------------------------------*/



/**
 * @brief  init the log feature 
 */
void ns_log_lpuart_init(void)
{
    LPUART_InitType LPUART_InitStructure;
    /* System Clocks Configuration */
#if 0    
    RCC_Configuration(RCC_LPUARTCLK_SRC_LSI_LSE);
#else
    RCC_Configuration(RCC_LPUARTCLK_SRC_APB1);
#endif
    
    /* Configure the GPIO ports */
    GPIO_Configuration();

    LPUART_DeInit(LPUARTx);
    /* LPUARTx configuration ------------------------------------------------------*/
    LPUART_InitStructure.BaudRate            = 115200;
    LPUART_InitStructure.Parity              = LPUART_PE_NO;
    LPUART_InitStructure.RtsThreshold        = LPUART_RTSTH_FIFOFU;
    LPUART_InitStructure.HardwareFlowControl = LPUART_HFCTRL_NONE;
    LPUART_InitStructure.Mode                = LPUART_MODE_TX;
    /* Configure LPUARTx */
    LPUART_Init(LPUARTx, &LPUART_InitStructure);

}

/**
 * @brief  Configures the different system clocks.
 * @param  LPUART_CLK_SRC: specifies the LPUARTx clock source.
 */
void RCC_Configuration(uint32_t LPUART_CLK_SRC)
{
    switch(LPUART_CLK_SRC)
    {
        case RCC_LPUARTCLK_SRC_LSI_LSE:
        {  
            /* Specifies the LPUARTx clock source, LSE selected as LPUARTx clock */
            RCC_ConfigLpuartClk(LPUARTx_CLK, RCC_LPUARTCLK_SRC_LSI_LSE);
        }
        break;
        default:
        {
            /* Specifies the LPUARTx clock source, APB1 selected as LPUARTx clock */
            RCC_ConfigLpuartClk(LPUARTx_CLK, RCC_LPUARTCLK_SRC_APB1);
        }
        break;
    }    
    
    /* Enable LPUARTx Clock */
    RCC_EnableLpuartClk(ENABLE);    
}

/**
 * @brief  Configures the different GPIO ports.
 */
void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;
    
    /* Enable GPIO clock */
    RCC_EnableAPB2PeriphClk(LPUARTx_GPIO_CLK | RCC_APB2_PERIPH_AFIO, ENABLE);
    
    /* Initialize GPIO_InitStructure */
    GPIO_InitStruct(&GPIO_InitStructure);  
    /* Configure LPUARTx Tx as alternate function push-pull */
    GPIO_InitStructure.Pin            = LPUARTx_TxPin;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = LPUARTx_Tx_GPIO_AF;
    GPIO_InitPeripheral(LPUARTx_GPIO, &GPIO_InitStructure);

}

void ns_log_lpuart_deinit(void)
{

}

/* retarget the C library printf function to the LPUARTx */
int fputc(int ch, FILE* f)
{
    static uint8_t func_lock = 1;
    if(func_lock)
    {
        func_lock = 0;
        LPUART_SendData(LPUARTx, (uint8_t)ch);
        while (LPUART_GetFlagStatus(LPUARTx, LPUART_FLAG_TXC) == RESET);
        LPUART_ClrFlag(LPUARTx, LPUART_FLAG_TXC); 
        func_lock = 1;
    }        
    
    return (ch);
}
#endif
/**
 * @}
 */

