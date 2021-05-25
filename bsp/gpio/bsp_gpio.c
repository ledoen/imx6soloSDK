#include "bsp_gpio.h"
#include "bsp_sysinit.h"

void gpio_init(void)
{
	/*设置GPIO4_IO16*/
	/*设置管脚复用*/
	IOMUXC_SetPinMux(IOMUXC_DI0_DISP_CLK_GPIO4_IO16, 0);
	
	/*设置管脚属性*/
	IOMUXC_SetPinConfig(IOMUXC_DI0_DISP_CLK_GPIO4_IO16, 0x10b0);
	
	/*设置方向寄存器*/
	GPIO4->GDIR |= (1<<16);	/*1为输出*/
	
	/*设置数据寄存器*/
	GPIO4->DR &= ~(1<<16);
	
	/*设置ICR寄存器，上升沿触发*/
	GPIO4->ICR2 |= 1<<1;
	
	/*设置IMR寄存器，使能中断*/
	GPIO4->IMR |= 1<<16;
	
	/*使能GPIO4中断*/
	GIC_EnableIRQ(GPIO4_Combined_16_31_IRQn);

	
	
	/*设置GPIO5_IO02*/
	/*设置管脚复用*/
	IOMUXC_SetPinMux(IOMUXC_EIM_ADDR25_GPIO5_IO02, 0);
	
	/*设置管脚属性*/
	IOMUXC_SetPinConfig(IOMUXC_EIM_ADDR25_GPIO5_IO02, 0x10b0);
	
	/*设置方向寄存器*/
	GPIO5->GDIR |= 1<<2;	/*1为输出*/
	
	/*设置数据寄存器*/
	GPIO5->DR &= ~(1<<2);
}

void gpio502(uint32_t ledswitch)
{
	if(ledswitch == 0)
		GPIO5->DR &= ~(1<<2);
	else
		GPIO5->DR |= (1<<2);	
}