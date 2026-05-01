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
 * @file n32wb03x_pwr.c
 * @author NSING Firmware Team 
 * @version v1.0.4
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#include "n32wb03x_pwr.h"

/** @addtogroup N32WB03X_StdPeriph_Driver
 * @{
 */

/**
 * @}
 */

/** @addtogroup PWR_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup PWR_Private_Variables
 * @{
 */

/**
 * @}
 */

/** @addtogroup PWR_Private_FunctionPrototypes
 * @{
 */

/**
 * @}
 */

/** @addtogroup PWR_Private_Functions
 * @{
 */
 
 
/**
 * @brief  Deinitializes the PWR peripheral registers to their default reset values.
 */
void PWR_DeInit(void)
{
    RCC_EnableAPB1PeriphReset(RCC_APB1_PERIPH_PWR, ENABLE);
    RCC_EnableAPB1PeriphReset(RCC_APB1_PERIPH_PWR, DISABLE);
}

/**
  * @brief  Enters IDLE mode.
  * @param  SLEEPONEXIT: specifies the SLEEPONEXIT state in IDLE mode.
  *   This parameter can be one of the following values:
  *     @arg DISABLE: SLEEP mode with SLEEPONEXIT disable
  *     @arg ENABLE : SLEEP mode with SLEEPONEXIT enable
  * @param  PWR_STOPEntry: specifies if SLEEP mode in entered with WFI or WFE instruction.
  *   This parameter can be one of the following values:
  *     @arg PWR_IDLEENTRY_WFI: enter IDLE mode with WFI instruction
  *     @arg PWR_IDLEENTRY_WFE: enter IDLE mode with WFE instruction
  * @retval None
  */
void PWR_EnterIDLEMode(uint8_t IDLEONEXIT, uint8_t PWR_IDLEEntry)
{  
  
    /* CLEAR SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP); 

      /* Select SLEEPONEXIT mode entry --------------------------------------------------*/
    if(IDLEONEXIT == ENABLE)
    {   
        /* the MCU enters Sleep mode as soon as it exits the lowest priority ISR */
        SCB->SCR |= SCB_SCR_SLEEPONEXIT;
    }
    else if(IDLEONEXIT == DISABLE)
    {
        /* Sleep-now */
        SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPONEXIT);
    }       
      
    /* Select SLEEP mode entry --------------------------------------------------*/
    if(PWR_IDLEEntry == PWR_IDLEENTRY_WFI)
    {   
        /* Request Wait For Interrupt */
        __WFI();        
    }
    else
    {
        /* Request Wait For Event */
        __SEV();
        __WFE();
        __WFE();
    }    
}

/**
  * @brief  Enters SLEEP mode.
  * @param  PWR_SLEEPEntry: specifies if SLEEP mode in entered with WFI or WFE instruction.
  *   This parameter can be one of the following values:
  *     @arg PWR_SLEEPENTRY_WFI: enter SLEEP mode with WFI instruction
  *     @arg PWR_SLEEPENTRY_WFE: enter SLEEP mode with WFE instruction
  * @retval None
  */
void PWR_EnterSLEEPMode(uint8_t PWR_SleepEntry)
{
    uint32_t tmpreg = 0;
    /* Set BLE modem sleep */
    *(uint32_t*)0x40011004 |= 0x40; //for without HSE
    *(uint32_t *)(BLE_BASE + 0x30) = 0x07;
    while(PWR->CR1&PWR_CR1_OSC_EN);
    *(uint32_t*)0x40011004 &= ~0x40;//for without HSE
      
    /* Select the regulator state in SLEEP mode ---------------------------------*/
    tmpreg = PWR->CR1;
    /* Clear PDDS and FLPDS bits */
    tmpreg &= ~PWR_CR1_MODE_SEL;
    /* Set FLPDS bit according to PWR_Regulator value */    
    tmpreg |= (PWR_CR1_MODE_SLEEP |PWR_CR1_MODE_EN);
    /* Store the new value */
    PWR->CR1 = tmpreg;
    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP;

    /* Select SLEEP mode entry --------------------------------------------------*/
    if(PWR_SleepEntry == PWR_SLEEPENTRY_WFI)
    {   
        /* Request Wait For Interrupt */
        __WFI();
    }
    else
    {
        /* Request Wait For Event */
        __SEV();
        __WFE();
        __WFE();
    }

    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP);  
}

 /**
  * @brief  Enters PD mode.
  * @param  PWR_PDEntry: specifies if PD mode in entered with WFI or WFE instruction.
  *   This parameter can be one of the following values:
  *     @arg PWR_PDENTRY_WFI: enter PD mode with WFI instruction
  *     @arg PWR_PDENTRY_WFE: enter PD mode with WFE instruction
  * @retval None
  */
void PWR_EnterPDMode(uint8_t PWR_PDEntry)
{
    uint32_t tmpreg = 0; 
    /* Select the regulator state in SHUTDOWN mode ---------------------------------*/
    tmpreg = PWR->CR1;
    /* Clear PDDS  bits */
    tmpreg &= ~PWR_CR1_MODE_SEL;
    /* Set FLPDS bit according to PWR_Regulator value */    
    tmpreg |= PWR_CR1_MODE_PD|PWR_CR1_MODE_EN;
    /* Store the new value */
    PWR->CR1 = tmpreg;    

    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP;
    /* This option is used to ensure that store operations are completed */
    #if defined ( __CC_ARM   )
    __force_stores();
    #endif
    /* Select SHUTDOWN mode entry --------------------------------------------------*/
    if(PWR_PDEntry == PWR_PDENTRY_WFI)
    {   
        /* Request Wait For Interrupt */
        __WFI();
    }
    else
    {
        /* Request Wait For Event */
        __SEV();
        __WFE();
        __WFE();
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
