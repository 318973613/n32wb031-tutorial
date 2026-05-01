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
 * @file ns_error.h
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */

 /** @addtogroup 
 * @{
 */
#ifndef __NS_LIB_ERROR_H__
#define __NS_LIB_ERROR_H__


#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>  
#include <stdbool.h>     
#include <stddef.h>      
/* Public define ------------------------------------------------------------*/
#define ERROR_BASE_NUM                      (0x0)       
 
#define ERROR_SUCCESS                     (ERROR_BASE_NUM + 0)   
#define ERROR_INTERNAL                    (ERROR_BASE_NUM + 1)  
#define ERROR_NO_MEM                      (ERROR_BASE_NUM + 2)  
#define ERROR_NOT_FOUND                   (ERROR_BASE_NUM + 3)  
#define ERROR_NOT_SUPPORTED               (ERROR_BASE_NUM + 4)  
#define ERROR_INVALID_PARAM               (ERROR_BASE_NUM + 5)  
#define ERROR_INVALID_STATE               (ERROR_BASE_NUM + 6)  
#define ERROR_INVALID_LENGTH              (ERROR_BASE_NUM + 7)  
#define ERROR_INVALID_FLAGS               (ERROR_BASE_NUM + 8) 
#define ERROR_INVALID_DATA                (ERROR_BASE_NUM + 9) 
#define ERROR_DATA_SIZE                   (ERROR_BASE_NUM + 10) 
#define ERROR_TIMEOUT                     (ERROR_BASE_NUM + 11) 
#define ERROR_NULL                        (ERROR_BASE_NUM + 12) 
#define ERROR_FORBIDDEN                   (ERROR_BASE_NUM + 13) 
#define ERROR_INVALID_ADDR                (ERROR_BASE_NUM + 14) 
#define ERROR_BUSY                        (ERROR_BASE_NUM + 15) 
#define ERROR_CRC                         (ERROR_BASE_NUM + 16) 
#define ERROR_HARD_FAULT                  (ERROR_BASE_NUM + 17)     
/* Public typedef -----------------------------------------------------------*/
typedef struct
{
    uint16_t        line_num;    
    uint8_t const * p_file_name; 
    uint32_t        err_code;    
} error_info_t;
/* Public constants ---------------------------------------------------------*/
/* Public function prototypes -----------------------------------------------*/
void error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name);


#define ERROR_HANDLER(ERR_CODE)                                    \
    do                                                                 \
    {                                                                  \
        error_handler((ERR_CODE), __LINE__, (uint8_t*) __FILE__);  \
    } while (0)



#define ERROR_CHECK(ERR_CODE)                           \
    do                                                      \
    {                                                       \
        const uint32_t LOCAL_ERR_CODE = (ERR_CODE);         \
        if (LOCAL_ERR_CODE != ERROR_SUCCESS)                  \
        {                                                   \
            ERROR_HANDLER(LOCAL_ERR_CODE);              \
        }                                                   \
    } while (0)
    







#ifdef __cplusplus
}
#endif








#endif //__NS_LIB_ERROR_H__
/**
 * @}
 */


