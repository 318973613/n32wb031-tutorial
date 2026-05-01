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
 * @file n32wb03x_adc.h
 * @author NSING Firmware Team
 * @version v1.0.4
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#ifndef __N32WB03X_ADC_H__
#define __N32WB03X_ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32wb03x.h"
#include <stdbool.h>

/** @addtogroup N32WB03X_StdPeriph_Driver
 * @{
 */

/** @addtogroup ADC
 * @{
 */

/** @addtogroup ADC_Exported_Types
 * @{
 */

/**
 * @brief  ADC Init structure definition
 */
typedef struct
{

    FunctionalState MicbiasEn;      /*!< MIC BIAS enable control signal
                                           This parameter can be set to ENABLE or DISABLE */

    FunctionalState AudiopgaInitEn; /*!< Audio PGA off-chip capacitorfast charge control
                                            This parameter can be set to ENABLE or DISABLE. */

    FunctionalState PgaEn;          /*!< Audio PGA Enable
                                             This parameter can be set to ENABLE or DISABLE. */

    uint32_t Micbias;               /*!< MIC BIAS output voltage range setting, step=0.1V
                                             This parameter must range from 1.6 to 2.3 */

    uint32_t AudiopgaPeak;          /*!< Audio PGA drive signal amplitude setting. */                                

    uint32_t AudiopgaDrive;         /*!< Audio PGA drive current setting */
                                                                     
    uint32_t PgaGain;               /*!< Audio PGA gain configuration (0??42dB) */

} ADC_PGA_InitType;
/**
 * @}
 */

/** @addtogroup ADC_Exported_Constants
 * @{
 */
#define IsAdcChannel(PERIPH)    (((PERIPH) == ADC_CTRL_CH_0) || ((PERIPH) == ADC_CTRL_CH_1)|| \
                                ((PERIPH) == ADC_CTRL_CH_2) || ((PERIPH) == ADC_CTRL_CH_3)|| \
                                ((PERIPH) == ADC_CTRL_CH_4) || ((PERIPH) == ADC_CTRL_CH_5)|| \
                                ((PERIPH) == ADC_CTRL_CH_6) || ((PERIPH) == ADC_CTRL_CH_7) )

#define IsAdcModule(PERIPH)     (((PERIPH) == ADC))

#define IsAdcDmaModule(PERIPH) (((PERIPH) == ADC))

/** @addtogroup MIC BIAS output voltage range setting, step=0.1V
 * @{
 */
 

/** @addtogroup Audio PGA drive signal amplitude setting
 * @{
 */

/** @addtogroup Audio PGA drive current setting
 * @{
 */

/** @addtogroup ADC_Exported_Constants
 * @{
 */


/**
 * @brief  ADC Init structure definition
 */
typedef struct
{
    FunctionalState VadZcrdEn; /*!< Audio PGA off-chip capacitorfast charge control
                                            This parameter can be set to ENABLE or DISABLE. */

    FunctionalState VadEdEn;          /*!< Audio PGA Enable
                                             This parameter can be set to ENABLE or DISABLE. */
    
        FunctionalState VadFilterByp;         /*!< Decides between Single and Continuous Conversion modes
                                             This parameter can be set to ENABLE or DISABLE. */
} ADC_DET_InitType;
             
/**
 * @}
 */

/** @addtogroup ADC_channels
 * @{
 */

/**
 * @}
 */


/** @addtogroup ADC_interrupts_definition
    the hign 8 bits (02,01,04) do not correspond to CTRL1 register, 
        the main poupose is to match the corresponding flag bit in the sts register 
 * @{
 */

#define ADC_INT_DONE     ((uint16_t)0x0100)
#define ADC_INT_AWD      ((uint16_t)0x0200)
#define ADC_INT_PGARDY   ((uint16_t)0x0400)

#define IsAdcInt(IT) ((((IT) & (uint16_t)0xF8FF) == 0x00) && ((IT) != 0x00))

#define IsAdcGetInt(IT) (((IT) == ADC_INT_DONE) || ((IT) == ADC_INT_AWD) || ((IT) == ADC_INT_PGARDY))
/**
 * @}
 */

/** @addtogroup ADC_flags_definition
 * @{
 */

#define ADC_FLAG_DONE       ((uint8_t)0x01)
#define ADC_FLAG_AWD          ((uint8_t)0x02)
#define ADC_FLAG_PGARDY     ((uint8_t)0x04)
#define IsAdcClrFlag(FLAG)  ((((FLAG) & (uint8_t)0xF8) == 0x00) && ((FLAG) != 0x00))
#define IsAdcGetFlag(FLAG)                                                                                             \
    (((FLAG) == ADC_FLAG_DONE) || ((FLAG) == ADC_FLAG_AWD) || ((FLAG) == ADC_FLAG_PGARDY))
/**
 * @}
 */
 
 
 /** @addtogroup ADC_MICBIAS_definition
 * @{
 */
#define MICBIAS_1V6     ADC_PGA_CFG_MICBIAS_0
#define MICBIAS_1V7     ADC_PGA_CFG_MICBIAS_1
#define MICBIAS_1V8     ADC_PGA_CFG_MICBIAS_2
#define MICBIAS_1V9     ADC_PGA_CFG_MICBIAS_3
#define MICBIAS_2V0     ADC_PGA_CFG_MICBIAS_4
#define MICBIAS_2V1     ADC_PGA_CFG_MICBIAS_5
#define MICBIAS_2V2     ADC_PGA_CFG_MICBIAS_6
#define MICBIAS_2V3     ADC_PGA_CFG_MICBIAS_7
#define IsAdcMicbias(Micbias)   (((Micbias) == MICBIAS_1V6) || ((Micbias) == MICBIAS_1V7) || \
                                ((Micbias) == MICBIAS_1V8) || ((Micbias) == MICBIAS_1V9) || \
                                ((Micbias) == MICBIAS_2V0) || ((Micbias) == MICBIAS_2V1) || \
                                ((Micbias) == MICBIAS_2V2) || ((Micbias) == MICBIAS_2V3) )

/**
 * @}
 */
 
#define IsAdcPgaPeak(Peak)     (((Peak) == ADC_PGA_CFG_PEAK_0) || ((Peak) == ADC_PGA_CFG_PEAK_1) || \
                                ((Peak) == ADC_PGA_CFG_PEAK_2) || ((Peak) == ADC_PGA_CFG_PEAK_3) )
                                
#define IsAdcPgaDriver(Drive)   (((Drive) == ADC_PGA_CFG_DRIVE_0) || ((Drive) == ADC_PGA_CFG_DRIVE_1) || \
                                ((Drive) == ADC_PGA_CFG_DRIVE_2) || ((Drive) == ADC_PGA_CFG_DRIVE_3))
                                
#define IsAdcPgaGain(Gain)      (((Gain) == ADC_PGA_CFG_GAIN_0dB) || ((Gain) == ADC_PGA_CFG_GAIN_6dB) || \
                                ((Gain) == ADC_PGA_CFG_GAIN_12dB) || ((Gain) == ADC_PGA_CFG_GAIN_18dB) || \
                                ((Gain) == ADC_PGA_CFG_GAIN_24dB) || ((Gain) == ADC_PGA_CFG_GAIN_30dB) || \
                                ((Gain) == ADC_PGA_CFG_GAIN_36dB) || ((Gain) == ADC_PGA_CFG_GAIN_42dB) )                   
                                
/**
 * @}
 */

/** @addtogroup ADC_thresholds
 * @{
 */
#define IsAdcValid(Value) ((Value) <= 0xFFF)

#define IsAdcVoiceZcrValid(THRESHOLD) ((THRESHOLD) <= 0xFF)
#define IsAdcVoiceEDValid(THRESHOLD)  ((THRESHOLD) <= 0x007FFFFF)
/**
 * @}
 */


/** @addtogroup ADC_NoiseOffset
 * @{
 */

#define IsAdcNoiseOffsetValid(OFFSET) ((OFFSET) <= 0x00FFFFFF)

/** @addtogroup ADC FIR FLT COEFFICIENT
 * @{
 */

#define IsAdcFIRFLTValid(COEFFICIENT) ((COEFFICIENT) <= 0xFFFF)
/**
 * @}
 */

/**
 * @}
 */



/**
 * @}
 */

/**
 * @}
 */

/** @addtogroup ADC_Exported_Functions
 * @{
 */

void ADC_DeInit(ADC_Module* ADCx);
void ADC_Enable(ADC_Module* ADCx, FunctionalState Cmd);
void ADC_EnableDMA(ADC_Module* ADCx, FunctionalState Cmd);
void ADC_EnableTS(ADC_Module* ADCx, FunctionalState Cmd);
void ADC_EnableAWD(ADC_Module* ADCx, FunctionalState Cmd);
void ADC_ConfigInt(ADC_Module* ADCx, uint16_t ADC_IT, FunctionalState Cmd);
void ADC_ConfigChannel(ADC_Module* ADCx, uint16_t Channel);
void ADC_ConfigContinuousMode(ADC_Module* ADCx, FunctionalState Cmd);
void ADC_ConfigAnalogWatchdogThresholds(ADC_Module* ADCx, uint32_t High_Threshold, uint32_t Low_Threshold);
void ADC_ConfigVoiceZcrThresholds(ADC_Module* ADCx, uint32_t High_Threshold, uint32_t Low_Threshold);
void ADC_ConfigEDThreshold(ADC_Module* ADCx, uint32_t High_Threshold, uint32_t Low_Threshold);
void ADC_SetNoiseOffset(ADC_Module* ADCx, uint32_t moffset);
uint16_t ADC_GetDat(ADC_Module* ADCx);
FlagStatus ADC_GetFlagStatus(ADC_Module* ADCx, uint8_t ADC_FLAG);
void ADC_ClearFlag(ADC_Module* ADCx, uint8_t ADC_FLAG);
void ADC_EnableZeroCross(ADC_Module* ADCx, FunctionalState Cmd);
void ADC_EnableEnergy(ADC_Module* ADCx, FunctionalState Cmd);
void ADC_EnableBypassFilter(ADC_Module* ADCx, FunctionalState Cmd);
void ADC_EnableMICBIAS(ADC_Module* ADCx, FunctionalState Cmd);
void ADC_SetMICOutputVoltage(ADC_Module* ADCx, uint32_t MIC_VoltageValue);
void ADC_EnableAUDIOPGA(ADC_Module* ADCx, FunctionalState Cmd);
void ADC_SetAUDIOAmplitude(ADC_Module* ADCx, uint32_t AMP_Value);
void ADC_SetAUDIOCurrent(ADC_Module* ADCx, uint32_t Drive_Value);
void ADC_EnablePGA(ADC_Module* ADCx, FunctionalState Cmd);
void ADC_SetPGAGain(ADC_Module* ADCx, uint32_t Gain_Value);
void ADC_SetOverSampleCounter(ADC_Module* ADCx, uint32_t Cnt);
uint32_t ADC_ConverValueToVoltage(uint16_t adc_value, uint16_t channel);
uint32_t ADC16Bit_ConverValueToVoltage(int16_t value_16bit, uint16_t channel,int16_t offset);
float ADC_ConverValueToTemperature(uint16_t adc_value);

#ifdef __cplusplus
}
#endif

#endif /*__N32WB03X_ADC_H__ */

/**
 * @}
 */
/**
 * @}
 */
