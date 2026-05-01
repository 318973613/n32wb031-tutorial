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
 * @file app_blps.h
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */


#ifndef APP_BLPS_H_
#define APP_BLPS_H_

/**
 * @addtogroup APP
 *
 * @brief Blood Pressure Application Module Entry point.
 *
 * @{
 **/

/* Includes ------------------------------------------------------------------*/

#include "rwip_config.h"     // SW Configuration

#if (BLE_APP_BLPS)

#include <stdint.h>
#include "blp_common.h"
/* Define ------------------------------------------------------------*/

#define APP_BLPS_ALL_FEATURES               0x1FF

#define APP_BLPS_SUP_FEATUEES               APP_BLPS_ALL_FEATURES

enum blps_cfg
{
    BLPS_CFG_NTFIND_STP,
    BLPS_CFG_NTFIND_NTF_EN,
    BLPS_CFG_NTFIND_IND_EN,
};

enum app_blps_prfl_cfg
{
    APP_BLPS_INTM_CUFF_PRESS_SUP = 1,
    APP_BLPS_RACP_SUP = 2,
};



/* Public variables ---------------------------------------------------------*/

/// Table of message handlers
extern const struct app_subtask_handlers app_blps_handlers;

/* Public function prototypes -----------------------------------------------*/

/**
 * @brief Initialize Blood Pressure Sensor Application
 **/
void app_blps_init(void);

/**
 * @brief Add a Blood Pressure Sensor instance in the DB
 **/
void app_blps_add_blps(void);

/**
 * @brief Enable Blood Pressure Sensor 
 */
void app_blps_enable_prf(uint8_t conidx);

/**
 * @brief  Send Blood Pressure Measurement value  
 */
void app_blps_measurement_send(uint8_t conidx, struct bps_bp_meas *data);

/**
 * @brief  Send Intermediate Cuff Pressure value 
 */
void app_blps_intm_cuff_press_send(uint8_t conidx, struct bps_bp_meas *data);

/**
 * @brief  send record access control point response value 
 */
void app_blps_racp_resp_send(uint8_t conidx, uint8_t *data, uint8_t data_len);

#endif //BLE_APP_BLPSS

/// @} APP

#endif //APP_BLPS_H_
