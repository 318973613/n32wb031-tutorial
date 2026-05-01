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
 * @file system_n32wb03x.c
 * @author NSING Firmware Team
 * @version v1.0.4
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#include "n32wb03x.h"
#include "string.h"
/* Uncomment the line corresponding to the desired System clock (SYSCLK)
   frequency (after reset the HSI is used as SYSCLK source)

   IMPORTANT NOTE:
   ==============
   1. After each device reset the HSI is used as System clock source.

   2. Please make sure that the selected System clock doesn't exceed your
   device's maximum frequency.

   3. If none of the define below is enabled, the HSI is used as System clock
    source.

   4. The System clock configuration functions provided within this file assume
   that:
        - HSI is configer as 64M and used to driverd the system clock.
        - External 32MHz crystal use for Bluetooth RF system only.
        - If Bluetooth stack is enable, we should select the LSI or LSE when configer
          the Bluetooth stack only.
*/

#define SYSCLK_USE_HSI     1
#define SYSCLK_USE_HSE     0

#ifndef SYSCLK_FREQ
#define SYSCLK_FREQ HSI_VALUE
#endif

/*
* SYSCLK_SRC *
** SYSCLK_USE_HSI     **
** SYSCLK_USE_HSE     **
*/
#ifndef SYSCLK_SRC
#define SYSCLK_SRC SYSCLK_USE_HSI
#endif

/************ start: add new otp read func  ************/
//Read function
#define TRIM_READ_CMD_CODE_LEN  0x140
#define TRIM_READ_CMD_CODE_CRC  0x3aa0
typedef uint32_t (*trim_read_cmd_func_t)(uint32_t,uint8_t*,uint32_t);
const unsigned char  TRIM_READ_CMD_CODE[] ={
0x40,0xba,0x70,0x47,0xc0,0xba,0x70,0x47,0x01,0x38,0xfd,0xd1,0x70,0x47,0x00,0x00,
0xf7,0xb5,0x03,0x25,0xad,0x06,0x28,0x6a,0x82,0xb0,0x16,0x46,0x0c,0x46,0x40,0x08,
0x40,0x00,0x28,0x62,0x28,0x6a,0x02,0x21,0x08,0x43,0x28,0x62,0x28,0x6a,0x80,0x07,
0xfc,0xd4,0x66,0x20,0x68,0x60,0x01,0x27,0x2f,0x61,0xa8,0x6a,0xc0,0x05,0xfc,0xd5,
0x99,0x20,0x68,0x60,0x2f,0x61,0xa8,0x6a,0xc0,0x05,0xfc,0xd5,0xff,0x20,0x91,0x30,
0xff,0xf7,0xda,0xff,0xff,0x23,0x01,0x33,0xab,0x62,0x68,0x46,0xef,0x60,0x35,0x21,
0x69,0x60,0x2f,0x61,0xa9,0x6a,0xc9,0x05,0xfc,0xd5,0xab,0x62,0xa9,0x69,0xef,0x60,
0xc9,0xb2,0x05,0x22,0x6a,0x60,0x2f,0x61,0xaa,0x6a,0xd2,0x05,0xfc,0xd5,0xab,0x62,
0xaa,0x69,0x09,0x02,0xd2,0xb2,0x11,0x43,0x01,0x80,0xc8,0x07,0x02,0xd0,0x03,0x20,
0x05,0xb0,0xf0,0xbd,0xab,0x62,0x68,0x69,0xff,0x21,0x08,0x31,0x88,0x43,0x68,0x61,
0x68,0x69,0xc9,0x1e,0x08,0x43,0x68,0x61,0x02,0x98,0x00,0x02,0x48,0x30,0x68,0x60,
0x08,0x20,0xa8,0x60,0xee,0x60,0x2f,0x61,0xa8,0x6a,0xc0,0x05,0xfc,0xd5,0xff,0x20,
0x01,0x30,0xa8,0x62,0x00,0x23,0xf6,0x1c,0xb0,0x08,0x0e,0xd0,0xb2,0x08,0x11,0x48,
0x00,0x68,0x99,0x00,0x60,0x54,0x06,0x0a,0x09,0x19,0x4e,0x70,0x06,0x0c,0x00,0x0e,
0x8e,0x70,0x5b,0x1c,0xc8,0x70,0x9a,0x42,0xf1,0xd8,0xff,0x20,0x01,0x30,0xa8,0x62,
0x68,0x69,0xff,0x21,0x08,0x31,0x88,0x43,0x68,0x61,0x68,0x69,0x38,0x43,0x68,0x61,
0x28,0x6a,0x80,0x08,0x80,0x00,0x28,0x62,0x28,0x6a,0x38,0x43,0x28,0x62,0x00,0x20,
0x05,0xb0,0xf0,0xbd,0x80,0x00,0x00,0x0c,0x03,0x20,0x80,0x06,0x41,0x69,0xff,0x22,
0x08,0x32,0x91,0x43,0x41,0x61,0x42,0x69,0x01,0x21,0x0a,0x43,0x42,0x61,0x02,0x6a,
0x92,0x08,0x92,0x00,0x02,0x62,0x02,0x6a,0x0a,0x43,0x02,0x62,0x70,0x47,0x00,0x00,
};

//Read ID function
typedef uint32_t (*rd_id_cmd_func_t)(void);
const unsigned char   CMD_RDID[] ={
  0x00, 0x20, 0x03, 0x21, 0x89, 0x06, 0x09, 0x6A, 0x49, 0x08, 0x49, 0x00, 0x03, 0x22, 0x92, 0x06,
  0x11, 0x62, 0x11, 0x46, 0x09, 0x6A, 0x02, 0x22, 0x11, 0x43, 0x03, 0x22, 0x92, 0x06, 0x11, 0x62,
  0x00, 0xBF, 0x03, 0x21, 0x89, 0x06, 0x09, 0x6A, 0x02, 0x22, 0x11, 0x40, 0x02, 0x29, 0xF8, 0xD0,
  0xD1, 0x01, 0x03, 0x22, 0x92, 0x06, 0x91, 0x62, 0x03, 0x21, 0xD1, 0x60, 0x9F, 0x21, 0x51, 0x60,
  0x01, 0x21, 0x11, 0x61, 0x00, 0xBF, 0x03, 0x21, 0x89, 0x06, 0x89, 0x6A, 0xFF, 0x22, 0x01, 0x32,
  0x11, 0x40, 0x91, 0x42, 0xF7, 0xD1, 0x03, 0x22, 0x92, 0x06, 0x91, 0x62, 0x11, 0x46, 0x88, 0x69,
  0x49, 0x69, 0xFF, 0x22, 0x08, 0x32, 0x91, 0x43, 0x03, 0x22, 0x92, 0x06, 0x51, 0x61, 0x11, 0x46,
  0x49, 0x69, 0x01, 0x22, 0x11, 0x43, 0x03, 0x22, 0x92, 0x06, 0x51, 0x61, 0x11, 0x46, 0x09, 0x6A,
  0x89, 0x08, 0x89, 0x00, 0x11, 0x62, 0x11, 0x46, 0x09, 0x6A, 0x01, 0x22, 0x11, 0x43, 0x03, 0x22,
  0x92, 0x06, 0x11, 0x62, 0x70, 0x47, 0x00, 0x00
};
static uint32_t read_flash_ID(void)
{
	uint32_t ramcode[TRIM_READ_CMD_CODE_LEN/4 +1 ];
	rd_id_cmd_func_t rd_id_cmd_func = NULL;
	uint32_t id = 0;
	uint32_t Status = __get_PRIMASK();	
	memset(ramcode,0,sizeof(ramcode));
	rd_id_cmd_func = (rd_id_cmd_func_t)((uint8_t*)&ramcode[0]+1);
	memcpy((void*)ramcode,(const void*)CMD_RDID,sizeof(CMD_RDID));	
	__set_PRIMASK(1); 
	id = rd_id_cmd_func();
	__set_PRIMASK(Status); 
	return id;
}

/**
 * @brief Read mutable length OTP data to ram.
 * @param[in] address OTP address.
 * @param[out] p_data data to read.
 * @param[in] byte_length data len.
 * @return none.
 */
uint32_t OTPTrim_Read(uint32_t address, uint8_t* p_data, uint32_t byte_length)
{  
		uint32_t otp_addr = 0;
    uint32_t error=FlashAddressInvalid;
		uint32_t ramcode[TRIM_READ_CMD_CODE_LEN/4 +1 ];
		uint32_t id = 0;
		trim_read_cmd_func_t trim_read_cmd_func = NULL;
		uint32_t Status = __get_PRIMASK();
		memset(ramcode,0,sizeof(ramcode));    
		id = read_flash_ID();
		if(id == 0x001340c4)//N32WB031KEQ7-1
		{
			if(address == 0x1000) otp_addr = 0x0;
			else if(address == 0x2000) otp_addr = 0x100;
			else if(address == 0x3000) otp_addr = 0x200;
			else return error; //no valid otp address;
		}else if(id == 0x00136085){//N32WB031KEQ6-1///N32WB031KEQ6-2
			if(address == 0x1000) otp_addr = 0x1000;
			else if(address == 0x2000) otp_addr = 0x2000;
			else if(address == 0x3000) otp_addr = 0x3000;
			else return error; //no valid otp address;

		}else{
			return error; //no valid flash detect; 
		}
		trim_read_cmd_func = (trim_read_cmd_func_t)((uint8_t*)&ramcode[0] + 0x11);
		memcpy((void*)ramcode,(const void*)TRIM_READ_CMD_CODE,TRIM_READ_CMD_CODE_LEN);
		__set_PRIMASK(1); 
		error = (*trim_read_cmd_func)(otp_addr, p_data, byte_length);	   
		assert_param(error == FlashOperationSuccess);		
		__set_PRIMASK(Status); 
    return error;
}
/************ end  ************/

#define TRIM_STORE_ADDR         0x1000 
trim_stored_t trim_stored;

/*******************************************************************************
 *  Clock Definitions
 *******************************************************************************/
uint32_t SystemCoreClock = SYSCLK_FREQ; /*!< System Clock Frequency (Core Clock) */
static void RCC_HsiCalib(uint32_t systemfreq);
bool system_authentication(const uint32_t* info, uint32_t len);
void SystemTrimValueRead(uint8_t* p_data,uint32_t byte_length)
{
		OTPTrim_Read(TRIM_STORE_ADDR, p_data, byte_length);
}

trim_stored_t* SystemTrimValueGet(void)
{
    //read the trim value if not in RAM yet
    if(trim_stored.stote_rc64m_trim_value == 0xFFFFFFFF || trim_stored.stote_rc64m_trim_value == 0)
    {
        SystemTrimValueRead((uint8_t*)&trim_stored,sizeof(trim_stored));
    }
    //check again if read trim value sucessful
    if(trim_stored.stote_rc64m_trim_value == 0xFFFFFFFF || trim_stored.stote_rc64m_trim_value == 0)
    {
        return NULL;
    }else{
        return &trim_stored;
    }
}

uint8_t* SystemGetUUID(void)
{
    //read the trim value if not in RAM yet
    if(trim_stored.stote_rc64m_trim_value == 0xFFFFFFFF || trim_stored.stote_rc64m_trim_value == 0)
    {
        SystemTrimValueRead((uint8_t*)&trim_stored,sizeof(trim_stored));
    }
    //check again if read trim value sucessful
    if(trim_stored.stote_rc64m_trim_value == 0xFFFFFFFF || trim_stored.stote_rc64m_trim_value == 0)
    {
        return NULL;
    }else{
        return trim_stored.flash_uuid;
    }
    
}

uint8_t* SystemGetMacAddr(void)
{
    //read the trim value if not in RAM yet
    if(trim_stored.stote_rc64m_trim_value == 0xFFFFFFFF || trim_stored.stote_rc64m_trim_value == 0)
    {
        SystemTrimValueRead((uint8_t*)&trim_stored,sizeof(trim_stored));
    }
    //check again if read trim value sucessful
    if(trim_stored.stote_rc64m_trim_value == 0xFFFFFFFF || trim_stored.stote_rc64m_trim_value == 0)
    {
        return NULL;
    }else{
        return &trim_stored.flash_uuid[5];
    }
    
}
    

/**
 * @brief  Setup the microcontroller system
 *         Initialize the Embedded Flash Interface, the PLL and update the
 *         SystemCoreClock variable.
 * @note   This function should be used only after reset.
 */
void SystemInit(void)
{
    uint32_t  tmp;
    RCC->APB1PCLKEN |= RCC_APB1_PERIPH_PWR; // PWR enable
    PWR->VTOR_REG = 0x81000000; //set irq vtor to flash address

    *(uint32_t*)0x40007014 = 0x0000080F; 
    *(uint32_t*)0x40007020 = 0x00020018; 
    *(uint32_t*)0x40011000 &= ~0xC000;

    SystemTrimValueRead((uint8_t*)&trim_stored,sizeof(trim_stored));
    /* check otp has been write */
    if(trim_stored.stote_rc64m_trim_value == 0xFFFFFFFF || trim_stored.stote_rc64m_trim_value == 0) 
    {
        RCC->CFG |= RCC_CFG_HSISRC_DIV1; // USE HSI as system clock
        RCC->CFG &= ~RCC_CFG_APB1PRES;
        RCC->CFG |=  RCC_HCLK_DIV2; //APB1 = HCLK/2, APB1 max is 32M
        /* Calib from HSE */
        RCC_HsiCalib(SYSCLK_FREQ);
    }
    else
    {
        tmp = (PWR->reserved4)&(~0X1F);
        tmp |= (trim_stored.stote_bg_vtrim_value)&0X1F;
        PWR->reserved4 = tmp;
		if(SYSCLK_FREQ == 64000000)
		{           
            RCC->CTRL &= ~0x8000;// Set HSI as 64M
            /* Configures LSI trim */
            tmp = RCC->CTRL & ~(0x7F << 8);   // TRIM 8-14 bit
            RCC->CTRL = tmp|(trim_stored.stote_rc64m_trim_value << 8);// clear and set TRIM value 
            
            RCC->CFG |= RCC_CFG_HSISRC_DIV1; // USE HSI as system clock
            
            RCC->CFG &= ~RCC_CFG_APB1PRES;
            RCC->CFG |=  RCC_HCLK_DIV2; //APB1 = HCLK/2, APB1 max is 32M
		}
//        else if(SYSCLK_FREQ == 96000000)
//        {                      
//            RCC->CTRL |= 0x8000; // Set HSI as 96M
//            /* Configures LSI trim */
//            tmp = RCC->CTRL & ~(0x7F << 8);   // TRIM 8-14 bit
//            RCC->CTRL = tmp|(trim_stored.stote_rc96m_trim_value << 8);// clear and set TRIM value 
//            
//            RCC->CFG |= RCC_CFG_HSISRC_DIV1; // USE HSI as system clock
//            tmp = RCC->CFG & ~RCC_CFG_APB1PRES;
//            RCC->CFG |=  RCC_HCLK_DIV4; //APB1 = HCLK/4, APB1 max is 32M
//            
//            tmp = RCC->CFG & ~RCC_CFG_APB2PRES;
//            RCC->CFG |=  RCC_HCLK_DIV4<<3; //APB2 = HCLK/2, APB1 max is 64M
//        }
        /* Configures LSI trim */
        RCC->LSCTRL &= ~RCC_LSCTRL_LSTTRIM;
        RCC->LSCTRL |=  trim_stored.stote_rc32768_trim_value << 8;        
    }
    #if (ENABLE_FIRMWARE_AUTH)
    /* user info just for demo, please change it in your project. */
    const uint32_t user_info[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
                                    0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    if(system_authentication(user_info, 16) == false)
    {
        //illegal firmware, stay here 
        while(1)
        {
            RCC_HsiCalib(SYSCLK_FREQ);
        }
    }
    #endif
 
}

/**
 * @brief  Update SystemCoreClock variable according to Clock Register Values.
 *         The SystemCoreClock variable contains the core clock (HCLK), it can
 *         be used by the user application to setup the SysTick timer or
 * configure other parameters.
 *
 * @note   Each time the core clock (HCLK) changes, this function must be called
 *         to update SystemCoreClock variable value. Otherwise, any
 * configuration based on this variable will be incorrect.
 *
 * @note   - The system frequency computed by this function is not the real
 *           frequency in the chip. It is calculated based on the predefined
 *           constant and the selected clock source:
 */
void SystemCoreClockUpdate(void)
{
    SystemCoreClock = HSI_VALUE;
}

/**
 * @brief dealy cycles.
 */
__ASM  void system_delay_cycles(uint32_t i)
{
    SUBS r0, #1
    BNE system_delay_cycles
    BX LR
}

/**
 * @brief  Dealy 10 us
 */
void system_delay_n_10us(uint32_t value)
{
    system_delay_cycles(107*value);
}

/**
 * @brief  Enable the HSI and calibration it.
 */
static void RCC_HsiCalib(uint32_t systemfreq) 
{
    uint32_t g_hsi_accuracy = 0;
    uint32_t g_timeoutcnt = 1000;
    uint32_t g_cal_hsi_cnt_value = 1024;

    uint32_t delta =0;
    uint32_t min = 0;
    uint32_t max = 127;                                 //32M TRIM 8-14 bit
    uint32_t mid = 64;
    uint16_t count_value = 0;
    uint32_t hsi_timeoutcnt = 0; 
    uint8_t tmp_trim;
    uint8_t min_delta_trim =0;
    uint16_t min_delta = 1024;
    uint32_t tmp =0;
    
    if(systemfreq<=64000000)
    {
        RCC->CTRL &= ~0x8000;
    }    
    else
    {
        RCC->CTRL |= 0x8000;
    }
    
    g_cal_hsi_cnt_value = systemfreq /1000/1000*32;
   
    
    do{
        tmp = RCC->CTRL & ~(0x7F << 8);   //32M TRIM 8-14 bit
        RCC->CTRL  =  tmp| (mid << 8);              // clear and set TRIM value  //and start to cnt
        system_delay_cycles(5);                                // delay scape          
        while(1)
        {
            system_delay_cycles(1);        
            if((RCC->OSCFCSR & 0x02))
            {
                break;
            }    
            if(hsi_timeoutcnt++ > g_timeoutcnt)
            {    
                break;
            }
        }
        count_value = RCC->OSCFCHSICNT;                 //ready cnt value       
		if(count_value > g_cal_hsi_cnt_value)
		{
			delta = count_value - g_cal_hsi_cnt_value; 
		}
		else
		{
			delta = g_cal_hsi_cnt_value - count_value; 
		}
        
        if(delta <  min_delta)
        {
            min_delta_trim = mid;
            min_delta = delta;
        }    
        
        if(count_value >=  g_cal_hsi_cnt_value)
        {
            max = mid;
        }
        else
        {
            min = mid;
        }
        
        tmp_trim = (min + max)/2;  
        if(tmp_trim == mid )                //0 and 127 if not used        
        {
            break;
        }
        mid = tmp_trim;
    }while(delta > g_hsi_accuracy); 
    
    tmp = RCC->CTRL & ~(0x7F << 8);   //32M TRIM 8-14 bit
    RCC->CTRL  =  tmp| (min_delta_trim << 8);              // clear and set TRIM value  //and start to cnt
    
    RCC->CFG &= ~1;
    while((RCC->CFG & (1<<2)));
}

#if (ENABLE_FIRMWARE_AUTH)
/**
 * @brief firmware authentication function. It will wite the authentication data 
 *        in flash when first power on, and then authenticate firmware each power on. 
 *        User can put it in other logic code to authenticate the firmware.
 * @param[in] info: data pointer of manufacture string.
 * @param[in] len : length of manufacture string . 
 * @return result of encrypt or verify, true for pass, false for fail.
 */
bool system_authentication(const uint32_t* info, uint32_t len)  
{
    /* AUTH_ADDRESS just for demo, production should seltect another address and
      write the Tag data on production line. */
    #ifdef BLE_OTA_ENABLE 
    #define AUTH_ADDRESS (0x01001FF0)
    #else
    const static uint32_t AUTH_ADDRESS[1] = {0xffffffff};
    #endif
    uint32_t index = 0;
    uint32_t crc_result = 0;
    volatile uint32_t crc_read = 0;
    RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_CRC, ENABLE);
    CRC32_ResetCrc();
    for (index = 0; index < len; index++)
    {
        CRC->CRC32DAT = info[index];
    }    
    /* get trim value */
    trim_stored_t *p_trim = SystemTrimValueGet();
    if(p_trim != 0)
    {
        for (index = 0; index < 16; index++)
        {
            CRC->CRC32DAT = p_trim->flash_uuid[index];
        }
    }

    crc_result = CRC->CRC32DAT;
    Qflash_Init();
    Qflash_Read((uint32_t)AUTH_ADDRESS,(uint8_t*)&crc_read,4);
    if(crc_read == 0xffffffff)
    {
        /* We suggest write the Tag data when flashing firmware or produciton testing. */
        // Wirte the Tag when  first run
        Qflash_Write((uint32_t)AUTH_ADDRESS,(uint8_t*)&crc_result,4);
        Qflash_Read( (uint32_t)AUTH_ADDRESS,(uint8_t*)&crc_read,4);
    }
    
    if(crc_read != crc_result){
        return false;
    }
    return true;
}
#endif
/**
 * @}
 */



