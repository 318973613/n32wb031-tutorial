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
 * @file rdts_common.c
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

 /* Includes ------------------------------------------------------------------*/
#include "rwip_config.h"              // SW configuration

#if (BLE_RDTS_ENABLE)
#include "rdts_common.h"
#if (BLE_RDTSS_SERVER)
#include "rdtss.h"
#endif // (BLE_RDTSS_SERVER)

#if (BLE_RDTSS_16BIT_SERVER)
#include "rdtss_16bit.h"
#endif // (BLE_RDTSS_16BIT_SERVER)

#include "gattc_task.h"
#include "att.h"
#include "attm_db.h"
#include "prf_types.h"
#include "prf_utils.h"
/* Private define ------------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

int check_client_char_cfg(bool is_notification, const struct gattc_write_req_ind *param)
{
    uint8_t status = GAP_ERR_NO_ERROR;
    uint16_t ntf_cfg = 0;

    if (param->length != sizeof(uint16_t))
    {
        status =  ATT_ERR_INVALID_ATTRIBUTE_VAL_LEN;
    }
    else
    {
        ntf_cfg = *((uint16_t*)param->value);

        if (is_notification)
        {
            if ( (ntf_cfg != PRF_CLI_STOP_NTFIND) && (ntf_cfg != PRF_CLI_START_NTF) )
            {
                status =  PRF_ERR_INVALID_PARAM;
            }
        }
        else
        {
            if ( (ntf_cfg != PRF_CLI_STOP_NTFIND) && (ntf_cfg != PRF_CLI_START_IND) )
            {
                status =  PRF_ERR_INVALID_PARAM;
            }
        }
    }

    return status;
}

uint16_t get_value_handle(uint16_t cfg_handle)
{
    uint8_t uuid[ATT_UUID_128_LEN];
    uint8_t uuid_len;
    uint16_t handle = cfg_handle;
    struct attm_svc *srv;

    srv = attmdb_get_service(handle);

    while ((handle >= srv->svc.start_hdl) && (handle <= srv->svc.end_hdl))
    {
        struct attm_elmt elmt;

        // Retrieve UUID
        attmdb_get_attribute(handle, &elmt);
        attmdb_get_uuid(&elmt, &uuid_len, uuid, false, false);

        // check for Characteristic declaration
        if (*(uint16_t *)&uuid[0] == ATT_DECL_CHARACTERISTIC)
            return handle + 1;

        handle--;
    }

    return 0;  //Should not reach this point. something is wrong with the database
}

uint16_t get_cfg_handle(uint16_t value_handle)
{
    uint8_t uuid[ATT_UUID_128_LEN];
    uint8_t uuid_len;
    uint16_t handle = value_handle;
    struct attm_svc *srv;

    srv = attmdb_get_service(handle);

    /* Iterate the database to find the client characteristic configuration.
    */
    while ((handle >= srv->svc.start_hdl) && (handle <= srv->svc.end_hdl))
    {
        struct attm_elmt elmt;

        // Retrieve UUID
        attmdb_get_attribute(handle, &elmt);
        attmdb_get_uuid(&elmt, &uuid_len, uuid, false, false);

        // check for Client Characteristic Configuration
        if (*(uint16_t *)&uuid[0] == ATT_DESC_CLIENT_CHAR_CFG && uuid_len == sizeof(uint16_t))
            return handle;
        else if (*(uint16_t *)&uuid[0] == ATT_DECL_CHARACTERISTIC && uuid_len == sizeof(uint16_t))
            break; // found the next Characteristic declaration without findig a CC CFG,

        handle++;
    }

    return 0;  //Should not reach this point. something is wrong with the database
}

#if (BLE_RDTSS_SERVER)
uint16_t rdtss_get_att_handle(uint8_t att_idx)
{
    struct rdtss_env_tag *rdtss_env = PRF_ENV_GET(RDTSS, rdtss);
    uint16_t handle = ATT_INVALID_HDL;

    if (att_idx < rdtss_env->max_nb_att)
    {
        handle = rdtss_env->shdl + att_idx;
    }

    return handle;
}

uint8_t rdtss_get_att_idx(uint16_t handle, uint8_t *att_idx)
{
    struct rdtss_env_tag *rdtss_env = PRF_ENV_GET(RDTSS, rdtss);
    uint8_t status = PRF_APP_ERROR;

    if ((handle >= rdtss_env->shdl) && (handle < rdtss_env->shdl + rdtss_env->max_nb_att))
    {
        *att_idx = handle - rdtss_env->shdl;
        status = ATT_ERR_NO_ERROR;
    }

    return status;
}
#endif // (BLE_RDTSS_SERVER)


#if (BLE_RDTSS_16BIT_SERVER)
uint16_t rdtss_16bit_get_att_handle(uint8_t att_idx)
{
    struct rdtss_16bit_env_tag *rdtss_16bit_env = PRF_ENV_GET(RDTSS_16BIT, rdtss_16bit);
    uint16_t handle = ATT_INVALID_HDL;

    if (att_idx < rdtss_16bit_env->max_nb_att)
    {
        handle = rdtss_16bit_env->shdl + att_idx;
    }

    return handle;
}

uint8_t rdtss_16bit_get_att_idx(uint16_t handle, uint8_t *att_idx)
{
    struct rdtss_16bit_env_tag *rdtss_16bit_env = PRF_ENV_GET(RDTSS_16BIT, rdtss_16bit);
    uint8_t status = PRF_APP_ERROR;

    if ((handle >= rdtss_16bit_env->shdl) && (handle < rdtss_16bit_env->shdl + rdtss_16bit_env->max_nb_att))
    {
        *att_idx = handle - rdtss_16bit_env->shdl;
        status = ATT_ERR_NO_ERROR;
    }

    return status;
}
#endif // (BLE_RDTSS_16BIT_SERVER)


#endif // (BLE_RDTS_ENABLE)
