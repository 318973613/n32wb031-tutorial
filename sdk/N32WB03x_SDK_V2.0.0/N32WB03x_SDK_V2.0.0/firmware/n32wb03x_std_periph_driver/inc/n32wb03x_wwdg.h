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
 * @file n32wb03x_wwdg.h
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#ifndef __N32WB03X_WWDG_H__
#define __N32WB03X_WWDG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32wb03x.h"

/** @addtogroup N32WB03X_StdPeriph_Driver
 * @{
 */

/** @addtogroup WWDG
 * @{
 */

/** @addtogroup WWDG_Exported_Types
 * @{
 */

/**
 * @}
 */

/** @addtogroup WWDG_Exported_Constants
 * @{
 */

/** @addtogroup WWDG_Prescaler
 * @{
 */

#define WWDG_PRESCALER_DIV1 ((uint32_t)0x00000000)
#define WWDG_PRESCALER_DIV2 ((uint32_t)0x00000080)
#define WWDG_PRESCALER_DIV4 ((uint32_t)0x00000100)
#define WWDG_PRESCALER_DIV8 ((uint32_t)0x00000180)
#define IS_WWDG_PRESCALER_DIV(PRESCALER)                                                                               \
    (((PRESCALER) == WWDG_PRESCALER_DIV1) || ((PRESCALER) == WWDG_PRESCALER_DIV2)                                      \
     || ((PRESCALER) == WWDG_PRESCALER_DIV4) || ((PRESCALER) == WWDG_PRESCALER_DIV8))
#define IS_WWDG_WVALUE(VALUE) ((VALUE) <= 0x7F)
#define IS_WWDG_CNT(COUNTER)  (((COUNTER) >= 0x40) && ((COUNTER) <= 0x7F))

/**
 * @}
 */

/**
 * @}
 */

/** @addtogroup WWDG_Exported_Macros
 * @{
 */
/**
 * @}
 */

/** @addtogroup WWDG_Exported_Functions
 * @{
 */

void WWDG_DeInit(void);
void WWDG_SetPrescalerDiv(uint32_t WWDG_Prescaler);
void WWDG_SetWValue(uint8_t WindowValue);
void WWDG_EnableInt(void);
void WWDG_SetCnt(uint8_t Counter);
void WWDG_Enable(uint8_t Counter);
FlagStatus WWDG_GetEWINTF(void);
void WWDG_ClrEWINTF(void);

#ifdef __cplusplus
}
#endif

#endif /* __N32WB03X__WWDG_H */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
