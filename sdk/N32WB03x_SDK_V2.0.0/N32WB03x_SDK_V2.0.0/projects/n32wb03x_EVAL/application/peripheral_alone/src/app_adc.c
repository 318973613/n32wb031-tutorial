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
 * @file app_adc.c
 * @author NSING Firmware Team
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#include "app_adc.h"
#include "main.h"
#include <stdio.h>


/** @addtogroup 
 * @{
 */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/



/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Configures the different system clocks.
 */
void RCC_Configuration_ADC(void)
{
    /* Enable peripheral clocks */
    /* Enable GPIOB clocks */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
    /* Enable ADC clocks */
    RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_ADC, ENABLE);
    RCC_Enable_ADC_CLK_SRC_AUDIOPLL(ENABLE);    
    
    RCC_ConfigAdcClk(RCC_ADCCLK_SRC_AUDIOPLL);

}

/**
 * @brief  Configures the different GPIO ports.
 */
void GPIO_Configuration_ADC(void)
{
    GPIO_InitType GPIO_InitStructure;

    GPIO_InitStruct(&GPIO_InitStructure);
    /* Configure PB.10 (ADC Channel1) as analog input --------*/
    GPIO_InitStructure.Pin       = GPIO_PIN_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_ANALOG;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
}

/**
 * @brief  Configures the ADC and Read CH1 CH2.
 */
void ADC_Configuration(void)
{
    RCC_Configuration_ADC();
    GPIO_Configuration_ADC();

    /* Select the ADC channel */
    ADC_ConfigChannel(ADC, ADC_CTRL_CH_1);
    
    ADC_EnableBypassFilter(ADC, ENABLE);


}

/**
 * @brief  Configures and read the selected ADC channel
 * @param  ADC_Channel the ADC channel to read.
 *   This parameter can be any combination of the following values:
 *     @arg ADC_CTRL_CH_0 ADC Channel0 selected (PGA PB11/PB13)
 *     @arg ADC_CTRL_CH_1 ADC Channel1 selected (PB10)
 *     @arg ADC_CTRL_CH_2 ADC Channel2 selected (PB9)
 *     @arg ADC_CTRL_CH_3 ADC Channel3 selected (PB8)
 *     @arg ADC_CTRL_CH_4 ADC Channel4 selected (PB7)
 *     @arg ADC_CTRL_CH_5 ADC Channel5 selected (PB6)
 *     @arg ADC_CTRL_CH_6 ADC Channel6 selected (external voltage)
 *     @arg ADC_CTRL_CH_7 ADC Channel7 selected (temperature)
 */
uint16_t ADC_GetDataBlocking(uint32_t ADC_Channel)
{
    uint16_t dat;
    uint32_t timer = 0;

    /* Select the ADC channel */
    ADC_ConfigChannel(ADC, ADC_Channel);
    /* Enable ADC */
    ADC_Enable(ADC, ENABLE);

    while(ADC_GetFlagStatus(ADC,ADC_FLAG_DONE)== RESET)
    {
      if(++timer > 0xfffff)
            return 0xffff;
    }
    ADC_ClearFlag(ADC,ADC_FLAG_DONE);
    dat=ADC_GetDat(ADC);
    return dat;
}


/**
 * @}
 */

