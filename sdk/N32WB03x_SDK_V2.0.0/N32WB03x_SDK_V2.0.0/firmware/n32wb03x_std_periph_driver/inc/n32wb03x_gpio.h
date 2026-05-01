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
 * @file n32wb03x_gpio.h
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#ifndef __N32WB03X_GPIO_H__
#define __N32WB03X_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32wb03x.h"

/** @addtogroup N32WB03X_StdPeriph_Driver
 * @{
 */

/** @addtogroup GPIO
 * @{
 */

/** @addtogroup GPIO_Exported_Types
 * @{
 */

/**
 * @brief  GPIO Init structure definition
 */
typedef struct
{
    uint32_t Pin; /*!< Specifies the GPIO pins to be configured.
                        This parameter can be any value of @ref GPIO_pins_define */
    
    uint32_t GPIO_Mode; /*!< Specifies the operating mode for the selected pins.
                                   This parameter can be a value of @ref GPIO_mode_define */    

    uint32_t GPIO_Pull;  /*!< Specifies the Pull-up or Pull-Down activation for the selected pins.
                                    This parameter can be a value of @ref GPIO_pull_define */                                     

    uint32_t GPIO_Speed; /*!< Specifies the slew rate for the selected pins.
                                         This parameter can be a value of @ref GPIO_speed_define */                                          

    uint32_t GPIO_Current; /*!<Driving current of the select pins>.
                                                            This paramter can be a value of @ref GPIO_current_define */          
    
    uint32_t GPIO_Alternate; /*!< Specifies the alternate function for the selected pins 
                                   This parameter can be a value of @ref GPIOEx_Alternate_function_selection */ 
} GPIO_InitType;

/**
 * @brief  Bit_SET and Bit_RESET enumeration
 */

typedef enum
{
    Bit_RESET = 0,
    Bit_SET
} Bit_OperateType;

#define IS_GPIO_BIT_OPERATE(OPERATE) (((OPERATE) == Bit_RESET) || ((OPERATE) == Bit_SET))

/**
 * @}
 */

/** @addtogroup GPIO_Exported_Constants GPIO Exported Constants
 * @{
 */

#define IS_GPIO_ALL_PERIPH(PERIPH)                                                                                     \
    (((PERIPH) == GPIOA) || ((PERIPH) == GPIOB))

#define GPIO_GET_INDEX(PERIPH) (((PERIPH) == (GPIOA))? 0 :1)

#define GPIO_GET_PERIPH(INDEX) (((INDEX)==((uint8_t)0x00))? GPIOA :GPIOB)

/** @addtogroup GPIO_pins_define Pin definition
 * @{
 */
#define GPIO_PIN_0   ((uint16_t)0x0001U) /*!< Pin 0 selected */
#define GPIO_PIN_1   ((uint16_t)0x0002U) /*!< Pin 1 selected */
#define GPIO_PIN_2   ((uint16_t)0x0004U) /*!< Pin 2 selected */
#define GPIO_PIN_3   ((uint16_t)0x0008U) /*!< Pin 3 selected */
#define GPIO_PIN_4   ((uint16_t)0x0010U) /*!< Pin 4 selected */
#define GPIO_PIN_5   ((uint16_t)0x0020U) /*!< Pin 5 selected */
#define GPIO_PIN_6   ((uint16_t)0x0040U) /*!< Pin 6 selected */
#define GPIO_PIN_7   ((uint16_t)0x0080U) /*!< Pin 7 selected */
#define GPIO_PIN_8   ((uint16_t)0x0100U) /*!< Pin 8 selected */
#define GPIO_PIN_9   ((uint16_t)0x0200U) /*!< Pin 9 selected */
#define GPIO_PIN_10  ((uint16_t)0x0400U) /*!< Pin 10 selected */
#define GPIO_PIN_11  ((uint16_t)0x0800U) /*!< Pin 11 selected */
#define GPIO_PIN_12  ((uint16_t)0x1000U) /*!< Pin 12 selected */
#define GPIO_PIN_13  ((uint16_t)0x2000U) /*!< Pin 13 selected */
#define GPIO_PIN_ALL ((uint16_t)0x3FFFU) /*!< All pins selected */

#define GPIOA_PIN_AVAILABLE  (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 |GPIO_PIN_3 |\
                              GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6)
#define GPIOB_PIN_AVAILABLE  GPIO_PIN_ALL
/**
 * @}
 */
#define IS_GPIO_PIN(PIN) ((((PIN) & (uint16_t)0x00) == 0x00) && ((PIN) != (uint16_t)0x00))

#define IS_GET_GPIO_PIN(PIN)                                                                                           \
    (((PIN) == GPIO_PIN_0) || ((PIN) == GPIO_PIN_1) || ((PIN) == GPIO_PIN_2) || ((PIN) == GPIO_PIN_3)                  \
     || ((PIN) == GPIO_PIN_4) || ((PIN) == GPIO_PIN_5) || ((PIN) == GPIO_PIN_6) || ((PIN) == GPIO_PIN_7)               \
     || ((PIN) == GPIO_PIN_8) || ((PIN) == GPIO_PIN_9) || ((PIN) == GPIO_PIN_10) || ((PIN) == GPIO_PIN_11)             \
     || ((PIN) == GPIO_PIN_12) || ((PIN) == GPIO_PIN_13))


#define IS_GPIO_PIN_AVAILABLE(__INSTANCE__,__PIN__)  \
           ((((__INSTANCE__) == GPIOA) && (((__PIN__) & (GPIOA_PIN_AVAILABLE)) != 0) && (((__PIN__) | (GPIOA_PIN_AVAILABLE)) == (GPIOA_PIN_AVAILABLE))) || \
            (((__INSTANCE__) == GPIOB) && (((__PIN__) & (GPIOB_PIN_AVAILABLE)) != 0) && (((__PIN__) | (GPIOB_PIN_AVAILABLE)) == (GPIOB_PIN_AVAILABLE))))

/** @addtogroup GPIO_mode_define Mode definition
  *        Values convention: 0xW0yz00YZ
  *           - W  : GPIO mode or EXTI Mode
  *           - y  : External IT or Event trigger detection
  *           - z  : IO configuration on External IT or Event
  *               - Y  : Output type (Push Pull or Open Drain)
  *               - Z  : IO Direction mode (Input, Output, Alternate or Analog)
 * @{
 */
#define GPIO_MODE_INPUT                 ((uint32_t)0x00000000U) /*!< Input Floating Mode */
#define GPIO_MODE_OUTPUT_PP             ((uint32_t)0x00000001U) /*!< Output Push Pull Mode */
#define GPIO_MODE_OUTPUT_OD             ((uint32_t)0x00000011U) /*!< Output Open Drain Mode */
#define GPIO_MODE_AF_PP                 ((uint32_t)0x00000002U) /*!< Alternate Function Push Pull Mode */  
#define GPIO_MODE_AF_OD                 ((uint32_t)0x00000012U) /*!< Alternate Function Open Drain Mode */

#define GPIO_MODE_ANALOG                ((uint32_t)0x00000003U)  /*!< Analog Mode */

#define GPIO_MODE_IT_RISING             ((uint32_t)0x10110000U)
#define GPIO_MODE_IT_FALLING            ((uint32_t)0x10210000U)
#define GPIO_MODE_IT_RISING_FALLING     ((uint32_t)0x10310000U)

#define GPIO_MODE_EVT_RISING            ((uint32_t)0x10132000U)
#define GPIO_MODE_EVT_FALLING           ((uint32_t)0x10120000U)
#define GPIO_MODE_EVT_RISING_FALLING    ((uint32_t)0x10320000U)
/**
 * @}
 */
#define IS_GPIO_MODE(__MODE__) (((__MODE__) == GPIO_MODE_INPUT)              ||\
                                ((__MODE__) == GPIO_MODE_OUTPUT_PP)          ||\
                                ((__MODE__) == GPIO_MODE_OUTPUT_OD)          ||\
                                ((__MODE__) == GPIO_MODE_AF_PP)              ||\
                                ((__MODE__) == GPIO_MODE_AF_OD)              ||\
                                ((__MODE__) == GPIO_MODE_IT_RISING)          ||\
                                ((__MODE__) == GPIO_MODE_IT_FALLING)         ||\
                                ((__MODE__) == GPIO_MODE_IT_RISING_FALLING)  ||\
                                ((__MODE__) == GPIO_MODE_EVT_RISING)         ||\
                                ((__MODE__) == GPIO_MODE_EVT_FALLING)        ||\
                                ((__MODE__) == GPIO_MODE_EVT_RISING_FALLING) ||\
                                ((__MODE__) == GPIO_MODE_ANALOG))

/**
 * @addtogroup GPIO_pull_define Pull definition
 * @brief GPIO Pull-Up or Pull-Down Activation
 * @{
 */
#define GPIO_NO_PULL   ((uint32_t)0x00000000U)
#define GPIO_PULL_UP   ((uint32_t)0x00000001U)
#define GPIO_PULL_DOWN ((uint32_t)0x00000002U)
/**
  * @}
  */
#define IS_GPIO_PULL(__PULL__) (((__PULL__) == GPIO_NO_PULL) || ((__PULL__) == GPIO_PULL_UP) || \
                                ((__PULL__) == GPIO_PULL_DOWN))

/**
 * @addtogroup  GPIO_speed_define Speed definition
 * @brief GPIO Output Maximum frequency
 * @{
 */
#define GPIO_SPEED_HIGH   ((uint32_t)0x00000000U)
#define GPIO_SPEED_LOW    ((uint32_t)0x00000001U)
/**
 * @}
 */
#define IS_GPIO_SPEED(_SPEED_)                           \
    (((_SPEED_) == GPIO_SPEED_HIGH) || ((_SPEED_) == GPIO_SPEED_LOW))

/**
 * @addtogroup GPIO_current_define Current definition
 * @brief GPIO Driver Strength Configuration
 * @{
 */
#define GPIO_DC_2MA    ((uint32_t)0x00000000U)
#define GPIO_DC_4MA    ((uint32_t)0x00000002U)
#define GPIO_DC_8MA    ((uint32_t)0x00000001U)
#define GPIO_DC_12MA   ((uint32_t)0x00000003U)
#define GPIO_DC_HIGH   GPIO_DC_12MA
#define GPIO_DC_LOW    GPIO_DC_2MA
/**
  * @}
  */
#define IS_GPIO_CURRENT(CURRENT)  \
        (((CURRENT) == GPIO_DC_HIGH)||((CURRENT) == GPIO_DC_LOW))

/** @addtogroup GPIO_Port_Sources
 * @{
 */

#define GPIOA_PORT_SOURCE ((uint8_t)0x00)
#define GPIOB_PORT_SOURCE ((uint8_t)0x01)
/**
 * @}
 */
/**
 * @}
 */
#define IS_GPIO_REMAP_PORT_SOURCE(PORTSOURCE)                                                                           \
    (((PORTSOURCE) == GPIOA_PORT_SOURCE) || ((PORTSOURCE) == GPIOB_PORT_SOURCE)) 
 
 
#define IS_GPIO_EXTI_PORT_SOURCE(PORTSOURCE)                                                                           \
    (((PORTSOURCE) == GPIOA_PORT_SOURCE) || ((PORTSOURCE) == GPIOB_PORT_SOURCE))

/** @addtogroup GPIO_Pin_sources
 * @{
 */
#define GPIO_PIN_SOURCE0  ((uint8_t)0x00)
#define GPIO_PIN_SOURCE1  ((uint8_t)0x01)
#define GPIO_PIN_SOURCE2  ((uint8_t)0x02)
#define GPIO_PIN_SOURCE3  ((uint8_t)0x03)
#define GPIO_PIN_SOURCE4  ((uint8_t)0x04)
#define GPIO_PIN_SOURCE5  ((uint8_t)0x05)
#define GPIO_PIN_SOURCE6  ((uint8_t)0x06)
#define GPIO_PIN_SOURCE7  ((uint8_t)0x07)
#define GPIO_PIN_SOURCE8  ((uint8_t)0x08)
#define GPIO_PIN_SOURCE9  ((uint8_t)0x09)
#define GPIO_PIN_SOURCE10 ((uint8_t)0x0A)
#define GPIO_PIN_SOURCE11 ((uint8_t)0x0B)
#define GPIO_PIN_SOURCE12 ((uint8_t)0x0C)
#define GPIO_PIN_SOURCE13 ((uint8_t)0x0D)
/**
 * @}
 */
#define IS_GPIO_PIN_SOURCE(PINSOURCE)                                                                                  \
    (((PINSOURCE) == GPIO_PIN_SOURCE0) || ((PINSOURCE) == GPIO_PIN_SOURCE1) || ((PINSOURCE) == GPIO_PIN_SOURCE2)       \
     || ((PINSOURCE) == GPIO_PIN_SOURCE3) || ((PINSOURCE) == GPIO_PIN_SOURCE4) || ((PINSOURCE) == GPIO_PIN_SOURCE5)    \
     || ((PINSOURCE) == GPIO_PIN_SOURCE6) || ((PINSOURCE) == GPIO_PIN_SOURCE7) || ((PINSOURCE) == GPIO_PIN_SOURCE8)    \
     || ((PINSOURCE) == GPIO_PIN_SOURCE9) || ((PINSOURCE) == GPIO_PIN_SOURCE10) || ((PINSOURCE) == GPIO_PIN_SOURCE11)  \
     || ((PINSOURCE) == GPIO_PIN_SOURCE12) || ((PINSOURCE) == GPIO_PIN_SOURCE13))

/** @addtogroup GPIOx_Alternate_function_selection Alternate function selection
 * @{
 */

/*
 * Alternate function AF0
 */
#define GPIO_AF0        ((uint8_t)0x00U)
#define GPIO_AF0_SWD    (GPIO_AF0)  /* SWD Alternate Function mapping */
#define GPIO_AF0_OSC    (GPIO_AF0)  /* OSC Alternate Function mapping */ 

/*
 * Alternate function AF1
 */
#define GPIO_AF1          ((uint8_t)0x01U)
#define GPIO_AF1_TIM1     (GPIO_AF1)  /* TIM1 Alternate Function mapping */
#define GPIO_AF1_SPI1     (GPIO_AF1)  /* SPI1 Alternate Function mapping */
#define GPIO_AF1_SPI2     (GPIO_AF1)  /* SPI2 Alternate Function mapping */


/*
 * Alternate function AF2
 */
#define GPIO_AF2          ((uint8_t)0x02U)
#define GPIO_AF2_TIM3     (GPIO_AF2)  /* TIM3 Alternate Function mapping */
#define GPIO_AF2_LPUART1  (GPIO_AF2)  /* LPUART1 Alternate Function mapping */
#define GPIO_AF2_USART2   (GPIO_AF2)  /* USART2 Alternate Function mapping */
#define GPIO_AF2_SPI2     (GPIO_AF2)  /* SPI2 Alternate Function mapping */
#define GPIO_AF2_I2C      (GPIO_AF2)  /* I2C Alternate Function mapping */  


/*
 * Alternate function AF3
 */
#define GPIO_AF3          ((uint8_t)0x03U)
#define GPIO_AF3_USART1   (GPIO_AF3)  /* USART1 Alternate Function mapping */
#define GPIO_AF3_USART2   (GPIO_AF3)  /* USART2 Alternate Function mapping */
#define GPIO_AF3_I2C      (GPIO_AF3)  /* I2C Alternate Function mapping */ 

/*
 * Alternate function AF4
 */
#define GPIO_AF4          ((uint8_t)0x04U)
#define GPIO_AF4_USART1   (GPIO_AF4)  /* USART1 Alternate Function mapping */ 
#define GPIO_AF4_LPUART1  (GPIO_AF4)  /* LPUART1 Alternate Function mapping */
#define GPIO_AF4_I2C      (GPIO_AF4)  /* I2C Alternate Function mapping */
#define GPIO_AF4_MCO      (GPIO_AF4)  /* RCC MCO Alternate Function mapping */ 

/*
 * Alternate function AF5
 */
#define GPIO_AF5           ((uint8_t)0x05U)
#define GPIO_AF5_KEYSCAN   (GPIO_AF5)  /* KEYSCAN Alternate Function mapping */

/*
 * Alternate function AF6
 */
#define GPIO_AF6          ((uint8_t)0x06U)
#define GPIO_AF6_SWD      (GPIO_AF6)  /* SWD Alternate Function mapping */


#define GPIO_AF15   ((uint8_t)0x0FU)  /* NON Alternate Function mapping */
#define GPIO_NO_AF  (GPIO_AF15)
/**
  * @}
  */


/**
 *  IS_GPIO_AF macro definition
 */
#define IS_GPIO_AF(AF)         ((AF) <= (uint8_t)0x0FU) 
/**
 * @}
 */
/**
 * @}
 */

/** @defgroup GPIO Alternate function remaping
 * @{
 */

#define AFIO_SPI1_NSS    (0x00000800UL)
#define AFIO_SPI2_NSS    (0x00000400UL)
#define IS_AFIO_SPIX(_PARAMETER_) \
            (((_PARAMETER_) == AFIO_SPI1_NSS) || ((_PARAMETER_) == AFIO_SPI2_NSS) )

#define AFIO_SPI_NSS_High_IMPEDANCE (0x0UL)
#define AFIO_SPI_NSS_High_LEVEL     (0x1UL)
#define IS_AFIO_SPI_NSS(_PARAMETER_) \
            (((_PARAMETER_) == AFIO_SPI_NSS_High_IMPEDANCE) ||((_PARAMETER_) == AFIO_SPI_NSS_High_LEVEL))

    
typedef enum
{
    AFIO_ADC_ETRI = 0U,
    AFIO_ADC_ETRR = 1U
}AFIO_ADC_ETRType;

typedef enum
{
    AFIO_ADC_TRIG_EXTI_0 = 0U,
    AFIO_ADC_TRIG_EXTI_1 = 1U,
    AFIO_ADC_TRIG_EXTI_2,
    AFIO_ADC_TRIG_EXTI_3,
    AFIO_ADC_TRIG_EXTI_4,
    AFIO_ADC_TRIG_EXTI_5,
    AFIO_ADC_TRIG_EXTI_6,
    AFIO_ADC_TRIG_EXTI_7,
    AFIO_ADC_TRIG_EXTI_8,
    AFIO_ADC_TRIG_EXTI_9,
    AFIO_ADC_TRIG_EXTI_10,
    AFIO_ADC_TRIG_EXTI_11,
    AFIO_ADC_TRIG_EXTI_12,
    AFIO_ADC_TRIG_EXTI_13,
}AFIO_ADC_Trig_RemapType;

#define IS_AFIO_ADC_ETR(_PARAMETER_) \
            (((_PARAMETER_) == AFIO_ADC_ETRI) ||((_PARAMETER_) == AFIO_ADC_ETRR))

#define IS_AFIO_ADC_ETRI(_PARAMETER_) \
            (((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_0) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_1)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_2) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_3)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_4) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_5)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_6) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_7)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_8) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_9)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_10) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_11)|| \
              ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_12) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_13))

#define IS_AFIO_ADC_ETRR(_PARAMETER_) \
            (((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_0) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_1)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_2) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_3)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_4) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_5)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_6) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_7)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_8) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_9)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_10) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_11)|| \
             ((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_12) ||((_PARAMETER_) == AFIO_ADC_TRIG_EXTI_13))

 /**
 * @}
 */
 
/** @addtogroup GPIO_Exported_Macros
 * @{
 */

/**
 * @}
 */

/** @addtogroup GPIO_Exported_Functions
 * @{
 */

void GPIO_DeInit(GPIO_Module* GPIOx);
void GPIO_DeInitPin(GPIO_Module* GPIOx, uint32_t GPIO_Pin);
void GPIO_AFIOInitDefault(void);
void GPIO_InitPeripheral(GPIO_Module* GPIOx, GPIO_InitType* GPIO_InitStruct);
void GPIO_InitStruct(GPIO_InitType* GPIO_InitStruct);
uint8_t GPIO_ReadInputDataBit(GPIO_Module* GPIOx, uint16_t Pin);
uint16_t GPIO_ReadInputData(GPIO_Module* GPIOx);
uint8_t GPIO_ReadOutputDataBit(GPIO_Module* GPIOx, uint16_t Pin);
uint16_t GPIO_ReadOutputData(GPIO_Module* GPIOx);
void GPIO_SetBits(GPIO_Module* GPIOx, uint16_t Pin);
void GPIO_ResetBits(GPIO_Module* GPIOx, uint16_t Pin);
void GPIO_WriteBit(GPIO_Module* GPIOx, uint16_t Pin, Bit_OperateType BitCmd);
void GPIO_Write(GPIO_Module* GPIOx, uint16_t PortVal);
void GPIO_TogglePin(GPIO_Module* GPIOx, uint16_t Pin);
void GPIO_ConfigPinLock(GPIO_Module* GPIOx, uint16_t Pin);

void GPIO_ConfigEXTILine(uint8_t PortSource, uint8_t PinSource);
void GPIO_ConfigPinRemap(uint8_t PortSource, uint8_t PinSource, uint32_t AlternateFunction);
void AFIO_ConfigSPINSSMode(uint32_t AFIO_SPIx_NSS, uint32_t SpiNssMode);

#ifdef __cplusplus
}
#endif

#endif /* __N32WB03X_GPIO_H__ */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
