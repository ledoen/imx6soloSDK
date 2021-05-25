#ifndef __BSP_GPIT_H
#define __BSP_GPIT_H

#include "commen_headers.h"

/* EPIT1初始化函数 */
void epit1_init(uint32_t prescale, uint32_t settime);

/*EPIT1中断服务函数 */
void epit1_irqhadler(uint32_t intnum, void *param);

#endif