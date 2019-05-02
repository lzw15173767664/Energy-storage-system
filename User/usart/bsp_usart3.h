#ifndef __USART3_H
#define	__USART3_H


#include "stm32f10x.h"
#include <stdio.h>


 

 


/**************************º¯ÊýÉùÃ÷********************************/
void               USART3_Config                             (void);
void GET_MasterControl_info( void) ;
int16_t get_AD (u8 at);
int16_t get_battery_current (void);
int get_powe (int current,int Value) ;
#endif /* __USART1_H */
