/**
  ******************************************************************************
  * @file    bsp_usart1.c----MCU_BAT
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   重现c库printf函数到usart端口
  ******************************************************************************
  * @attention
 
  *
  ******************************************************************************
  */ 
  
#include "bsp_usart1.h"
#include "bsp_SysTick.h"
#include "Relay.h"
/********************USART参数定义**************************/
#define             macUSART_BAUD_RATE                       115200
#define             macUSARTx                                USART1
#define             macUSART_APBxClock_FUN                   RCC_APB2PeriphClockCmd
#define             macUSART_CLK                             RCC_APB2Periph_USART1
#define             macUSART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macUSART_GPIO_CLK                        RCC_APB2Periph_GPIOA     
#define             macUSART_TX_PORT                         GPIOA   
#define             macUSART_TX_PIN                          GPIO_Pin_9
#define             macUSART_RX_PORT                         GPIOA 
#define             macUSART_RX_PIN                          GPIO_Pin_10

/****************************************************************************
* 函数名: ConfigUartNVIC
* 功  能: 配置硬件中断.
* 输  入: 无
* 输  出: 无
* 返  回: 无
*/
 
void bat_data_buf(u16 data) ;
 u8 Bat_Buffer[256]; /* Input buffer for SCI communication */
static void ConfigUartNVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
 /**
  * @brief  USART1 GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	/* config USART1 clock */
	macUSART_APBxClock_FUN(RCC_APB2Periph_USART1, ENABLE);
	macUSART_GPIO_APBxClock_FUN(RCC_APB2Periph_GPIOA, ENABLE);
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  macUSART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(macUSART_TX_PORT, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = macUSART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(macUSART_RX_PORT, &GPIO_InitStructure);
	
 /* Configure RS485 contrel pin: CTL */   
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;       
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
 GPIO_Init(macUSART_RX_PORT, &GPIO_InitStructure);    
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = macUSART_BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(macUSARTx, &USART_InitStructure);
	//USART_Cmd(macUSARTx, ENABLE);
		/*	配置中断优先级 */
	ConfigUartNVIC();
		/* 使能串口2接收中断 */
	USART_ITConfig(macUSARTx, USART_IT_RXNE, ENABLE);
	USART_Cmd(macUSARTx, ENABLE);
	//USART_ClearFlag(macUSARTx, USART_FLAG_TC);
}
 void RS485_SendByte(unsigned char temp)
{
// while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
 //GPIO_SetBits(GPIOA, GPIO_Pin_11); 
   //Delay(100);
//	Delay_us(90);
    USART_SendData(USART1, temp);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
 //Delay_us(10);
 //GPIO_ResetBits(GPIOA, GPIO_Pin_11);
 //return(temp);
}

/*****************  发送一个字符 **********************/
  void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch )
{
	/* 发送一个字节数据到USART1 */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}
/*****************  指定长度的发送字符串 **********************/
void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(k < strlen);
}

void Send485_length( uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;
	GPIO_SetBits(GPIOA, GPIO_Pin_11);
	//Delay_us(1);
    do 
    {
        RS485_SendByte( *(str + k) );
        k++;
    } while(k < strlen);
	//	 Delay_us(100);
		//delay_ms(1);
		delay_10us(20);
   GPIO_ResetBits(GPIOA, GPIO_Pin_11);
}
/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str)
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');
}
///// 重定向c库函数printf到macUSARTx
//int fputc(int ch, FILE *f)
//{
//		/* 发送一个字节数据到macUSARTx */
//		USART_SendData(UART4, (uint8_t) ch);
//		
//		/* 等待发送完毕 */
//		while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);		
//	
//		return (ch);
//}

///// 重定向c库函数scanf到macUSARTx
//int fgetc(FILE *f)
//{
//		/* 等待串口1输入数据 */
//		while (USART_GetFlagStatus(macUSARTx, USART_FLAG_RXNE) == RESET);

//		return (int)USART_ReceiveData(macUSARTx);
//}


void USART1_IRQHandler(void)
{
		USART_ClearFlag(USART1,USART_FLAG_TC);
     //USART_GetITStatus(USART3,USART_IT_RXNE)!=Bit_RESET)
		if(USART_GetITStatus(USART1,USART_IT_RXNE)!=Bit_RESET)//?????USART??????
		{
			//u8	k=USART_ReceiveData(USART1);
			// USART_SendData(USART3,USART_ReceiveData(USART1));
			bat_data_buf(USART_ReceiveData(USART1));
		}
}

 
void bat_data_buf(u16 data) 
{
 
	static u8 length=0,rdcount=0,datalen=0;
	//static u8 percent=0,IVT1_Receive_OK_Flag=0;
	//static u16 remote_check1=0, remote_check2=0;  
	u16 i=0;
	Bat_Buffer[rdcount]=data;//&0x00FF;
	if ((Bat_Buffer[rdcount-2]==0x32)&&(Bat_Buffer[rdcount-3]==0x7E)&&(Bat_Buffer[rdcount]==0x30))
	{
		Bat_Buffer[0]=0x7e;
		Bat_Buffer[1]=0x32;
		Bat_Buffer[2]=0x30;
		Bat_Buffer[3]=0x30;
		rdcount=3;
	}
	//remote_check1 += Bat_Buffer[rdcount];
	rdcount++;
	if(rdcount == 128&&Bat_Buffer[0]==0x7e&&Bat_Buffer[1]==0x32)
	{	 
			rdcount = 0;
			Bat_Buffer[128]=0xa5;
 
	}
			if(rdcount > 128)
			{			
				rdcount = 0;
				 
			}
}
 char hextoascii(char hex_byte)
{
    char result;
    if((hex_byte>=0)&&(hex_byte<=9))            //??ascii??
        result = hex_byte + 0x30;
    else if((hex_byte >= 10)&&(hex_byte <= 15)) //??ascii????
        result = hex_byte + 0x37;
    else
        result = 0xff;
    return result;
}
 
 
 
/*********************************************END OF FILE**********************/

