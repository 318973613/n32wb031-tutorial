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
 * @file app_batt.c
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

#if (BLE_APP_BATT)

#include "app_batt.h"                // Battery Application Module Definitions
#include "ns_ble.h"                     // Application Definitions
#include "ns_ble_task.h"                // application task definitions
#include "bass_task.h"               // health thermometer functions
#include "co_bt.h"
#include "bass.h"
#include "prf_types.h"               // Profile common types definition
#include "arch.h"                    // Platform Definitions
#include "prf.h"
#include <string.h>

/* Private define ------------------------------------------------------------*/
/// Battery Application Module Environment Structure
struct app_batt_env_tag app_batt_env;

/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  bat server init
 * @param  
 * @return 
 * @note   
 */
void app_batt_init(void)
{
    // Reset the environment
    memset(&app_batt_env, 0, sizeof(struct app_batt_env_tag));

    // Initial battery level: 100
    app_batt_env.batt_lvl = 100;
    
    //register application subtask to app task
    struct prf_task_t prf;
    prf.prf_task_id = TASK_ID_BASS;
    prf.prf_task_handler = &app_batt_handlers;
    ns_ble_prf_task_register(&prf);
    
    //register get itf function to prf.c
    struct prf_get_func_t get_func;
    get_func.task_id = TASK_ID_BASS;
    get_func.prf_itf_get_func = bass_prf_itf_get;
    prf_get_itf_func_register(&get_func);

}

/**
 * @brief  add bat server
 * @param  
 * @return 
 * @note   
 */
void app_batt_add_bas(void)
{
    NS_LOG_DEBUG("%s\r\n",__func__);
    struct bass_db_cfg* db_cfg;
    // Allocate the BASS_CREATE_DB_REQ
    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                                  TASK_GAPM, TASK_APP,
                                                  gapm_profile_task_add_cmd, sizeof(struct bass_db_cfg));
    // Fill message
    req->operation   = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl     = PERM(SVC_AUTH, NO_AUTH);
    req->prf_task_id = TASK_ID_BASS;
    req->app_task    = TASK_APP;
    req->start_hdl   = 0;

    // Set parameters
    db_cfg = (struct bass_db_cfg* ) req->param;
 
    // Add a BAS instance
    db_cfg->bas_nb      = 1;
    // Sending of notifications is supported
    db_cfg->features[0] = BAS_BATT_LVL_NTF_SUP;

    // Send the message
    ke_msg_send(req);
    
    app_batt_init();
}

/**
 * @brief  enable batt server profile
 * @param  
 * @return 
 * @note   
 */
void app_batt_enable_prf(uint8_t conidx)
{
    app_batt_env.conidx = conidx;

    // Allocate the message
    struct bass_enable_req * req = KE_MSG_ALLOC(BASS_ENABLE_REQ,
                                                prf_get_task_from_id(TASK_ID_BASS),
                                                TASK_APP,
                                                bass_enable_req);

    // Fill in the parameter structure
    req->conidx             = conidx;

    // NTF initial status - Disabled
    req->ntf_cfg           = PRF_CLI_STOP_NTFIND;
    req->old_batt_lvl[0]   = 50;

    // Send the message
    ke_msg_send(req);
}

/**
 * @brief  send battery level 
 * @param  
 * @return 
 * @note   
 */
void app_batt_send_lvl(uint8_t batt_lvl)
{
    ASSERT_ERR(batt_lvl <= BAS_BATTERY_LVL_MAX);

    // Allocate the message
    struct bass_batt_level_upd_req * req = KE_MSG_ALLOC(BASS_BATT_LEVEL_UPD_REQ,
                                                        prf_get_task_from_id(TASK_ID_BASS),
                                                        TASK_APP,
                                                        bass_batt_level_upd_req);

    // Fill in the parameter structure
    req->bas_instance = 0;
    req->batt_level   = batt_lvl;

    // Send the message
    ke_msg_send(req);
}

static int bass_batt_level_ntf_cfg_ind_handler(ke_msg_id_t const msgid,
                                               struct bass_batt_level_ntf_cfg_ind const *param,
                                               ke_task_id_t const dest_id,
                                               ke_task_id_t const src_id)
{
    return (KE_MSG_CONSUMED);
}

static int batt_level_upd_handler(ke_msg_id_t const msgid,
                                      struct bass_batt_level_upd_rsp const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    return (KE_MSG_CONSUMED);
}

/** 
 * @brief
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not. 
 */
static int app_batt_msg_dflt_handler(ke_msg_id_t const msgid,
                                     void const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    // Drop the message

    return (KE_MSG_CONSUMED);
}

/*
 * LOCAL VARIABLE DEFINITIONS 
 */

/// Default State handlers definition
const struct ke_msg_handler app_batt_msg_handler_list[] =
{
    // Note: first message is latest message checked by kernel so default is put on top.
    {KE_MSG_DEFAULT_HANDLER,        (ke_msg_func_t)app_batt_msg_dflt_handler},

    {BASS_BATT_LEVEL_NTF_CFG_IND,   (ke_msg_func_t)bass_batt_level_ntf_cfg_ind_handler},
    {BASS_BATT_LEVEL_UPD_RSP,       (ke_msg_func_t)batt_level_upd_handler},
};

const struct app_subtask_handlers app_batt_handlers = APP_HANDLERS(app_batt);

#endif //BLE_APP_BATT

/// @} APP
