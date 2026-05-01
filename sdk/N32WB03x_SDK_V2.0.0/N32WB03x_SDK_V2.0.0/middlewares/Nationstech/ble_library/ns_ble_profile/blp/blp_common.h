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
 * @file blps_common.h
 * @author NSING Firmware Team
 * @version v1.0.2
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */



#ifndef _BLP_COMMON_H_
#define _BLP_COMMON_H_

/**
 ****************************************************************************************
 * @addtogroup BLP Blood Pressure Profile
 * @ingroup PROFILE
 * @brief Blood Pressure Profile
 *
 * The BLP module is the responsible block for implementing the Blood Pressure Profile
 * functionalities in the BLE Host.
 *
 * The Blood Pressure Profile defines the functionality required in a device that allows
 * the user (Collector device) to configure and recover blood pressure measurements from
 * a blood pressure device.
 *****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "prf_types.h"
#include <stdint.h>
#include "prf_utils.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/// BLPC codes for the 2 possible client configuration characteristic descriptors determination in BPS
enum blp_ccc_code
{
    ///Blood Pressure Measurement
    BPS_BP_MEAS_CODE = 0x01,
    ///Intermediate Cuff Pressure Measurement
    BPS_INTERM_CP_CODE,
    ///Record Access Control Point
    BPS_RACP_CODE,
};

/// Blood Pressure Measurement Flags bit field values
enum blp_meas_bf
{
    /// Blood Pressure Units Flag
    /// 0 : Blood pressure for Systolic, Diastolic and MAP in units of mmHg
    /// 1 : Blood pressure for Systolic, Diastolic and MAP in units of kPa
    BPS_MEAS_FLAG_BP_UNITS_POS = 0,
    BPS_MEAS_FLAG_BP_UNITS_BIT = CO_BIT(BPS_MEAS_FLAG_BP_UNITS_POS),

    /// Time Stamp Flag
    /// 0 : not present
    /// 1 : present
    BPS_MEAS_FLAG_TIME_STAMP_POS = 1,
    BPS_MEAS_FLAG_TIME_STAMP_BIT = CO_BIT(BPS_MEAS_FLAG_TIME_STAMP_POS),

    /// Pulse Rate Flag
    /// 0 : not present
    /// 1 : present
    BPS_MEAS_PULSE_RATE_POS = 2,
    BPS_MEAS_PULSE_RATE_BIT = CO_BIT(BPS_MEAS_PULSE_RATE_POS),

    /// User ID Flag
    /// 0 : not present
    /// 1 : present
    BPS_MEAS_USER_ID_POS = 3,
    BPS_MEAS_USER_ID_BIT = CO_BIT(BPS_MEAS_USER_ID_POS),

    /// Measurement Status Flag
    /// 0 : not present
    /// 1 : present
    BPS_MEAS_MEAS_STATUS_POS = 4,
    BPS_MEAS_MEAS_STATUS_BIT = CO_BIT(BPS_MEAS_MEAS_STATUS_POS),

    // Bit 5 - 7 RFU
};

/// Blood Pressure Measurement Status Flags field bit values
enum blp_meas_status_bf
{
    /// Body Movement Detection Flag
    /// 0 : No body movement
    /// 1 : Body movement during measurement
    BPS_STATUS_MVMT_DETECT_POS = 0,
    BPS_STATUS_MVMT_DETECT_BIT = CO_BIT(BPS_STATUS_MVMT_DETECT_POS),

    /// Cuff Fit Detection Flag
    /// 0 : Cuff fits properly
    /// 1 : Cuff too loose
    BPS_STATUS_CUFF_FIT_DETECT_POS = 1,
    BPS_STATUS_CUFF_FIT_DETECT_BIT = CO_BIT(BPS_STATUS_CUFF_FIT_DETECT_POS),

    /// Irregular Pulse Detection Flag
    /// 0 : No irregular pulse detected
    /// 1 : Irregular pulse detected
    BPS_STATUS_IRREGULAR_PULSE_DETECT_POS = 2,
    BPS_STATUS_IRREGULAR_PULSE_DETECT_BIT = CO_BIT(BPS_STATUS_IRREGULAR_PULSE_DETECT_POS),

    /// Pulse Rate Range Detection Flags
    /// value 0 : Pulse rate is within the range
    /// value 1 : Pulse rate exceeds upper limit
    /// value 2 : Pulse rate is less than lower limit
    BPS_STATUS_PR_RANGE_DETECT_LSB_POS = 3,
    BPS_STATUS_PR_RANGE_DETECT_LSB_BIT = CO_BIT(BPS_STATUS_PR_RANGE_DETECT_LSB_POS),

    BPS_STATUS_PR_RANGE_DETECT_MSB_POS = 4,
    BPS_STATUS_PR_RANGE_DETECT_MSB_BIT = CO_BIT(BPS_STATUS_PR_RANGE_DETECT_MSB_POS),

    /// Measurement Position Detection Flag
    /// 0 : Proper measurement position
    /// 1 : Improper measurement position
    BPS_STATUS_MEAS_POS_DETECT_POS = 5,
    BPS_STATUS_MEAS_POS_DETECT_BIT = CO_BIT(BPS_STATUS_MEAS_POS_DETECT_POS),

    // Bit 6 - 15 RFU
};

/// Blood Pressure Feature Flags field bit values
enum blp_feat_flags_bf
{
    ///Body Movement Detection Support bit
    BPS_F_BODY_MVMT_DETECT_SUP_POS = 0,
    BPS_F_BODY_MVMT_DETECT_SUP_BIT = CO_BIT(BPS_F_BODY_MVMT_DETECT_SUP_POS),

    /// Cuff Fit Detection Support bit
    BPS_F_CUFF_FIT_DETECT_SUP_POS = 1,
    BPS_F_CUFF_FIT_DETECT_SUP_BIT = CO_BIT(BPS_F_CUFF_FIT_DETECT_SUP_POS),

    /// Irregular Pulse Detection Support bit
    BPS_F_IRREGULAR_PULSE_DETECT_SUP_POS = 2,
    BPS_F_IRREGULAR_PULSE_DETECT_SUP_BIT = CO_BIT(BPS_F_IRREGULAR_PULSE_DETECT_SUP_POS),

    /// Pulse Rate Range Detection Support bit
    BPS_F_PULSE_RATE_RANGE_DETECT_SUP_POS = 3,
    BPS_F_PULSE_RATE_RANGE_DETECT_SUP_BIT = CO_BIT(BPS_F_PULSE_RATE_RANGE_DETECT_SUP_POS),

    /// Measurement Position Detection Support bit
    BPS_F_MEAS_POS_DETECT_SUP_POS = 4,
    BPS_F_MEAS_POS_DETECT_SUP_BIT = CO_BIT(BPS_F_MEAS_POS_DETECT_SUP_POS),

    /// Multiple Bond Support bit
    BPS_F_MULTIPLE_BONDS_SUP_POS = 5,
    BPS_F_MULTIPLE_BONDS_SUP_BIT = CO_BIT(BPS_F_MULTIPLE_BONDS_SUP_POS),

    // Bit 6 - 15 RFU
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// Blood Pressure measurement structure
struct bps_bp_meas
{
    /// Flag
    uint8_t flags;
    /// User ID
    uint8_t user_id;
    /// Systolic (mmHg/kPa)
    prf_sfloat systolic;
    /// Diastolic (mmHg/kPa)
    prf_sfloat diastolic;
    /// Mean Arterial Pressure (mmHg/kPa)
    prf_sfloat mean_arterial_pressure;
    /// Pulse Rate
    prf_sfloat pulse_rate;
    /// Measurement Status
    uint16_t meas_status;
    /// Time stamp
    struct prf_date_time time_stamp;
};

/// Record Access Control Point structure
struct bps_racp
{
    /// Opcode
    uint8_t opcode;
    /// Operator
    uint8_t op_operator;
    /// Operand
    uint8_t operand;
    /// Data
    uint8_t data[17];           //BLPS_RACP_MAX_LEN-3
};

/// Record Access Control Point structure
struct bps_racp_rsp
{
    /// Opcode
    uint8_t opcode;
    /// Operator
    uint8_t op_operator;
    /// Operand
    uint8_t operand[18];        //BLPS_RACP_MAX_LEN-2
};

/// @} blp_common

#endif /* _BLP_COMMON_H_ */
