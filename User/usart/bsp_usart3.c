/**
  ******************************************************************************
  * @file    bsp_usart1.c--MCU_Zigbee
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����c��printf������usart�˿�
  ******************************************************************************
  * @attention
 
  *
  ******************************************************************************
  */ 
#include "stm32f10x.h"
#include "bsp_usart3.h"
#include "bsp_usart1.h"
#include "bsp_adc.h"
#include "Relay.h"
#include "bsp_SysTick.h"
#include "math.h"
#include "stm_flash.h"
 #include "bsp_rtc.h"
 
void GET_BAT_info( void) ;
void Zigbee_DataReceived(u16 data) ;
void SetPower ( u8 invt,u8 data) ;
void GET_safety_invt2( u8 data) ;
 
 
int get_GridValue (void);
 
 
 
extern __IO uint16_t ADC_ConvertedValue[macNOFCHANEL];
 u8 invt1sign=0;
 u8 invt2sign=0;
 extern u8 state;
 
  
 u8 ZigbeTxFalg=0; 
 u8 GET_ad=0;
 u8 ChargEnd=0;
 uint32_t UNIX_Time=0;//��׼��UNIXʱ��
/********************USART��������**************************/
//#define             macUSART_BAUD_RATE                       115200

//#define             macUSARTx                                USART3
//#define             macUSART_APBxClock_FUN                   RCC_APB1PeriphClockCmd
//#define             macUSART_CLK                             RCC_APB1Periph_USART3
//#define             macUSART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
//#define             macUSART_GPIO_CLK                        (RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO)     
//#define             macUSART_TX_PORT                         GPIOB   
//#define             macUSART_TX_PIN                          GPIO_Pin_10
//#define             macUSART_RX_PORT                         GPIOB   
//#define             macUSART_RX_PIN                          GPIO_Pin_11
//#define             macUSART_IRQ                             USART3_IRQn
//#define             macUSART_INT_FUN                         USART3_IRQHandler
 /****************************************************************************
* ������: ConfigUartNVIC
* ��  ��: ����Ӳ���ж�.
* ��  ��: ��
* ��  ��: ��
* ��  ��: ��
*/
static void ConfigUart3NVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
 /**
  * @brief  USART1 GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */
void USART3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	/* config USART1 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	//USART_Cmd(USART3, ENABLE);
		/*	�����ж����ȼ� */
	ConfigUart3NVIC();
		/* ʹ�ܴ���2�����ж� */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);
//	USART_ClearFlag(USART3, USART_FLAG_TC);
}
/*********************************************END OF FILE**********************/

void USART3_IRQHandler(void)
{
		USART_ClearFlag(USART3,USART_FLAG_TC);

		if(USART_GetITStatus(USART3,USART_IT_RXNE)!=Bit_RESET)//?????USART??????
		{
			Zigbee_DataReceived(USART_ReceiveData(USART3));
		//	Usart_SendByte(USART3,USART_ReceiveData(USART3));
			//u8	k=USART_ReceiveData(USART2);
		//	USART_ClearFlag(USART3,USART_FLAG_TC);
		}
//		else	if(USART_GetITStatus(USART3,USART_IT_TC)!=Bit_RESET)//?????USART??????
//		{
// 			USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
//			USART_ITConfig(USART3, USART_IT_TC, ENABLE);
//		}
//		else if(USART_GetITStatus(USART3,USART_IT_TXE)!=Bit_RESET)//?????USART??????
//		{
// 			/* ��ֹ���ͻ��������жϣ�ʹ�ܷ�������ж� */
//			USART_ITConfig(USART3, USART_IT_TC, DISABLE);
//		}		
}
//��� 1.��ŵ�ʱ��Ρ�2.ʱ��ͬ��
struct rtc_time mtime;
void Zigbee_DataReceived(u16 data) 
{
	static u8 InpBuffer[64]; /* Input buffer for SCI communication */
	static uint8_t length=0,rdcount=0,datalen=0;
	static uint8_t percent=0,Receive_OK_Flag=0;
	static uint16_t remote_check1=0, remote_check2=0;  
 
	uint8_t datatemp [ 6 ];
	if(rdcount == 64)
 		rdcount = 0;
	InpBuffer[rdcount]=data;//&0x00FF;
	if ((InpBuffer[rdcount-2]==0x00)&&(InpBuffer[rdcount-3]==0x7E)&&(InpBuffer[rdcount]==0x10))
	//if ((InpBuffer[rdcount-1]==0xaa)&&(InpBuffer[rdcount]==0x55))
	{
 		InpBuffer[0]=0x7E;
		InpBuffer[1]=0x00;
		InpBuffer[2]=InpBuffer[rdcount-1];
		InpBuffer[3]=0x10;
		rdcount=3;
	}
	remote_check1 +=InpBuffer[rdcount];
	rdcount++;
 	if (rdcount>3)
	{
		length = InpBuffer[2]+4;
	}
	     
	if(rdcount == length&&rdcount>0)
	{			
		//Receive_OK_Flag = 0;
		rdcount=0;
			remote_check1 = remote_check1-InpBuffer[length-1]- InpBuffer[0]- InpBuffer[1]- InpBuffer[2];
			remote_check1 = (0xFFFF-remote_check1) & 0x00FF;
			remote_check2 =  InpBuffer[length-1];

			if(remote_check1 == remote_check2&&(InpBuffer[3]==0x10))
			{
				remote_check2=0;
				Receive_OK_Flag = 1;
	//			fSciRx=0;3e38
			}
			//datalen=length-16;
			length = 0;	
			remote_check1 = 0;
		}
	if(Receive_OK_Flag==1)
	{
		Receive_OK_Flag = 0;
		if(InpBuffer[28]==4)//ʱ��ͬ��
		{
			mtime.tm_year=InpBuffer[4]+2000;
			mtime.tm_mon=InpBuffer[5];
			mtime.tm_mday=InpBuffer[6];
			mtime.tm_hour=InpBuffer[7];
			mtime.tm_min=InpBuffer[8];
			mtime.tm_sec=InpBuffer[9];
			UNIX_Time=mktimev(&mtime);
			RTC_Configuration();
			RTC_WaitForLastTask();
			RTC_SetCounter(UNIX_Time);
			RTC_WaitForLastTask();
			//(InpBuffer[4]<<24)+(InpBuffer[5]<<16)+(InpBuffer[6]<<8)+InpBuffer[7];
			//����Ҫʱ�����ô���
		}
		if(InpBuffer[28]==3&&InpBuffer[27]==0XA5)//��ŵ�ʱ���
		{
			//InpBuffer[29]=InpBuffer[28]/5;
			mtime.tm_year=20019;
			mtime.tm_mon=3;
			mtime.tm_mday=1;
			mtime.tm_hour=InpBuffer[30];
			mtime.tm_min=InpBuffer[31];
			mtime.tm_sec=0;
			UNIX_Time=mktimev(&mtime);
			STMFLASH_Write ( FLASH_SAVE_ADDR, ( uint16_t * ) &InpBuffer[29], InpBuffer[29]+1);
			//
			RTC_Configuration();
			RTC_WaitForLastTask();
			RTC_SetCounter(UNIX_Time);
			RTC_WaitForLastTask();
			// STMFLASH_Read ( FLASH_SAVE_ADDR, ( uint16_t * ) datatemp, 10 );
			
		}
		
//		switch(InpBuffer[2])
//		{			
//			case 0x00://���պ�
//				 
//				SD1_OFF;
//				SD2_OFF;
//				delay_10us(10); 
//				SC_ON;
//			  //state=0;
//			  state=1;
//				break;
//			case 0x0f://���Ͽ�
//				 
//				SD1_OFF;
//				SD2_OFF;
//				delay_10us(10); 
//				SC_OFF;
//			  state=0;
//			   
//				break;
//			case 0x01://�����1�պϣ���磩
//				SC_OFF;
//				//SD1_ON;
//			  invt1sign=1;
//			  state=state|0X02;
//			  state&=0xfe;
//			  //state|=1<<1;
//			  
//				break;
//			case 0x02://�����1�Ͽ�
//				SC_OFF;
//				SD1_OFF;
//			  invt1sign=0;
//			
//			  state&=0xfd;
//				break;
//			case 0x03:///�����2�պϣ���磩
//				SC_OFF;
//				//SD2_ON;
//			  invt2sign=1;
//			  state=state|0X04;
//			  state&=0xfe;
//				break;
//			case 0x04://�����1�Ͽ�
//				SC_OFF;
//				SD2_OFF;
//			  invt2sign=0;
//			  state&=0xfb;
//				break;
//			case 0x05://�����1������
//			//	GET_safety_invt2(InpBuffer[3]);
//				//SetPower(1,0);
//				break;
//			case 0x06://�����2������
//				//SetPower(2,0);
//				break;
// 			case 0x07://��ȡ�����Ϣ
//				GET_BAT_info();
//				break;
//			case 0x08://��ȡ��Ϣ
//				//GET_MasterControl_info();//get_AD(InpBuffer[3]);
//				GET_ad=0xa5;
//				break;
//			default:
//				break;
//		}
			
	}
}
void SetPower ( u8 invt,u8 data) 
{
		  u8 getdatacom[]={ 0x7E,0x00,0x19,0x90,0x00,0x00,0x00,0x00,0x00,0x00,
												0x00,0x00,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,
												0x00,0x00,0x00,0x00,0x11,0x02,0x01,0x32,0x2C};  
		if(invt==1)
		{
			   Usart_SendStr_length( USART2, getdatacom, 29 );//�����1
		}
		if(invt==2)
		{
				Usart_SendStr_length( UART4, getdatacom, 29 );//�����2
		}
 
}

// 
void GET_BAT_info( void) 
{
	  u8 getdatacom[]={ 0x7e,0x32,0x30,0x30,0x32,
											0x34,0x36,0x34,0x32,0x45,
											0x30,0x30,0x32,0x30,0x32,
											0x46,0x44,0x33,0x33,0x0d};  
		//Usart_SendString(USART1,getdatacom); 
    Send485_length(getdatacom,20); 
}
void GET_safety_invt2( u8 data) 
{
	  u8 arra[]={ 0x7e,0x00,0x19,0x90,0x00,0x00,0x00,0x00,0x00,0x00,
								0x00,0x00,0xff,0xfe,0x01,0x00,0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,0x11,0x03,0x01,0x01,0x5b};  
		//Usart_SendString(USART1,getdatacom); 
		arra[27]=	data;		
		arra[28]=	0x5c-data;												
		Usart_SendStr_length( USART2, arra,29); 
		Usart_SendStr_length( UART4, arra,29); 
}
// 
void GET_MasterControl_info( void) 
{
	int16_t data=0;
	int i=0;
 
// 				Usart_SendByte(USART3,0xaa);
//				Usart_SendByte(USART3,0x55);
//				Usart_SendByte(USART3,0x04);
//				Usart_SendByte(USART3,16);
				//  data = Temperature_record();
				//Usart_SendByte(USART3,Temperature>>8);
				//Usart_SendByte(USART3,Temperature);
				data = get_GridValue();
	
				printf("\r\n  ������ѹ��%.1f", (double)data/10);
//	    	printf("  �¶ȣ�%d C��", Temperature/128);
				data = get_battery_current();
				if (data> 30)
				{
						printf("��������%.2f", (double)data/100);
				}
				else
				{				
					if(data>10)
					{
						ChargEnd=1;
						printf("����������");
						
					}
					else if(data>-30)
					{
						printf("  ��ش���״̬");
					}
					else
					{
						printf("  ��طŵ������%.2f", (double)data/100);
						 
					}
				}
 	     Delay_us(1000);
				//Usart_SendByte(USART3,data>>8);
				//Usart_SendByte(USART3,data);
 
//				for(i=1;i<6;i++)
//				{
//							data= get_AD (i);
//							//Usart_SendByte(USART3,data>>8);
//							//Usart_SendByte(USART3,data);
//					  printf("��ѹ��%d\r\n", data);
//				}
								data= get_AD (1);
				                 
                printf("\r\  ��ص�ѹ��%.2f", (double)data/100);
								data= get_AD (2);
				                 Delay_us(1000);
                printf("\r\  DC_BUS��%.2f", (double)data/100);
								Delay_us(100);
								 
								data= get_AD (3);
				                 
                printf("\r\  �������ѹ��%.2f", (double)data/100);
 
								data= get_AD (4);
				            Delay_us(100);     
                printf("\r\  INV1_DC��%.2f", (double)data/100);
								data= get_AD (5);
				                 Delay_us(100);
                printf("\r\  INV2_DC��%.2f", (double)data/100);
 
				 Delay_us(1000);
}

 
int16_t get_battery_current (void)
{
	//static int Ttemp=0,wSumCnt=0;
	float tempSum=0;
	int i=0;
//	tempSum=(float)(ADC_ConvertedValue[0]/4096*3.3*0.0705+2.5)*33/49.5;
	//	tempSum=(float) ADC_ConvertedValue[0]/4096*4.95/0.0705-2.5/0.0705;
	for(i=0;i<100;i++)
	{
		tempSum+=(float) ADC_ConvertedValue[0]/4096*4.95/0.0705-2.5/0.0705;
		Delay_us(10);
	}
		return tempSum;

}
int16_t get_AD (u8 at) 
{
	int16_t Charger_value=0;
	float tempSum=0;
	int i=0;
	for(i=0;i<100;i++)
	{
		tempSum+=(float)ADC_ConvertedValue[at];
		Delay_us(10);
	}
	tempSum=tempSum/4096*69.43226;
	Charger_value=tempSum;//*100;
   return Charger_value;
}
 
 int get_powe (int current,int Value) 
{
	int Charger_value=0;
	float tempSumA=0;	
	float tempSumV=0;
	int i=0;
 
	for(i=0;i<100;i++)
	{
		tempSumA+=(float) ADC_ConvertedValue[0]/4096*4.95/0.0705-2.5/0.0705;
		tempSumV+=(float)ADC_ConvertedValue[1];
		Delay_us(10);
	}
	Value=tempSumV/4096*69.43226;
	
	current=tempSumA;
	
	Charger_value=current*Value;//*100;
	//printf("tempSumA%.2f", (double)tempSumA/100);
	//printf("tempSumV%.2f", (double)Value/100);
   return Charger_value;
}
//STMFLASH_Write ( FLASH_SAVE_ADDR, ( uint16_t * ) TEXT_Buffer, SIZE );
// STMFLASH_Read ( FLASH_SAVE_ADDR, ( uint16_t * ) datatemp, 10 );
 /// �ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART3, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}
 ///// �ض���c�⺯��scanf��macUSARTx
//int fgetc(FILE *f)
//{
//		/* �ȴ�����1�������� */
//		while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);

//		return (int)USART_ReceiveData(USART3);
//}

