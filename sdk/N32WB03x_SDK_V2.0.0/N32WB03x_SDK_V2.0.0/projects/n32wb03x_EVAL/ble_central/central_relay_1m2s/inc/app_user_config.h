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
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

#ifndef _APP_USER_CONFIG_H_
#define _APP_USER_CONFIG_H_

#include "ns_adv_data_def.h"

/* Device name */
#define CUSTOM_DEVICE_NAME                  "NS_RDTS_RELAY"                            /**<The device name of this device*/

/* adv configer*/
#define CUSTOM_ADV_FAST_INTERVAL               160                                        /**< Fast advertising interval (in units of 0.625 ms. This value corresponds to 100 ms.). */
#define CUSTOM_ADV_SLOW_INTERVAL               3200                                       /**< Slow advertising interval (in units of 0.625 ms. This value corresponds to 2 seconds). */

#define CUSTOM_ADV_FAST_DURATION               0//30                                         /**< The advertising duration of fast advertising in units of 1 seconds. maximum is 655 seconds */
#define CUSTOM_ADV_SLOW_DURATION               180                                        /**< The advertising duration of slow advertising in units of 1 seconds. maximum is 655 seconds */

// Advertise data
#define CUSTOM_USER_ADVERTISE_DATA \
            "\x03"\
            ADV_TYPE_SERVICE_DATA_16BIT_UUID\
            ADV_UUID_DEVICE_INFORMATION_SERVICE\


#define CUSTOM_USER_ADVERTISE_DATA_LEN (sizeof(CUSTOM_USER_ADVERTISE_DATA)-1)

// Scan response data
#define CUSTOM_USER_ADV_SCNRSP_DATA  \
            "\x0a"\
            ADV_TYPE_MANUFACTURER_SPECIFIC_DATA\
            "\xff\xffNations"

// Scan response data length- maximum 31 bytes
#define CUSTOM_USER_ADV_SCNRSP_DATA_LEN (sizeof(CUSTOM_USER_ADV_SCNRSP_DATA)-1)

/* connection config  */
#define MIN_CONN_INTERVAL                   10                                          /**< Minimum connection interval (15 ms) */
#define MAX_CONN_INTERVAL                   30                                          /**< Maximum connection interval (30 ms). */
#define SLAVE_LATENCY                       0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                    7000                                        /**< Connection supervisory timeout (5000ms). */
    
#define FIRST_CONN_PARAMS_UPDATE_DELAY      (5000)                                      /**<  Time of initiating event to update connection params (5 seconds). */
    
/* sec config */    
#define SEC_PARAM_IO_CAPABILITIES           GAP_IO_CAP_NO_INPUT_NO_OUTPUT               /**< No I/O capabilities. (@enum gap_io_cap) */
#define SEC_PARAM_OOB                       0                                           /**< Out Of Band data not available. */
#define SEC_PARAM_KEY_SIZE                  16                                          /**< Minimum encryption key size. 7 to 16 */
#define SEC_PARAM_BOND                      1                                           /**< Perform bonding. */
#define SEC_PARAM_MITM                      1                                           /**< Man In The Middle protection not required. */
#define SEC_PARAM_LESC                      0                                           /**< LE Secure Connections not enabled. */
#define SEC_PARAM_KEYPRESS                  0                                           /**< Keypress notifications not enabled. */
#define SEC_PARAM_IKEY                      GAP_KDIST_NONE                              /**< Initiator Key Distribution. (@enum gap_kdist) */
#define SEC_PARAM_RKEY                      GAP_KDIST_ENCKEY                            /**< Responder Key Distribution. (@enum gap_kdist) */
#define SEC_PARAM_SEC_MODE_LEVEL            GAP_NO_SEC                                  /**< Device security requirements (minimum security level). (@enum see gap_sec_req) */

/* bond conifg */
#define MAX_BOND_PEER                       5
#define BOND_STORE_ENABLE                   0
#define BOND_DATA_BASE_ADDR                 0x0103B000

/* profiles config  */
#define CFG_PRF_RDTSC                       1
#define CFG_PRF_RDTSS                       1

#define BLE_MASTER_CONN                     1   /**< make sure less than APP_CON_IDX_MAX */
#define BLE_SLAVE_CONN                      2   /**<make sure less than APP_CON_IDX_MAX */
#if (BLE_MASTER_CONN + BLE_SLAVE_CONN > 3)
#error BLE_SLAVE_CONN exceed APP_CON_IDX_MAX 
#endif

/* scan config */
#define SCAN_PARAM_TYPE                     GAPM_SCAN_TYPE_OBSERVER                     /**< scan type (@enum gapm_scan_type) */ 
#define SCAN_PARAM_PROP_ACTIVE              1                                           /**< if requre scan respond data. 1:active, 0:passive*/ 
#define SCAN_PARAM_DUP_FILT_POL             GAPM_DUP_FILT_DIS                           /**< (@enum gapm_dup_filter_pol) */ 
#define SCAN_PARAM_INTV                     160                                         /**< scan interval, unit:0.625ms */ 
#define SCAN_PARAM_WD                       150                                         /**< scan window, unit:0.625ms */ 
#define SCAN_PARAM_DURATION                 0                                           /**< scan duration, unit:10ms, 0 mean forever */ 
#define SCAN_PARAM_CONNECT_EN               true 

/* User config  */
#define NS_LOG_ERROR_ENABLE                 1
#define NS_LOG_WARNING_ENABLE               1
#define NS_LOG_INFO_ENABLE                  1
#define NS_LOG_DEBUG_ENABLE                 0

#define NS_LOG_LPUART_ENABLE                1
#define NS_TIMER_ENABLE                     1

#define FIRMWARE_VERSION                    "1.0.0"
#define HARDWARE_VERSION                    "1.0.0"

#endif // _APP_USER_CONFIG_H_

