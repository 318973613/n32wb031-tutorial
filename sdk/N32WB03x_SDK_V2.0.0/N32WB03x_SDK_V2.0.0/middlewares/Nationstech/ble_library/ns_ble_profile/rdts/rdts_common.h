 /**
 *	   Copyright (c) 2025, NSING Technologies Inc.
 * 
 *	   All rights reserved.
 *
 *	   This software is the exclusive property of NSING Technologies Inc. (Hereinafter 
 * referred to as NSING). This software, and the product of NSING described herein 
 * (Hereinafter referred to as the Product) are owned by NSING under the laws and treaties
 * of the People's Republic of China and other applicable jurisdictions worldwide.
 *
 *	   NSING does not grant any license under its patents, copyrights, trademarks, or other 
 * intellectual property rights. Names and brands of third party may be mentioned or referred 
 * thereto (if any) for identification purposes only.
 *
 *	   NSING reserves the right to make changes, corrections, enhancements, modifications, and 
 * improvements to this software at any time without notice. Please contact NSING and obtain 
 * the latest version of this software before placing orders.
 
 *	   Although NSING has attempted to provide accurate and reliable information, NSING assumes 
 * no responsibility for the accuracy and reliability of this software.
 * 
 *	   It is the responsibility of the user of this software to properly design, program, and test 
 * the functionality and safety of any application made of this information and any resulting product. 
 * In no event shall NSING be liable for any direct, indirect, incidental, special,exemplary, or 
 * consequential damages arising in any way out of the use of this software or the Product.
 *
 *	   NSING Products are neither intended nor warranted for usage in systems or equipment, any
 * malfunction or failure of which may cause loss of human life, bodily injury or severe property 
 * damage. Such applications are deemed, "Insecure Usage".
 *
 *	   All Insecure Usage shall be made at user's risk. User shall indemnify NSING and hold NSING 
 * harmless from and against all claims, costs, damages, and other liabilities, arising from or related 
 * to any customer's Insecure Usage.
 
 *	   Any express or implied warranty with regard to this software or the Product, including,but not 
 * limited to, the warranties of merchantability, fitness for a particular purpose and non-infringement
 * are disclaimed to the fullest extent permitted by law.
 
 *	   Unless otherwise explicitly permitted by NSING, anyone may not duplicate, modify, transcribe
 * or otherwise distribute this software for any purposes, in whole or in part.
 *
 *	   NSING products and technologies shall not be used for or incorporated into any products or systems
 * whose manufacture, use, or sale is prohibited under any applicable domestic or foreign laws or regulations. 
 * User shall comply with any applicable export control laws and regulations promulgated and administered by 
 * the governments of any countries asserting jurisdiction over the parties or transactions.
 **/


/**
 * @file rdts_common.h
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

#ifndef __RDTS_COMMON_H__
#define __RDTS_COMMON_H__

#include "gattc_task.h"

/** 
 * @brief Validate the value of the Client Characteristic CFG.
 * @param[in] is_notification indicates whether the CFG is Notification (true) or Indication (false)
 * @param[in] param Pointer to the parameters of the message.
 * @return status. 
 */
int check_client_char_cfg(bool is_notification, const struct gattc_write_req_ind *param);

/** 
 * @brief Find the handle of the Characteristic Value having as input the Client Characteristic CFG handle
 * @param[in] cfg_handle the Client Characteristic CFG handle
 * @return the corresponding value handle 
 */
uint16_t get_value_handle(uint16_t cfg_handle);

/** 
 * @brief Find the handle of Client Characteristic CFG having as input the Characteristic value handle
 * @param[in] value_handle the Characteristic value handle
 * @return the corresponding Client Characteristic CFG handle 
 */
uint16_t get_cfg_handle(uint16_t value_handle);

#if (BLE_RDTSS_SERVER)
/** 
 * @brief Compute the handle of a given attribute based on its index
 * @details Specific to raw data transfer server in 128bit uuid
 * @param[in] att_idx attribute index
 * @return the corresponding handle 
 */
uint16_t rdtss_get_att_handle(uint8_t att_idx);

/** 
 * @brief Compute the handle of a given attribute based on its index
 * @details Specific to raw data transfer server in 128bit uuid
 * @param[in] handle attribute handle
 * @param[out] att_idx attribute index
 * @return high layer error code 
 */
uint8_t rdtss_get_att_idx(uint16_t handle, uint8_t *att_idx);
#endif // (BLE_RDTSS_SERVER)


#if (BLE_RDTSS_16BIT_SERVER)
/** 
 * @brief Compute the handle of a given attribute based on its index
 * @details Specific to raw data transfer server in 16bit uuid
 * @param[in] att_idx attribute index
 * @return the corresponding handle 
 */
uint16_t rdtss_16bit_get_att_handle(uint8_t att_idx);

/** 
 * @brief Compute the handle of a given attribute based on its index
 * @details Specific to raw data transfer server in 16bit uuid
 * @param[in] handle attribute handle
 * @param[out] att_idx attribute index
 * @return high layer error code 
 */
uint8_t rdtss_16bit_get_att_idx(uint16_t handle, uint8_t *att_idx);
#endif // (BLE_RDTSS_16BIT_SERVER)


#endif // __RDTS_COMMON_H__
