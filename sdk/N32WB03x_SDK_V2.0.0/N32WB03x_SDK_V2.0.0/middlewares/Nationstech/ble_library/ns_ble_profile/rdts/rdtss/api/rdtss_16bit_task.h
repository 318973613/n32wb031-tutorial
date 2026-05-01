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
 * @file rdtss_16bit_task.h
 * @author NSING Firmware Team
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
 
#ifndef __RDTSS_16BIT_TASK_PRF_H
#define __RDTSS_16BIT_TASK_PRF_H

 /* Includes ------------------------------------------------------------------*/
#if (BLE_RDTSS_16BIT_SERVER)

#include <stdint.h>
#include "ke_task.h"
#include "prf_types.h"
#include "compiler.h"        // compiler definition
#include "att.h"
#include "attm.h"

#include "rwip_task.h"
/* Public typedef -----------------------------------------------------------*/

/// Possible states of the rdtss_16bit task
enum rdtss_16bit_state
{
    /// Idle state
    RDTSS_16BIT_IDLE,
    /// Busy state
    RDTSS_16BIT_BUSY,
    /// Number of defined states.
    RDTSS_16BIT_STATE_MAX,
};

/// Messages for RDTSS_16BIT
enum
{
    /// Add a rdtss_16bit instance into the database
    RDTSS_16BIT_CREATE_DB_REQ = TASK_FIRST_MSG(TASK_ID_RDTSS_16BIT),
    /// Inform APP of database creation status
    RDTSS_16BIT_CREATE_DB_CFM,

    /// Start the Custom Service Task - at connection
    RDTSS_16BIT_ENABLE_REQ,
    /// Set/update characteristic value
    RDTSS_16BIT_VAL_SET_REQ,
    /// Peer device request to get a non-database value (RI enabled)
    RDTSS_16BIT_VALUE_REQ_IND,
    /// Response to non-database value request
    RDTSS_16BIT_VALUE_REQ_RSP,
    /// Set/update characteristic value and trigger a notification
    RDTSS_16BIT_VAL_NTF_REQ,
    /// Response after receiving a RDTSS_16BIT_VAL_NTF_REQ message and a notification is triggered
    RDTSS_16BIT_VAL_NTF_CFM,
    /// Set/update characteristic value and trigger an indication
    RDTSS_16BIT_VAL_IND_REQ,
    ///Response after receiving a RDTSS_16BIT_VAL_IND_REQ message and an indication is triggered
    RDTSS_16BIT_VAL_IND_CFM,
    /// Indicate that the characteristic value has been written
    RDTSS_16BIT_VAL_WRITE_IND,
    /// Inform the application that the profile service role task has been disabled after a disconnection
    RDTSS_16BIT_DISABLE_IND,
    /// Profile error report
    RDTSS_16BIT_ERROR_IND,
    /// Inform the application that there is an attribute info request that shall be processed
    RDTSS_16BIT_ATT_INFO_REQ,
    /// Inform back that the attribute info request has been processed
    RDTSS_16BIT_ATT_INFO_RSP,
};



/// Parameters of the @ref RDTSS_16BIT_CREATE_DB_CFM message
struct rdtss_16bit_create_db_cfm
{
    ///Status
    uint8_t status;
};

/// Parameters of the @ref RDTSS_16BIT_ENABLE_REQ message
struct rdtss_16bit_enable_req
{
    /// Connection index
    uint8_t conidx;
    /// security level: b0= nothing, b1=unauthenticated, b2=authenticated, b3=authorized; b1 or b2 and b3 can go together
    uint8_t sec_lvl;
    /// Type of connection
    uint8_t con_type;
};

/// Parameters of the @ref RDTSS_16BIT_DISABLE_IND message
struct rdtss_16bit_disable_ind
{
    /// Connection index
    uint8_t conidx;
};

/// Parameters of the @ref RDTSS_16BIT_VAL_WRITE_IND massage
struct rdtss_16bit_val_write_ind
{
    /// Connection index
    uint8_t  conidx;
    /// Handle of the attribute that has to be written
    uint16_t handle;
    /// Data length to be written
    uint16_t length;
    /// Data to be written in attribute database
    uint8_t  value[__ARRAY_EMPTY];
};

/// Parameters of the @ref RDTSS_16BIT_VAL_NTF_CFM massage
struct rdtss_16bit_val_ntf_cfm
{
    /// Connection index
//    uint8_t  conidx;
    /// Handle of the attribute that has been updated
    uint16_t handle;
    /// Confirmation status
    uint8_t status;
};

/// Parameters of the @ref RDTSS_16BIT_VAL_IND_CFM massage
struct rdtss_16bit_val_ind_cfm
{
    /// Connection index
    uint8_t  conidx;
    /// Handle of the attribute that has been updated
    uint16_t handle;
    /// Confirmation status
    uint8_t status;
};

/// Parameters of the @ref RDTSS_16BIT_VAL_SET_REQ massage
struct rdtss_16bit_val_set_req
{
    /// Connection index
    uint8_t  conidx;
    /// Handle of the attribute that has to be written
    uint16_t handle;
    /// Data length to be written
    uint16_t length;
    /// Data to be written in attribute database
    uint8_t  value[__ARRAY_EMPTY];
};

/// Parameters of the @ref RDTSS_16BIT_VAL_REQ_IND message
struct rdtss_16bit_value_req_ind
{
    /// Connection index
    uint8_t  conidx;
    /// Index of the attribute for which value has been requested
    uint16_t att_idx;
};

/// Parameters of the @ref RDTSS_16BIT_VAL_REQ_RSP message
struct rdtss_16bit_value_req_rsp
{
    /// Connection index
    uint8_t  conidx;
    /// Index of the attribute for which value has been requested
    uint16_t att_idx;
    /// Current length of that attribute
    uint16_t length;
    /// ATT error code
    uint8_t  status;
    /// Data value
    uint8_t  value[__ARRAY_EMPTY];
};

/// Parameters of the @ref RDTSS_16BIT_VAL_NTF_REQ massage
struct rdtss_16bit_val_ntf_ind_req
{
    /// Connection index
    uint8_t  conidx;
    /// Notificatin/indication
    bool     notification;
    /// Handle of the attribute that has to be written
    uint16_t handle;
    /// Data length to be written
    uint16_t length;
    /// Data to be written in attribute database
    uint8_t  value[__ARRAY_EMPTY];
};

/// Parameters of the @ref RDTSS_16BIT_VAL_IND_REQ massage
struct rdtss_16bit_val_ind_req
{
    /// Connection index
    uint8_t  conidx;
    /// Handle of the attribute that has to be written
    uint16_t handle;
    /// Data length to be written
    uint16_t length;
    /// Data to be written in attribute database
    uint8_t  value[__ARRAY_EMPTY];
};

/// Parameters of the @ref RDTSS_16BIT_ATT_INFO_REQ message
struct rdtss_16bit_att_info_req
{
    /// Connection index
    uint8_t  conidx;
    /// Index of the attribute for which info has been requested
    uint16_t att_idx;
};

/// Parameters of the @ref RDTSS_16BIT_ATT_INFO_RSP message
struct rdtss_16bit_att_info_rsp
{
    /// Connection index
    uint8_t  conidx;
    /// Index of the attribute for which info has been requested
    uint16_t att_idx;
    /// Current length of that attribute
    uint16_t length;
    /// ATT error code
    uint8_t  status;
};

/** 
 * @brief Initialize Client Characteristic Configuration fields.
 * @details Function initializes all CCC fields to default value.
 * @param[in] att_db         Id of the message received.
 * @param[in] max_nb_att     Pointer to the parameters of the message. 
 */
void rdtss_16bit_init_ccc_values(const struct attm_desc *att_db, int max_nb_att);

/** 
 * @brief Set per connection CCC value for attribute
 * @details Function sets CCC for specified connection.
 * @param[in] conidx         Connection index.
 * @param[in] att_idx        Attribute index.
 * @param[in] ccc            Value of ccc. 
 */
void rdtss_16bit_set_ccc_value(uint8_t conidx, uint8_t att_idx, uint16_t ccc);


#endif // BLE_RDTSS_16BIT_SERVER
#endif // __RDTSS_16BIT_TASK_PRF_H
