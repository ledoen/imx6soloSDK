#ifndef __BSP_GPT_H
#define __BSP_GPT_H
#include "commen_headers.h"

extern uint32_t flightTime;		//用于记录超声波测距时间信号

/*GPT模块实现功能：
*1.初始化GPT1
*2.使用中断功能捕捉并测量超声波返回的时间电平信号
*/

void gpt_init(uint32_t precaler);

void gpt_irqhandler(uint32_t intnum, void *param);

#endif