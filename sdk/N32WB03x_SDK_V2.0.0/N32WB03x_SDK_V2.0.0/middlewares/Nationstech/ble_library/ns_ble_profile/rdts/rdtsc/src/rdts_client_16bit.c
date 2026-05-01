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
 * @file rdts_client_16bit.c
 * @author NSING Firmware Team
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

/**
 ****************************************************************************************
 * @addtogroup RDTS_CLIENT
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"

#if (BLE_RDTS_16BIT_CLIENT)
#include "prf.h"
#include "rdts_client_16bit.h"
#include "rdts_client_16bit_task.h"
#include "prf_utils.h"
#include "ke_mem.h"
/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @brief     Profile Initialization
 * @param[in] env enviroment 
 * @param[in] start_hdl start handle 
 * @param[in] app_task application task
 * @param[in] sec_lvl security level
 * @param[in] params configuration parameters
 * @return    void
 ****************************************************************************************
 */
uint8_t rdtsc_16bit_init(struct prf_task_env *env, uint16_t *start_hdl, uint16_t app_task, uint8_t sec_lvl, struct rdtsc_db_cfg *params)
{
    //-------------------- allocate memory required for the profile  ---------------------

    struct rdtsc_env_tag* rdtsc_env =
            (struct rdtsc_env_tag* ) ke_malloc(sizeof(struct rdtsc_env_tag), KE_MEM_ATT_DB);

    // allocate RDTSC required environment variable
    env->env = (prf_env_t*) rdtsc_env;
    rdtsc_env->prf_env.app_task = app_task | PERM(PRF_MI, DISABLE);
    rdtsc_env->prf_env.prf_task = env->task | PERM(PRF_MI, DISABLE);
    rdtsc_env->nb_svc = 0;

    // initialize environment variable
    env->id = TASK_ID_RDTSC_16BIT;
    rdtsc_16bit_task_init(&(env->desc));
    
    ke_state_set(rdtsc_env->prf_env.prf_task, RDTSC_IDLE);
    
    // save db cfg 
    memcpy(&(rdtsc_env->rdtsc_cfg),params,sizeof(struct rdtsc_db_cfg));
   
    return GAP_ERR_NO_ERROR;
}

/**
 ****************************************************************************************
 * @brief     Profile enable confirmation
 * @param[in] env_tag enviroment 
 * @param[in] conidx Connection index
 * @param[in] status
 * @return    void
 ****************************************************************************************
 */
void rdtsc_16bit_enable_cfm_send(struct rdtsc_env_tag *rdtsc_env, uint8_t conidx, uint8_t status)
{
   
    struct rdtsc_enable_cfm * cfm = KE_MSG_ALLOC(RDTSC_ENABLE_CFM, 
                                                TASK_APP, 
                                                prf_src_task_get(&(rdtsc_env->prf_env), conidx), 
                                                rdtsc_enable_cfm); 
    
    cfm->status = status;
    if (status == ATT_ERR_NO_ERROR)
    {
        cfm->rdts  = rdtsc_env->rdts;
        
        prf_register_atthdl2gatt(&(rdtsc_env->prf_env), conidx, &rdtsc_env->rdts.svc); 
        
        // Set value 0x0001 to CFG
        prf_gatt_write_ntf_ind(&rdtsc_env->prf_env, conidx, rdtsc_env->rdts.descs[RDTSC_SRV_TX_DATA_CLI_CFG].desc_hdl, PRF_CLI_START_NTF);
        
        // Reset counter
        rdtsc_env->pending_wr_no_rsp_cmp = 0;
        rdtsc_env->pending_tx_ntf_cmp = false;

        //Place in connected state after discover state   
        ke_state_set(rdtsc_env->prf_env.prf_task, RDTSC_CONNECTED);
    }
    else
    {
        
    }
    
    ke_msg_send(cfm);
}

/**
 ****************************************************************************************
 * @brief Confirm that data has been sent
 * @param[in] rdtsc_env enviroment 
 * @param[in] status
 * @return    void
 ****************************************************************************************
 */
void rdtsc_16bit_confirm_data_tx(struct rdtsc_env_tag *rdtsc_env, uint8_t status)
{
    struct rdtsc_data_tx_cfm * cfm = KE_MSG_ALLOC(RDTSC_DATA_TX_CFM,
                                                        TASK_APP,
                                                        prf_src_task_get(&(rdtsc_env->prf_env), rdtsc_env->conidx),
                                                        rdtsc_data_tx_cfm);
    cfm->status = status;

    ke_msg_send(cfm);
}

/**
 ****************************************************************************************
 * @brief Receives data and forwards it to application
 * @param[in] rdtsc_env enviroment 
 * @param[in] message received
 * @param[in] length
 * @return    void
 ****************************************************************************************
 */

void rdtsc_16bit_data_receive(struct rdtsc_env_tag *rdtsc_env, void *msg )
{
    // Forward the message
    struct ke_msg * kmsg = ke_param2msg(msg);
    kmsg->dest_id = TASK_APP;
    kmsg->src_id  = prf_src_task_get(&(rdtsc_env->prf_env), rdtsc_env->conidx);
    kmsg->id      = RDTSC_DATA_RX_IND;
      
    struct rdtsc_data_rx_ind *req =  ( struct rdtsc_data_rx_ind *) msg; 
	
    ke_msg_send(req);
}

/**
 ****************************************************************************************
 * @brief  Find next empty characteristic description
 * @param[in] rdtsc_env   enviroment 
 * @param[in] desc_def  service characteristic description information table
 * @return    position of next characteristic description
 ****************************************************************************************
 */
uint8_t rdtsc_16bit_get_next_desc_char_code(struct rdtsc_env_tag *rdtsc_env,
                                     const struct prf_char_desc_def *desc_def)
{
    uint8_t i;
    uint8_t next_char_code;

    for (i=0; i<RDTSC_DESC_MAX; i++)
    {
        next_char_code = desc_def[i].char_code;

        if (next_char_code > rdtsc_env->last_char_code)
        {
            //If Char. has been found and number of attributes is upper than 2
            if ((rdtsc_env->rdts.chars[next_char_code].char_hdl != 0)
                    & (rdtsc_env->rdts.chars[next_char_code].char_ehdl_off > 2))
            {
                return next_char_code;
            }
        }
    }

    return RDTSC_CHAR_MAX;
}

/**
 ****************************************************************************************
 * @brief Handles Connection creation
 *
 * @param[in|out]    env        environment data.
 * @param[in]        conidx     Connection index
 ****************************************************************************************
 */
static void rdtsc_16bit_create(struct prf_task_env *env, uint8_t conidx)
{
     struct rdtsc_env_tag *rdts_env = PRF_ENV_GET(RDTSC, rdtsc);
    
     ke_state_set(env->task, RDTSC_CONNECTED);
}
/**
 ****************************************************************************************
 * @brief Handles Disconnection
 *
 * @param[in|out]    env        Collector or Service allocated environment data.
 * @param[in]        conidx     Connection index
 * @param[in]        reason     Detach reason
 ****************************************************************************************
 */
static void rdtsc_16bit_cleanup(struct prf_task_env *env, uint8_t conidx, uint8_t reason)
{
    struct rdtsc_env_tag *rdts_env = PRF_ENV_GET(RDTSC, rdtsc);
    rdts_env->nb_svc = 0;
}
/**
 ****************************************************************************************
 * @brief Destruction of the rdts module - due to a reset for instance.
 * This function clean-up allocated memory (attribute database is destroyed by another
 * procedure)
 *
 * @param[in|out]    env        Collector or Service allocated environment data.
 ****************************************************************************************
 */
static void rdtsc_16bit_destroy(struct prf_task_env *env)
{
    struct rdtsc_env_tag *rdtsc_env = PRF_ENV_GET(RDTSC, rdtsc);
    
    // free profile environment variables
    env->env = NULL;
    ke_free(rdtsc_env);
}
/**
 ****************************************************************************************
 * @brief Receives data and forwards it to application
 * @param[in] msg voi pointer to RDTSC_DATA_TX_REQ message
 * @param[in] prf_env rdtsc profile enviroment 
 * @param[in] conidx connection index
 * @param[in] handle RDTSC_SRV_RX_DATA_CHAR handle
 * @param[in] value data pointer to be written
 * @param[in] operation GATTC_WRITE_NO_RESPONSE
 * @return    void
 ****************************************************************************************
 */
void rdtsc_16bit_write_data_tx(void *msg, prf_env_t *prf_env, uint8_t conidx, uint16_t handle, uint8_t* value, uint16_t length, uint8_t operation)
{
    if(handle != ATT_INVALID_HANDLE)
    {
        struct ke_msg * kmsg = ke_param2msg(msg);
        
        kmsg->dest_id = KE_BUILD_ID(TASK_GATTC, conidx);
        kmsg->src_id  =  prf_src_task_get(prf_env, conidx);
        kmsg->id = GATTC_WRITE_CMD;
         
        struct gattc_write_cmd *wr_char =  ( struct gattc_write_cmd *) msg; 
            
        // Offset
        wr_char->offset         = 0x0000;
        // cursor always 0
        wr_char->cursor         = 0x0000;
        // Write Type
        wr_char->operation       = operation;
        // Characteristic Value attribute handle
        wr_char->handle         = handle;
        // Value Length
        wr_char->length         = length;
        // Auto Execute
        wr_char->auto_execute   = true;
        // Value
        
        // Send the message
        ke_msg_send(wr_char);
    }
}

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// RDTS Task interface required by profile manager
const struct prf_task_cbs spc_itf =
{
        (prf_init_fnct) rdtsc_16bit_init,
        rdtsc_16bit_destroy,
        rdtsc_16bit_create,
        rdtsc_16bit_cleanup,
};

 /*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Retrieve RDTS profile interface
 *
 * @return RDTS service profile interface
 ****************************************************************************************
 */
const struct prf_task_cbs* rdtsc_16bit_prf_itf_get(void)
{
    return &spc_itf;
}

#endif //BLE_RDTS_16BIT_CLIENT

/// @} RDTS_CLIENT
