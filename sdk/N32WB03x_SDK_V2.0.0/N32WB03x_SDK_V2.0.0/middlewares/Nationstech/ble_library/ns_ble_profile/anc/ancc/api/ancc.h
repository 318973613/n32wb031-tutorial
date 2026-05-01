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
 * @file ancc.h
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
 
#ifndef ANCC_H_
#define ANCC_H_

/**
 ****************************************************************************************
 * @addtogroup ANCC Apple Notification Center Client
 * @ingroup ANCC
 * @brief Apple Notification Center Client
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"

#if (BLE_ANC_CLIENT)

#include "ke_task.h"
#include "prf_types.h"
#include "prf_utils.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/// Maximum number of ANCC task instances
#define ANCC_IDX_MAX                    (BLE_CONNECTION_MAX)
/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// ANCS Characteristics
enum ancc_anc_chars
{
    /// Notification Source
    ANCC_CHAR_NTF_SRC,
    /// Control Point
    ANCC_CHAR_CTRL_PT,
    /// Data Source
    ANCC_CHAR_DATA_SRC,

    ANCC_CHAR_MAX
};

/// ANCS Characteristic Descriptors
enum ancc_anc_descs
{
    /// Notification Source - Client Characteristic Configuration
    ANCC_DESC_NTF_SRC_CL_CFG,
    /// Data Source - Client Characteristic Configuration
    ANCC_DESC_DATA_SRC_CL_CFG,

    ANCC_DESC_MAX,

    ANCC_DESC_MASK = 0x10
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
///Structure containing the characteristics handles, value handles and descriptors

struct ancc_content
{
    /// Service info
    struct prf_svc svc;
    /// Characteristics
    struct prf_char_inf chars[ANCC_CHAR_MAX];
    /// Descriptors
    struct prf_char_desc_inf descs[ANCC_DESC_MAX];
};

/// Structure for ANCS Notification reception variables
struct ancc_data_src_ntf_tag
{
    /// Notification Reception state
    uint8_t state;
    /// Number of requested attributes
    uint8_t nb_req_atts;
    /// Current attribute id
    uint8_t curr_att_id;
    /// Current attribute id
    uint8_t curr_app_id_length;
    /// Current attribute id
    uint8_t curr_att_length;
    /// Remaining bytes for current value
    uint16_t curr_value_idx;
    /// Last allocated notification
    void *alloc_rsp_ind;
};

struct ancc_cnx_env
{
    /// Last requested operation code
    uint8_t last_op_code;
    /// Provide an indication about the last operation
    uint16_t last_req;
    /// Last requested UUID(to keep track of the two services and char)
    uint16_t last_uuid_req;
    /// Last characteristic code used in a read or a write request
    uint16_t last_char_code;
    /// Counter used to check service uniqueness
    uint8_t nb_svc;
    /// Data Source Notification reception variables structure
    struct ancc_data_src_ntf_tag data_src;
    /// ANC characteristics
    struct ancc_content anc;
    /// Current operation code
    void *operation;
};

/// ANCC environment variable
struct ancc_env_tag
{
    /// Profile environment
    prf_env_t prf_env;
    /// Environment variable pointer for each connections
    struct ancc_cnx_env* env[ANCC_IDX_MAX];
    /// State of different task instances
    ke_state_t state[ANCC_IDX_MAX];
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Retrieve ANC client profile interface
 * @return ANC client profile interface
 ****************************************************************************************
 */
const struct prf_task_cbs *ancc_prf_itf_get(void);

/**
 ****************************************************************************************
 * @brief Send ANC ATT DB discovery results to ANCC host.
 ****************************************************************************************
 */
void ancc_enable_rsp_send(struct ancc_env_tag *ancc_env, uint8_t conidx, uint8_t status);

/**
 ****************************************************************************************
 * @brief Send Notification Configuration Response
 ****************************************************************************************
 */
void ancc_wr_cfg_ntf_rsp_send(struct ancc_env_tag *ancc_env, uint8_t conidx,
                              uint8_t op_code, uint8_t status);

/**
 ****************************************************************************************
 * @brief Send Read Notification Configuration Response
 ****************************************************************************************
 */
void ancc_rd_cfg_ntf_rsp_send(struct ancc_env_tag *ancc_env, uint8_t conidx,
                              uint8_t op_code, uint16_t value, uint8_t status);

/**
 ****************************************************************************************
 * @brief Send Operation Completion Event
 ****************************************************************************************
 */
void ancc_cmp_evt_send(struct ancc_env_tag *ancc_env, uint8_t conidx, uint8_t op_code,
                       uint8_t status);

/**
 ****************************************************************************************
 * @brief Handles reception of response to a Get Notification Attributes command.
 * @param[in] ancc_env Pointer to the profile environment.
 * @param[in] conidx Connection index
 * @param[in] value Pointer to the response value.
 * @param[in] length Length of the response value.
 ****************************************************************************************
 */
void ancc_get_ntf_atts_rsp(struct ancc_env_tag *ancc_env, uint8_t conidx,
                           const uint8_t *value, uint16_t length);

/**
 ****************************************************************************************
 * @brief Handles reception of response to a Get App Attributes command.
 * @param[in] ancc_env Pointer to the profile environment.
 * @param[in] value Pointer to the response value.
 * @param[in] length Length of the response value.
 ****************************************************************************************
 */
void ancc_get_app_atts_rsp(struct ancc_env_tag *ancc_env, uint8_t conidx,
                           const uint8_t *value, uint16_t length);

/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * Initialize task handler
 *
 * @param task_desc Task descriptor to fill
 ****************************************************************************************
 */
void ancc_task_init(struct ke_task_desc *p_task_desc);

#endif // (BLE_ANC_CLIENT)

/// @} ANCC

#endif // ANCC_H_
