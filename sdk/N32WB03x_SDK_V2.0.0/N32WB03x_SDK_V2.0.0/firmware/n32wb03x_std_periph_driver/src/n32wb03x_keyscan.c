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
 * @file n32wb03x_keyscan.c
 * @author NSING Firmware Team
 * @version v1.0.4
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#include "n32wb03x_keyscan.h"
#include "n32wb03x_rcc.h"

/** @addtogroup N32WB03X_StdPeriph_Driver
 * @{
 */

/**
 * @brief  Initializes the KEYSCAN peripheral according to the specified
 *         parameters in the KEYSCAN_InitType.
 * @param KEYSCAN_InitStruct pointer to a KEYSCAN_InitType structure that
 *         contains the configuration information for the KEYSCAN peripheral.
 */
 
void KEYSCAN_Init(KEYSCAN_InitType* KEYSCAN_InitStruct)
{
    KEYSCAN->KEYCR = 0;
        
    KEYSCAN->KEYCR =   (KEYSCAN_InitStruct->Mask<<KEY_MASK_POS)
                    | (KEYSCAN_InitStruct->Mode<<KEY_MODE_POS)
                    | (KEYSCAN_InitStruct->Dts<<KEY_DTS_POS)
                    | (KEYSCAN_InitStruct->Wts<<KEY_WTS_POS)
                    | (KEYSCAN_InitStruct->Int_en<<KEY_INT_EN_POS);
    //configer retention voltag
    *(uint32_t*)0x40007014 = 0x00000818; 
}


/**
 * @brief Enables or disables the KEYSCAN.
 * @param Cmd new state of KEYSCAN.This parameter can be: ENABLE or DISABLE.
 */
void KEYSCAN_Enable(FunctionalState Cmd)
{    
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(Cmd));

    if (Cmd != DISABLE)
    {
        /* Enable the selected USART by setting the UE bit in the CTRL1 register */
        KEYSCAN->KEYCR |= (1<<KEYSCAN_EN_POS);
    }
    else
    {
        /* Disable the selected USART by clearing the UE bit in the CTRL1 register */
        KEYSCAN->KEYCR &= ~(1<<KEYSCAN_EN_POS);
    }
}

/**
 * @brief Clear the KEYDATA information of KEYSCAN peripheral.
 */
void KEYSCAN_InfoClear(void)
{
    KEYSCAN->KEYCR |= (1<<KEY_INFO_CLR_POS);
}

/**
 * @brief Start Software mode scan with KEYSCAN peripheral.
 */
void KEYSCAN_SoftwareStartScan(void)
{
    KEYSCAN->KEYCR |= (1<<KEY_SW_START_POS);
}

/**
 * @brief Get the interrupt status of KEYSCAN peripheral.
 */
FlagStatus KEYSCAN_GetInterruptState(void)
{
    if(KEYSCAN->KEYCR & (1<<KEY_IRP_POS))
    {
        return SET;
    }
    return RESET;
}

/**
 * @brief Clear the interrupt pending status of KEYSCAN peripheral.
 */
void KEYSCAN_ClearInterrupt(void)
{
    KEYSCAN->KEYCR |= (1<<KEY_IRP_POS);
}

/**
 * @brief Read the key data.
 * @param key_data  An array with 5 word len to read out the key data.
 */
void KEYSCAN_ReadKeyData(uint32_t *key_data)
{
    key_data[0] = KEYSCAN->KEYDATA0;
    key_data[1] = KEYSCAN->KEYDATA1;
    key_data[2] = KEYSCAN->KEYDATA2;
    key_data[3] = KEYSCAN->KEYDATA3;
    key_data[4] = KEYSCAN->KEYDATA4;
}

/**
 * @}
 */

