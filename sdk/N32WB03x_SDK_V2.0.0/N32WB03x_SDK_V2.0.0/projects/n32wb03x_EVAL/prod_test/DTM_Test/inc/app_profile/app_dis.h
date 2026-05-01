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
 * @file app_dis.h
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */


#ifndef APP_DIS_H_
#define APP_DIS_H_

/**
 * @addtogroup APP
 *
 * @brief Device Information Application Module Entry point.
 *
 * @{
 **/

/* Includes ------------------------------------------------------------------*/

#include "rwip_config.h"     // SW Configuration

#if (BLE_APP_DIS)

#include <stdint.h>

/* Define ------------------------------------------------------------*/

/// Manufacturer Name Value
#define APP_DIS_MANUFACTURER_NAME       ("NSING")
#define APP_DIS_MANUFACTURER_NAME_LEN   (5)

/// Model Number String Value
#define APP_DIS_MODEL_NB_STR            ("NS-BLE-1.0")
#define APP_DIS_MODEL_NB_STR_LEN        (10)

/// Serial Number
#define APP_DIS_SERIAL_NB_STR           ("1.0.0.0-LE")
#define APP_DIS_SERIAL_NB_STR_LEN       (10)

/// Firmware Revision
#define APP_DIS_FIRM_REV_STR            ("1.0.0")
#define APP_DIS_FIRM_REV_STR_LEN        (5)

/// System ID Value - LSB -> MSB
#define APP_DIS_SYSTEM_ID               ("\x12\x34\x56\xFF\xFE\x9A\xBC\xDE")
#define APP_DIS_SYSTEM_ID_LEN           (8)

/// Hardware Revision String
#define APP_DIS_HARD_REV_STR           ("1.0.0")
#define APP_DIS_HARD_REV_STR_LEN       (5)

/// Software Revision String
#define APP_DIS_SW_REV_STR              ("1.0.0")
#define APP_DIS_SW_REV_STR_LEN          (5)

/// IEEE
#define APP_DIS_IEEE                    ("\xFF\xEE\xDD\xCC\xBB\xAA")
#define APP_DIS_IEEE_LEN                (6)

/**
 * PNP ID Value - LSB -> MSB
 *      Vendor ID Source : 0x02 (USB Implementer’s Forum assigned Vendor ID value)
 *      Vendor ID : 0x045E      (Microsoft Corp)
 *      Product ID : 0x0040
 *      Product Version : 0x0300
 */
#define APP_DIS_PNP_ID               ("\x02\x5E\x04\x40\x00\x00\x03")
#define APP_DIS_PNP_ID_LEN           (7)

#if (BLE_APP_HID)
#define APP_DIS_FEATURES             (DIS_MANUFACTURER_NAME_CHAR_SUP_BIT |\
                                      DIS_MODEL_NB_STR_CHAR_SUP_BIT      |\
                                      DIS_SYSTEM_ID_CHAR_SUP_BIT         |\
                                      DIS_PNP_ID_CHAR_SUP_BIT)
    
#else
#define APP_DIS_FEATURES             (DIS_ALL_FEAT_SUP)
#endif //(BLE_APP_HID)


/* Public variables ---------------------------------------------------------*/

/// Table of message handlers
extern const struct app_subtask_handlers app_dis_handlers;

/* Public function prototypes -----------------------------------------------*/

/**
 * @brief Initialize Device Information Service Application
 **/
void app_dis_init(void);

/**
 * @brief Add a Device Information Service instance in the DB
 **/
void app_dis_add_dis(void);

/**
 * @brief Enable the Device Information Service
 **/
void app_dis_enable_prf(uint16_t conhdl);

#endif //BLE_APP_DIS

/// @} APP

#endif //APP_DIS_H_
