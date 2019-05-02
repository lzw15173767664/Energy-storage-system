/**
  ******************************************************************************
  * @file    bsp_usart2.c--MCU_IVT1
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   重现c库printf函数到usart端口
 
  *
  ******************************************************************************
  */ 
  
#include "bsp_usart2.h"
#include "bsp_usart1.h"
#include "bsp_SysTick.h"
/********************USART参数定义**************************/
//#define             macUSART_BAUD_RATE                       115200

//#define             macUSARTx                                USART2
//#define             macUSART_APBxClock_FUN                   RCC_APB1PeriphClockCmd
//#define             macUSART_CLK                             RCC_APB1Periph_USART2
//#define             macUSART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
//#define             macUSART_GPIO_CLK                        (RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO)     
//#define             macUSART_TX_PORT                         GPIOA   
//#define             macUSART_TX_PIN                          GPIO_Pin_2
//#define             macUSART_RX_PORT                         GPIOA 
//#define             macUSART_RX_PIN                          GPIO_Pin_3
//#define             macUSART_IRQ                             USART2_IRQn
//#define             macUSART_INT_FUN                         USART1_IRQHandler
void IVT1_DataReceived(u16 data) ;
u8 invt1_Buffer[100]; /* Input buffer for SCI communication */
u8 rdcount=0;
 /**
  * @brief  USART1 GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
	 /****************************************************************************
* 函数名: ConfigUartNVIC
* 功  能: 配置硬件中断.
* 输  入: 无
* 输  出: 无
* 返  回: 无
*/
static void ConfigUart2NVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
 /**
  * @brief  USART1 GPIO 配置,工作模式配置  MCU_IVT1
  * @param  无
  * @retval 无
  */
void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	/* config USART1 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO), ENABLE);
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 4800;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	//USART_Cmd(USART2, ENABLE);
		/*	配置中断优先级 */
	ConfigUart2NVIC();
		/* 使能串口2接收中断 */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);
	//USART_ClearFlag(USART2, USART_FLAG_TC);
}
 void USART2_IRQHandler(void)
{
	  u8 length=0;
		USART_ClearFlag(USART2,USART_FLAG_TC);

		if(USART_GetITStatus(USART2,USART_IT_RXNE)!=Bit_RESET)//?????USART??????
		{
			//u8	k=USART_ReceiveData(USART2);
		//	IVT1_DataReceived(  USART_ReceiveData(USART2));
				invt1_Buffer[rdcount++]= USART_ReceiveData(USART2);
				if ((invt1_Buffer[rdcount]==0x00)&&(invt1_Buffer[rdcount-1]==0x7E))
				{
					invt1_Buffer[0]=0x7E;
					invt1_Buffer[1]=0x00;
					rdcount=1;
				}
					if (rdcount>3)
				{
					length = invt1_Buffer[2]+4;
					if(rdcount == length)
					{
							invt1_Buffer[length]=0xa5;
						 rdcount = 0;
					}
				}
					if (rdcount>99)
				{
					rdcount = 0;
				}
			//Usart_SendByte(USART3,USART_ReceiveData(USART2));
			//	USART_SendData(UART4,USART_ReceiveData(USART2));
			//up_state_machine(USART_ReceiveData(USART2));
		}
		
}

//static u8 InpBuffer[200]; /* Input buffer for SCI communication */
void IVT1_DataReceived(u16 data) 
{
 
	static u8 length=0,datalen=0;
	static u8 percent=0,IVT1_Receive_OK_Flag=0;
	static u16 remote_check1=0, remote_check2=0;  
	u16 Snd=0;
	invt1_Buffer[rdcount]=data;//&0x00FF;
	if(rdcount>4)
	{
	}
	if ((invt1_Buffer[rdcount-2]==0x00)&&(invt1_Buffer[rdcount-3]==0x7E)&&(invt1_Buffer[rdcount]==0x10))
	{
		invt1_Buffer[0]=0x7e;
		invt1_Buffer[1]=0x00;
		invt1_Buffer[2]=invt1_Buffer[rdcount-1];
		invt1_Buffer[3]=0x10;
		rdcount=3;
	}
	remote_check1 += invt1_Buffer[rdcount];
	rdcount++;
	if (rdcount>3)
	{
		length = invt1_Buffer[2]+4;
	}
 
	if((rdcount == length)&&(rdcount>0))
	{	 
			rdcount = 0;
			//remote_check1 = remote_check1 - (UInt16)invt1_Buffer[length-1]-(UInt16)invt1_Buffer[0]-(UInt16)invt1_Buffer[1];
			remote_check1 = remote_check1 - (u16)invt1_Buffer[length-1]-(u16)invt1_Buffer[0]-(u16)invt1_Buffer[1]-(u16)invt1_Buffer[2];
			remote_check1 = (0xFFFF-remote_check1) & 0x00FF;
			remote_check2 = (u16)invt1_Buffer[length-1];

			if((remote_check1 == remote_check2)&&(invt1_Buffer[3]==0x10))
			{
				remote_check2=0;
			//	IVT1_Receive_OK_Flag = 1;
//				Usart_SendByte(USART3,0xaa);
//				Usart_SendByte(USART3,0x55);
//				Usart_SendByte(USART3,0x01);
//				Usart_SendByte(USART3,length-18);
//				Usart_SendStr_length( USART3, &invt1_Buffer[17],  length-18 );
				//			fSciRx=0;3e38
				//Delay_us(500);
				invt1_Buffer[length]=0xa5;
			}
			//datalen=length-16;
			length = 0;	
			remote_check1 = 0;

	}
	if (rdcount>99)
	{
		rdcount = 0;
	}
//			if(IVT1_Receive_OK_Flag == 1)
//			{			
//				IVT1_Receive_OK_Flag = 0;
//				  Usart_SendStr_length( USART3, InpBuffer,  length );
//			}
}

/*********************************************END OF FILE**********************/

