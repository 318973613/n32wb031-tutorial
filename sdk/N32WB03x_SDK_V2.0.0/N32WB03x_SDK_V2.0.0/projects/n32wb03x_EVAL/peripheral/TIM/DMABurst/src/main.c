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

/** @addtogroup TIM_DMABurst
 * @{
 */

#define TIM1_DMAR_ADDRESS ((uint32_t)0x40012C4C) /* TIM AR (Auto Reload Register) address */

GPIO_InitType GPIO_InitStructure;
DMA_InitType DMA_InitStructure;
TIM_TimeBaseInitType TIM_TimeBaseStructure;
OCInitType TIM_OCInitStructure;
uint16_t SRC_Buffer[6] = {0x0FFF, 0x0000, 0x0800};
uint16_t DmaAgain      = 0;

/**
 * @brief  Main program
 */
int main(void)
{
    /* TIM1 and GPIOA clock enable */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_TIM1 | RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, ENABLE);

    /* DMA clock enable */
    RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_DMA, ENABLE);

    GPIO_InitStruct(&GPIO_InitStructure);
    /* GPIOB Configuration: Channel 1 as alternate function push-pull */
    GPIO_InitStructure.Pin            = GPIO_PIN_8;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Current   = GPIO_DC_LOW;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF1_TIM1;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

    /* TIM1 DeInit */
    TIM_DeInit(TIM1);

    /* DMA Channel5 Config */
    DMA_DeInit(DMA_CH5);
    DMA_InitStructure.PeriphAddr     = (uint32_t)&TIM1->DADDR;   //TIM1_DMAR_ADDRESS;
    DMA_InitStructure.MemAddr        = (uint32_t)SRC_Buffer;
    DMA_InitStructure.Direction      = DMA_DIR_PERIPH_DST;
    DMA_InitStructure.BufSize        = 3;
    DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc  = DMA_MEM_INC_ENABLE;
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_SIZE_HALFWORD;
    DMA_InitStructure.MemDataSize    = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.CircularMode   = DMA_MODE_NORMAL;
    DMA_InitStructure.Priority       = DMA_PRIORITY_HIGH;
    DMA_InitStructure.Mem2Mem        = DMA_M2M_DISABLE;
    DMA_Init(DMA_CH5, &DMA_InitStructure);
    DMA_RequestRemap(DMA_REMAP_TIM1_UP,DMA,DMA_CH5,ENABLE);

    /* Time base configuration */
    TIM_TimeBaseStructure.Period    = 0xFFFF;
    TIM_TimeBaseStructure.Prescaler = (uint16_t)(SystemCoreClock / 24000000) - 1;
    TIM_TimeBaseStructure.ClkDiv    = 0x0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    TIM_InitTimeBase(TIM1, &TIM_TimeBaseStructure);

    /* TIM Configuration in PWM Mode */
    TIM_OCInitStructure.OcMode      = TIM_OCMODE_PWM1;
    TIM_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse       = 0xFFF;
    TIM_InitOc1(TIM1, &TIM_OCInitStructure);

    /* TIM1 DADDR Base register and DMA Burst Length Config */
    TIM_ConfigDma(TIM1, TIM_DMABASE_AR, TIM_DMABURST_LENGTH_3TRANSFERS);

    /* TIM1 DMA Update enable */
    TIM_EnableDma(TIM1, TIM_DMA_UPDATE, ENABLE);

    /* TIM1 enable */
    TIM_Enable(TIM1, ENABLE);

    /* TIM1 PWM Outputs Enable */
    TIM_EnableCtrlPwmOutputs(TIM1, ENABLE);

    /* DMA Channel5 enable */
    DMA_EnableChannel(DMA_CH5, ENABLE);

    /* Wait until DMA Channel5 end of Transfer */
    while (!DMA_GetFlagStatus(DMA_FLAG_TC5, DMA))
    {
    }

    DMA_ClearFlag(DMA_FLAG_GL5 | DMA_FLAG_TC5 | DMA_FLAG_HT5 | DMA_FLAG_TE5, DMA);
    /* Infinite loop */
    while (1)
    {
        if(DmaAgain)
        {
            DmaAgain = 0;
            DMA_DeInit(DMA_CH5);
            DMA_Init(DMA_CH5, &DMA_InitStructure);
            DMA_RequestRemap(DMA_REMAP_TIM1_UP,DMA,DMA_CH5,ENABLE);
            
            TIM_ConfigDma(TIM1, TIM_DMABASE_AR, TIM_DMABURST_LENGTH_3TRANSFERS);
            TIM_EnableDma(TIM1, TIM_DMA_UPDATE, ENABLE);

            /* DMA Channel5 enable */
            DMA_EnableChannel(DMA_CH5, ENABLE);

            /* Wait until DMA Channel5 end of Transfer */
            while (!DMA_GetFlagStatus(DMA_FLAG_TC5, DMA))
            {
            }

            DMA_ClearFlag(DMA_FLAG_GL5 | DMA_FLAG_TC5 | DMA_FLAG_HT5 | DMA_FLAG_TE5, DMA);
        }

    }
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
