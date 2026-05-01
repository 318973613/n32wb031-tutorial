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
 * @file n32wb03x_iwdg.c
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#include "n32wb03x_iwdg.h"

/** @addtogroup N32WB03X_StdPeriph_Driver
 * @{
 */

/** @addtogroup IWDG
 * @brief IWDG driver modules
 * @{
 */

/** @addtogroup IWDG_Private_TypesDefinitions
 * @{
 */

/**
 * @}
 */

/** @addtogroup IWDG_Private_Defines
 * @{
 */

/* ---------------------- IWDG registers bit mask ----------------------------*/

/* KEY register bit mask */
#define KEY_ReloadKey ((uint16_t)0xAAAA)
#define KEY_EnableKey ((uint16_t)0xCCCC)

/**
 * @}
 */

/** @addtogroup IWDG_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup IWDG_Private_Variables
 * @{
 */

/**
 * @}
 */

/** @addtogroup IWDG_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup IWDG_Private_Functions
 * @{
 */

/**
 * @brief  Enables or disables write access to IWDG_PR and IWDG_RLR registers.
 * @param IWDG_WriteAccess new state of write access to IWDG_PR and IWDG_RLR registers.
 *   This parameter can be one of the following values:
 *     @arg IWDG_WRITE_ENABLE Enable write access to IWDG_PR and IWDG_RLR registers
 *     @arg IWDG_WRITE_DISABLE Disable write access to IWDG_PR and IWDG_RLR registers
 */
void IWDG_WriteConfig(uint16_t IWDG_WriteAccess)
{
    /* Check the parameters */
    assert_param(IS_IWDG_WRITE(IWDG_WriteAccess));
    IWDG->KEY = IWDG_WriteAccess;
}

/**
 * @brief  Sets IWDG Prescaler value.
 * @param IWDG_Prescaler specifies the IWDG Prescaler value.
 *   This parameter can be one of the following values:
 *     @arg IWDG_PRESCALER_DIV4 IWDG prescaler set to 4
 *     @arg IWDG_PRESCALER_DIV8 IWDG prescaler set to 8
 *     @arg IWDG_PRESCALER_DIV16 IWDG prescaler set to 16
 *     @arg IWDG_PRESCALER_DIV32 IWDG prescaler set to 32
 *     @arg IWDG_PRESCALER_DIV64 IWDG prescaler set to 64
 *     @arg IWDG_PRESCALER_DIV128 IWDG prescaler set to 128
 *     @arg IWDG_PRESCALER_DIV256 IWDG prescaler set to 256
 */
void IWDG_SetPrescalerDiv(uint8_t IWDG_Prescaler)
{
    /* Check the parameters */
    assert_param(IS_IWDG_PRESCALER_DIV(IWDG_Prescaler));
    IWDG->PREDIV = IWDG_Prescaler;
}

/**
 * @brief  Sets IWDG Reload value.
 * @param Reload specifies the IWDG Reload value.
 *   This parameter must be a number between 0 and 0x0FFF.
 */
void IWDG_CntReload(uint16_t Reload)
{
    /* Check the parameters */
    assert_param(IS_IWDG_RELOAD(Reload));
    IWDG->RELV = Reload;
}

/**
 * @brief  Reloads IWDG counter with value defined in the reload register
 *   (write access to IWDG_PR and IWDG_RLR registers disabled).
 */
void IWDG_ReloadKey(void)
{
    IWDG->KEY = KEY_ReloadKey;
}

/**
 * @brief  Enables IWDG (write access to IWDG_PR and IWDG_RLR registers disabled).
 */
void IWDG_Enable(void)
{
    IWDG->KEY = KEY_EnableKey;
}

/**
 * @brief  Checks whether the specified IWDG flag is set or not.
 * @param IWDG_FLAG specifies the flag to check.
 *   This parameter can be one of the following values:
 *     @arg IWDG_PVU_FLAG Prescaler Value Update on going
 *     @arg IWDG_CRVU_FLAG Reload Value Update on going
 * @return The new state of IWDG_FLAG (SET or RESET).
 */
FlagStatus IWDG_GetStatus(uint16_t IWDG_FLAG)
{
    FlagStatus bitstatus = RESET;
    /* Check the parameters */
    assert_param(IS_IWDG_FLAG(IWDG_FLAG));
    if ((IWDG->STS & IWDG_FLAG) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    /* Return the flag status */
    return bitstatus;
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
