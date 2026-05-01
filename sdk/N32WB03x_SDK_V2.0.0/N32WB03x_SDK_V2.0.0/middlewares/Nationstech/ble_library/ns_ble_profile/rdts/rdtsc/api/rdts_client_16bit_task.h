 /**
 *	   Copyright (c) 2025, NSING Technologies Inc.
 * 
 *	   All rights reserved.
 *
 *	   This software is the exclusive property of NSING Technologies Inc. (Hereinafter 
 * referred to as NSING). This software, and the product of NSING described herein 
 * (Hereinafter referred to as the Product) are owned by NSING under the laws and treaties
 * of the People's Republic of China and other applicable jurisdictions worldwide.
 *
 *	   NSING does not grant any license under its patents, copyrights, trademarks, or other 
 * intellectual property rights. Names and brands of third party may be mentioned or referred 
 * thereto (if any) for identification purposes only.
 *
 *	   NSING reserves the right to make changes, corrections, enhancements, modifications, and 
 * improvements to this software at any time without notice. Please contact NSING and obtain 
 * the latest version of this software before placing orders.
 
 *	   Although NSING has attempted to provide accurate and reliable information, NSING assumes 
 * no responsibility for the accuracy and reliability of this software.
 * 
 *	   It is the responsibility of the user of this software to properly design, program, and test 
 * the functionality and safety of any application made of this information and any resulting product. 
 * In no event shall NSING be liable for any direct, indirect, incidental, special,exemplary, or 
 * consequential damages arising in any way out of the use of this software or the Product.
 *
 *	   NSING Products are neither intended nor warranted for usage in systems or equipment, any
 * malfunction or failure of which may cause loss of human life, bodily injury or severe property 
 * damage. Such applications are deemed, "Insecure Usage".
 *
 *	   All Insecure Usage shall be made at user's risk. User shall indemnify NSING and hold NSING 
 * harmless from and against all claims, costs, damages, and other liabilities, arising from or related 
 * to any customer's Insecure Usage.
 
 *	   Any express or implied warranty with regard to this software or the Product, including,but not 
 * limited to, the warranties of merchantability, fitness for a particular purpose and non-infringement
 * are disclaimed to the fullest extent permitted by law.
 
 *	   Unless otherwise explicitly permitted by NSING, anyone may not duplicate, modify, transcribe
 * or otherwise distribute this software for any purposes, in whole or in part.
 *
 *	   NSING products and technologies shall not be used for or incorporated into any products or systems
 * whose manufacture, use, or sale is prohibited under any applicable domestic or foreign laws or regulations. 
 * User shall comply with any applicable export control laws and regulations promulgated and administered by 
 * the governments of any countries asserting jurisdiction over the parties or transactions.
 **/


/**
 * @file rdts_client_16bit_task.h
 * @author NSING Firmware Team
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

#ifndef RDTS_CLIENT_TASK_H_
#define RDTS_CLIENT_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup RDTSCTASK 16bit Raw Data Transfer Service client Task
 * @ingroup RDTSCTASK
 * @brief 16bit Raw Data Transfer Service client Task
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"
#include "ke_task.h"
#include "rdts_client_16bit.h"

#if (BLE_RDTS_16BIT_CLIENT)
/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// Possible states of the RTDS_CLIENT task
enum
{
    /// IDLE state
    RDTSC_IDLE,
    
    /// Connected state
    RDTSC_CONNECTED,
    
    /// Discovering
    RDTSC_DISCOVERING,
    
    /// Number of defined states.
    RDTSC_STATE_MAX
};

///RDTSC Host API messages
enum
{
    /// RDTSC client role enable request from application.
    RDTSC_ENABLE_REQ = TASK_FIRST_MSG(TASK_ID_RDTSC_16BIT),
    
    /// RDTSC Host role enable confirmation to application.
    RDTSC_ENABLE_CFM,
 
    /// Request to transmit data
    RDTSC_DATA_TX_REQ,
    
    /// Confirm that data has been sent
    RDTSC_DATA_TX_CFM,
    
    /// Send data to app
    RDTSC_DATA_RX_IND,

    /// Indicate flow control state
    RDTSC_TX_FLOW_CTRL_IND,
};

/*
 * API Messages Structures
 ****************************************************************************************
 */

///Parameters of the @ref RDTSC_ENABLE_REQ message
struct rdtsc_enable_req
{
    /// Connection handle
    uint8_t conidx;
    
    /// Connection type
    uint8_t con_type;
    
};

///Parameters of the @ref RDTSC_ENABLE_CFM message
struct rdtsc_enable_cfm
{
   
    /// Status
    uint8_t status;
    
    /// RDTS Device details to keep in APP
    struct rdtsc_rdts_content rdts;
};

///Parameters of the @ref RDTSC_DATA_TX_REQ message
///WARNING, DO NOT ALTER THIS STRUCT, IT SHOULD BE COMPATIBLE WITH gattc_write_cmd struct
struct rdtsc_data_tx_req
{
    uint8_t operation;
    /// Perform automatic execution
    /// (if false, an ATT Prepare Write will be used this shall be use for reliable write)
    bool auto_execute;
    /// operation sequence number
    uint16_t seq_num;
    /// Attribute handle
    uint16_t handle;
    /// Write offset
    uint16_t offset;
    /// Write length
    uint16_t length;
    /// Internal write cursor shall be initialized to 0
    uint16_t cursor;
    /// Value to write
    uint8_t data[__ARRAY_EMPTY];
};
///Parameters of the @ref RDTSC_DATA_TX_CFM message
struct rdtsc_data_tx_cfm
{
    ///Status
    uint8_t status;
};

///Parameters of the @ref RDTSC_DATA_RX_IND message
///WARNING, DO NOT ALTER THIS STRUCT, IT SHOULD BE COMPATIBLE WITH gattc_event_ind struct
struct rdtsc_data_rx_ind
{
    /// Event Type
    uint8_t type;
    /// Data length
    uint16_t length;
    /// Attribute handle
    uint16_t handle;
    /// Event Value
    uint8_t data[__ARRAY_EMPTY];
};

///Parameters of the @ref RDTSC_RX_FLOW_CTRL_REQ message
struct rdtsc_rx_flow_ctrl_req
{
    // flow control state
    uint8_t flow_control_state;
};

///Parameters of the @ref RDTSC_TX_FLOW_CTRL_IND message
struct rdtsc_tx_flow_ctrl_ind
{
    // flow control state
    uint8_t flow_control_state;
};

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

extern const struct ke_state_handler rdtsc_16bit_default_handler;

/*
 * Functions
 ****************************************************************************************
 */

#endif //BLE_RDTS_16BIT_CLIENT

/// @} RDTSCTASK

#endif // RDTS_CLIENT_TASK_H_
