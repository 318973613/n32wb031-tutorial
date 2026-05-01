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
 * @file misc.c
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#include "misc.h"

/** @addtogroup N32WB03X_StdPeriph_Driver
 * @{
 */

/** @addtogroup MISC
 * @brief MISC driver modules
 * @{
 */

/** @addtogroup MISC_Private_TypesDefinitions
 * @{
 */

/**
 * @}
 */

/** @addtogroup MISC_Private_Defines
 * @{
 */

#define AIRCR_VECTKEY_MASK ((uint32_t)0x05FA0000)
/**
 * @}
 */

/** @addtogroup MISC_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup MISC_Private_Variables
 * @{
 */

/**
 * @}
 */

/** @addtogroup MISC_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup MISC_Private_Functions
 * @{
 */

/**
 * @brief  Initializes the NVIC peripheral according to the specified
 *         parameters in the NVIC_InitStruct.
 * @param NVIC_InitStruct pointer to a NVIC_InitType structure that contains
 *         the configuration information for the specified NVIC peripheral.
 */
void NVIC_Init(NVIC_InitType* NVIC_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NVIC_InitStruct->NVIC_IRQChannelCmd));
    assert_param(IS_NVIC_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelPriority));

    /* Set Interrupt Priority */
    NVIC_SetPriority(NVIC_InitStruct->NVIC_IRQChannel, NVIC_InitStruct->NVIC_IRQChannelPriority);

    if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE)
    {
        /* Enable the Selected IRQ Channels --------------------------------------*/
        NVIC_EnableIRQ(NVIC_InitStruct->NVIC_IRQChannel);
    }
    else
    {
        /* Disable the Selected IRQ Channels -------------------------------------*/
        NVIC_DisableIRQ(NVIC_InitStruct->NVIC_IRQChannel);
    }
}

/**
 * @brief  Configures the SysTick clock source.
 * @param SysTick_CLKSource specifies the SysTick clock source.
 *   This parameter can be one of the following values:
 *     @arg SysTick_CLKSource_HCLK_Div8 AHB clock divided by 8 selected as SysTick clock source.
 *     @arg SysTick_CLKSource_HCLK AHB clock selected as SysTick clock source.
 */
void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource)
{
    /* Check the parameters */
    assert_param(IS_SYSTICK_CLK_SOURCE(SysTick_CLKSource));
    if (SysTick_CLKSource == SysTick_CLKSource_HCLK)
    {
        SysTick->CTRL |= SysTick_CLKSource_HCLK;
    }
    else
    {
        SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;
    }
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
