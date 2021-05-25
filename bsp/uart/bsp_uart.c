#include "bsp_uart.h"

void uart2_init(void)
{
	/* 设置管脚 */
	IOMUXC_SetPinMux(IOMUXC_EIM_D26_DATA_UART2_TX, 0);
	IOMUXC_SetPinMux(IOMUXC_EIM_D27_DATA_UART2_RX, 0);
	
	/* 设置UCR1寄存器，关闭UART2 */
	UART2->UCR1 = 0;
	
	/* 重启UART2 */
	UART2->UCR2 = 0;
	while(!(UART2->UCR2 & 1<<0));
	
	/* 设置UCR2寄存器
	 * bit[14] 	1 	不使用RTS管脚
	 * bit[13] 	1	不使用CTS管脚
	 * bit[8] 	0	不使用奇偶校验
	 * bit[6]	0	1位停止位
	 * bit[5]	1	wordsize8位
	 * bit[2:1]	11	使能收发
	 * bit[0]	1	不重启
	*/
	//UART2->UCR2 = 0x6027;
	UART2->UCR2 = 1<<14 | 1<<13 | 1<<5 | 1<<2 | 1<<1 | 1<<0;
	
	/* 设置UCR3寄存器，芯片手册要求第二位置1 */
	UART2->UCR3 = 1<<2;
	
	/* 设置波特率 */
	/* 设置分频系数
	 * UFCR[9:7] 1 分频系数为5
	 * 默认时钟为80MHz
	 * 分频后为16MHz
	*/
	UART2->UFCR = 1<<7;
	/* 设置UBIR寄存器及UBMIR寄存器
	 * 使波特率为115200
	*/
	UART2->UBIR = 0x47;
	UART2->UBMR = 0x270;

	/* 设置UCR4寄存器，使能RDR中断*/
	UART2->UCR4 = 1;
	
	/* 设置UCR1寄存器，使能UART2*/
	UART2->UCR1 = 1 << 0;
	
	UART2_WriteByte('\r');
	UART2_WriteByte('\n');
}


void UART2_WriteBlocking(const uint8_t *data, size_t length)
{
    while (length--)
    {
        /* Wait for TX fifo valid.
         * This API can only ensure that the data is written into the data buffer but can't
         * ensure all data in the data buffer are sent into the transmit shift buffer.
         */
        while (!(UART2->USR2 & (1<<3)))
        {
        }
        UART2_WriteByte(*(data++));
    }
	UART2_WriteByte('\r');
	UART2_WriteByte('\n');
}

void UART2_WriteNum(const uint32_t data)
{
	/* 从高到低取出每四位进行处理，共进行8次 */
	uint8_t ByteData;
	for(int i=0; i<8; i++){
		ByteData = ((data >> (28-i*4)) & 0xf);	/* 取出每4位 */
		switch(ByteData){
			case 0x0:{
				UART2_WriteByte('0');
				break;
			}
			case 0x1:{
				UART2_WriteByte('1');
				break;
			}
			case 0x2:{
				UART2_WriteByte('2');
				break;
			}
			case 0x3:{
				UART2_WriteByte('3');
				break;
			}
			case 0x4:{
				UART2_WriteByte('4');
				break;
			}
			case 0x5:{
				UART2_WriteByte('5');
				break;
			}
			case 0x6:{
				UART2_WriteByte('6');
				break;
			}
			case 0x7:{
				UART2_WriteByte('7');
				break;
			}
			case 0x8:{
				UART2_WriteByte('8');
				break;
			}case 0x9:{
				UART2_WriteByte('9');
				break;
			}
			case 0xa:{
				UART2_WriteByte('A');
				break;
			}
			case 0xb:{
				UART2_WriteByte('B');
				break;
			}
			case 0xc:{
				UART2_WriteByte('C');
				break;
			}
			case 0xd:{
				UART2_WriteByte('D');
				break;
			}
			case 0xe:{
				UART2_WriteByte('E');
				break;
			}
			case 0xf:{
				UART2_WriteByte('F');
				break;
			}
		}
	}
	UART2_WriteByte('\r');
	UART2_WriteByte('\n');
}