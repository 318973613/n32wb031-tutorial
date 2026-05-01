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
 * @file main.c
 * @author NSING Firmware Team
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "n32wb03x.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LED1_PORT GPIOB
#define LED1_PIN  GPIO_PIN_0
#define LED2_PORT GPIOA
#define LED2_PIN  GPIO_PIN_6

#define FLASH_TEST_ADDRESS       0x1020000
#define BUFFER_SIZE              4096
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t m_buffer[BUFFER_SIZE];
//Allocate a buffer for read flash memory and for compare with a static buffer.
static uint8_t buffer[BUFFER_SIZE];
GPIO_InitType GPIO_InitStructure;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static void assert_handle(void)
{
    GPIO_SetBits(LED1_PORT, LED1_PIN);
    while(1){

    }
}


/**
 * @brief  Main program.
 */
int main(void)
{
    //Initialize dev board leds
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);    
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);    
    GPIO_InitStructure.Pin = LED1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_NO_PULL;
    GPIO_InitPeripheral(LED1_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = LED2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_NO_PULL;    
    GPIO_InitPeripheral(LED2_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(LED1_PORT, LED1_PIN);
    GPIO_ResetBits(LED2_PORT, LED2_PIN);    
    
    //Initialize Qflash.
    Qflash_Init();
    //Erase one Flash Sector
    Qflash_Erase_Sector(FLASH_TEST_ADDRESS);
    //Read test flash sector into buffer ram
    Qflash_Read(FLASH_TEST_ADDRESS, buffer, BUFFER_SIZE);
    for(uint32_t i=0;i<BUFFER_SIZE;i++){
        if(buffer[i] != 0xFF){
            assert_handle();
        }
    }
    //Assign value to m_buffer
    for(uint32_t i=0;i<BUFFER_SIZE;i++){
        m_buffer[i] = i;
    }    
    //Write buffer data to flash.
    Qflash_Write(FLASH_TEST_ADDRESS, m_buffer, BUFFER_SIZE); 
    //Read test flash sector into buffer ram
    Qflash_Read(FLASH_TEST_ADDRESS, buffer, BUFFER_SIZE);
    if(memcmp(buffer,m_buffer,BUFFER_SIZE) != 0){
        assert_handle();
    }
    GPIO_SetBits(LED2_PORT, LED2_PIN);
    while(1){
        
    }
    
    
    
}



/**
 * @}
 */
