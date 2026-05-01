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
#include "main.h"
#include <stdint.h>

static void LedAllOff(void);
static void Pulse(GPIO_Module* GPIOx, uint16_t Pin, uint32_t onDelay, uint32_t offDelay);
static void StartupAnimation(void);
static void HeartbeatLoop(void);

/**
 * @brief  Main program.
 */
int main(void)
{
    /* PB0 and PA6 are connected to the two user LEDs on N32WB031-STB. */
    LedInit(LED1_PORT, LED1_PIN);
    LedInit(LED2_PORT, LED2_PIN);

    LedAllOff();
    StartupAnimation();

    while (1)
    {
        HeartbeatLoop();
    }
}

void Delay(volatile uint32_t count)
{
    while (count-- > 0U)
    {
        __NOP();
    }
}

/**
 * @brief  Configures LED GPIO.
 * @param GPIOx x can be A to G to select the GPIO port.
 * @param Pin This parameter can be GPIO_PIN_0~GPIO_PIN_15.
 */
void LedInit(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIO_InitType GPIO_InitStructure;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    /* Enable the GPIO Clock */
    if (GPIOx == GPIOA)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
    }
    else if (GPIOx == GPIOB)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
    }
    else
    {
        return;
    }

    /* Configure the GPIO pin */
    if (Pin <= GPIO_PIN_ALL)
    {
        GPIO_InitStruct(&GPIO_InitStructure);
        GPIO_InitStructure.Pin = Pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitPeripheral(GPIOx, &GPIO_InitStructure);
    }
}

/**
 * @brief  Turns selected Led on.
 * @param GPIOx x can be A to G to select the GPIO port.
 * @param Pin This parameter can be GPIO_PIN_0~GPIO_PIN_15.
 */
void LedOn(GPIO_Module *GPIOx, uint16_t Pin)
{
    GPIO_SetBits(GPIOx, Pin);
}

void LedOff(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIO_ResetBits(GPIOx, Pin);
}

static void LedAllOff(void)
{
    LedOff(LED1_PORT, LED1_PIN);
    LedOff(LED2_PORT, LED2_PIN);
}

static void Pulse(GPIO_Module* GPIOx, uint16_t Pin, uint32_t onDelay, uint32_t offDelay)
{
    LedOn(GPIOx, Pin);
    Delay(onDelay);
    LedOff(GPIOx, Pin);
    Delay(offDelay);
}

static void StartupAnimation(void)
{
    uint32_t i;

    for (i = 0; i < 2U; ++i)
    {
        Pulse(LED1_PORT, LED1_PIN, 0x18000U, 0x08000U);
        Pulse(LED2_PORT, LED2_PIN, 0x18000U, 0x08000U);
    }

    for (i = 0; i < 2U; ++i)
    {
        LedOn(LED1_PORT, LED1_PIN);
        LedOn(LED2_PORT, LED2_PIN);
        Delay(0x18000U);
        LedAllOff();
        Delay(0x12000U);
    }
}

static void HeartbeatLoop(void)
{
    Pulse(LED1_PORT, LED1_PIN, 0x0C000U, 0x06000U);
    Pulse(LED1_PORT, LED1_PIN, 0x0C000U, 0x16000U);
    Pulse(LED2_PORT, LED2_PIN, 0x0C000U, 0x06000U);
    Pulse(LED2_PORT, LED2_PIN, 0x0C000U, 0x2A000U);
}

/**
 * @brief Assert failed function by user.
 * @param file The name of the call that failed.
 * @param line The source line number of the call that failed.
 */
#ifdef USE_FULL_ASSERT
void assert_failed(const uint8_t* expr, const uint8_t* file, uint32_t line)
{
    while (1)
    {
    }
}
#endif // USE_FULL_ASSERT

/**
 * @}
 */
