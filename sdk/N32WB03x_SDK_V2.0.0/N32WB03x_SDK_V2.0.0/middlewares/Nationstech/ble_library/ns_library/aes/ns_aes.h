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
 * @file ns_aes.h
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

/** @addtogroup 
 * @{
 */
#ifndef __NS_AES_H__
#define __NS_AES_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -----------------------------------------------------------------*/
#include "global_func.h"
#include "aes.h"
#include "aes_int.h"
/* Public typedef -----------------------------------------------------------*/

/* Public define ------------------------------------------------------------*/  
#ifndef AES_DECRYPT_ENABLE
#define AES_DECRYPT_ENABLE 0
#endif
#define DECRYPT_SRC_MARK  0xffffffff
/* Public constants ---------------------------------------------------------*/
/* Public function prototypes -----------------------------------------------*/
void ns_ase_swap_array(uint8_t * p_out, uint8_t const * p_in, uint8_t size);
void ns_aes_xor_128_swap(uint8_t* result, const uint8_t* a, const uint8_t* b, uint8_t size);
void ns_aes_xor_128_lsb(uint8_t* result, const uint8_t* a, const uint8_t* b, uint8_t size, uint8_t offset);
void ns_aes_xor_128_msb(uint8_t* result, const uint8_t* a, const uint8_t* b, uint8_t size);

void ns_aes_start(struct aes_func_env* env, const uint8_t* key, const uint8_t *val);
#if AES_DECRYPT_ENABLE
void ns_aes_deciphering(uint8_t *key, uint8_t *val, aes_func_result_cb res_cb);
#endif
void ns_aes_ciphering(uint8_t *key, uint8_t *val, aes_func_result_cb res_cb);



/**
 * @brief Start the AES CBC crypto function. Allocate memory for the CBC and start processing it
 *        Execute result callback at end of function execution
 *
 * @param[in]  key               Pointer to the Key to be used
 * @param[in]  iv                16 Bytes iv to use for cipher/decipher
 * @param[in]  in_message        Input message for AES-CBC exectuion
 * @param[out] out_message       Output message that will contain cipher or decipher data
 * @param[in]  message_len       Length of Input/Output message without mic
 * @param[in]  cipher            True to encrypt message, False to decrypt it.
 * @param[in]  res_cb            Function that will handle the AES CCM result
 */
void ns_aes_cbc(const uint8_t* key, const uint8_t* iv, const uint8_t* in_message, uint8_t* out_message,
                bool cipher, uint16_t message_len, aes_func_result_cb res_cb);


/**
 * @brief Start the AES CCM crypto function. Allocate memory for the CCM and start processing it
 *        Execute result callback at end of function execution
 *
 * @param[in]  key               Pointer to the Key to be used (LSB mode!!!!)
 * @param[in]  nonce             13 Bytes Nonce to use for cipher/decipher (MSB)
 * @param[in]  in_message        Input message for AES-CCM exectuion (MSB)
 * @param[out] out_message       Output message that will contain cipher+mic or decipher data
 * @param[in]  message_len       Length of Input/Output message without mic
 * @param[in]  mic_len           Length of the mic to use (2, 4, 6, 8, 10, 12, 14, 16 valid)
 * @param[in]  cipher            True to encrypt message, False to decrypt it.
 * @param[in]  add_auth_data     Additional Authentication data used for computation of MIC (MSB)
 * @param[in]  add_auth_data_len Length of Additional Authentication data
 * @param[in]  res_cb            Function that will handle the AES CCM result
 * @param[in]  src_info          Information used retrieve requester
 */
void aes_ccm(const uint8_t* key, const uint8_t* nonce, const uint8_t* in_message,
             uint8_t* out_message, uint16_t message_len, uint8_t mic_len, bool cipher,
             const uint8_t* add_auth_data, uint8_t add_auth_data_len, aes_ccm_func_result_cb res_cb, uint32_t src_info);
#define ns_aes_ccm aes_ccm

#endif /* __NS_AES_H__ */
/**
 * @}
 */


