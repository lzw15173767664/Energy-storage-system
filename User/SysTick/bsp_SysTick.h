#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_us(__IO u32 nTime);
void delay_ms(u16 time);
void delay_10us(u16 time);
//#define Delay_ms(x) Delay_us(100*x)	 //µ¥Î»ms

#endif /* __SYSTICK_H */
