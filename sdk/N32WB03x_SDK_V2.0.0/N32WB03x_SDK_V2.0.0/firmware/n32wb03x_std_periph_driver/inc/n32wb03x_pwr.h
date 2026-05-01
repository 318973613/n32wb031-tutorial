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
 * @file n32wb03x_pwr.h
 * @author NSING Firmware Team
 * @version v1.1.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#ifndef __N32WB03X_PWR_H__
#define __N32WB03X_PWR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32wb03x.h"

/** @addtogroup N32WB03X_StdPeriph_Driver
 * @{
 */

/**
 * @}
 */

/** @defgroup SLEEP_mode_entry 
  * @{
  */
#define PWR_IDLEENTRY_WFI         ((uint8_t)0x01)
#define PWR_IDLEENTRY_WFE         ((uint8_t)0x02)

 
/**
  * @}
  */


/** @defgroup SLEEP_mode_entry 
  * @{
  */

#define PWR_SLEEPENTRY_WFI        ((uint8_t)0x01)
#define PWR_SLEEPENTRY_WFE        ((uint8_t)0x02)

/** @defgroup Powerdown_mode_entry 
  * @{
  */

#define PWR_PDENTRY_WFI         ((uint8_t)0x01)
#define PWR_PDENTRY_WFE         ((uint8_t)0x02)


/** @addtogroup PWR_Exported_Functions
 * @{
 */

void PWR_DeInit(void);
void PWR_EnterIDLEMode(uint8_t IDLEONEXIT, uint8_t PWR_IdleEntry);
void PWR_EnterSLEEPMode(uint8_t PWR_SleepEntry);
void PWR_EnterPDMode(uint8_t PWR_PDEntry);

#ifdef __cplusplus
}
#endif

#endif /* __N32WB03X_PWR_H__ */
       /**
        * @}
        */

/**
 * @}
 */

/**
 * @}
 */
