#include "commen_headers.h"
#include "bsp_sysinit.h"
#include "bsp_gpio.h"
#include "bsp_epit.h"
#include "bsp_uart.h"

uint32_t regdata;

int main(void)
{
	regdata = 0;
	system_Init();
	gpio_init();
	epit1_init(66, 1000000);
	uart2_init();
	
	uint8_t hello[] = "hello world!";
	UART2_WriteBlocking(hello, sizeof(hello));
	UART2_WriteBlocking(hello, sizeof(hello));
	
	UART2_WriteNum(regdata);

	while(1)
	{

	}
}
