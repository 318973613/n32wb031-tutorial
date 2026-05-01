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
 * @file ns_ecc.c
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

/** @addtogroup 
 * @{
 */

 /* Includes ------------------------------------------------------------------*/
#include "ns_ecc.h"
#include <stdint.h>
#include <stdio.h>
#include "uECC.h"
#include "sha256.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




/**
 * @brief Calculate hash degest.
 * @param[in] p_data raw data to process.
 * @param[in] data_len raw data length.
 * @param[out] p_hash hash degest. 
 * @return error code
 */
uint32_t ns_lib_ecc_hash_sha256(uint8_t *p_data, uint32_t data_len, uint8_t *p_hash)
{
    sha256_context_t   hash_context;
    sha256_init(&hash_context);
    if(!sha256_update(&hash_context, p_data, data_len)){
        sha256_final(&hash_context, p_hash, 0);
        return 0;
    }
    return 1;
}


/**
 * @brief big to little endian swap function.
 * @param[out] p_out output little endian data.
 * @param[in] p_in big endian raw data.
 * @param[in] size data length. 
 * @return none
 */
static void swap_endian(uint8_t * p_out, uint8_t const * p_in, size_t size)
{
    uint8_t const * p_first = p_in;
    uint8_t * p_last = p_out + size - 1;
    while (p_last >= p_out)
    {
        *p_last = *p_first;
        p_first++;
        p_last--;
    }
}

/**
 * @brief big to little endian swap function with two sets of data.
 * @param[out] p_out output little endian data.
 * @param[in] p_in big endian raw data.
 * @param[in] size half data length. 
 * @return none
 */
static void double_swap_endian(uint8_t * p_out, uint8_t const * p_in, size_t part_size)
{
    swap_endian(p_out, p_in, part_size);
    swap_endian(&p_out[part_size], &p_in[part_size], part_size);
}



/**
 * @brief ECDSA SHA256 digital signature verification function.
 * @param[in] p_public_key public key data.
 * @param[in] p_hash sha256 hash degest data.
 * @param[in] hash_size hash degest length. 
 * @param[in] p_signature digital signature. 
 * @return error code
 */
uint32_t ns_lib_ecc_ecdsa_verify(uint8_t *p_public_key, uint8_t *p_hash, uint32_t hash_size, uint8_t *p_signature)
{
    uint32_t error = 0;    
    uint8_t hash_le     [32];
    uint8_t signature_le[64];
    uint8_t public_key_le[64];    
    swap_endian(hash_le, p_hash, sizeof(hash_le));
    double_swap_endian(signature_le,p_signature,32);    
    double_swap_endian(public_key_le,p_public_key,32);
    int result = uECC_verify(public_key_le, hash_le, hash_size, signature_le, &curve_secp256r1);
    if(result != 1){
        error = 1;
    }
    return error;
}

















