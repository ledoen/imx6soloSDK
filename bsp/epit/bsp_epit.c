#include "bsp_epit.h"
#include "bsp_sysinit.h"
#include "bsp_gpio.h"


void epit1_init(uint32_t prescale, uint32_t settime)
{
	IOMUXC_SetPinMux(IOMUXC_GPIO00_EPIT1_OUT, 0);

	/* EPIT1软重启 */
	EPIT1->CR |= 1 << 16;
	while(EPIT1->CR & (1 << 16));
	
	/* 设置EPIT1 CR寄存器 
	* bit[25:24] 	01 ipg66MHz
	* bit[23:22] 	01 输出管脚为翻转模式
	* bit[15:4 ] 	分频系数
	* bit[3]		1 forget and set 模式
	* bit[2]		1 使能中断
	* bit[0]		0 关闭定时器
	*/
	EPIT1->CR = 0;
	EPIT1->CR |= 1 << 24;
	EPIT1->CR |= 1 << 22;
	EPIT1->CR |= (prescale -1) << 4;
	EPIT1->CR |= 1 << 3;
	EPIT1->CR |= 1 << 2;
	EPIT1->CR |= 1 << 1;

	
	/* 设置EPIT1 LOAD寄存器 */
	EPIT1->LR = settime;
	
	/* 设置EPIT1 CMPR寄存器 */
	EPIT1->CMPR = 0;
	
	/* 使能GIC控制器相应EPIT1中断 */
	GIC_EnableIRQ(EPIT1_IRQn);
		
	/* 注册EPIT1中断服务函数 */
	SystemInstallIrqHandler(EPIT1_IRQn, epit1_irqhadler, NULL);
	
	/* 使能EPIT1 */
	EPIT1->CR |= 1 << 0;
}

void epit1_irqhadler(uint32_t intnum, void *param)
{
	static uint32_t ledstate;
	ledstate = !ledstate;
	gpio502(ledstate);
	/* 清除标志位 */
	EPIT1->SR |= 1 << 0;
}