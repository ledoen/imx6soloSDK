#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "commen_headers.h"

/* 初始化UART 
 * 参数		无
 * 返回值	无
 */
void uart2_init(void);

/* 发送字符 */
static inline void UART2_WriteByte(uint8_t data)
{
	UART2->UTXD = data & 0xFF;
}

/* 发送字符串 */
void UART2_WriteBlocking(const uint8_t *data, size_t length);

/* 发送数字 */
void UART2_WriteNum(const uint32_t data);

#endif