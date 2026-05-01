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
 * @version v1.0.2
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#include "main.h"
#include "log.h"

void RCC_Configuration(void);
void GPIO_Configuration(void);
void ADC_Configuration(void);

uint16_t ADCConvertedValue[2];
uint32_t voltage[2] = {0}; 
/**
 * @brief  Main program
 */
int main(void)
{
	log_init();
    log_info("\nthis is adc single read Demo.\n");
	log_info("Please make sure J15 and J16 connect the IO to pin on board!\n");
	/* System Clocks Configuration */
	RCC_Configuration();
	
	/* Configure the GPIO ports */
    GPIO_Configuration();
	
	ADC_EnableBypassFilter(ADC, ENABLE);
    while (1)
    {
		ADC_ConfigChannel(ADC, ADC_CTRL_CH_3);
		ADC_Enable(ADC, ENABLE);
		while(ADC_GetFlagStatus(ADC,ADC_FLAG_DONE) == RESET);
		ADC_ClearFlag(ADC,ADC_FLAG_DONE);  
		ADCConvertedValue[1] = ADC_GetDat(ADC);
    
		voltage[1] = ADC_ConverValueToVoltage(ADCConvertedValue[1], ADC_CTRL_CH_3);
		log_info("ADC CH3 value: %4d  |  ADC CH3 vol_mV: %4d .\r\n",ADCConvertedValue[1],voltage[1]);  
	
		ADC_ConfigChannel(ADC, ADC_CTRL_CH_1);
		ADC_Enable(ADC, ENABLE);
		while(ADC_GetFlagStatus(ADC,ADC_FLAG_DONE) == RESET);
		ADC_ClearFlag(ADC,ADC_FLAG_DONE);  
		ADCConvertedValue[0] = ADC_GetDat(ADC);

		voltage[0] = ADC_ConverValueToVoltage(ADCConvertedValue[0], ADC_CTRL_CH_1);
		log_info("ADC CH1 value: %4d  |  ADC CH1 vol_mV: %4d .\r\n\r\n",ADCConvertedValue[0],voltage[0]);    
		Delay_ms(1000);
    }
}


/**
 * @brief  Configures the different system clocks.
 */
void RCC_Configuration(void)
{
	/* Enable peripheral clocks */
    /* Enable GPIOB clocks */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
    /* Enable ADC clocks */
    RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_ADC, ENABLE);
        
    RCC_ConfigAdcClk(RCC_ADCCLK_SRC_AUDIOPLL);

    /* enable ADC 4M clock */
    RCC_Enable_ADC_CLK_SRC_AUDIOPLL(ENABLE);
}


/**
 * @brief  Configures the different GPIO ports.
 */
void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;

    GPIO_InitStruct(&GPIO_InitStructure);
    /* Configure PB.10 (ADC Channel1) PB.8 (ADC Channel3) as analog input --------*/
    GPIO_InitStructure.Pin       = GPIO_PIN_10|GPIO_PIN_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_ANALOG;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
}


#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param file pointer to the source file name
 * @param line assert_param error line source number
 */
void assert_failed(const uint8_t* expr, const uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    while (1)
    {
    }
}

#endif

/**
 * @}
 */

/**
 * @}
 */

/*************** (C) COPYRIGHT NSING Technologies Inc *****END OF FILE***************/


