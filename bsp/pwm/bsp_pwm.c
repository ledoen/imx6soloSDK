#include "bsp_pwm.h"

void pwm_init(PWM_Type *base, uint32_t prescaler, uint16_t duty, uint16_t period)
{
	/*设置管脚复用*/
	IOMUXC_SetPinMux(IOMUXC_DISP0_DATA08_PWM1_OUT, 0);
	IOMUXC_SetPinMux(IOMUXC_DISP0_DATA09_PWM2_OUT, 0);
	
	IOMUXC_SetPinConfig(IOMUXC_DISP0_DATA08_PWM1_OUT, 0x10b0);
	IOMUXC_SetPinConfig(IOMUXC_DISP0_DATA09_PWM2_OUT, 0x10b0);

	/*设置寄存器
	*PWMCR寄存器：
	*bit[17:16]		01				时钟源选66MHz的ipg_clk
	*bit[15:4]		prescaler-1		分频系数
	*bit[0]			0				使能
	*/
	base->PWMCR = 0;
	base->PWMCR |= (1<<16) | ((prescaler-1)<<4);
	
	/*设置占空比*/
	pwm_setduty(base, duty);
	
	/*设置周期*/
	base->PWMPR  = (0x0000FFFF & period);
	
	/*使能*/
	base->PWMCR |= 1<<0;
}

void pwm_setduty(PWM_Type *base, uint16_t duty)
{
	if(base->PWMSR & (1<<6))
		base->PWMSR |= (1<<6);
	if(base->PWMSR & (1<<5))
		base->PWMSR |= (1<<5);
	if(base->PWMSR & (1<<4))
		base->PWMSR |= (1<<4);
	
	for(int i=0; i<3; i++)
		base->PWMSAR = (0x0000FFFF & duty);
}