#include "Relay.h"

void Relay_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitTypeDef  GPIO_B8;
  /* ʹ��GPIOBʱ�� */
	/*??GPIOB?GPIOF?????*/
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); 
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 
 RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE); 

  /* ���ü̵�����Ӧ���� sd1*/
  GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	  /* ���ü̵�����Ӧ���� sd1*/
  GPIO_B8.GPIO_Pin |= GPIO_Pin_12;
  GPIO_B8.GPIO_Mode = GPIO_Mode_IPU;    
 
  GPIO_Init(GPIOB, &GPIO_B8);
	  /* ���ü̵�����Ӧ���� SC*/
  GPIO_InitStructure.GPIO_Pin =    GPIO_Pin_13  ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	  /* ���ü̵�����Ӧ����SD2*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	   
//  /* ����LED��Ӧ����P13*/
//  GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_13;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
