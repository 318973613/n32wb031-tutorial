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
 * @file rdtss.c
 * @author NSING Firmware Team
 * @version v1.0.4
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */


 /* Includes ------------------------------------------------------------------*/
#include "rwip_config.h"              // SW configuration

#if (BLE_RDTSS_SERVER)
#include "rdtss.h"
#include "rdtss_task.h"
#include "attm_db.h"
#include "gapc.h"
#include "prf.h"
#include "ke_mem.h"


/* Private functions ---------------------------------------------------------*/

/** 
 * @brief Initialization of the RDTSS module.
 * This function performs all the initializations of the Profile module.
 *  - Creation of database (if it's a service)
 *  - Allocation of profile required memory
 *  - Initialization of task descriptor to register application
 *      - Task State array
 *      - Number of tasks
 *      - Default task handler
 *
 * @param[out]    env        Collector or Service allocated environment data.
 * @param[in|out] start_hdl  Service start handle (0 - dynamically allocated), only applies for services.
 * @param[in]     app_task   Application task number.
 * @param[in]     sec_lvl    Security level (AUTH, EKS and MI field of @see enum attm_value_perm_mask)
 * @param[in]     param      Configuration parameters of profile collector or service (32 bits aligned)
 *
 * @return status code to know if profile initialization succeed or not. 
 */
static uint8_t rdtss_init(struct prf_task_env *env, uint16_t *start_hdl, uint16_t app_task, 
                            uint8_t sec_lvl, struct rdtss_db_cfg *params)
{
    //------------------ create the attribute database for the profile -------------------

    // DB Creation Status
    uint8_t status = ATT_ERR_NO_ERROR;

    uint32_t cfg_flag = ((1<<params->max_nb_att)-1);
    
    status = attm_svc_create_db_128(start_hdl, params->svc_uuid , (uint8_t*)&cfg_flag,
            params->max_nb_att, NULL, env->task, params->att_tbl,
            (sec_lvl & PERM_MASK_SVC_AUTH) | (sec_lvl & PERM_MASK_SVC_EKS) | 
             PERM(SVC_SECONDARY, DISABLE) | PERM_VAL(SVC_UUID_LEN,2));    

    //-------------------- allocate memory required for the profile  ---------------------
    if (status == ATT_ERR_NO_ERROR)
    {
        struct rdtss_env_tag *rdtss_env =
                (struct rdtss_env_tag *) ke_malloc(sizeof(struct rdtss_env_tag), KE_MEM_ATT_DB);
        
        //load config from init 
        memcpy(&rdtss_env->db_cfg,params,sizeof(struct rdtss_db_cfg));

        // allocate RDTSS required environment variable
        env->env = (prf_env_t *)rdtss_env;
        rdtss_env->shdl = *start_hdl;
        rdtss_env->max_nb_att = rdtss_env->db_cfg.max_nb_att;
        rdtss_env->prf_env.app_task = app_task
                | (PERM_GET(sec_lvl, SVC_MI) ? PERM(PRF_MI, ENABLE) : PERM(PRF_MI, DISABLE));
        rdtss_env->prf_env.prf_task = env->task | PERM(PRF_MI, DISABLE);

        // initialize environment variable
        env->id                     = TASK_ID_RDTSS;

        rdtss_task_init(&(env->desc));
        
        co_list_init(&(rdtss_env->values));
        rdtss_init_ccc_values(rdtss_env->db_cfg.att_tbl, rdtss_env->db_cfg.max_nb_att);

        // service is ready, go into an Idle state
        ke_state_set(env->task, RDTSS_IDLE);
    }

    return status;
}
/** 
 * @brief Destruction of the RDTSS module - due to a reset for instance.
 * This function clean-up allocated memory (attribute database is destroyed by another
 * procedure)
 *
 * @param[in|out]    env        Collector or Service allocated environment data. 
 */
static void rdtss_destroy(struct prf_task_env *env)
{
    struct rdtss_env_tag *rdtss_env = (struct rdtss_env_tag *)env->env;

    // remove all values present in list
    while (!co_list_is_empty(&(rdtss_env->values)))
    {
        struct co_list_hdr *hdr = co_list_pop_front(&(rdtss_env->values));
        ke_free(hdr);
    }

    // free profile environment variables
    env->env = NULL;
    ke_free(rdtss_env);
}

/** 
 * @brief Handles Connection creation
 *
 * @param[in|out]    env        Collector or Service allocated environment data.
 * @param[in]        conidx     Connection index 
 */
static void rdtss_create(struct prf_task_env *env, uint8_t conidx)
{
    int att_idx;
    struct rdtss_env_tag *rdtss_env = (struct rdtss_env_tag *)env->env;
    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);

    // Find all ccc fields and clean them
    for (att_idx = 1; att_idx < rdtss_env->db_cfg.max_nb_att; att_idx++)
    {
        // Find only CCC characteristics
        if( PERM_GET(rdtss_env->db_cfg.att_tbl[att_idx].perm, UUID_LEN) == 0 &&
            (rdtss_env->db_cfg.att_tbl[att_idx].uuid[0] + 
            (rdtss_env->db_cfg.att_tbl[att_idx].uuid[1]<<8) ) == ATT_DESC_CLIENT_CHAR_CFG)
        {
            // Clear CCC value
            rdtss_set_ccc_value(conidx, att_idx, 0);
        }
    }
}

/** 
 * @brief Handles Disconnection
 *
 * @param[in|out]    env        Collector or Service allocated environment data.
 * @param[in]        conidx     Connection index
 * @param[in]        reason     Detach reason 
 */
static void rdtss_cleanup(struct prf_task_env *env, uint8_t conidx, uint8_t reason)
{
    int att_idx;
    
    struct rdtss_env_tag *rdtss_env = (struct rdtss_env_tag *)env->env;
    
    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);

    // Find all ccc fields and clean them
    for (att_idx = 1; att_idx < rdtss_env->db_cfg.max_nb_att; att_idx++)
    {
        // Find only CCC characteristics
        if( PERM_GET(rdtss_env->db_cfg.att_tbl[att_idx].perm, UUID_LEN) == 0 &&
            (rdtss_env->db_cfg.att_tbl[att_idx].uuid[0] + 
            (rdtss_env->db_cfg.att_tbl[att_idx].uuid[1]<<8) ) == ATT_DESC_CLIENT_CHAR_CFG)
        {
            // Clear CCC value
            rdtss_set_ccc_value(conidx, att_idx, 0);
        }
    }
}

/* Public variables ---------------------------------------------------------*/

/// RDTSS Task interface required by profile manager
const struct prf_task_cbs rdtss_itf =
{
        (prf_init_fnct) rdtss_init,
        rdtss_destroy,
        rdtss_create,
        rdtss_cleanup,
};

/* Private functions ---------------------------------------------------------*/

const struct prf_task_cbs* rdtss_prf_itf_get(void)
{
    return &rdtss_itf;
}

#endif // (BLE_RDTSS_SERVER)
