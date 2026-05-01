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
 * @file n32wb03x_irgen.h
 * @author NSING Firmware Team
 * @version v1.0.0
 *
 * @copyright Copyright (c) 2025, NSING Technologies Inc. All rights reserved.
 */
#ifndef __N32WB03x_IRGEN_H__
#define __N32WB03x_IRGEN_H__

#include <stdbool.h>
#include <stdint.h>
#include <n32wb03x.h>

/**
 * \brief IR logic bit format
 *
 */
typedef enum {
    HW_IRGEN_LOGIC_FORMAT_MARK = 0, /**< logic bit starts with mark followed by space */
    HW_IRGEN_LOGIC_FORMAT_SPACE = 1 /**< logic bit starts with space followed by mark */
} HW_IRGEN_LOGIC_FORMAT;

/**
 * \brief IR FIFO
 *
 */
typedef enum {
    HW_IRGEN_FIFO_CODE = 0,         /**< code FIFO */
    HW_IRGEN_FIFO_REPEAT = 1        /**< repeat FIFO */
} HW_IRGEN_FIFO;

/**
 * \brief Paint symbol
 *
 */
typedef enum {
    HW_IRGEN_PAINT_SPACE = 0,       /**< space */
    HW_IRGEN_PAINT_MARK = 1         /**< mark */
} HW_IRGEN_PAINT;

/**
 * \brief IR generator output mode
 *
 */
typedef enum {
    HW_IRGEN_OUTPUT_NORMAL = 0,     /**< output is not inverted */
    HW_IRGEN_OUTPUT_INVERTED = 1    /**< output is inverted */
} HW_IRGEN_OUTPUT;

/**
 * \brief Set carrier frequency
 *
 * \param [in] hi_time   carrier signal high duration in IR clock cycles (must be >0)
 * \param [in] lo_time   carrier signal low duration in IR clock cycles (must be >0)
 *
 */
static __INLINE void hw_irgen_set_carrier_freq(uint16_t hi_time, uint16_t lo_time)
{
    IRC->FREQ_CARRIER_ON = hi_time;
    IRC->FREQ_CARRIER_OFF = lo_time;
}

/**
 * \brief Set "logic 0" parameters
 *
 * \param [in] format       "logic 0" format
 * \param [in] mark_time    mark duration in carrier clock cycles (must be >0)
 * \param [in] space_time   space duration in carrier clock cycles (must be >0)
 *
 * \sa hw_ir_insert_digital_message
 */
static __INLINE void hw_irgen_set_logic0_param(HW_IRGEN_LOGIC_FORMAT format, uint8_t mark_time,
                                                                                uint8_t space_time)
{
    IRC->LOGIC_ZERO_TIME = (uint16_t)(mark_time << IR_LOGIC_ZERO_TIME_LOGIC_ZERO_MARK_Pos | space_time);
    IRC->CTRL = ((IRC->CTRL & ~(IR_CTRL_LOGIC_ZERO_FORMAT_Msk)) | \
        ((IR_CTRL_LOGIC_ZERO_FORMAT_Msk) & ((format) << (IR_CTRL_LOGIC_ZERO_FORMAT_Pos))));
}

/**
 * \brief Set "logic 1" parameters
 *
 * \param [in] format       "logic 1" format
 * \param [in] mark_time    mark duration in carrier clock cycles (must be >0)
 * \param [in] space_time   space duration in carrier clock cycles (must be >0)
 *
 * \sa hw_ir_insert_digital_message
 */
static __INLINE void hw_irgen_set_logic1_param(HW_IRGEN_LOGIC_FORMAT format, uint8_t mark_time,
                                                                                uint8_t space_time)
{
    IRC->LOGIC_ONE_TIME = (uint16_t)(mark_time << IR_LOGIC_ONE_TIME_LOGIC_ONE_MARK_Pos | space_time);
    IRC->CTRL = ((IRC->CTRL & ~(IR_CTRL_LOGIC_ONE_FORMAT_Msk)) | \
        ((IR_CTRL_LOGIC_ONE_FORMAT_Msk) & ((format) << (IR_CTRL_LOGIC_ONE_FORMAT_Pos))));
}

/**
 * \brief Set command repeat time
 *
 * \param [in] time repeat time in carrier clock cycles
 *
 * \sa hw_irgen_set_repeat_type
 *
 */
static __INLINE void hw_irgen_set_repeat_time(uint16_t time)
{
    IRC->REPEAT_TIME = time;
}

/**
 * \brief Insert Digital message into FIFO
 *
 * Digital message is represented by \p payload as sequence of "logic 0" and "logic 1".
 *
 * \param [in] fifo      FIFO to be used
 * \param [in] length    number of valid bits in \p message (must be >0)
 * \param [in] payload   message content
 *
 * \sa hw_irgen_set_logic0_format
 * \sa hw_irgen_set_logic1_format
 *
 */
static __INLINE void hw_irgen_insert_digital_message(HW_IRGEN_FIFO fifo, uint8_t length, uint16_t payload)
{
    /* reg[20] = digital message (1), reg[19:16] = length-1, reg[15:0] = payload */
    uint32_t value = 0x100000 | (((length - 1) & 0xF) << 16) | (payload & 0xFFFF);
    if (fifo == HW_IRGEN_FIFO_REPEAT) {
        IRC->REPEAT_FIFO = value;
    } else {
        IRC->CODE_FIFO = value;
    }
}

/**
 * \brief Insert Paint message into FIFO
 *
 * Paint message is represented by either mark or space and its duration. This allows to represent
 * any custom "painted" waveform.
 *
 * \param [in] fifo       FIFO to be used
 * \param [in] symbol     symbol type
 * \param [in] duration   symbol duration in carrier clock cycles
 *
 */
static __INLINE void hw_irgen_insert_paint_message(HW_IRGEN_FIFO fifo, HW_IRGEN_PAINT symbol, uint16_t duration)
{
    /* reg[20] = paint message (0), reg[14] = symbol, reg[13:0] = duration */
    uint16_t value = (!!symbol << 14) | (duration & 0x3FFF);
    if (fifo == HW_IRGEN_FIFO_REPEAT) {
        IRC->REPEAT_FIFO = value;
    } else {
        IRC->CODE_FIFO = value;
    }
}

/**
 * \brief Flush FIFO
 *
 * Paint message is represented by either mark or space and its duration. This allows to represent
 * any custom "painted" waveform.
 *
 * \param [in] fifo   FIFO to be flushed
 *
 */
static __INLINE void hw_irgen_flush_fifo(HW_IRGEN_FIFO fifo)
{
    if (fifo == HW_IRGEN_FIFO_REPEAT) { 
        IRC->CTRL = ((IRC->CTRL & ~(IR_CTRL_REP_FIFO_RESET_Msk)) | \
            ((IR_CTRL_REP_FIFO_RESET_Msk) & ((1) << (IR_CTRL_REP_FIFO_RESET_Pos))));
    } else {
        IRC->CTRL = ((IRC->CTRL & ~(IR_CTRL_CODE_FIFO_RESET_Msk)) | \
            ((IR_CTRL_CODE_FIFO_RESET_Msk) & ((1) << (IR_CTRL_CODE_FIFO_RESET_Pos))));
    }
}

/**
 * \brief Set output type (normal or inverted)
 *
 * \param [in] output   output type
 *
 */
static __INLINE void hw_irgen_set_output_type(HW_IRGEN_OUTPUT output)
{
	IRC->CTRL = ((IRC->CTRL & ~(IR_CTRL_INVERT_OUTPUT_Msk)) | \
                ((IR_CTRL_INVERT_OUTPUT_Msk) & ((output) << (IR_CTRL_INVERT_OUTPUT_Pos))));
}

/**
 * \brief Start IR data transmission
 *
 */
static __INLINE void hw_irgen_start(void)
{
	IRC->CTRL = ((IRC->CTRL & ~(IR_CTRL_TX_START_Msk)) | \
                ((IR_CTRL_TX_START_Msk) & ((1) << (IR_CTRL_TX_START_Pos))));
	
}

/**
 * \brief Stop IR data transmission
 *
 * When called after transmission is started, this will also flush code FIFO.
 *
 */
static __INLINE void hw_irgen_stop(void)
{
	IRC->CTRL = ((IRC->CTRL & ~(IR_CTRL_TX_START_Msk)) | \
                ((IR_CTRL_TX_START_Msk) & ((0) << (IR_CTRL_TX_START_Pos))));
}

/**
 * \brief Check if IR generator is busy
 *
 * \return true if IR generator is busy, false otherwise
 *
 */
static __INLINE bool hw_irgen_is_busy(void)
{
	return ((IRC->STATUS) & (IR_STATUS_BUSY));
}

/**
 * \brief Get number of messages in FIFO
 *
 * \param [in] fifo   FIFO
 *
 * \return number of messages
 *
 */
static __INLINE uint16_t hw_irgen_get_fifo_level(HW_IRGEN_FIFO fifo)
{
    if (fifo == HW_IRGEN_FIFO_REPEAT) {
        return (((IRC->STATUS) & (IR_STATUS_REP_FIFO_WORDS_Msk)) >> (IR_STATUS_REP_FIFO_WORDS_Pos));
    } else {
        return (((IRC->STATUS) & (IR_STATUS_CODE_FIFO_WORDS_Msk)) >> (IR_STATUS_CODE_FIFO_WORDS_Pos));
    }
}

/**
 * \brief Reverse bit order
 *
 * Helper function to reverse order of bits in message.
 *
 * \param [in] val    message value
 * \param [in] bits   number of valid bits in \p val
 *
 * \return \p val with bit order reversed
 *
 */
static __INLINE uint16_t hw_irgen_reverse_bit_order(uint16_t val, uint8_t bits)
{
    uint16_t ret = 0;

    while (bits--) {
        ret = (ret << 1) | (val & 1);
        val >>= 1;
    }

    return ret;
}


#endif /* __N32WB03x_IRGEN_H__ */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

