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
 * @file app_user_config.h
 * @author NSING Firmware Team
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

#ifndef _APP_USER_CONFIG_H_
#define _APP_USER_CONFIG_H_

#include "ns_adv_data_def.h"

/* Device name */
#define CUSTOM_DEVICE_NAME                  "ImageUpdate"
#define CUSTOM_BLE_MAC_ADDRESS              "\x8A\x22\x77\x44\x55\x66"


/* adv configer*/
#define CUSTOM_ADV_FAST_INTERVAL               64                                     /**< Fast advertising interval (in units of 0.625 ms. This value corresponds to 40 ms.). */
#define CUSTOM_ADV_SLOW_INTERVAL               64                                     /**< Slow advertising interval (in units of 0.625 ms. This value corresponds to 40 ms.). */

#define CUSTOM_ADV_FAST_DURATION               0//30                                         /**< The advertising duration of fast advertising in units of 1 seconds. maximum is 655 seconds */
#define CUSTOM_ADV_SLOW_DURATION               180                                        /**< The advertising duration of slow advertising in units of 1 seconds. maximum is 655 seconds */


#define CUSTOM_USER_ADVERTISE_DATA \
            "\x03"\
            ADV_TYPE_SERVICE_DATA_16BIT_UUID\
            ADV_UUID_DEVICE_INFORMATION_SERVICE\


#define CUSTOM_USER_ADVERTISE_DATA_LEN (sizeof(CUSTOM_USER_ADVERTISE_DATA)-1)

/// Scan response data
#define CUSTOM_USER_ADV_SCNRSP_DATA  \
            "\x0a"\
            ADV_TYPE_MANUFACTURER_SPECIFIC_DATA\
            "\xff\xffNations"

/// Scan response data length- maximum 31 bytes
#define CUSTOM_USER_ADV_SCNRSP_DATA_LEN (sizeof(CUSTOM_USER_ADV_SCNRSP_DATA)-1)


/*  connection config  */
#define MIN_CONN_INTERVAL                   7.5                                        /**< Minimum connection interval (7.5 ms) */
#define MAX_CONN_INTERVAL                   30                                         /**< Maximum connection interval (30 ms). */
#define SLAVE_LATENCY                       6                                          /**< Slave latency. */
#define CONN_SUP_TIMEOUT                    5000                                       /**< Connection supervisory timeout (5000ms). */

#define FIRST_CONN_PARAMS_UPDATE_DELAY      (0)                                        /**<  Time of initiating event to update connection params (5 seconds). */

//forfiles config
#define CFG_APP_NS_IUS     1


/* User config  */

#define NS_LOG_ERROR_ENABLE      1
#define NS_LOG_WARNING_ENABLE    1
#define NS_LOG_INFO_ENABLE       1
#define NS_LOG_DEBUG_ENABLE      0

#define NS_LOG_LPUART_ENABLE     0
#define NS_LOG_USART_ENABLE      0
#define NS_LOG_RTT_ENABLE        0

#define NS_TIMER_ENABLE          0

#define FIRMWARE_VERSION            "0.0.01"
#define HARDWARE_VERSION            "0.0.01"


#endif // _APP_USER_CONFIG_H_

