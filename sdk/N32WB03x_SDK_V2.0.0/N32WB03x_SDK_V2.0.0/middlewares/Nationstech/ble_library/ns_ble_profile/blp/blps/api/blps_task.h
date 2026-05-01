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
 * @file blps_task.h
 * @author NSING Firmware Team
 * @version v1.0.2
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */



#ifndef _BLPS_TASK_H_
#define _BLPS_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup BLPSTASK Task
 * @ingroup BLPS
 * @brief Blood Pressure Profile Task.
 *
 * The BLPSTASK is responsible for handling the messages coming in and out of the
 * @ref BLPS collector block of the BLE Host.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>
#include "rwip_task.h" // Task definitions
#include "blp_common.h"

/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

enum
{
    /// measurement sent by profile
    BLPS_BP_MEAS_SEND,
    /// peer device confirm reception
    BLPS_CENTRAL_IND_CFM,
};

/// Messages for Blood Pressure Profile Sensor
/*@TRACE*/
enum blps_msg_id
{
    /// Start the Blood Pressure Profile Sensor - at connection
    BLPS_ENABLE_REQ = TASK_FIRST_MSG(TASK_ID_BLPS),
    /// Start the Blood Pressure Profile Sensor - at connection
    BLPS_ENABLE_RSP,

    /// Send blood pressure measurement value from APP
    BLPS_MEAS_SEND_CMD,

    /// Inform APP of new configuration value
    BLPS_CFG_INDNTF_IND,

    /// Inform APP of new RACP value
    BLPS_RACP_WRITE_IND,
    
    /// Send record access control point response value from APP
    BLPS_RACP_RESP_SEND_CMD,
    
    /// Complete Event Information
    BLPS_CMP_EVT,
};

/// Operation codes
enum blps_op_codes
{
    /// Database Creation Procedure
    BLPS_RESERVED_OP_CODE = 0,

    /// Indicate Measurement Operation Code
    BLPS_MEAS_SEND_CMD_OP_CODE = 1,
    
    /// Indicate Record Access Control Point Code
    BLPS_RACP_RSP_SEND_CMD_OP_CODE = 2,
};

/// Parameters of the @ref BLPS_ENABLE_REQ message
struct blps_enable_req
{
    ///Connection index
    uint8_t conidx;

    /// Blood Pressure indication configuration
    uint16_t bp_meas_ind_en;
    /// Intermediate Cuff Pressure Notification configuration
    uint16_t interm_cp_ntf_en;
    /// Record Access Control Point configuration
    uint16_t racp_ind_en;
};

/// Parameters of the @ref BLPS_ENABLE_RSP message
struct blps_enable_rsp
{
    ///Connection index
    uint8_t conidx;
    ///Status
    uint8_t status;
};

///Parameters of the @ref BLPS_CFG_INDNTF_IND message
struct blps_cfg_indntf_ind
{
    ///Connection index
    uint8_t conidx;
    ///Own code for differentiating between Blood Pressure Measurement, and Intermediate
    /// Cuff Pressure Measurement characteristics
    uint8_t char_code;
    ///Stop/notify/indicate value to configure into the peer characteristic
    uint16_t cfg_val;
};

/////Parameters of the @ref BLPS_MEAS_SEND_CMD message
struct blps_meas_send_cmd
{
    ///Connection index
    uint8_t conidx;
    /// Flag indicating if it is a intermediary cuff pressure measurement (0) or
    /// stable blood pressure measurement (1).
    uint8_t flag_interm_cp;
    ///Blood Pressure measurement
    struct bps_bp_meas meas_val;
};

/////Parameters of the @ref BLPS_RACP_WRITE_IND message
struct blps_racp_write_ind
{
    ///Connection index
    uint8_t conidx;
    /// RACP write value
    struct bps_racp write_val;
    /// RACP write value length
    uint16_t write_val_len;
};

/////Parameters of the @ref BLPS_RACP_RESP_SEND_CMD message
struct blps_racp_resp_send_cmd
{
    ///Connection index
    uint8_t conidx;
    /// RACP response value
    struct bps_racp_rsp write_val;
    /// RACP response value length
    uint16_t write_val_len;
};

///Parameters of the @ref BLPS_CMP_EVT message
struct blps_cmp_evt
{
    /// Operation
    uint8_t operation;
    /// Operation code      see enum blps_op_codes
    uint8_t operation_code;
    /// Status
    uint8_t status;
};

///Parameters of the @ref BLPS_CREATE_DB_REQ message
struct blps_db_cfg
{
    /// Supported features
    uint16_t features;
    /// Profile Configuration
    uint8_t prfl_cfg;
};

/// @} BLPSTASK

#endif /* _BLPS_TASK_H_ */
