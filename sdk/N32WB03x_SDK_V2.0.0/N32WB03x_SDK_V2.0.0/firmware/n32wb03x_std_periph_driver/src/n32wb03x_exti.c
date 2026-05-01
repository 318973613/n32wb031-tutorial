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
 * @file n32wb03x_exti.c
 * @author NSING Firmware Team
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#include "n32wb03x_exti.h"

/** @addtogroup N32WB03X_StdPeriph_Driver
 * @{
 */

/** @addtogroup EXTI
 * @brief EXTI driver modules
 * @{
 */

/** @addtogroup EXTI_Private_TypesDefinitions
 * @{
 */

/**
 * @}
 */

/** @addtogroup EXTI_Private_Defines
 * @{
 */

#define EXTI_LINENONE ((uint32_t)0x00000000) /* No interrupt selected */

/**
 * @}
 */

/** @addtogroup EXTI_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup EXTI_Private_Variables
 * @{
 */

/**
 * @}
 */

/** @addtogroup EXTI_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup EXTI_Private_Functions
 * @{
 */

/**
 * @brief  Deinitializes the EXTI peripheral registers to their default reset values.
 */
void EXTI_DeInit(void)
{
    EXTI->IMASK  = 0x00000000;
    EXTI->EMASK  = 0x00000000;
    EXTI->RT_CFG = 0x00000000;
    EXTI->FT_CFG = 0x00000000;
    EXTI->PEND   = 0x00FFFFFF;
}

/**
 * @brief  Initializes the EXTI peripheral according to the specified
 *         parameters in the EXTI_InitStruct.
 * @param EXTI_InitStruct pointer to a EXTI_InitType structure
 *         that contains the configuration information for the EXTI peripheral.
 */
void EXTI_InitPeripheral(EXTI_InitType* EXTI_InitStruct)
{
    uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_EXTI_MODE(EXTI_InitStruct->EXTI_Mode));
    assert_param(IS_EXTI_TRIGGER(EXTI_InitStruct->EXTI_Trigger));
    assert_param(IS_EXTI_LINE(EXTI_InitStruct->EXTI_Line));
    assert_param(IS_FUNCTIONAL_STATE(EXTI_InitStruct->EXTI_LineCmd));

    tmp = (uint32_t)EXTI_BASE;

    if (EXTI_InitStruct->EXTI_LineCmd != DISABLE)
    {
        /* Clear EXTI line configuration */
        EXTI->IMASK &= ~EXTI_InitStruct->EXTI_Line;
        EXTI->EMASK &= ~EXTI_InitStruct->EXTI_Line;

        tmp += EXTI_InitStruct->EXTI_Mode;

        *(__IO uint32_t*)tmp |= EXTI_InitStruct->EXTI_Line;

        /* Clear Rising Falling edge configuration */
        EXTI->RT_CFG &= ~EXTI_InitStruct->EXTI_Line;
        EXTI->FT_CFG &= ~EXTI_InitStruct->EXTI_Line;

        /* Select the trigger for the selected external interrupts */
        if (EXTI_InitStruct->EXTI_Trigger == EXTI_Trigger_Rising_Falling)
        {
            /* Rising Falling edge */
            EXTI->RT_CFG |= EXTI_InitStruct->EXTI_Line;
            EXTI->FT_CFG |= EXTI_InitStruct->EXTI_Line;
        }
        else
        {
            tmp = (uint32_t)EXTI_BASE;
            tmp += EXTI_InitStruct->EXTI_Trigger;

            *(__IO uint32_t*)tmp |= EXTI_InitStruct->EXTI_Line;
        }
    }
    else
    {
        tmp += EXTI_InitStruct->EXTI_Mode;

        /* Disable the selected external lines */
        *(__IO uint32_t*)tmp &= ~EXTI_InitStruct->EXTI_Line;
    }
}

/**
 * @brief  Fills each EXTI_InitStruct member with its reset value.
 * @param EXTI_InitStruct pointer to a EXTI_InitType structure which will
 *         be initialized.
 */
void EXTI_InitStruct(EXTI_InitType* EXTI_InitStruct)
{
    EXTI_InitStruct->EXTI_Line    = EXTI_LINENONE;
    EXTI_InitStruct->EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStruct->EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct->EXTI_LineCmd = DISABLE;
}

/**
 * @brief  Generates a Software interrupt.
 * @param EXTI_Line specifies the EXTI lines to be enabled or disabled.
 *   This parameter can be any combination of EXTI_Linex where x can be (0..13).
 */
void EXTI_TriggerSWInt(uint32_t EXTI_Line)
{
    /* Check the parameters */
    assert_param(IS_EXTI_LINE(EXTI_Line));

    EXTI->SWIE |= EXTI_Line;
}

/**
 * @brief  Checks whether the specified EXTI line flag is set or not.
 * @param EXTI_Line specifies the EXTI line flag to check.
 *   This parameter can be:
 *     @arg EXTI_Linex External interrupt line x where x(0..13)
 * @return The new state of EXTI_Line (SET or RESET).
 */
FlagStatus EXTI_GetStatusFlag(uint32_t EXTI_Line)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_GET_EXTI_LINE(EXTI_Line));

    if ((EXTI->PEND & EXTI_Line) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}

/**
 * @brief  Clears the EXTI's line pending flags.
 * @param EXTI_Line specifies the EXTI lines flags to clear.
 *   This parameter can be any combination of EXTI_Linex where x can be (0..13).
 */
void EXTI_ClrStatusFlag(uint32_t EXTI_Line)
{
    /* Check the parameters */
    assert_param(IS_EXTI_LINE(EXTI_Line));

    EXTI->PEND = EXTI_Line;
}

/**
 * @brief  Checks whether the specified EXTI line is asserted or not.
 * @param EXTI_Line specifies the EXTI line to check.
 *   This parameter can be:
 *     @arg EXTI_Linex External interrupt line x where x(0..13)
 * @return The new state of EXTI_Line (SET or RESET).
 */
INTStatus EXTI_GetITStatus(uint32_t EXTI_Line)
{
    INTStatus bitstatus   = RESET;
    uint32_t enablestatus = 0;
    /* Check the parameters */
    assert_param(IS_GET_EXTI_LINE(EXTI_Line));

    enablestatus = EXTI->IMASK & EXTI_Line;
    if (((EXTI->PEND & EXTI_Line) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}

/**
 * @brief  Clears the EXTI's line pending bits.
 * @param EXTI_Line specifies the EXTI lines to clear.
 *   This parameter can be any combination of EXTI_Linex where x can be (0..13).
 */
void EXTI_ClrITPendBit(uint32_t EXTI_Line)
{
    /* Check the parameters */
    assert_param(IS_EXTI_LINE(EXTI_Line));

    EXTI->PEND = EXTI_Line;
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
