#include "bsp_gpt.h"
#include "bsp_sysinit.h"

uint32_t flightTime;

void gpt_init(uint32_t precaler)
{
	flightTime = 0;		//初始化flightTime
	
	/*设置管脚复用寄存器，将SD1_DATA1设置为功能ALT3
	*即为GPT_Capture2
	*/
	IOMUXC_SetPinMux(IOMUXC_SD1_DATA1_GPT_CAPTURE2, 0);
	IOMUXC_SetPinConfig(IOMUXC_SD1_DATA1_GPT_CAPTURE2, 0x10b0);
	
	/*设置GPT寄存器*/
	/*CR寄存器
	*bit[19:18]		11		Capture2双边沿触发
	*bit[9]			1		freerun mode
	*bit[8:6}		01		选择66MHz的ipg_clk
	*bit[1]			1		enmod
	*bit[0]			0		使能
	*/
	GPT->CR = 0;
	GPT->CR = (3<<18) | (1<<9) | (1<<6) | (1<<1);
	
	/*设置分频系数*/
	GPT->PR = precaler-1;
	
	/*设置IR寄存器，使能Capture2中断*/
	GPT->IR = 1<<4;
	
	/*注册中断服务函数*/
	SystemInstallIrqHandler(GPT_IRQn, gpt_irqhandler, NULL);
	
	/*使能GIC对应中断*/
	GIC_EnableIRQ(GPT_IRQn);
	
	/*使能GPT*/
	GPT->CR |= 1<<0;
}

void gpt_irqhandler(uint32_t intnum, void *param)
{
	/*定义变量，记录时间
	*startTime		static局部变量
	*time			普通局部变量
	*flightTime 	全局变量，在头文件声明，在此文件定义，在init中初始化
	*/
	static uint32_t startTime = 0;
	uint32_t time = 0;
	
	/*定义一个状态量，标记上升沿和下降沿*/
	static uint32_t edgeState = 0;
	edgeState = !edgeState;
	
	/*记录中断发生时的时间*/
	time = GPT->ICR[1];
	
	/*判断上升沿下降沿*/
	if(edgeState == 1){		//上升沿
		startTime = time;
	}
	else{					//下降沿
		flightTime = time - startTime;
	}
	
	/*判断flightTime的有效性*/
	if(flightTime > 15000)
		flightTime = 20000 - flightTime;
	
	/*清除中断标志位*/
	GPT->SR |= 1<<4;
}