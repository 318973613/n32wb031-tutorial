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
 * @file main.h
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32wb03x.h"
#include "log.h"
#include "string.h"
#define  KEYSCAN_INFO_CHECK         2
static const uint32_t keyscan_map[][KEYSCAN_INFO_CHECK] = 
{
    0x00,0x00,               //NULL
    0x00000000,0x00000001,
    0x00000000,0x00000002,
    0x00000000,0x00000004,
    0x00000000,0x00000008,
    0x00000000,0x00000010,
    0x00000000,0x00000020,
    0x00000000,0x00000040,
    0x00000000,0x00000080,
    0x00000000,0x00000100,
    0x00000000,0x00000200,
    0x00000000,0x00000400,
    0x00000000,0x00000800,
    0x00000000,0x00001000,
    0x00000000,0x00002000,
    0x00000000,0x00004000,
    0x00000000,0x00010000,
    0x00000000,0x00020000,
    0x00000000,0x00040000,
    0x00000000,0x00080000,
    0x00000000,0x00100000,
    0x00000000,0x00200000,
    0x00000000,0x00400000,
    0x00000000,0x00800000,
    0x00000000,0x01000000,
    0x00000000,0x02000000,
    0x00000000,0x04000000,
    0x00000000,0x08000000,
    0x00000000,0x10000000,
    0x00000001,0x00000000,
    0x00000002,0x00000000,
    0x00000004,0x00000000,
    0x00000008,0x00000000,
    0x00000010,0x00000000,
    0x00000020,0x00000000,
    0x00000040,0x00000000,
    0x00000080,0x00000000,
    0x00010000,0x00000000,
    0x00020000,0x00000000,
    0x00040000,0x00000000,
    0x00080000,0x00000000,
    0x00100000,0x00000000,
    0x00200000,0x00000000,
    0x00400000,0x00000000,
    0x00800000,0x00000000,

    0x00,0x00,              //NULL

};

enum KEY_NUM{
    KEYSCAN_NULL = 0,
    
    KEYSCAN_KEY1,
    KEYSCAN_KEY2,
    KEYSCAN_KEY3,
    KEYSCAN_KEY4,
    KEYSCAN_KEY5,
    KEYSCAN_KEY6,
    KEYSCAN_KEY7,
    KEYSCAN_KEY8,
    KEYSCAN_KEY9, 
    KEYSCAN_KEY10,
    KEYSCAN_KEY11,
    KEYSCAN_KEY12,
    KEYSCAN_KEY13,
    KEYSCAN_KEY14,
    KEYSCAN_KEY15,
    KEYSCAN_KEY16, 
    KEYSCAN_KEY17,
    KEYSCAN_KEY18,
    KEYSCAN_KEY19,
    KEYSCAN_KEY20,
    KEYSCAN_KEY21,
    KEYSCAN_KEY22,
    KEYSCAN_KEY23,
    KEYSCAN_KEY24,
    KEYSCAN_KEY25,
    KEYSCAN_KEY26,
    KEYSCAN_KEY27,
    KEYSCAN_KEY28,
    KEYSCAN_KEY29,
    KEYSCAN_KEY30,
    KEYSCAN_KEY31,
    KEYSCAN_KEY32,
    KEYSCAN_KEY33,
    KEYSCAN_KEY34,
    KEYSCAN_KEY35,
    KEYSCAN_KEY36,
    KEYSCAN_KEY37,
    KEYSCAN_KEY38,
    KEYSCAN_KEY39,
    KEYSCAN_KEY40,
    KEYSCAN_KEY41,
    KEYSCAN_KEY42,
    KEYSCAN_KEY43,
    KEYSCAN_KEY44,

    KEYSCAN_KEY_MAX
}; 


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */
