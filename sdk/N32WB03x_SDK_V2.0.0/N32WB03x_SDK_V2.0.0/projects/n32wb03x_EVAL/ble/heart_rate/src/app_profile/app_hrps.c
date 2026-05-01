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
 * @file app_hrps.c
 * @author NSING Firmware Team
 * @version v1.0.2
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

/** 
 * @addtogroup APP
 * @{ 
 */

#include "rwip_config.h"     // SW configuration

#if (BLE_APP_HRPS)
/* Includes ------------------------------------------------------------------*/
#include "ns_ble.h"                     // Application Manager Definitions
#include "app_hrps.h"                // Device Information Service Application Definitions
#include "hrps_task.h"               // Device Information Profile Functions
#include "prf_types.h"               // Profile Common Types Definitions
#include "ke_task.h"                 // Kernel
#include "gapm_task.h"               // GAP Manager Task API
#include <string.h>
#include "co_utils.h"
#include "hrps.h"
#include "stdio.h"
#include "ns_timer.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/// Heart Rate Application environment
struct app_hrps_env_tag app_hrps_env;

uint16_t heart_rate_timer_id;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
 * @brief  hrps server Initialize
 * @param  
 * @return 
 * @note   
 */
void app_hrps_init(void)
{
    app_hrps_env.hrps_value = 35;
    
    //register application subtask to app task
    struct prf_task_t prf;
    prf.prf_task_id = TASK_ID_HRPS;
    prf.prf_task_handler = &app_hrps_handlers;
    ns_ble_prf_task_register(&prf);
    
    //register get itf function to prf.c
    struct prf_get_func_t get_func;
    get_func.task_id = TASK_ID_HRPS;
    get_func.prf_itf_get_func = hrps_prf_itf_get;
    prf_get_itf_func_register(&get_func);
}

/**
 * @brief  add hrps server
 * @param  
 * @return 
 * @note   
 */
void app_hrps_add_hrps(void)
{
    struct hrps_db_cfg* db_cfg;
    // Allocate the DISS_CREATE_DB_REQ
    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                                  TASK_GAPM, TASK_APP,
                                                  gapm_profile_task_add_cmd, sizeof(struct hrps_db_cfg));
    // Fill message
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl =  PERM(SVC_AUTH, NO_AUTH);

    req->prf_task_id = TASK_ID_HRPS;
    req->app_task = TASK_APP;
    req->start_hdl = 0;

    // Set parameters
    db_cfg = (struct hrps_db_cfg* ) req->param;
    db_cfg->features = APP_HRPS_FEATURES;

    // Send the message
    ke_msg_send(req);
    
    app_hrps_init();
}

/**
 * @brief  enable heart rate profile
 * @param  
 * @return 
 * @note   
 */
void app_hrps_enable_prf(uint8_t conidx, uint8_t cfg)
{
    app_hrps_env.conidx = conidx;

    // Allocate the message
    struct hrps_enable_req * req = KE_MSG_ALLOC(HRPS_ENABLE_REQ,
                                                prf_get_task_from_id(TASK_ID_HRPS),
                                                TASK_APP,
                                                hrps_enable_req);

    // Fill in the parameter structure
    req->conidx             = conidx;

    // NTF status
    req->hr_meas_ntf        = cfg;

    // Send the message
    ke_msg_send(req);
}

/**
 * @brief  send heart rate value 
 * @param  
 * @return 
 * @note   
 */
void app_heart_rate_send(uint16_t heart_rate)
{
    // Allocate the message
    struct hrps_meas_send_cmd * cmd = KE_MSG_ALLOC(HRPS_MEAS_SEND_CMD,
                                                        prf_get_task_from_id(TASK_ID_HRPS),
                                                        TASK_APP,
                                                        hrps_meas_send_cmd);

    // Fill in the parameter structure
    cmd->meas_val.heart_rate = heart_rate;

    // Send the message
    ke_msg_send(cmd);
}

/**
 * @brief heart rate send timeout handler
 * @param  
 * @return 
 * @note   
 */
void app_heart_rate_timeout_handler(void)
{
    app_hrps_env.hrps_value ++;
    
    if(app_hrps_env.hrps_value >= 180)
    {
        app_hrps_env.hrps_value = 35;
    }
    
    app_heart_rate_send(app_hrps_env.hrps_value);
    
    ns_timer_cancel(heart_rate_timer_id);
    
    heart_rate_timer_id = ns_timer_create(HEART_RATE_SEND_DELAY,app_heart_rate_timeout_handler);
}

/**
 * @brief  HRPS config indicate handler
 * @param  
 * @return 
 * @note   
 */
static int hrps_cfg_indntf_ind_handler(ke_msg_id_t const msgid,
                                          struct hrps_cfg_indntf_ind const *param,
                                          ke_task_id_t const dest_id,
                                          ke_task_id_t const src_id)
{
    if( (param->cfg_val & HRPS_CFG_NTF_EN) != 0)
    {
        app_hrps_enable_prf(0, PRF_CLI_START_NTF);
        heart_rate_timer_id = ns_timer_create(HEART_RATE_SEND_DELAY,app_heart_rate_timeout_handler);
    }
    else
    {
        app_hrps_enable_prf(0, PRF_CLI_STOP_NTFIND);
        ns_timer_cancel(heart_rate_timer_id);
    }
    
    return KE_MSG_CONSUMED;
}


/* Public variables ---------------------------------------------------------*/

/// Default State handlers definition
const struct ke_msg_handler app_hrps_msg_handler_list[] =
{
    {HRPS_CFG_INDNTF_IND,     (ke_msg_func_t)hrps_cfg_indntf_ind_handler},
};

const struct app_subtask_handlers app_hrps_handlers = APP_HANDLERS(app_hrps);


#endif //BLE_APP_HRPS

/// @} APP
