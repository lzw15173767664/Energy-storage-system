#include "Relay.h"

void Relay_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitTypeDef  GPIO_B8;
  /* 使能GPIOB时钟 */
	/*??GPIOB?GPIOF?????*/
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); 
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 
 RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE); 

  /* 配置继电器相应引脚 sd1*/
  GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	  /* 配置继电器相应引脚 sd1*/
  GPIO_B8.GPIO_Pin |= GPIO_Pin_12;
  GPIO_B8.GPIO_Mode = GPIO_Mode_IPU;    
 
  GPIO_Init(GPIOB, &GPIO_B8);
	  /* 配置继电器相应引脚 SC*/
  GPIO_InitStructure.GPIO_Pin =    GPIO_Pin_13  ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	  /* 配置继电器相应引脚SD2*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	   
//  /* 配置LED相应引脚P13*/
//  GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_13;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
