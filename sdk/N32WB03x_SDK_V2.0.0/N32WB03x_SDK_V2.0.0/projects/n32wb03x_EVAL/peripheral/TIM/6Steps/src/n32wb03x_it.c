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

__IO uint32_t step = 1;

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
    /* Generate TIM1 COM event by software */
    TIM_GenerateEvent(TIM1, TIM_EVT_SRC_COM);
}

/******************************************************************************/
/*            n32wb03x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
 * @brief  This function handles TIM1 Break Update Trigger and commutation interrupts
 *   requests.
 */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    /* Clear TIM1 COM pending bit */
    TIM_ClrIntPendingBit(TIM1, TIM_INT_COM);

    if (step == 1)
    {
        /* Next step: Step 2 Configuration ---------------------------- */
        /*  Channel3 configuration */
        TIM_EnableCapCmpCh(TIM1, TIM_CH_3, TIM_CAP_CMP_DISABLE);
        TIM_EnableCapCmpChN(TIM1, TIM_CH_3, TIM_CAP_CMP_N_DISABLE);

        /*  Channel1 configuration */
        TIM_SelectOcMode(TIM1, TIM_CH_1, TIM_OCMODE_PWM1);
        TIM_EnableCapCmpCh(TIM1, TIM_CH_1, TIM_CAP_CMP_ENABLE);
        TIM_EnableCapCmpChN(TIM1, TIM_CH_1, TIM_CAP_CMP_N_DISABLE);

        /*  Channel2 configuration */
        TIM_SelectOcMode(TIM1, TIM_CH_2, TIM_OCMODE_PWM1);
        TIM_EnableCapCmpCh(TIM1, TIM_CH_2, TIM_CAP_CMP_DISABLE);
        TIM_EnableCapCmpChN(TIM1, TIM_CH_2, TIM_CAP_CMP_N_ENABLE);
        step++;
    }
    else if (step == 2)
    {
        /* Next step: Step 3 Configuration ---------------------------- */
        /*  Channel2 configuration */
        TIM_SelectOcMode(TIM1, TIM_CH_2, TIM_OCMODE_PWM1);
        TIM_EnableCapCmpCh(TIM1, TIM_CH_2, TIM_CAP_CMP_DISABLE);
        TIM_EnableCapCmpChN(TIM1, TIM_CH_2, TIM_CAP_CMP_N_ENABLE);

        /*  Channel3 configuration */
        TIM_SelectOcMode(TIM1, TIM_CH_3, TIM_OCMODE_PWM1);
        TIM_EnableCapCmpCh(TIM1, TIM_CH_3, TIM_CAP_CMP_ENABLE);
        TIM_EnableCapCmpChN(TIM1, TIM_CH_3, TIM_CAP_CMP_N_DISABLE);

        /*  Channel1 configuration */
        TIM_EnableCapCmpCh(TIM1, TIM_CH_1, TIM_CAP_CMP_DISABLE);
        TIM_EnableCapCmpChN(TIM1, TIM_CH_1, TIM_CAP_CMP_N_DISABLE);
        step++;
    }
    else if (step == 3)
    {
        /* Next step: Step 4 Configuration ---------------------------- */
        /*  Channel3 configuration */
        TIM_SelectOcMode(TIM1, TIM_CH_3, TIM_OCMODE_PWM1);
        TIM_EnableCapCmpCh(TIM1, TIM_CH_3, TIM_CAP_CMP_ENABLE);
        TIM_EnableCapCmpChN(TIM1, TIM_CH_3, TIM_CAP_CMP_N_DISABLE);

        /*  Channel2 configuration */
        TIM_EnableCapCmpCh(TIM1, TIM_CH_2, TIM_CAP_CMP_DISABLE);
        TIM_EnableCapCmpChN(TIM1, TIM_CH_2, TIM_CAP_CMP_N_DISABLE);

        /*  Channel1 configuration */
        TIM_SelectOcMode(TIM1, TIM_CH_1, TIM_OCMODE_PWM1);
        TIM_EnableCapCmpCh(TIM1, TIM_CH_1, TIM_CAP_CMP_DISABLE);
        TIM_EnableCapCmpChN(TIM1, TIM_CH_1, TIM_CAP_CMP_N_ENABLE);
        step++;
    }
    else if (step == 4)
    {
        /* Next step: Step 5 Configuration ---------------------------- */
        /*  Channel3 configuration */
        TIM_EnableCapCmpCh(TIM1, TIM_CH_3, TIM_CAP_CMP_DISABLE);
        TIM_EnableCapCmpChN(TIM1, TIM_CH_3, TIM_CAP_CMP_N_DISABLE);

        /*  Channel1 configuration */
        TIM_SelectOcMode(TIM1, TIM_CH_1, TIM_OCMODE_PWM1);
        TIM_EnableCapCmpCh(TIM1, TIM_CH_1, TIM_CAP_CMP_DISABLE);
        TIM_EnableCapCmpChN(TIM1, TIM_CH_1, TIM_CAP_CMP_N_ENABLE);

        /*  Channel2 configuration */
        TIM_SelectOcMode(TIM1, TIM_CH_2, TIM_OCMODE_PWM1);
        TIM_EnableCapCmpCh(TIM1, TIM_CH_2, TIM_CAP_CMP_ENABLE);
        TIM_EnableCapCmpChN(TIM1, TIM_CH_2, TIM_CAP_CMP_N_DISABLE);
        step++;
    }
    else if (step == 5)
    {
        /* Next step: Step 6 Configuration ---------------------------- */
        /*  Channel3 configuration */
        TIM_SelectOcMode(TIM1, TIM_CH_3, TIM_OCMODE_PWM1);
        TIM_EnableCapCmpCh(TIM1, TIM_CH_3, TIM_CAP_CMP_DISABLE);
        TIM_EnableCapCmpChN(TIM1, TIM_CH_3, TIM_CAP_CMP_N_ENABLE);

        /*  Channel1 configuration */
        TIM_EnableCapCmpCh(TIM1, TIM_CH_1, TIM_CAP_CMP_DISABLE);
        TIM_EnableCapCmpChN(TIM1, TIM_CH_1, TIM_CAP_CMP_N_DISABLE);

        /*  Channel2 configuration */
        TIM_SelectOcMode(TIM1, TIM_CH_2, TIM_OCMODE_PWM1);
        TIM_EnableCapCmpCh(TIM1, TIM_CH_2, TIM_CAP_CMP_ENABLE);
        TIM_EnableCapCmpChN(TIM1, TIM_CH_2, TIM_CAP_CMP_N_DISABLE);
        step++;
    }
    else
    {
        /* Next step: Step 1 Configuration ---------------------------- */
        /*  Channel1 configuration */
        TIM_SelectOcMode(TIM1, TIM_CH_1, TIM_OCMODE_PWM1);
        TIM_EnableCapCmpCh(TIM1, TIM_CH_1, TIM_CAP_CMP_ENABLE);
        TIM_EnableCapCmpChN(TIM1, TIM_CH_2, TIM_CAP_CMP_N_DISABLE);

        /*  Channel3 configuration */
        TIM_SelectOcMode(TIM1, TIM_CH_3, TIM_OCMODE_PWM1);
        TIM_EnableCapCmpCh(TIM1, TIM_CH_3, TIM_CAP_CMP_DISABLE);
        TIM_EnableCapCmpChN(TIM1, TIM_CH_3, TIM_CAP_CMP_N_ENABLE);

        /*  Channel2 configuration */
        TIM_EnableCapCmpCh(TIM1, TIM_CH_2, TIM_CAP_CMP_DISABLE);
        TIM_EnableCapCmpChN(TIM1, TIM_CH_2, TIM_CAP_CMP_N_DISABLE);
        step = 1;
    }
}
