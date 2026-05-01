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
 * @file rwapp_config.h
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */


#ifndef _RWAPP_CONFIG_H_
#define _RWAPP_CONFIG_H_

/**
 * @addtogroup app
 * @brief Application configuration definition
 *
 * @{
 **/

#include "app_user_config.h"
#include "rwip_config.h"
/* Includes ------------------------------------------------------------------*/

/* Define ------------------------------------------------------------*/


/// Application Profile
#if defined(CFG_APP_PRF)
#define BLE_APP_PRF          1
#else // defined(CFG_APP_PRF)
#define BLE_APP_PRF          0
#endif // defined(CFG_APP_PRF)


/// Health Thermometer Application
#if defined(CFG_APP_HT)
#define BLE_APP_HT           1
#else // defined(CFG_APP_HT)
#define BLE_APP_HT           0
#endif // defined(CFG_APP_HT)

/// HID Application
#if defined(CFG_APP_HID)
#define BLE_APP_HID          1
#else // defined(CFG_APP_HID)
#define BLE_APP_HID          0
#endif // defined(CFG_APP_HID)

/// DIS Application
#if defined(CFG_APP_DIS)
#define BLE_APP_DIS          1
#else // defined(CFG_APP_DIS)
#define BLE_APP_DIS          0
#endif // defined(CFG_APP_DIS)



/// Battery Service Application
#if (CFG_APP_BATT)
#define BLE_APP_BATT         1
#else
#define BLE_APP_BATT         0
#endif //(CFG_APP_BATT)

/// Security Application
#if (defined(CFG_APP_SEC) || BLE_APP_HID)
#define BLE_APP_SEC          1
#else //(defined(CFG_APP_SEC) || BLE_APP_HID)
#define BLE_APP_SEC          0
#endif //(defined(CFG_APP_SEC) || BLE_APP_HID)


#define AM0_APP_OPTIONAL_CHARACTERISTICS        0

#if defined(CFG_APP_NS_IUS)
#define BLE_APP_NS_IUS          1
#else // defined(CFG_APP_NS_IUS)
#define BLE_APP_NS_IUS          0
#endif // defined(CFG_APP_NS_IUS)

/// @} rwapp_config

#endif /* _RWAPP_CONFIG_H_ */
