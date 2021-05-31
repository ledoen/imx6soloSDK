#ifndef __BSP_PWM_H
#define __BSP_PWM_H

#include "commen_headers.h"
/*pwm实现功能：
*1.pwm1产生触发超声波测距的方波信号，高电平持续20us，周期20ms
*2.pwm2产生控制舵机的方波信号，高电平持续时间1.5ms~2.5ms，周期20ms
*/
void pwm_init(PWM_Type *base, uint32_t prescaler, uint16_t duty, uint16_t period);

void pwm_setduty(PWM_Type *base, uint16_t duty);

#endif