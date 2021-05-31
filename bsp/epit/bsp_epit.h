#ifndef __BSP_GPIT_H
#define __BSP_GPIT_H

#include "commen_headers.h"

/*EPIT1实现功能：
 *1.驱动led45闪烁
 *2.定期向上位机发送数据
*/

/* EPIT1初始化函数 */
void epit1_init(uint32_t prescale, uint32_t settime);

/*EPIT1中断服务函数 */
void epit1_irqhadler(uint32_t intnum, void *param);

#endif