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
 * @file software_i2c.h
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#ifndef __SOFTWARE_I2C_H__
#define __SOFTWARE_I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "n32wb03x.h"


 
 
//??IIC??????
typedef struct 
{
    GPIO_Module    *SDA_GPIOx;       //SDA port
    GPIO_Module    *SCL_GPIOx;       //SCL port
    uint32_t        SDA_PINx;        //SDA pin
    uint32_t        SCL_PINx;        //SCL pin
    uint32_t        DelayUS;         //delay time in US
    //port function
    void (*Start)(void *pHandle);                //SI2C start signal
    void (*Stop)(void *pHandle);                //SI2C stop signal
    bool (*SendByte)(void *pHandle, uint8_t data);    //SI2C send a byte
    uint8_t (*ReadByte)(void *pHandle,bool isAck);    //SI2C read a byte
}SI2C_HANDLE;
 
 
bool SI2C_Init(SI2C_HANDLE *pHandle, GPIO_Module *SDA_GPIOx, GPIO_Module *SCL_GPIOx,\
                uint32_t SDA_Pin, uint32_t SCL_Pin,uint8_t DelayUS);    //??IIC???
 
void SI2C_Start(SI2C_HANDLE *pHandle);                //SI2C start signal
void SI2C_Stop(SI2C_HANDLE *pHandle);                //SI2C stop signal
bool SI2C_WaitAck(SI2C_HANDLE *pHandle);            //SI2C wait ack signal
void SI2C_Ack(SI2C_HANDLE *pHandle);                //SI2C send ack signal
void SI2C_NAck(SI2C_HANDLE *pHandle);                //SI2C send nack signal
bool SI2C_SendByte(SI2C_HANDLE *pHandle, uint8_t data);    //SI2C send a byte
uint8_t SI2C_ReadByte(SI2C_HANDLE *pHandle,bool isAck);    //SI2C read a byte
bool SI2C_ReadReg(SI2C_HANDLE *pHandle, uint8_t SlaveAddr, uint16_t RegAddr, \
                  bool is8bitRegAddr, uint8_t *pDataBuff, uint16_t ReadByteNum); //SI2C read reg
bool SI2C_WriteReg(SI2C_HANDLE *pHandle, uint8_t SlaveAddr, uint16_t RegAddr, \
                   bool is8bitRegAddr, uint8_t *pDataBuff, uint16_t WriteByteNum); //SI2C write reg

bool SI2C_MasterWrite(SI2C_HANDLE *pHandle, uint8_t SlaveAddr, uint8_t *pDataBuff, uint16_t WriteByteNum);
bool SI2C_MasterRead(SI2C_HANDLE *pHandle, uint8_t SlaveAddr, uint8_t *pDataBuff, uint16_t ReadByteNum);
#ifdef __cplusplus
}
#endif

#endif /* __SOFTWARE_I2C_H__ */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
