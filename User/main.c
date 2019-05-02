
// ADC 6通道采集实验

#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_usart2.h"
#include "bsp_usart3.h"
#include "bsp_usart4.h"
#include "bsp_adc.h"
#include "Relay.h"
#include "bsp_SysTick.h"
#include "bsp_TiMbase.h" 
#include "stm_flash.h"
#include "math.h"
#include "bsp_rtc.h"
// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue[macNOFCHANEL];

// 局部变量，用于保存转换计算后的电压值 	 
float ADC_ConvertedValueLocal[macNOFCHANEL];        
unsigned char HexToChar(unsigned char bChar);
 /*时间结构体*/
struct rtc_time systmtime;
u8 getdatacomTEX[]={0xff,0xff,0x00,0x00};  
 // ms 计时变量 
extern u8 invt1sign;
extern u8 invt2sign;
extern u8 Bat_Buffer[256]; 
extern u8 invt1_Buffer[100];  
extern u8 invt2_Buffer[100];  
extern u8 GET_ad;
extern u8 Delay60s;
extern uint32_t TimeDisplay;
extern u8 ChargEnd;
void invt_on_off( USART_TypeDef * pUSARTx) ;
u8 state =0;
int Temperature=0;
 uint16_t GridValue;
 
u8 DataArr[]={//0x7E,0xE1,0x82,0x92,0x9B,0xF3,
	//0x7E,0x00,0x1F,0x90,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x01,
							0x7E,0x00,0x21,0x10,0x01,
							//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//zigbeemac
							0xf5,0xc5,0x02,0xfe,0xff,0x6f,0x0d,0x00,
							0xFF,0xFE,0x00,0x00,0x00,
							//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								0xFF,0xFE,0x60,0x0F,0x00,0x69,0x00,0x00,//sn
								0x11,0x26,//ERemain  LEN32
								0x01,0x00,//Emax
								0x01,0x00,//Current
								0x01,0x00,//voltage
								0x01,0x00,//temperature
								0x41};
//7E E1 82 92 9B F3 7E 00 21 10 01 00 00 00 00 00 00 00 00 FF FE 00 00 00 00 00 FF FE 60 0F 00 62 BF 68 C3 50 FF D8 0D A4 0B C3 93
	u8 arra[]={0x7E,0x00,0x19,0x90,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x01,
						 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x26,0x01,0x01,0x40};
  
	u8 arra1[]={0x7E,0x00,0x19,0x90,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x01,
						  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x26,0x01,0x00,0x41};
u8 arraMAC[]={0x7E,0xE1,0x82,0x92,0x9B,0xF3	};
u8 arraSN[]={0xFF,0xFE,0x60,0x0F,0x00,0x62	};//FFFE600F0062 
u8 zigbeeSN[]={0xf5,0xc5,0x02,0xfe,0xff,0x6f,0x0d,0x00	};//f5c502feff6f0d00
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int get_GridValue (void);
int main(void)
{		
	// 配置串口
	int i=0;
//  int32_t Emax=9000000;//最大电量
//  int32_t ERemain=4000000;//当前电量
	uint32_t Emax=0;//最大电量
  int32_t ERemain=0;//当前电量
  u8 Grid=0;
	uint32_t  electric=0;
	u8 electric_arr[8];  
  int16_t get_AD0=0;
  int16_t get_AD1=0;	
	u16 crc=0;
	//uint32_t  electricTime=0;
	uint32_t BJ_TimeVar;
	uint8_t Time_slot[64];
 
 
 	SysTick_Init(); 
	Relay_Init();
  TIMx_Configuration();
	
	POW12V_ON;
	//USART1_Config();
	USART4_Config();
	USART2_Config();
	USART3_Config();//--MCU_Zigbee
	// ADC 初始化
	ADCx_Init();
	
	//读eeprom获得时间
	//STMFLASH_Write ( FLASH_SAVE_ADDR, ( uint16_t * ) datatemp, 8 );
	//printf("write %s\r\n", datatemp);
 
 
	/*  把标准时间转换为北京时间*/
	//BJ_TimeVar =TimeVar + 8*60*60;
	//to_tm(BJ_TimeVar, tm);
	
 	RTC_CheckAndConfig(&systmtime);
	//Time_Show(&systmtime);
	//printf("\r\n ----这是一个ADC多通道采集实验----\r\n");
 	//Usart_SendString(USART3,getdatacom); 
	//Usart_SendStr_length(USART1,getdatacom,17);
	//Send485_length(getdatacom,17);
	//LED_B_TURN;
	//SC_ON;(ADC_ConvertedValue[0]/4096*3.3*0.0705+2.5)*33/49.5;
	//自捡	
	//STMFLASH_Read ( FLASH_SAVE_ADDR+256, ( uint16_t * ) electric_arr, 8 );
 
 
	while(get_AD1<4400)//
	{
		Delay_us(10); 
		if (TimeDisplay >= 1)
		{	
			get_AD1=get_AD(1);
		//	printf("电池电压：%.2f", (double)get_AD1/100);
			TimeDisplay = 0;
		}
		state=0xf8;
	}
	while(1)
	{	
		Delay_us(10);
		//放电
		if (TimeDisplay >=5)
		{			
				SD1_ON;
				SD2_ON;
				SC_OFF;
				state=4;
				TimeDisplay = 0;
				//data = get_battery_current();
				get_AD0=get_battery_current();
				get_AD1=get_AD(1);
				electric+= get_AD1*abs(get_AD0)/10000;
				//printf("容量1:%d w/h",electric/3600);
				 if(get_AD1<4650&&get_AD0>-50)
				 {				
					// printf("电流：%.2f", (double)get_AD0/100);
					// printf("电压：%.2f", (double)get_AD1/100);	
					 electric=0;//放电结束
					
					 break;
				 } 
		  //GET_MasterControl_info();
		}
		//充电
	}
	state=0;
  Delay_us(10000); 
	electric=0;
	while(1)
	{	
		//放电
//		SD1_OFF;
//		SD2_OFF;
//		delay_10us(10);
//		SC_ON;
//		state=1;
		Delay_us(1); 
		if (TimeDisplay >=5)
		{	
		 	SD1_OFF;
			SD2_OFF;
			SC_ON;
			state=5;
			TimeDisplay = 0;
			get_AD0=get_battery_current();
			get_AD1=get_AD(1);
			 
			electric+= get_AD1*get_AD0/10000;
			
			// printf("容量2:%d w/h",electric/3600);
			//electricTime++;
			 if(get_AD1>5350&&get_AD0<50)
			 {
				 ERemain=electric;//w/s
				// electric=electric/3600;//w/h
				 Emax=0;
				 Emax=electric;
				 electric_arr[0]=electric>>24;
				 electric_arr[1]=electric>>16;
				 electric_arr[2]=electric>>8;
				 electric_arr[3]=electric;
				 STMFLASH_Write ( FLASH_SAVE_ADDR+256, ( uint16_t * ) &electric_arr, 8);
					//printf("电池容量:%d w/h",Emax);
					break;
			 }
			// printf("电流：%.2f", (double)get_AD0/100);
			// printf("电压：%.2f", (double)get_AD1/100);
		  //GET_MasterControl_info(); 	
		}
		//充电
	}
		SD1_OFF;
		SD2_OFF;
		SC_OFF;
		state=0;
	while(1)
	{	
		Delay_us(10);	 
		//	Usart_SendString(USART3,getdatacom); 
		//	SD2_ON;
		//	printf("1:%d w/h",electric);
		if (TimeDisplay >= 3)
		{	 
				TimeDisplay = 0;
				//	if(Grid==0)
				//	{
						//printf("并网: ");
						 /*  把标准时间转换为北京时间*/
						BJ_TimeVar =RTC_GetCounter() ;//+ 8*60*60;
				 		//Time_Display( BJ_TimeVar,&systmtime); 	
						to_tm(BJ_TimeVar, &systmtime);/*把定时器的值转换为北京时间*/	
						STMFLASH_Read ( FLASH_SAVE_ADDR, ( uint16_t * ) Time_slot, 64 );
						if(Time_slot[0]/5>10)Time_slot[0]=0;
						for(i=0;i<Time_slot[0]/5;i++)
						{
							if((systmtime.tm_hour*60+systmtime.tm_min)>=(Time_slot[i*5+2]*60+Time_slot[i*5+3])&&
								(systmtime.tm_hour*60+systmtime.tm_min)<(Time_slot[i*5+4]*60+Time_slot[i*5+5]))
							{
								if(Time_slot[i*5+1]==0XFF)//Charge
								{					
										if(get_AD1>5350&&get_AD0<50&&get_AD0>5&&ERemain>Emax/5)
										{
											ChargEnd=1;
										}									
//									if(ChargEnd==1)//充电饱和电流0.1-0.3A后 关闭继电器进入空闲状态
//									{
//										SC_OFF;
//										state=0;
//										ChargEnd=0;
//									}
//									else
									{
										SD1_OFF;
										SD2_OFF;
										Delay_us(10);
										SC_ON;
										//state=0;
										state=1;
 
									}
									 invt2sign=1;
								}
								if(Time_slot[i*5+1]==0X00)//disCharge
								{
									if(ERemain<Emax/10)//电量低于Emax10%(0.1*Emax)停止放电
									{
											SD1_OFF;
											SD2_OFF;
											Delay_us(10); 
											SC_OFF;
											state=0;
										  invt2sign=1;
											ERemain = Emax/10;
									}
									else
									{
										SC_OFF;
										invt1sign=1;
										state=2;
										if(invt2sign==1)//发开机指令
										{	
											Delay_us(500000);
											invt2sign=0;
											invt_on_off(USART2);
											Delay_us(100);
											invt_on_off(UART4);
											Delay_us(100);
										}
									}

									
								}
							}
							if((systmtime.tm_hour*60+systmtime.tm_min)==(Time_slot[i*5+4]*60+Time_slot[i*5+5]))
							{
									SD1_OFF;
									SD2_OFF;
									Delay_us(10); 
									SC_OFF;
									state=0;
								 invt2sign=1;
									//闲置
							}
							
						}
						
						
//							if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 0 )//&&(get_GridValue()< 200))  //离网
//							{	  
//									Grid=1;
//									SD1_OFF;
//									SD2_OFF;
//									SC_OFF;
//									Delay_us(2000);
//									//printf("离网状态");
//									/*等待按键释放 */
//									// 7E 00 19 90 FF FF FF FF FF FF FF FF FF FE 01 00 00 00 00 00 00 00 00 00 11 26 01 01 40  //
//									//while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin) == KEY_ON);   
//								 // Time_Display( BJ_TimeVar,&systmtime); 	
//									Usart_SendStr_length( USART2, arra,29); 
//									Delay_us(80000);
//							//	  printf("发送离网指令");
//							}
 
//				}
//				else//离网
//				{	
//					printf("离网: ");
//							state = 3;
//							if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 1 )//||get_GridValue()> 200) 
//							{	 	
// 
//								printf("in[72]%d", invt1_Buffer[72]);
//								//printf("in[73]%d", invt1_Buffer[73]);
//								//Usart_SendStr_length( USART2, arra1,29);
//										if(invt1_Buffer[72]==3)
//										{
//											printf(":,");
//											Usart_SendStr_length( USART2, arra1,29);
//											//printf("in[72]%d", invt1_Buffer[72]);
//											SC_OFF;
//											state=0;
//											Grid=0;
//											Delay_us(80000);
//										//	printf("开机指令");
//											invt_on_off(UART4);
//											invt_on_off(USART2);
//											//printf("离网结束指令");
//											//printf("in--[72]%d", invt1_Buffer[72]);
//										}
//							}
//				
//				}
			  //ERemain+=get_powe (get_AD0,get_AD1)*2/10000;
					 
				get_AD1=get_AD(1);
				get_AD0=get_battery_current();
				ERemain+=get_AD1*get_AD0*2/10000;//w/s
//				if(get_AD1>5350&&get_AD0<50&&get_AD0>5&&(ERemain*100/36/get_AD1>40000))
//				{
//					ChargEnd=1;
//				}
//			 	printf("电量：%.2f Ah", (double)ERemain/3600/50);
//				printf("tempSumA%.2f", (double)get_AD0/100);
//				printf("tempSumV%.2f ", (double)get_AD1/100);
				//printf("Temperature%.2f", (double)Temperature/128);	
			//
			 DataArr[30]=get_AD0>>8;
			 DataArr[31]=get_AD0;
			 DataArr[32]=get_AD1*10/256;
			 DataArr[33]=get_AD1*10;
			 get_AD1=get_AD1/10;
			 if(get_AD1<450)get_AD1=500;
			 DataArr[26]=ERemain*100/36/get_AD1/256;//ma/h
			 DataArr[27]=ERemain*100/36/get_AD1;
			 DataArr[28]=Emax*100/36/get_AD1/256;
			 DataArr[29]=Emax*100/36/get_AD1;
			 
//			 printf("24:%d ", (int)DataArr[24]);
//			 printf("25:%d ", (int)DataArr[25]);
//			 printf("26:%d ", (int)DataArr[26]);
//			 printf("27:%d ", (int)DataArr[27]);
//			 printf("ERemain:%d ", (int)ERemain);
//			 printf("Emax:%d ", (int)Emax);

			 DataArr[34]=(Temperature/128*10+2731)>>8;
			 DataArr[35]=Temperature/128*10+2731;
			 crc=0;
			 for(i=3; i<36;i++)
			 {
				 crc+=DataArr[i];
			 }
			 DataArr[36]=0xffff-crc;
			// Usart_SendStr_length( USART3, arraMAC,  6 );
		   Usart_SendStr_length( USART3, DataArr,  37 );
				//	GET_MasterControl_info(); 	
			//printf("%d",state);			
 
			}
 
//				if(Bat_Buffer[128]==0xa5&&Bat_Buffer[127]==0x0d)
//				{
//						Bat_Buffer[127]=0x0;
//						Bat_Buffer[128]=0x0;
//						for(i=0;i<54;i++)
//						Bat_Buffer[i]= HexToChar(Bat_Buffer[17+i*2])*16+HexToChar(Bat_Buffer[18+i*2]);
//			//		if((remote_check1 == remote_check2)&&(Bat_Buffer[3]==0x10))
//						 
//							//remote_check2=0;
//						//	IVT1_Receive_OK_Flag = 1;
//							Usart_SendByte(USART3,0xaa);
//							Usart_SendByte(USART3,0x55);
//							Usart_SendByte(USART3,0x03);
//							Usart_SendByte(USART3,54);
//							for(i=0;i<54;i++)
//							Usart_SendByte(USART3, Bat_Buffer[i]);
//							Delay_us(10000);
//				}
//				if(invt1_Buffer[91]==0xa5)
//				{
//						invt1_Buffer[91]=0x0;

////						Usart_SendByte(USART3,0xaa);
////						Usart_SendByte(USART3,0x55);
////						Usart_SendByte(USART3,0x01);
////						Usart_SendByte(USART3,73);
////						Usart_SendStr_length( USART3, &invt1_Buffer[17],  73 );
////						Delay_us(10000);
//					//这个时候应该关闭接收数据
//					}
//				if(invt2_Buffer[91]==0xa5)
//				{
//							invt2_Buffer[91]=0x0;
////						Usart_SendByte(USART3,0xaa);
////						Usart_SendByte(USART3,0x55);
////						Usart_SendByte(USART3,0x02);
////						Usart_SendByte(USART3,73);
////						Usart_SendStr_length( USART3, &invt2_Buffer[17],  73 );
////						Delay_us(10000);
//					}	 
// 
//				if(GET_ad==0xa5)
//				{
//					GET_ad=0;
////					GET_MasterControl_info();//get_AD(InpBuffer[3]);
//					Delay_us(10000);
//				}
			//  get_GridValue ( );
			//	Delay_us(20000);
			//	SD2_OFF;
			//	Send485_length(getdatacomTEX,4);
			//	Usart_SendString(USART3,getdatacom); 
			//	Send485_length(getdatacom,20);
	}
}
void invt_on_off( USART_TypeDef * pUSARTx) 
{
	  u8 arra[]={ 0x7e,0x00,0x19,0x90,0x00,0x00,0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,0x11,0x01,0x01,0xff,0x5c};  
 										
		Usart_SendStr_length( pUSARTx, arra,29); 
	  
}
  int get_GridValue (void)
{ 
		u64 CUSUM=0;
	  uint16_t value=0;
	  int i=0;
	  float tempSum=0;
		for(;i<1000;i++)
		{
			CUSUM+= pow(ADC_ConvertedValue[7]-2048,2);
			Delay_us(40);
		}
		value=	sqrt (CUSUM/1000);
		tempSum=(float)value/4096*4.95*4258;
   return (int)tempSum;
}
unsigned char HexToChar(unsigned char bChar)
{
		if((bChar>=0x30)&&(bChar<=0x39))
		{
			bChar -= 0x30;
		}
		else if((bChar>=0x41)&&(bChar<=0x46)) // Capital
		{
			bChar -= 0x37;
		}
		else if((bChar>=0x61)&&(bChar<=0x66)) //littlecase
		{
			bChar -= 0x57;
		}
		else 
		{
			bChar = 0xff;
		}
		return bChar;
}
/*********************************************END OF FILE**********************/

