#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"

// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
/********************ADC1����ͨ�������ţ�����**************************/
#define    macADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    macADC_CLK                       RCC_APB2Periph_ADC1

#define    macADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    macADC_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    macADC_PORT                      GPIOC

#define    macADC_GPIO_CLK_B                  RCC_APB2Periph_GPIOB  
#define    macADC_PORT_B                      GPIOB

// ת��ͨ������
#define    macNOFCHANEL											10

#define    macADC_PINB_1                      GPIO_Pin_0
#define    macADC_CHANNEL1                  ADC_Channel_8

#define    macADC_PINB_2                      GPIO_Pin_1
#define    macADC_CHANNEL2                  ADC_Channel_9

#define    macADC_PIN1                      GPIO_Pin_0
#define    macADC_CHANNEL3                  ADC_Channel_10

#define    macADC_PIN2                      GPIO_Pin_1
#define    macADC_CHANNEL4                  ADC_Channel_11

#define    macADC_PIN3                      GPIO_Pin_2
#define    macADC_CHANNEL5                  ADC_Channel_12

#define    macADC_PIN4                      GPIO_Pin_3
#define    macADC_CHANNEL6                  ADC_Channel_13

#define    macADC_PIN5                      GPIO_Pin_4
#define    macADC_CHANNEL7                  ADC_Channel_14

#define    macADC_PIN6                      GPIO_Pin_5
#define    macADC_CHANNEL8                ADC_Channel_15


// ADC1 ��Ӧ DMA1ͨ��1��ADC3��ӦDMA2ͨ��5��ADC2û��DMA����
#define    macADCx                          ADC1
#define    macADC_DMA_CHANNEL               DMA1_Channel1


/**************************��������********************************/
void               ADCx_Init                               (void);


#endif /* __ADC_H */

