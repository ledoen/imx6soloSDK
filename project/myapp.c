#include "commen_headers.h"
#include "bsp_sysinit.h"
#include "bsp_gpio.h"
#include "bsp_epit.h"
#include "bsp_uart.h"
#include "bsp_pwm.h"
#include "bsp_gpt.h"

uint32_t regdata;

int main(void)
{
	regdata = 0;
	system_Init();
	gpio_init();
	epit1_init(66, 500000);
	uart2_init();
	pwm_init(PWM1, 66, 2500, 20000);
	pwm_init(PWM2, 66, 500, 20000);
	gpt_init(66);

	
	uint8_t hello[] = "hello world!";
	UART2_WriteBlocking(hello, sizeof(hello));
	UART2_WriteBlocking(hello, sizeof(hello));
	
	UART2_WriteNum(regdata);

	while(1)
	{

	}
}
