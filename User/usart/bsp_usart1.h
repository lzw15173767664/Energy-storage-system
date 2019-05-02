#ifndef __USART1_H
#define	__USART1_H


#include "stm32f10x.h"
#include <stdio.h>

/**************************º¯ÊýÉùÃ÷********************************/
void               USART1_Config                             (void);

void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str);
  void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch );
	void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen );
	 void RS485_SendByte(unsigned char temp);
	 void Send485_length( uint8_t *str,uint32_t strlen );
#endif /* __USART1_H */
