#ifndef __RELAY_H
#define __RELAY_H

#include "stm32f10x.h"

#define SC_ON                       GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define SC_OFF                        GPIO_ResetBits(GPIOC,GPIO_Pin_13)
//#define LED_TURN                      GPIOB->ODR ^= 0x02
#define SD1_ON                      GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define SD1_OFF                        GPIO_ResetBits(GPIOB,GPIO_Pin_8)

#define SD2_ON                       GPIO_SetBits(GPIOA,GPIO_Pin_0)
#define SD2_OFF                        GPIO_ResetBits(GPIOA,GPIO_Pin_0)

#define POW12V_ON                       GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define POW12V_OFF                        GPIO_ResetBits(GPIOA,GPIO_Pin_1)
//#define S3_ON                       GPIO_SetBits(GPIOB,GPIO_Pin_15)
//#define S3_OFF                        GPIO_ResetBits(GPIOB,GPIO_Pin_15)
//#define S4_ON                       GPIO_SetBits(GPIOB,GPIO_Pin_13)
//#define S4_OFF                       GPIO_ResetBits(GPIOB,GPIO_Pin_13)

#define LED_R_ON                       GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define LED_R_OFF                        GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define LED_R_TURN                      GPIOB->ODR ^= GPIO_Pin_7

#define LED_G_ON                       GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define LED_G_OFF                        GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define LED_G_TURN                      GPIOB->ODR ^= GPIO_Pin_6

#define LED_B_ON                       GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define LED_B_OFF                        GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define LED_B_TURN                      GPIOB->ODR ^= GPIO_Pin_5

 
//#define LED_Bug_TURN                      GPIOB->ODR ^= GPIO_Pin_12
void Relay_Init(void);

#endif
