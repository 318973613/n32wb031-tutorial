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
 * @file prf_utils_128.h
 * @author NSING Firmware Team
 * @version v1.0.2
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

#ifndef _PRF_UTILS_128_H_
#define _PRF_UTILS_128_H_

/**
 ****************************************************************************************
 * @addtogroup PRF_UTILS
 * @ingroup PROFILE
 *
 * @brief Definitions of shared profiles functions that can be used by several profiles
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwprf_config.h"

#if (BLE_CLIENT_PRF)
#include "ke_msg.h"
#include "prf_types.h"
#include "gattc_task.h"
#include "gapc.h"
#include "gapc_task.h"
#include "attm_db.h"
#include "prf.h"
#endif /* (BLE_CLIENT_PRF) */

/*
 * MACROS
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

#if (BLE_CLIENT_PRF)

/// Characteristic definition
struct prf_char_def_128
{
    /// Characteristic UUID
    uint8_t uuid[ATT_UUID_128_LEN];
    /// Requirement Attribute Flag
    uint8_t req_flag;
    /// Mandatory Properties
    uint8_t prop_mand;
};

/**
 ****************************************************************************************
 * @brief Request service discovery with 128-bit UUID on peer device.
 *
 * This request will be used to retrieve start and end handles of the service.
 *
 * @param con_info Pointer to connection information (connection handle, app task id,
 *                 profile task id)
 *
 * @param uuid_128 128-bit service UUID
 ****************************************************************************************
 */
void prf_disc_svc_send_128(prf_env_t* env, uint8_t *uuid_128, uint8_t conidx);
 
/**
 ****************************************************************************************
 * @brief Check validity for service characteristic with 128-bit UUID
 *
 * For each characteristic in service it verifies handles.
 *
 * If some handles are not present, it checks if they shall be present or they are optional.
 *
 * @param nb_chars   Number of Characteristics in the service
 * @param chars      Characteristics values (char handles, val handles, properties)
 * @param chars_req  Characteristics requirements.
 *
 * @return 0x1 if service is valid, 0x00 else.
 ****************************************************************************************
 */
uint8_t prf_check_svc_char_validity_128(uint8_t nb_chars,
                                    const struct prf_char_inf* chars,
                                    const struct prf_char_def_128* chars_req);

/**
 ****************************************************************************************
 * @brief Look for requested characteristics in the list provided by the
 * gattc_disc_char_ind message.
 *
 * @param svc_ehdl         End handle of the service for which characteristics are discovered
 * @param nb_chars         Length of provided arrays (chars and chars_req)
 * @param chars            Characteristics
 * @param chars_req        Characteristics requirements
 * @param param            List of found characteristics
 * @param last_found_char  Characteristics code of the last characteristic that has been found.
 ****************************************************************************************
 */
void prf_search_chars_128(uint16_t svc_ehdl, uint8_t nb_chars,
                      struct prf_char_inf* chars, const struct prf_char_def_128* chars_req,
                      const struct gattc_disc_char_ind* param,
                      uint8_t* last_found_char);

void prf_disc_svc_sdp_send_128(prf_env_t *prf_env, uint8_t conidx,const uint8_t* uuid_128);
void prf_disc_char_by_uuid_128_send(prf_env_t *prf_env, uint8_t conidx, const uint8_t *uuid_128);
void prf_extract_svc_sdp_info_128(const struct gattc_sdp_svc_ind *param,
                                  uint8_t nb_chars,
                                  const struct prf_char_def_128* chars_req,
                                  struct prf_char_inf* chars,
                                  uint8_t nb_descs,
                                  const struct prf_char_desc_def* descs_req,
                                  struct prf_char_desc_inf* descs);

#endif //(BLE_CLIENT_PRF)


/// @} prf_utils

#endif /* _PRF_UTILS_128_H_ */
