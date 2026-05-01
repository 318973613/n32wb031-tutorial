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
 * @file n32wb03x_wwdg.c
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#include "n32wb03x_wwdg.h"
#include "n32wb03x_rcc.h"

/** @addtogroup N32WB03X_StdPeriph_Driver
 * @{
 */

/** @addtogroup WWDG
 * @brief WWDG driver modules
 * @{
 */

/** @addtogroup WWDG_Private_TypesDefinitions
 * @{
 */

/**
 * @}
 */

/** @addtogroup WWDG_Private_Defines
 * @{
 */



/* --------------------- WWDG registers bit mask ------------------------ */

/* CTRL register bit mask */
#define CTRL_ACTB_SET ((uint32_t)0x00000080)

/* CFG register bit mask */
#define CFG_TIMERB_MASK ((uint32_t)0xFFFFFE7F)
#define CFG_W_MASK      ((uint32_t)0xFFFFFF80)
#define BIT_MASK        ((uint8_t)0x7F)

/**
 * @}
 */

/** @addtogroup WWDG_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup WWDG_Private_Variables
 * @{
 */

/**
 * @}
 */

/** @addtogroup WWDG_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup WWDG_Private_Functions
 * @{
 */

/**
 * @brief  Deinitializes the WWDG peripheral registers to their default reset values.
 */
void WWDG_DeInit(void)
{
    RCC_EnableAPB1PeriphReset(RCC_APB1_PERIPH_WWDG, ENABLE);
    RCC_EnableAPB1PeriphReset(RCC_APB1_PERIPH_WWDG, DISABLE);
}

/**
 * @brief  Sets the WWDG Prescaler.
 * @param WWDG_Prescaler specifies the WWDG Prescaler.
 *   This parameter can be one of the following values:
 *     @arg WWDG_PRESCALER_DIV1 WWDG counter clock = (PCLK1/4096)/1
 *     @arg WWDG_PRESCALER_DIV2 WWDG counter clock = (PCLK1/4096)/2
 *     @arg WWDG_PRESCALER_DIV4 WWDG counter clock = (PCLK1/4096)/4
 *     @arg WWDG_PRESCALER_DIV8 WWDG counter clock = (PCLK1/4096)/8
 */
void WWDG_SetPrescalerDiv(uint32_t WWDG_Prescaler)
{
    uint32_t tmpregister = 0;
    /* Check the parameters */
    assert_param(IS_WWDG_PRESCALER_DIV(WWDG_Prescaler));
    /* Clear WDGTB[1:0] bits */
    tmpregister = WWDG->CFG & CFG_TIMERB_MASK;
    /* Set WDGTB[1:0] bits according to WWDG_Prescaler value */
    tmpregister |= WWDG_Prescaler;
    /* Store the new value */
    WWDG->CFG = tmpregister;
}

/**
 * @brief  Sets the WWDG window value.
 * @param WindowValue specifies the window value to be compared to the downcounter.
 *   This parameter value must be lower than 0x80.
 */
void WWDG_SetWValue(uint8_t WindowValue)
{
    __IO uint32_t tmpregister = 0;

    /* Check the parameters */
    assert_param(IS_WWDG_WVALUE(WindowValue));
    /* Clear W[6:0] bits */

    tmpregister = WWDG->CFG & CFG_W_MASK;

    /* Set W[6:0] bits according to WindowValue value */
    tmpregister |= WindowValue & (uint32_t)BIT_MASK;

    /* Store the new value */
    WWDG->CFG = tmpregister;
}

/**
 * @brief  Enables the WWDG Early Wakeup interrupt(EWI).
 */
void WWDG_EnableInt(void)
{
   
    WWDG->CFG |= (uint32_t)( ENABLE << 9);
}

/**
 * @brief  Sets the WWDG counter value.
 * @param Counter specifies the watchdog counter value.
 *   This parameter must be a number between 0x40 and 0x7F.
 */
void WWDG_SetCnt(uint8_t Counter)
{
    /* Check the parameters */
    assert_param(IS_WWDG_CNT(Counter));
    /* Write to T[6:0] bits to configure the counter value, no need to do
       a read-modify-write; writing a 0 to WDGA bit does nothing */
    WWDG->CTRL = Counter & BIT_MASK;
}

/**
 * @brief  Enables WWDG and load the counter value.
 * @param Counter specifies the watchdog counter value.
 *   This parameter must be a number between 0x40 and 0x7F.
 */
void WWDG_Enable(uint8_t Counter)
{
    /* Check the parameters */
    assert_param(IS_WWDG_CNT(Counter));
    WWDG->CTRL = CTRL_ACTB_SET | Counter;
}

/**
 * @brief  Checks whether the Early Wakeup interrupt flag is set or not.
 * @return The new state of the Early Wakeup interrupt flag (SET or RESET)
 */
FlagStatus WWDG_GetEWINTF(void)
{
    return (FlagStatus)(WWDG->STS);
}

/**
 * @brief  Clears Early Wakeup interrupt flag.
 */
void WWDG_ClrEWINTF(void)
{
    WWDG->STS = (uint32_t)RESET;
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
