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

uint16_t capture = 0;
extern __IO uint16_t CCR1_Val;
extern __IO uint16_t CCR2_Val;
extern __IO uint16_t CCR3_Val;
extern __IO uint16_t CCR4_Val;

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
 * @brief  This function handles TIM3 global interrupt request.
 */
void TIM3_IRQHandler(void)
{
    /* TIM3_CH1 toggling with frequency = 488.28 Hz */
    if (TIM_GetIntStatus(TIM3, TIM_INT_CC1) != RESET)
    {
        TIM_ClrIntPendingBit(TIM3, TIM_INT_CC1);
        capture = TIM_GetCap1(TIM3);
        TIM_SetCmp1(TIM3, capture + CCR1_Val);
    }
    
    /* TIM3_CH2 toggling with frequency = 976.56 Hz */
    if (TIM_GetIntStatus(TIM3, TIM_INT_CC2) != RESET)
    {
        TIM_ClrIntPendingBit(TIM3, TIM_INT_CC2);
        capture = TIM_GetCap2(TIM3);
        TIM_SetCmp2(TIM3, capture + CCR2_Val);
    }

    /* TIM3_CH3 toggling with frequency = 1953.125 Hz */
    if (TIM_GetIntStatus(TIM3, TIM_INT_CC3) != RESET)
    {
        TIM_ClrIntPendingBit(TIM3, TIM_INT_CC3);
        capture = TIM_GetCap3(TIM3);
        TIM_SetCmp3(TIM3, capture + CCR3_Val);
    }

    /* TIM3_CH4 toggling with frequency = 3906.26 Hz */
    if (TIM_GetIntStatus(TIM3, TIM_INT_CC4) != RESET)
    {
        TIM_ClrIntPendingBit(TIM3, TIM_INT_CC4);
        capture = TIM_GetCap4(TIM3);
        TIM_SetCmp4(TIM3, capture + CCR4_Val);
    }
}

/**
 * @}
 */
