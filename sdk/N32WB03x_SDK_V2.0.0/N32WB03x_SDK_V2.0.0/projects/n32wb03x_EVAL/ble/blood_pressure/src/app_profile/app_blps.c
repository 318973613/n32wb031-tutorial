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
 * @file app_blps.c
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

/** 
 * @addtogroup APP
 * @{ 
 */

#include "rwip_config.h"             // SW configuration

#if (BLE_APP_BLPS)
/* Includes ------------------------------------------------------------------*/
#include "ns_ble.h"                     // Application Manager Definitions
#include "app_blps.h"                // BLPS Service Application Definitions
#include "blps_task.h"               // BLPS Profile Functions
#include "prf_types.h"               // Profile Common Types Definitions
#include "ke_task.h"                 // Kernel
#include "gapm_task.h"               // GAP Manager Task API
#include <string.h>
#include "co_utils.h"
#include "ns_timer.h"
#include "blps.h"
#include "ns_ble_task.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BLOOD_PRESSURE_SEND_DELAY 1000
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
struct bps_bp_meas  app_meas;
    

void app_blood_pressure_timeout_handler(void)
{
    //send simulated data
    app_blps_measurement_send(app_env.conidx, &app_meas);
    
    //update simulated data  
    app_meas.diastolic -= 1;
    app_meas.systolic  += 1;
    app_meas.mean_arterial_pressure -= 1;
    app_meas.pulse_rate += 1;
    
    //update time_stamp 
    app_meas.time_stamp.sec++;
    if(app_meas.time_stamp.sec>59)
    {
        app_meas.time_stamp.sec = 0;
        app_meas.time_stamp.min++;
        if(app_meas.time_stamp.min>59) 
        {
            app_meas.time_stamp.min = 0;
        }
    }
    
    //reset the timer
    if(ke_state_get(TASK_APP)== APP_CONNECTED)
    {
        ns_timer_create(BLOOD_PRESSURE_SEND_DELAY,app_blood_pressure_timeout_handler);
    }
}




/**
 * @brief  BLPS config indicate handler
 * @param  
 * @return 
 * @note   
 */
static int blps_cfg_indntf_ind_handler(ke_msg_id_t const msgid,
                                          struct blps_cfg_indntf_ind const *param,
                                          ke_task_id_t const dest_id,
                                          ke_task_id_t const src_id)
{
    if(param->char_code == BPS_BP_MEAS_CHAR)
    {
        if(param->cfg_val & PRF_CLI_START_IND)
        {
            NS_LOG_INFO("BPS_BP_MEAS_CHAR START_IND\r\n");
            ns_timer_create(BLOOD_PRESSURE_SEND_DELAY,app_blood_pressure_timeout_handler);
        }
        else
        {
            NS_LOG_INFO("BPS_BP_MEAS_CHAR STOP\r\n");
        }
    }
    else if(param->char_code == BPS_INTM_CUFF_MEAS_CHAR)
    {
        if(param->cfg_val & PRF_CLI_START_NTF)
        {
            NS_LOG_INFO("BPS_INTM_CUFF_MEAS_CHAR START_IND\r\n");
        }
        else
        {
            NS_LOG_INFO("BPS_INTM_CUFF_MEAS_CHAR STOP\r\n");
        }
    }
    else if(param->char_code == BPS_RACP_CHAR)
    {
        if(param->cfg_val & PRF_CLI_START_IND)
        {
            NS_LOG_INFO("BPS_RACP_CHAR START_IND\r\n");
        }
        else
        {
            NS_LOG_INFO("BPS_RACP_CHAR STOP\r\n");
        }
    }

    return KE_MSG_CONSUMED;
}


/**
 * @brief  enable blood pressure sensor
 * @param  
 * @return 
 * @note   
 */
void app_blps_enable_prf(uint8_t conidx)
{
    // Allocate the message
    struct blps_enable_req * req = KE_MSG_ALLOC(BLPS_ENABLE_REQ,
                                                prf_get_task_from_id(TASK_ID_BLPS),
                                                TASK_APP,
                                                blps_enable_req);

    // Fill in the parameter structure
    req->conidx             = conidx;

    req->bp_meas_ind_en     = PRF_CLI_STOP_NTFIND;
    req->interm_cp_ntf_en   = PRF_CLI_STOP_NTFIND;
    req->racp_ind_en        = PRF_CLI_STOP_NTFIND;
    
    // Send the message
    ke_msg_send(req);
}

/**
 * @brief  blood pressure sensor Initialize
 * @param  
 * @return 
 * @note
 */
void app_blps_init(void)
{
    //Initialize simulated data 
    app_meas.flags   = BPS_MEAS_PULSE_RATE_BIT|BPS_MEAS_FLAG_TIME_STAMP_BIT;
    app_meas.systolic               = 115;
    app_meas.diastolic              = 74;
    app_meas.mean_arterial_pressure = 102;
    app_meas.pulse_rate             = 61;
    //time stamp
    app_meas.time_stamp.year        = 2022;
    app_meas.time_stamp.month       = 3;
    app_meas.time_stamp.day         = 21;
    app_meas.time_stamp.hour        = 17;
    app_meas.time_stamp.sec         = 55;
    
    //register application subtask to app task
    struct prf_task_t prf;
    prf.prf_task_id = TASK_ID_BLPS;
    prf.prf_task_handler = &app_blps_handlers;
    ns_ble_prf_task_register(&prf);
    
    //register get itf function to prf.c
    struct prf_get_func_t get_func;
    get_func.task_id = TASK_ID_BLPS;
    get_func.prf_itf_get_func = blps_prf_itf_get;
    prf_get_itf_func_register(&get_func);
    
}

/**
 * @brief  add blood pressure sensor service
 * @param  
 * @return 
 * @note   
 */
void app_blps_add_blps(void)
{
    struct blps_db_cfg* db_cfg;
    
    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                                  TASK_GAPM, TASK_APP,
                                                  gapm_profile_task_add_cmd, sizeof(struct blps_db_cfg));
    // Fill message
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl =  PERM(SVC_AUTH, NO_AUTH);

    req->prf_task_id = TASK_ID_BLPS;
    req->app_task = TASK_APP;
    req->start_hdl = 0;

    // Set parameters
    db_cfg = (struct blps_db_cfg* ) req->param;
    
    db_cfg->prfl_cfg = APP_BLPS_INTM_CUFF_PRESS_SUP | APP_BLPS_RACP_SUP;
    db_cfg->features = APP_BLPS_SUP_FEATUEES;
    
    // Send the message
    ke_msg_send(req);
    
    app_blps_init();
}

/**
 * @brief  send blood pressure value 
 * @param  
 * @return 
 * @note   
 */
void app_blps_measurement_send(uint8_t conidx, struct bps_bp_meas *data)
{    
    struct blps_env_tag *p_blps_env = PRF_ENV_GET(BLPS, blps);

    if (GETB(p_blps_env->prfl_ntf_ind_cfg[conidx], BLPS_BP_MEAS_IND_CFG))
    {
        // Allocate the message
        struct blps_meas_send_cmd * cmd = KE_MSG_ALLOC(BLPS_MEAS_SEND_CMD,
                                                            prf_get_task_from_id(TASK_ID_BLPS),
                                                            TASK_APP,
                                                            blps_meas_send_cmd);
        
        // Fill in the parameter structure
        cmd->conidx = conidx;
        cmd->flag_interm_cp = 0;        //refer: flag_interm_cp
        memcpy(&(cmd->meas_val), data, sizeof(struct bps_bp_meas));

        // Send the message
        ke_msg_send(cmd);
    }
    else
    {
        NS_LOG_INFO("BP_MEAS_SEND ERR\r\n");
    }
}

/**
 * @brief  send intermediate cuff pressure value 
 * @param  
 * @return 
 * @note   
 */
void app_blps_intm_cuff_press_send(uint8_t conidx, struct bps_bp_meas *data)
{    
    struct blps_env_tag *p_blps_env = PRF_ENV_GET(BLPS, blps);

    if (GETB(p_blps_env->prfl_ntf_ind_cfg[conidx], BLPS_INTM_CUFF_PRESS_NTF_CFG))
    {
        // Allocate the message
        struct blps_meas_send_cmd * cmd = KE_MSG_ALLOC(BLPS_MEAS_SEND_CMD,
                                                            prf_get_task_from_id(TASK_ID_BLPS),
                                                            TASK_APP,
                                                            blps_meas_send_cmd);

        // Fill in the parameter structure
        cmd->conidx = conidx;
        cmd->flag_interm_cp = 1;        //refer: flag_interm_cp
        memcpy(&(cmd->meas_val), data, BLPS_BP_MEAS_MAX_LEN);

        // Send the message
        ke_msg_send(cmd);
    }
    else
    {
        NS_LOG_INFO("BP_INTF_CUFF_SEND ERR\r\n");
    }
}

/**
 * @brief  send record access control point response value 
 * @param  
 * @return 
 * @note
 */
void app_blps_racp_resp_send(uint8_t conidx, uint8_t *data, uint8_t data_len)
{    
    struct blps_env_tag *p_blps_env = PRF_ENV_GET(BLPS, blps);

    if (GETB(p_blps_env->prfl_ntf_ind_cfg[conidx], BLPS_RACP_IND_CFG))
    {
        // Allocate the message
        struct blps_racp_resp_send_cmd * cmd = KE_MSG_ALLOC_DYN(BLPS_RACP_RESP_SEND_CMD,
                                                            prf_get_task_from_id(TASK_ID_BLPS),
                                                            TASK_APP,
                                                            blps_racp_resp_send_cmd,
                                                            BLPS_RACP_MAX_LEN);
        
        
        // Fill in the parameter structure
        cmd->conidx = conidx;
        cmd->write_val_len = data_len;
        
        memcpy(&cmd->write_val, data, data_len);

        // Send the message
        ke_msg_send(cmd);
    }
    else
    {
        NS_LOG_INFO("BP_RACP_SEND ERR\r\n");
    }
}

/**
 * @brief  BLPS enable response handler
 * @param  
 * @return 
 * @note   
 */
static int blps_enable_rsp_handler(ke_msg_id_t const msgid,
                                          struct blps_enable_rsp const *param,
                                          ke_task_id_t const dest_id,
                                          ke_task_id_t const src_id)
{
    NS_LOG_INFO("BLPS PROFILE ENABLE STATUS: %x\r\n", param->status);
    return KE_MSG_CONSUMED;
}

/**
 * @brief  BLPS RACP write handler
 * @param  
 * @return 
 * @note   
 */
static int blps_racp_write_ind_handler(ke_msg_id_t const msgid,
                                          struct blps_racp_write_ind const *param,
                                          ke_task_id_t const dest_id,
                                          ke_task_id_t const src_id)
{
    NS_LOG_INFO("RACP length: %x\r\n", param->write_val_len);
    NS_LOG_INFO("RACP opcode: %x\r\n", param->write_val.opcode);
    NS_LOG_INFO("RACP operator: %x\r\n", param->write_val.op_operator);
    
    if(param->write_val_len >= 3)
    {
        NS_LOG_INFO("RACP operand: %x\r\n", param->write_val.operand);
        
        NS_LOG_INFO("RACP data: ");
        for(uint8_t i=0; i<(param->write_val_len-3); i++)
        {
            NS_LOG_INFO("%x ", param->write_val.data[i]);
        }
        NS_LOG_INFO("\r\n");
    }
    
    return KE_MSG_CONSUMED;
}

/**
 * @brief  BLPS RACP write handler
 * @param  
 * @return 
 * @note   
 */
static int blps_cmp_evt_handler(ke_msg_id_t const msgid,
                                          struct blps_cmp_evt const *param,
                                          ke_task_id_t const dest_id,
                                          ke_task_id_t const src_id)
{
    
    NS_LOG_INFO("BLPS CMP operation: %x, code: %x, status: %x \r\n ", \
                        param->operation, param->operation_code, param->status);
    
    return KE_MSG_CONSUMED;
}

/// Default State handlers definition
const struct ke_msg_handler app_blps_msg_handler_list[] =
{
    {BLPS_ENABLE_RSP,         (ke_msg_func_t)blps_enable_rsp_handler},
    {BLPS_CFG_INDNTF_IND,     (ke_msg_func_t)blps_cfg_indntf_ind_handler},
    {BLPS_RACP_WRITE_IND,     (ke_msg_func_t)blps_racp_write_ind_handler},
    {BLPS_CMP_EVT,            (ke_msg_func_t)blps_cmp_evt_handler},
};

const struct app_subtask_handlers app_blps_handlers = APP_HANDLERS(app_blps);


#endif //BLE_APP_DIS

/// @} APP
