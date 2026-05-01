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
 * @file app_hogprh.c
 * @author NSING Firmware Team
 * @version v1.0.4
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

/** 
 * @addtogroup APP
 * @{ 
 */

#include "rwip_config.h"     // SW configuration

#if (CFG_PRF_HOGPRH)

/* Includes ------------------------------------------------------------------*/
#include "app_hogprh.h"
#include "hogprh_task.h"
#include "hogprh.h"
#include "ns_ble.h"
#include "ns_ble_task.h"
#include "ns_timer.h"
#include "app_gpio.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/// Parameters of the @ref HOGPRH_CACHE 
struct hogprh_cache
{
    /// Number of HIDS instances
    uint8_t hids_nb;
    /// Existing handle values hids
    struct hogprh_content hids[HOGPRH_NB_HIDS_INST_MAX];
};
/* Private constants ---------------------------------------------------------*/
static uint8_t cache_save_latency = 200; 
static bool cache_done = false;
static struct hogprh_cache hids_cache;
extern bool ns_sec_get_bond_status(void);
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void app_hogprh_save_cache(void)
{
	    NS_LOG_DEBUG("%s\r\n", __func__);
			rwip_time_t current_time = rwip_time_get();
			rwip_time_t target_time = {0};
			target_time.hs  = ip_clkntgt1_getf();
			target_time.hus = ip_hmicrosectgt1_getf();
			int32_t duration = CLK_DIFF(current_time.hs, target_time.hs);	
			if((duration > 64) || (cache_save_latency == 0))// 20ms, xMS / 0.3125
			{
				Qflash_Erase_Sector(CACHE_DATA_BASE_ADDR);		    
				Qflash_Write(CACHE_DATA_BASE_ADDR, (uint8_t*)(&hids_cache), sizeof(struct hogprh_content)*hids_cache.hids_nb + 1 );				
			}
			else
			{
				cache_save_latency --;
				ns_timer_create(20,app_hogprh_save_cache);
			}	
	}

void app_hogprh_init(void)
{
    //register application subtask to app task
    struct prf_task_t prf;
    prf.prf_task_id = TASK_ID_HOGPRH;
    prf.prf_task_handler = &app_hogprh_handlers;
    ns_ble_prf_task_register(&prf);
    
    //register get itf function to prf.c
    struct prf_get_func_t get_func;
    get_func.task_id = TASK_ID_HOGPRH;
    get_func.prf_itf_get_func = hogprh_prf_itf_get;
    prf_get_itf_func_register(&get_func);
}

/** 
 * @brief Create raw data transfer server profile database.
 * @return void
 */
void app_hogprh_add_hid(void)
{
    NS_LOG_DEBUG("%s\r\n", __func__);

    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                                             TASK_GAPM,
                                                             TASK_APP,
                                                             gapm_profile_task_add_cmd,
                                                             0);

    // Fill message
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl = PERM(SVC_AUTH, UNAUTH);// NO_AUTH UNAUTH AUTH
    req->prf_task_id = TASK_ID_HOGPRH;
    req->app_task = TASK_APP;
    req->start_hdl = 0;


    // Send the message
    ke_msg_send(req);
    
    app_hogprh_init();
}


void app_hogprh_enable_prf(uint8_t conidx)
{
    NS_LOG_DEBUG("%s\r\n", __func__);
    struct hogprh_enable_req * req = KE_MSG_ALLOC(HOGPRH_ENABLE_REQ,
                                                  prf_get_task_from_id(TASK_ID_HOGPRH),
                                                  TASK_APP,
                                                  hogprh_enable_req);
                                                        
    // Fill in the parameter structure
		cache_done = false;
		if (ns_sec_get_bond_status())
		{
		cache_done = true;
		req->con_type = PRF_CON_NORMAL;
		struct hogprh_cache* p_cache = (void*)CACHE_DATA_BASE_ADDR;
		memcpy(&hids_cache, p_cache, sizeof(struct hogprh_content)*p_cache->hids_nb + 1);	
		req->hids_nb = p_cache->hids_nb;
		memcpy((req->hids), (p_cache->hids), sizeof(struct hogprh_content)*p_cache->hids_nb);
		NS_LOG_DEBUG("hids_cache hids_nb: %d,shdl: %d,ehdl:%d\r\n",hids_cache.hids_nb,hids_cache.hids[0].svc.shdl,hids_cache.hids[0].svc.ehdl);			
		}
		else
    req->con_type = PRF_CON_DISCOVERY; 
    
    // Send the message
    ke_msg_send(req);
}

void hogprh_read_ble_data(uint8_t info, uint8_t hid_idx, uint8_t report_idx)
{
    NS_LOG_DEBUG("%s\r\n", __func__);
    struct hogprh_read_info_req * req = KE_MSG_ALLOC_DYN(HOGPRH_READ_INFO_REQ,
                                                      prf_get_task_from_id(TASK_ID_HOGPRH),
                                                      TASK_APP,
                                                      hogprh_read_info_req,
                                                      0);

    req->info = info;
		req->hid_idx = hid_idx;
		req->report_idx = report_idx;

    ke_msg_send(req);		
}

void hogprh_write_ble_data(uint8_t info, uint8_t hid_idx, uint8_t report_idx,uint8_t *data)
{
    NS_LOG_DEBUG("%s\r\n", __func__);
    struct hogprh_write_req * req = KE_MSG_ALLOC_DYN(HOGPRH_WRITE_REQ,
                                                      prf_get_task_from_id(TASK_ID_HOGPRH),
                                                      TASK_APP,
                                                      hogprh_write_req,
                                                      0);
    
    req->info = info;
		req->hid_idx = hid_idx;
		req->report_idx = report_idx;		
		req->wr_cmd = 0;
    switch (info)
    {
        case HOGPRH_PROTO_MODE:
						memcpy(&req->data.proto_mode, data, 1);
            break;
        case HOGPRH_HID_CTNL_PT:
            memcpy(&req->data.hid_ctnl_pt, data, 1);
            break;
        case HOGPRH_REPORT:
						memcpy(&req->data.report, data, sizeof(struct hogprh_report));
            break;
        case HOGPRH_REPORT_NTF_CFG:
            memcpy(&req->data.report_cfg, data, 1);
            break;
        default:
            break;
    }	
    ke_msg_send(req);
}


/**
 ****************************************************************************************
 * @brief Handles client enable confirmation
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int hogprh_enable_cfm_handler(ke_msg_id_t const msgid,
                             struct hogprh_enable_rsp const *param,
                             ke_task_id_t const dest_id,
                             ke_task_id_t const src_id)
{
//    NS_LOG_DEBUG("%s\r\n", __func__);
		NS_LOG_INFO("HOGPRH profile was enabled,status: %d,hids_nb:%d\r\n",param->status,param->hids_nb);
		NS_LOG_DEBUG("SVC shdl: %d,hids_nb:%d,report_nb: %d\r\n",param->hids[0].svc.shdl,param->hids[0].svc.ehdl,param->hids[0].report_nb);
			
		if (!param->status && !cache_done)
		{
			hids_cache.hids_nb = param->hids_nb;
			memcpy(&hids_cache.hids[0], &(param->hids[0]), sizeof(struct hogprh_content)*param->hids_nb);
			NS_LOG_DEBUG("hids_cache[0] shdl: %d,ehdl:%d,report_nb: %d\r\n",hids_cache.hids[0].svc.shdl,hids_cache.hids[0].svc.ehdl,hids_cache.hids[0].report_nb);	
		//need to save it by timer			
	    ns_timer_create(1000,app_hogprh_save_cache);
		}
		hogprh_read_ble_data(HOGPRH_REPORT_MAP,0,0);
//    ns_ble_get_peer_info(GAPC_GET_PEER_FEATURES);
    return (KE_MSG_CONSUMED);
}

/*
 ****************************************************************************************
 * @brief Handles confirmation that data has been sent
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int hogprh_write_cfm_handler(ke_msg_id_t const msgid,
                                      struct hogprh_write_rsp const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
	NS_LOG_DEBUG("%s,%d,hid_idx: %d,report_idx: %d,info: %d\r\n", __func__,param->status,param->hid_idx,param->report_idx,param->info);
if (param->info == HOGPRH_REPORT_NTF_CFG && ((param->report_idx + 1) < (hids_cache.hids[param->hid_idx].report_nb)))
{
		uint8_t ntg_enable[2] = {0x01,0x00};
		hogprh_write_ble_data(HOGPRH_REPORT_NTF_CFG,param->hid_idx,param->report_idx+1,ntg_enable);
}
    
    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles report data from notification
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

int hogprh_report_ind_handler(ke_msg_id_t const msgid,
                              struct hogprh_report_ind const *param,
                              ke_task_id_t const dest_id,
                              ke_task_id_t const src_id)
{
    NS_LOG_DEBUG("%s\r\n", __func__);
    NS_LOG_INFO("HOGP Data was received\r\n");
    NS_LOG_DEBUG("length = %x\r\n", param->report.length);
    NS_LOG_DEBUG("report_idx = %x\r\n", param->report_idx);
    NS_LOG_DEBUG("hid_idx = %x\r\n", param->hid_idx);
    NS_LOG_DEBUG("data ");
    for(uint8_t i=0; i<param->report.length; i++)
    {
        NS_LOG_DEBUG(" %x", param->report.value[i]);
    }
    NS_LOG_DEBUG("\r\n");

		if (param->report.value[1] == 0x0A)
		{
			LedOff(LED2_PORT,LED2_PIN);
		}
		else if (param->report.value[0] == 0x08)
		{
			LedOn(LED2_PORT,LED2_PIN);
		}
    
    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles the data recieved from read command
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int hogprh_read_info_rsp_handler(ke_msg_id_t const msgid,
                                      struct hogprh_read_info_rsp const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
	NS_LOG_DEBUG("%s,%d,hid_idx: %d,report_idx: %d\r\n", __func__,param->status,param->hid_idx,param->report_idx);
		
    switch (param->info)
    {
        case HOGPRH_REPORT_MAP:
						NS_LOG_DEBUG("Report Map: ");
						for(uint8_t i=0; i<param->data.report_map.length; i++)
						{
								NS_LOG_DEBUG(" %x", param->data.report_map.value[i]);
						}
						NS_LOG_DEBUG("\r\n");					
            hogprh_read_ble_data(HOGPRH_HID_INFO,param->hid_idx,0);
            break;
        case HOGPRH_HID_INFO:
            hogprh_read_ble_data(HOGPRH_REPORT_REF,param->hid_idx,0);
            break;
        case HOGPRH_REPORT_REF:
						NS_LOG_DEBUG("Report REF: \r\n");
						NS_LOG_DEBUG("id %x\r\n", param->data.report_ref.id);
						NS_LOG_DEBUG("type %x\r\n", param->data.report_ref.type);
						if ((param->report_idx + 1) < (hids_cache.hids[param->hid_idx].report_nb))
						{
							hogprh_read_ble_data(HOGPRH_REPORT_REF,param->hid_idx,(param->report_idx + 1));
						}
						else
						{
						uint8_t ntg_enable[2] = {0x01,0x00};
            hogprh_write_ble_data(HOGPRH_REPORT_NTF_CFG,param->hid_idx,0,ntg_enable);
						}
            break;
        default:
            break;
    }	
    return (KE_MSG_CONSUMED);
}

/// Default State handlers definition
const struct ke_msg_handler app_hogprh_msg_handler_list[] =
{
    {HOGPRH_ENABLE_RSP,                 (ke_msg_func_t)hogprh_enable_cfm_handler},
    {HOGPRH_WRITE_RSP,                  (ke_msg_func_t)hogprh_write_cfm_handler},
    {HOGPRH_REPORT_IND,                 (ke_msg_func_t)hogprh_report_ind_handler},
    {HOGPRH_READ_INFO_RSP,              (ke_msg_func_t)hogprh_read_info_rsp_handler},		
};

const struct app_subtask_handlers app_hogprh_handlers = APP_HANDLERS(app_hogprh);


#endif //CFG_PRF_HOGPRH
