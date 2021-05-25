#include "bsp_sysinit.h"
#include "bsp_gpio.h"
#include "core_ca7.h"

static mod_irqhandle_t irqTable[160];

void system_Init()
{
	/* 使能全部时钟 */
	clk_enable();
	
	/* 调用core_ca7.h中的GIC_Init函数初始化GIC寄存器 */
	GIC_Init();

	/* 设置中断向量表地址 */	
	__set_VBAR((uint32_t)0x27800000);
	
	/* 初始化二级中断向量表 */
	SystemInitIrqTable();
}

static void defaultIrqHandler(uint32_t intnum, void *param)
{
	while(1)
	{
	
	}
}

void SystemInitIrqTable()
{
	uint32_t i;

	/* First set all handler to default */
	for (i = 0; i < NUMBER_OF_INT_VECTORS; i++) {
		SystemInstallIrqHandler((IRQn_Type)i, defaultIrqHandler, NULL);
	}
}

void system_irqhandler(uint32_t intnum)
{
	/* 验证中断号是否有效 */
	/* 系统有效中断号为0-159 */
	gpio502(1);

	uint32_t intnumber = intnum & 0x3FF;
	if(intnumber >= 160){
		return;
	}
		
	/* 调用中断处理函数*/
	irqTable[intnumber].handler(intnum, irqTable[intnumber].param);
}

void SystemInstallIrqHandler(IRQn_Type irq, mod_irqhandler_t handler, void *param)
{
	irqTable[irq].handler = handler;
	irqTable[irq].param = param;
}

void clk_enable(void)
{
    CCM_CCGR0 = 0xffffffff;
    CCM_CCGR1 = 0xffffffff;
    CCM_CCGR2 = 0xffffffff;
    CCM_CCGR3 = 0xffffffff;
    CCM_CCGR4 = 0xffffffff;
    CCM_CCGR5 = 0xffffffff;
    CCM_CCGR6 = 0xffffffff;
}