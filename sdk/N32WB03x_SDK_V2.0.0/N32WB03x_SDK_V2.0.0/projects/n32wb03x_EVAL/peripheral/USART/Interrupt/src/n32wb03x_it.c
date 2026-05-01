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
 * @file n32wb03x_it.c
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#include "n32wb03x_it.h"
#include "main.h"

/** @addtogroup N32WB03X_StdPeriph_Template
 * @{
 */

extern uint8_t TxBuffer1[];
extern uint8_t TxBuffer2[];
extern uint8_t RxBuffer1[];
extern uint8_t RxBuffer2[];
extern __IO uint8_t TxCounter1;
extern __IO uint8_t TxCounter2;
extern __IO uint8_t RxCounter1;
extern __IO uint8_t RxCounter2;
extern uint8_t NbrOfDataToTransfer1;
extern uint8_t NbrOfDataToTransfer2;
extern uint8_t NbrOfDataToRead1;
extern uint8_t NbrOfDataToRead2;

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
 * @brief  This function handles SVCall exception.
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles PendSV_Handler exception.
 */
void PendSV_Handler(void)
{
}

/**
 * @brief  This function handles SysTick Handler.
 */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 N32WB03X Peripherals Interrupt Handlers                     */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_n32wb03x.s).                                                 */
/******************************************************************************/

/**
 * @brief  This function handles USARTy global interrupt request.
 */
void USARTy_IRQHandler(void)
{
    if (USART_GetIntStatus(USARTy, USART_INT_RXDNE) != RESET)
    {
        /* Read one byte from the receive data register */
        RxBuffer1[RxCounter1++] = USART_ReceiveData(USARTy);

        if (RxCounter1 == NbrOfDataToRead1)
        {
            /* Disable the USARTy Receive interrupt */
            USART_ConfigInt(USARTy, USART_INT_RXDNE, DISABLE);
        }
    }

    if (USART_GetIntStatus(USARTy, USART_INT_TXDE) != RESET)
    {
        /* Write one byte to the transmit data register */
        USART_SendData(USARTy, TxBuffer1[TxCounter1++]);

        if (TxCounter1 == NbrOfDataToTransfer1)
        {
            /* Disable the USARTy Transmit interrupt */
            USART_ConfigInt(USARTy, USART_INT_TXDE, DISABLE);
        }
    }
}

/**
 * @brief  This function handles USARTz global interrupt request.
 */
void USARTz_IRQHandler(void)
{
    if (USART_GetIntStatus(USARTz, USART_INT_RXDNE) != RESET)
    {
        /* Read one byte from the receive data register */
        RxBuffer2[RxCounter2++] = USART_ReceiveData(USARTz);

        if (RxCounter2 == NbrOfDataToRead1)
        {
            /* Disable the USARTz Receive interrupt */
            USART_ConfigInt(USARTz, USART_INT_RXDNE, DISABLE);
        }
    }

    if (USART_GetIntStatus(USARTz, USART_INT_TXDE) != RESET)
    {
        /* Write one byte to the transmit data register */
        USART_SendData(USARTz, TxBuffer2[TxCounter2++]);

        if (TxCounter2 == NbrOfDataToTransfer2)
        {
            /* Disable the USARTz Transmit interrupt */
            USART_ConfigInt(USARTz, USART_INT_TXDE, DISABLE);
        }
    }
}

/**
 * @}
 */
