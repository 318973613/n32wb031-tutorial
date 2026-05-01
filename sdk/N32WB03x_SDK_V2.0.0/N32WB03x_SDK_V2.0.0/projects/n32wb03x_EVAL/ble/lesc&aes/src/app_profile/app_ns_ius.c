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
 * @file app_ns_ius.c
 * @author NSING Firmware Team
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

/** 
 * @addtogroup APP
 * @{ 
 */
/* Includes ------------------------------------------------------------------*/
#include "rwip_config.h"     // SW configuration

#if (BLE_APP_NS_IUS)

#include "app_ns_ius.h"
#include <stdio.h>
#include "ns_iuss.h"
#include "ns_iuss_task.h"
#include "ns_dfu_ble.h"
#include "attm.h"
#include "ke_task.h"
#include "gapc.h"
#include "gapc_task.h"
#include "gattc_task.h"
#include "attm_db.h"
#include "prf_utils.h"
#include "ke_mem.h"
#include "co_utils.h"
#include "ke_msg.h"
#include "ns_ble.h"
#include "ns_log.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


struct attm_desc_128 ns_ius_att_db[NS_IUS_IDX_NB] =
{
    [NS_IUS_IDX_SVC] = {{0x00,0x28}, PERM(RD, ENABLE),0,0},

    [NS_IUS_IDX_RC_CHAR] = {{0x03,0x28}, PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE),0, 0},
    [NS_IUS_IDX_RC_VAL] = {CHAR_NS_IUS_RC, PERM(WRITE_COMMAND, ENABLE), PERM_VAL(UUID_LEN, 0x02), 247},
    [NS_IUS_IDX_RC_CFG] = {{0x02,0x29}, PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE), 0, 0},

    [NS_IUS_IDX_CC_CHAR] = {{0x03,0x28}, PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE), 0, 0 },
    [NS_IUS_IDX_CC_VAL] = {CHAR_NS_IUS_CC, PERM(WRITE_REQ, ENABLE) | PERM(NTF, ENABLE)  , PERM(RI, ENABLE)| PERM_VAL(UUID_LEN, 0x02), 20 },   
    [NS_IUS_IDX_CC_CFG] = {{0x02,0x29}, PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE), 0, 0},
};





static bool notification_enable = false;





static int ns_ius_val_write_ind_handler(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    const struct ns_ius_val_write_ind *p_param = (const struct ns_ius_val_write_ind *)param;

    switch (p_param->handle)
    {
        case NS_IUS_IDX_CC_CFG:{
            
            if(p_param->length == 2)
            {
                uint16_t cfg_value = p_param->value[0] + p_param->value[1];
                
                if(cfg_value == PRF_CLI_START_NTF)
                {
                    notification_enable = true;
                }
                else if(cfg_value == PRF_CLI_STOP_NTFIND)
                {
                    notification_enable = false;
                }
                NS_LOG_DEBUG("notification_enable %d\r\n",notification_enable);
            }
            
        }break;
        
        
        case NS_IUS_IDX_CC_VAL:{
            uint8_t output[20];
            uint8_t output_len = 0;
            ns_dfu_ble_handler_cc(p_param->value, p_param->length, output, &output_len);
            if(output_len)ns_ble_ius_app_cc_send(output, output_len);
        }break;        
        
        case NS_IUS_IDX_RC_VAL:{
            ns_dfu_ble_handler_rc(p_param->value, p_param->length);
        }break;
        default:
            break;
        
    }    
    
    return KE_MSG_CONSUMED;
}



void ns_ble_ius_app_cc_send(uint8_t *p_data, uint16_t length)
{    
    uint8_t state = ke_state_get(ns_ble_ius_task);
    if (state == NS_IUS_BUSY || notification_enable == false)
    {
            return;
    }    
    ke_state_set(ns_ble_ius_task, NS_IUS_BUSY);    
    
    struct ns_ius_env_tag *ns_ius_env = PRF_ENV_GET(NS_IUS, ns_ius);
  struct gattc_send_evt_cmd * req = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD, KE_BUILD_ID(TASK_GATTC, app_env.conidx), ns_ius_env->prf_env.prf_task, gattc_send_evt_cmd, length);
    req->operation = GATTC_NOTIFY;
  req->handle = ns_ius_env->shdl + NS_IUS_IDX_CC_VAL;
  req->length = length;
    memcpy(req->value, p_data, length);
    ke_msg_send(req);        
}



static int ns_ius_disconnect_handler(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    notification_enable = false;
  return KE_MSG_CONSUMED;
}


const struct ke_msg_handler ns_ius_app_msg_handler_list[] =
{
    {NS_IUS_VAL_WRITE_IND,          ns_ius_val_write_ind_handler},
    {NS_IUS_DISCONNECT,             ns_ius_disconnect_handler},    
};


const struct app_subtask_handlers ns_ius_app_handlers = APP_HANDLERS(ns_ius_app);







void app_ns_ius_add_ns_ius(void)
{
    Qflash_Init();
    
    struct ns_ius_db_cfg *db_cfg;
    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                                             TASK_GAPM,
                                                             TASK_APP,
                                                             gapm_profile_task_add_cmd,
                                                             sizeof(struct ns_ius_db_cfg));
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl = PERM(SVC_AUTH, NO_AUTH);        
    req->prf_task_id = TASK_ID_NS_IUS;
    req->app_task = TASK_APP;
    req->start_hdl = 0;
    db_cfg = (struct ns_ius_db_cfg *) req->param;
    db_cfg->att_tbl = NULL;
    db_cfg->cfg_flag = 0;
    db_cfg->features = 0;    
    ke_msg_send(req);

    app_ns_ius_init();
}

void app_ns_ius_init(void)
{
    //register application subtask to app task
    struct prf_task_t prf;
    prf.prf_task_id = TASK_ID_NS_IUS;
    prf.prf_task_handler = &ns_ius_app_handlers;
    ns_ble_prf_task_register(&prf);
    
    //register get itf function to prf.c
    struct prf_get_func_t get_func;
    get_func.task_id = TASK_ID_NS_IUS;
    get_func.prf_itf_get_func = ns_ius_prf_itf_get;
    prf_get_itf_func_register(&get_func);
}


#endif  //BLE_APP_NS_IUS


/// @} APP



