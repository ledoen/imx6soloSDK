#ifndef __BSP_SYSINIT_H
#define __BSP_SYSINIT_H

#include "commen_headers.h"

/* 声明二级irq中断处理函数的指针类型 */
typedef void (*mod_irqhandler_t) (uint32_t intnum, void *param);

/* 声明结构体，包含中断处理函数*/
typedef struct mod_irqhandle{
	mod_irqhandler_t handler;
	void *param;
} mod_irqhandle_t;

/* 系统初始化（主要是中断初始化）函数 */
void system_Init();

/* 初始化二级中断向量表 */
void SystemInitIrqTable();

/* 一级irq中断处理函数 */
void system_irqhandler(uint32_t intnum);

/* 注册二级中断处理函数 */
void SystemInstallIrqHandler(IRQn_Type irq, mod_irqhandler_t handler, void *param);


/**
 *  SoC中CCM_CCGR寄存器地址
 */
#define CCM_CCGR0   *((volatile unsigned int *)0x020c4068)
#define CCM_CCGR1   *((volatile unsigned int *)0x020c406c)
#define CCM_CCGR2   *((volatile unsigned int *)0x020c4070)
#define CCM_CCGR3   *((volatile unsigned int *)0x020c4074)
#define CCM_CCGR4   *((volatile unsigned int *)0x020c4078)
#define CCM_CCGR5   *((volatile unsigned int *)0x020c407c)
#define CCM_CCGR6   *((volatile unsigned int *)0x020c4080)

/* 使能全部时钟 */
void clk_enable(void);

#endif