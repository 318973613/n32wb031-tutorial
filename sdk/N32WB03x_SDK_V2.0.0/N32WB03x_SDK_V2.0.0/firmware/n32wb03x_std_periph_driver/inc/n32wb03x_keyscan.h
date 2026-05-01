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
 * @file n32wb03x_keyscan.h
 * @author NSING Firmware Team
 * @version v1.0.4
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#ifndef __N32WB03X__KEYSCAN_H__
#define __N32WB03X__KEYSCAN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32wb03x.h"

/** @addtogroup n32wb03x_StdPeriph_Driver
 * @{
 */

/** @addtogroup KEYSCAN
 * @{
 */

/** @addtogroup KEYSCAN_Exported_Types
 * @{
 */

/**
 * @brief  KEYSCAN Init Structure definition
 */
typedef struct
{
    uint32_t Mask; 
    uint16_t Mode; 
    uint16_t Wts; 
    uint16_t Dts; 
    uint16_t Int_en; 
    
} KEYSCAN_InitType;


/**
 * @brief  KEYSCAN NUM enumeration
 */
typedef enum 
{
    KEY_104,
    KEY_44,
    KEY_65,
}KEY_NUM;

/**
 * @brief  KEYSCAN NUM enumeration
 */
typedef enum 
{
    MODE_FIXED_INTV,
    MODE_SW_TRIG,
    MODE_PRESS_TRIG,
}KEY_MODE;

/**
 * @brief  KEYSCAN debounce time enumeration
 */
typedef enum 
{
    DTS_10MS,
    DTS_20MS,
    DTS_40MS,
    DTS_80MS,
    DTS_160MS,
    DTS_320MS,
    DTS_640MS,
    DTS_640MS_2,
}KEY_DTS;

/**
 * @brief  KEYSCAN scan wait time enumeration
 */
typedef enum 
{
    WTS_0MS,
    WTS_32MS,
    WTS_64MS,
    WTS_96MS,
    WTS_128MS,
    WTS_160MS,
    WTS_192MS,
    WTS_224MS,
}KEY_WTS;

/**
 * @brief  KEYSCAN INT trigger enumeration
 */
typedef enum 
{
    INT_DIS,
    INT_EN,
}KEY_INT_EN;
    
/**
 * @}
 */

/** @addtogroup KEYSCAN_Exported_Constants
 * @{
 */

/** @addtogroup KEYSCAN_Prescaler
 * @{
 */

/**
 * @}
 */

/**
 * @}
 */

/** @addtogroup KEYSCAN_Exported_Macros
 * @{
 */

#define KEYSCAN_EN_POS            0
#define KEY_MODE_POS              2
#define KEY_DTS_POS               4
#define KEY_WTS_POS               7
#define KEY_MASK_POS              10
#define KEY_SW_START_POS          12
#define KEY_INFO_CLR_POS          13
#define KEY_INT_EN_POS            22
#define KEY_IRP_POS               23

/**
 * @}
 */

/** @addtogroup KEYSCAN_Exported_Functions
 * @{
 */
void KEYSCAN_Init(KEYSCAN_InitType* KEYSCAN_InitStruct);
void KEYSCAN_Enable(FunctionalState Cmd);
void KEYSCAN_InfoClear(void);
void KEYSCAN_SoftwareStartScan(void);
FlagStatus KEYSCAN_GetInterruptState(void);
void KEYSCAN_ClearInterrupt(void);
void KEYSCAN_ReadKeyData(uint32_t *key_data);
#ifdef __cplusplus
}
#endif

#endif /* __N32WB03X__KEYSCAN_H */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
