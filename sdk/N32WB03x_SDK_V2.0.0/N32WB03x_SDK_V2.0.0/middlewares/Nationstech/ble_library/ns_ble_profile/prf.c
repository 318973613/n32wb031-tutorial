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
 * @file prf.c
 * @author NSING Firmware Team
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */



/**
 ****************************************************************************************
 * @addtogroup PRF
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <string.h>
#include "rwip_config.h"

#if (BLE_PROFILES)
#include "rwip.h"
#include "prf.h"
#include "att.h"


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * MACROS
 ****************************************************************************************
 */


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
struct prf_env_tag prf_env;
struct prf_get_itf_tag get_itf;
/*
 * LOCAL FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Retrieve profile interface
 ****************************************************************************************
 */
static const struct prf_task_cbs * prf_itf_get(uint16_t task_id)
{
    const struct prf_task_cbs* prf_cbs = NULL;
    
    uint16_t task_type = KE_TYPE_GET(task_id);
    for(uint8_t id = 0; id < get_itf.prf_num;id++)
    {
        //find the prf task id
        if(task_type == get_itf.get_func_list[id].task_id )
        {
            //found prf
            prf_cbs = get_itf.get_func_list[id].prf_itf_get_func();
        }
    }

    return prf_cbs;
}

/*
 * EXPORTED FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */
void prf_init(uint8_t init_type)
{
    uint8_t i;
    
    if(init_type == RWIP_INIT)
    {
        get_itf.prf_num = 0;
    }

    for(i = 0; i < BLE_NB_PROFILES ; i++)
    {
        switch (init_type)
        {
            case RWIP_INIT:
            {
                // FW boot profile initialization
                prf_env.prf[i].task = TASK_GAPC + i +1;
                ke_task_delete(prf_env.prf[i].task);    //TODO CHECK
                ke_task_create(prf_env.prf[i].task, &(prf_env.prf[i].desc));
            }
            break;

            case RWIP_RST:
            {
                // FW boot profile destruction
                // Get Profile API
                const struct prf_task_cbs * cbs = prf_itf_get(prf_env.prf[i].id);

                if(cbs != NULL)
                {
                    // request to destroy profile
                    cbs->destroy(&(prf_env.prf[i]));
                }
                // Request kernel to flush task messages
                ke_task_msg_flush(KE_TYPE_GET(prf_env.prf[i].task));
            }
            // No break

            case RWIP_1ST_RST:
            {
                // FW boot profile destruction
                prf_env.prf[i].env  = NULL;
                // unregister profile
                prf_env.prf[i].id   = TASK_ID_INVALID;
                prf_env.prf[i].desc.msg_handler_tab = NULL;
                prf_env.prf[i].desc.state           = NULL;
                prf_env.prf[i].desc.idx_max         = 0;
                prf_env.prf[i].desc.msg_cnt         = 0;
            }
            break;

            default:
            {
                // Do nothing
            }
            break;
        }
    }
}


uint8_t prf_add_profile(struct gapm_profile_task_add_cmd * params, ke_task_id_t* prf_task)
{
    uint8_t i;
    uint8_t status = GAP_ERR_NO_ERROR;

    // retrieve profile callback
    const struct prf_task_cbs * cbs = prf_itf_get(params->prf_task_id);
    if(cbs == NULL)
    {
        // profile API not available
        status = GAP_ERR_INVALID_PARAM;
    }

    // check if profile not already present in task list
    if(status == GAP_ERR_NO_ERROR)
    {
        for(i = 0; i < BLE_NB_PROFILES ; i++)
        {
            if(prf_env.prf[i].id == params->prf_task_id)
            {
                status = GAP_ERR_NOT_SUPPORTED;
                break;
            }
        }
    }

    if(status == GAP_ERR_NO_ERROR)
    {
        // find first available task
        for(i = 0; i < BLE_NB_PROFILES ; i++)
        {
            // available task found
            if(prf_env.prf[i].id == TASK_ID_INVALID)
            {
                // initialize profile
                status = cbs->init(&(prf_env.prf[i]), &(params->start_hdl), params->app_task, params->sec_lvl, params->param);

                // initialization succeed
                if(status == GAP_ERR_NO_ERROR)
                {
                    // register profile
                    prf_env.prf[i].id = params->prf_task_id;
                    *prf_task = prf_env.prf[i].task;
                }
                break;
            }
        }

        if(i == BLE_NB_PROFILES)
        {
            status = GAP_ERR_INSUFF_RESOURCES;
        }
    }

    return (status);
}



void prf_create(uint8_t conidx)
{
    uint8_t i;
    /* simple connection creation handler, nothing to do. */

    // execute create function of each profiles
    for(i = 0; i < BLE_NB_PROFILES ; i++)
    {
        // Get Profile API
        const struct prf_task_cbs * cbs = prf_itf_get(prf_env.prf[i].id);
        if(cbs != NULL)
        {
            // call create callback
            cbs->create(&(prf_env.prf[i]), conidx);
        }
    }
}


void prf_cleanup(uint8_t conidx, uint8_t reason)
{
    uint8_t i;
    /* simple connection creation handler, nothing to do. */

    // execute create function of each profiles
    for(i = 0; i < BLE_NB_PROFILES ; i++)
    {
        // Get Profile API
        const struct prf_task_cbs * cbs = prf_itf_get(prf_env.prf[i].id);
        if(cbs != NULL)
        {
            // call cleanup callback
            cbs->cleanup(&(prf_env.prf[i]), conidx, reason);
        }
    }
}


prf_env_t* prf_env_get(uint16_t prf_id)
{
    prf_env_t* env = NULL;
    uint8_t i;
    // find if profile present in profile tasks
    for(i = 0; i < BLE_NB_PROFILES ; i++)
    {
        // check if profile identifier is known
        if(prf_env.prf[i].id == prf_id)
        {
            env = prf_env.prf[i].env;
            break;
        }
    }

    return env;
}

ke_task_id_t prf_src_task_get(prf_env_t* env, uint8_t conidx)
{
    ke_task_id_t task = PERM_GET(env->prf_task, PRF_TASK);

    if(PERM_GET(env->prf_task, PRF_MI))
    {
        task = KE_BUILD_ID(task, conidx);
    }

    return task;
}

ke_task_id_t prf_dst_task_get(prf_env_t* env, uint8_t conidx)
{
    ke_task_id_t task = PERM_GET(env->app_task, PRF_TASK);

    if(PERM_GET(env->app_task, PRF_MI))
    {
        task = KE_BUILD_ID(task, conidx);
    }

    return task;
}


ke_task_id_t prf_get_id_from_task(ke_msg_id_t task)
{
    ke_task_id_t id = TASK_ID_INVALID;
    uint8_t idx = KE_IDX_GET(task);
    uint8_t i;
    task = KE_TYPE_GET(task);

    // find if profile present in profile tasks
    for(i = 0; i < BLE_NB_PROFILES ; i++)
    {
        // check if profile identifier is known
        if(prf_env.prf[i].task == task)
        {
            id = prf_env.prf[i].id;
            break;
        }
    }

    return KE_BUILD_ID(id, idx);
}

ke_task_id_t prf_get_task_from_id(ke_msg_id_t id)
{
    ke_task_id_t task = TASK_NONE;
    uint8_t idx = KE_IDX_GET(id);
    uint8_t i;
    id = KE_TYPE_GET(id);

    // find if profile present in profile tasks
    for(i = 0; i < BLE_NB_PROFILES ; i++)
    {
        // check if profile identifier is known
        if(prf_env.prf[i].id == id)
        {
            task = prf_env.prf[i].task;
            break;
        }
    }

    return KE_BUILD_ID(task, idx);
}


bool prf_get_itf_func_register(struct prf_get_func_t *prf)
{
    if(get_itf.prf_num < BLE_NB_PROFILES)
    {
        //add profile to list 
        memcpy(&get_itf.get_func_list[get_itf.prf_num],
                prf,sizeof(struct prf_get_func_t));
        get_itf.prf_num++;
        return true;
    }

    return false;
}


#endif // (BLE_PROFILES)

/// @} PRF
