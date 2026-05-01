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
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#include "main.h"
#include <stdio.h>
#include "log.h"
/**
 *  Cortex-M0 SysTick
 */


#define SYSTICK_1MS           ((uint32_t)1000)
#define DEMO_USART_BAUDRATE    ((uint32_t)115200)

uint32_t Tick_num = 0;

void rcc_debug(void)
{
    GPIO_InitType GPIO_InitStructure;
    
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, ENABLE);
    
    GPIO_InitStruct(&GPIO_InitStructure);
    
    /* Configure rcc_mco pin*/
    GPIO_InitStructure.Pin               = GPIO_PIN_5;
    GPIO_InitStructure.GPIO_Mode         = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Speed        = GPIO_SPEED_HIGH;
    GPIO_InitStructure.GPIO_Alternate    = GPIO_AF4_MCO;
    GPIO_InitStructure.GPIO_Pull         = GPIO_NO_PULL;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
    
    RCC_ConfigMco(RCC_MCO_SYSCLK);
    printf("RCC->CFG:0x%02x\r\n", RCC->CFG);
}

/**
 * @brief  Main program.
 */
int main(void)
{
    /* USART Init */
    log_init();
    printf("Cortex-M0 SysTick \r\n");
    
    rcc_debug();
    /* Get SystemCoreClock */
    SystemCoreClockUpdate();
    printf("SystemCoreClock:%d\r\n", SystemCoreClock);
    /* Config 1s SysTick  */
    SysTick_Config(SystemCoreClock/SYSTICK_1MS);

    while (1)
    {
        if((Tick_num%1000) == 0)
        {
            printf("Cortex-M0 SysTick IRQ \r\n");
        }
    }
}

/**
 * @}
 */

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
*          line: assert_param error line source number
 * @return None
 */
void assert_failed(const uint8_t* expr, const uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {}
}

/**
 * @}
 */
#endif
