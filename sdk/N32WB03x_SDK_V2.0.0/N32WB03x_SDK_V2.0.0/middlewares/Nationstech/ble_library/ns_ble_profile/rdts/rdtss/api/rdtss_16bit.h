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
 * @file rdtss_16bit.h
 * @author NSING Firmware Team
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

#ifndef _RDTSS_16BIT_H_
#define _RDTSS_16BIT_H_

#include "rwip_config.h"              // SW configuration

#if (BLE_RDTSS_16BIT_SERVER)

 /* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "prf_types.h"
#include "prf.h"
#include "attm.h"

#include "rdts_common.h"

#define RDTSS_16BIT_IDX_MAX        (1)

/* Public typedef -----------------------------------------------------------*/

/// Parameters for the database creation
struct rdtss_16bit_db_cfg
{
    ///max number of service characteristics
    uint8_t max_nb_att;
    const struct attm_desc* att_tbl;
    const uint16_t *svc_uuid;

};

/// Value element
struct rdtss_16bit_val_elmt
{
    /// list element header
    struct co_list_hdr hdr;
    /// value identifier
    uint8_t att_idx;
    /// value length
    uint8_t length;
    /// value data
    uint8_t data[__ARRAY_EMPTY];
};

/// rdtss environment variable
struct rdtss_16bit_env_tag
{
    /// profile environment
    prf_env_t prf_env;
    /// Service Start Handle
    uint16_t shdl;
    /// To store the DB max number of attributes
    uint8_t max_nb_att;
    /// On-going operation
    struct ke_msg *operation;

    /// Cursor on connection used to notify peer devices
    uint8_t cursor;
    /// CCC handle index, used during notification/indication busy state
    uint8_t ccc_idx;

    /// List of values set by application
    struct co_list values;
    /// rdtss_16bit task state
    ke_state_t state[RDTSS_16BIT_IDX_MAX];
    /// rdtss_16bit db configer
    struct rdtss_16bit_db_cfg db_cfg;
};

/* Private functions ---------------------------------------------------------*/

/** 
 * @brief Disable actions grouped in getting back to IDLE and sending configuration to requester task
 * @param[in] conhdl    Connection Handle 
 */
void rdtss_16bit_disable(uint16_t conhdl);

const struct prf_task_cbs *rdtss_16bit_prf_itf_get(void);
    
/** 
 * Initialize task handler
 *
 * @param p_task_desc Task descriptor to fill 
 */
void rdtss_16bit_task_init(struct ke_task_desc *p_task_desc);

#endif // (BLE_RDTSS_16BIT_SERVER)

#endif // _RDTSS_16BIT_H_
