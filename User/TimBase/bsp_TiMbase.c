
// �߼���ʱ��TIMx,x[6,7]��ʱ��ʼ������
#include "bsp_TiMbase.h" 
#include "bsp_adc.h"
#include  "math.h"
#include "Relay.h"
#include "bsp_SysTick.h"
#include "bsp_usart3.h"
 
extern __IO uint16_t ADC_ConvertedValue[macNOFCHANEL];
extern u8 invt1sign;
extern u8 invt2sign;
extern uint16_t GridValue;   
extern int Temperature;
extern u8 state;
extern uint32_t TimeDisplay;
void Temperature_record(  void );
/// TIMx,x[6,7]�ж����ȼ�����
void TIMx_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = macTIM_IRQ;
	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * �ж�����Ϊ = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
 */

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{  TIM_Prescaler         ����
 *	TIM_CounterMode			 TIMx,x[6,7]û�У���������
 *  TIM_Period            ����
 *  TIM_ClockDivision     TIMx,x[6,7]û�У���������
 *  TIM_RepetitionCounter TIMx,x[1,8,15,16,17]����
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */


void TIMx_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// ����TIMx_CLK,x[6,7],���ڲ�ʱ��CK_INT=72M
    macTIM_APBxClock_FUN (macTIM_CLK, ENABLE);
	
		// �Զ���װ�ؼĴ����ܵ�ֵ(����ֵ)
    TIM_TimeBaseStructure.TIM_Period=40;
	
    // �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж�
	  // ʱ��Ԥ��Ƶ��Ϊ71����������������ʱ��CK_CNT = CK_INT / (71+1)=1M
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		// ʱ�ӷ�Ƶ���� ��������ʱ��TIM6��TIM7û�У����ù�
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// ����������ģʽ��������ʱ��TIM6��TIM7ֻ�����ϼ�����û�м���ģʽ�����ã����ù�
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// �ظ���������ֵ��������ʱ��TIM6��TIM7û�У����ù�
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // ��ʼ����ʱ��TIMx, x[6,7]
    TIM_TimeBaseInit(macTIMx, &TIM_TimeBaseStructure);
		
		// ����������жϱ�־λ
    TIM_ClearFlag(macTIMx, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(macTIMx,TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(macTIMx, ENABLE);																		
    
		// ��ʱ�ر�TIMx,x[6,7]��ʱ�ӣ��ȴ�ʹ��
    macTIM_APBxClock_FUN (macTIM_CLK, DISABLE);   
 	
	/* ���û�����ʱ�� TIMx,x[6,7]���ж����ȼ� */
	TIMx_NVIC_Configuration();

	/* ������ʱ�� TIMx,x[6,7] ���¿�ʱ�ӣ���ʼ��ʱ */
	macTIM_APBxClock_FUN (macTIM_CLK, ENABLE);
}
u32 CUSUM=0;
u32 DatumValue =0;
u16 cycletime=0;
u64 CUSUM1=0;
uint16_t value1=0;
u8 Delay60s=0;
 void  macTIM_INT_FUN (void)//25k
{
	static u16 commn=0;
	static u16 cycle=0;
//	static u16 Tout=0;
  static   u32 time;
	static uint16_t  ADC_Value=0;
	if ( TIM_GetITStatus( macTIMx, TIM_IT_Update) != RESET ) 
	{	
		
		cycletime++;
		time++;
				if ( time > 25000 ) /* 1000 * 1 ms = 1s ʱ�䵽 */
			{
				TimeDisplay++;
				Delay60s++;
					time = 0;
				 if(state==0)//����
				{
					LED_B_ON;
					LED_G_ON;
					LED_R_OFF;
				}
//				if(state&0x04&&state&0x02)
//				{
//					time=12500;
//				} 
//				if(state&0x04||state&0x02)
//				{
//					LED_G_OFF;
//					LED_B_TURN;
//				} 
				if(state==0x01)//Charge 
				{
					LED_B_OFF;
					LED_G_TURN;
				} 
				if(state==0x02)//discharge 
				{
					LED_G_OFF;
					LED_R_OFF;
					LED_B_TURN;
				} 
				if(state==0x03)//���� 
				{
					LED_B_OFF;
					LED_R_OFF;
					LED_G_ON;
				} 
				if(state==0x04)//�ŵ��Լ�״̬
				{
					time=20000;
					LED_R_OFF;
					LED_G_OFF;
					LED_B_TURN;
				} 
				if(state==0x05)//�Լ�״̬
				{
					time=20000;
					LED_R_OFF;
					LED_B_OFF;
					LED_G_TURN;
				} 
				if(state&0xf8)//error
				{
					LED_B_OFF;
					LED_G_OFF;
					LED_R_TURN;
				} 
			} 
		ADC_Value=ADC_ConvertedValue[7];
		CUSUM+=ADC_Value;
		CUSUM1+= pow(ADC_Value-2048,2);
		Temperature_record();//�¶�
		
//		if(commn==0&&time==500)
//		{		
//				time=0;
//				DatumValue=CUSUM/500;
//				CUSUM=0;
//			  Value=(float)DatumValue/4096*3.3;
//				commn=1;
//		}
		
//		if(commn==1&&(ADC_ConvertedValue[7]>DatumValue))
//		{
//			time=0;
//			commn=2;
//		}
		if(commn==0&&ADC_Value>2048)
		{
			cycletime=0;
			commn=1;
			//	LED_Bug_TURN;
		}
		if(commn==1&&ADC_Value<2048)//��õڶ���Ԥ�����������
		{
			commn=2;
			cycle=0;
			cycletime=0;
			CUSUM=0;
		 
 
		}
		if(commn==2&&ADC_Value>2048)//��õڶ���Ԥ�����������
		{
			commn=3;
			cycle=cycletime;
			cycletime=0;
			CUSUM=0;
			 
 
		}
		if(commn==3&&cycletime==cycle)//���һ������������ƽ��ֵ����׼��ѹֵ��
		{		
				cycletime=0;
				DatumValue=CUSUM/cycle;
				CUSUM=0;
			  //Value=(float)DatumValue/4096*3.3;
				cycle=0;
				commn=4;

		}
		if(commn==4&&ADC_Value>DatumValue)//��һ�������
		{
			cycletime=0;
			commn=5;
			
		}
		if(commn==5&&ADC_Value<DatumValue)//��һ�������
		{
			cycletime=0;
			commn=6;
				CUSUM1=0;
		}
		if(commn==6&&ADC_Value>DatumValue)//�ڶ��������
		{
			cycle=cycletime;
			cycletime=0;
			commn=7;
			value1=	cycle*2;//sqrt (CUSUM1/cycle);
		 
		}
		if(commn==7&&cycletime>(cycle-300))//
		{
			//cycle=time;
			commn=8;
			cycletime=0;
			CUSUM=0;
			DatumValue=0;
//			LED_Bug_TURN;
//			delay_10us(1);
//			LED_Bug_TURN;
			if(invt1sign==1)
			{
				SD1_ON;
				SD2_ON;
				invt1sign=0;
			}
//			if(invt2sign==1)
//			{
//				
//				//invt2sign=0;
//			}
			CUSUM1=0;CUSUM1=0;
		}
		if(commn==8&&cycletime>=value1)//
		{
				commn=0;
				cycletime=0;
				CUSUM=0;
				DatumValue=0;
			  value1=	sqrt (CUSUM1/value1);
			GridValue=(float)value1/4096*4.95*4258;
			CUSUM1=0;
		}			
		TIM_ClearITPendingBit(macTIMx , TIM_FLAG_Update);
		
	}		 	
}	
 
 void Temperature_record(void)
{

	static int T=0,T2=0,T1=0,T_SENSE_TO_REAL_FIX=27034;
	static int V_T_MINUS_40=25773,V_T_0=19859,V_T_50=7905,V_T_70=4913,V_T_125=1381;
	static int KT1=-22847,KT2=25194,KT3=-27686,KT4=22186,KT5=-21742,KT6=11229;
	static int KT7=17273,KT8=-27415,KT9=15620,KT10=26406,KT11=-17760,KT12=21024;
	static int Ttemp=0,wSumCnt=0;
	static long tempSum=0;
	
	T=(int)((long)(ADC_ConvertedValue[6]*8*T_SENSE_TO_REAL_FIX)>>15);		//Q15*Q13>>15=Q13
    //wTempLostTest = T;
	T2=(int)((long)T*T>>15);		//Q13*Q13>>15=Q11
	T1=T;					//Q13
	if(T>V_T_MINUS_40)
	{ 
		Ttemp= -5120;
	}
	else if(T>V_T_0)
	{ 
	 	Ttemp= (((int)((long)KT1*T2>>15) + (int)((long)KT2*T1>>15))<<2) + KT3;
	}
	else if(T>V_T_50)
	{ 
		Ttemp= (int)((long)KT4*T2>>17) + (int)((long)KT5*T1>>15) + KT6;
	}
	else if(T>V_T_70)
	{ 
		Ttemp= (int)((long)KT7*T2>>13) + (int)((long)KT8*T1>>14) + KT9;
	}
	else if(T>V_T_125)
	{ 
		Ttemp= (int)((long)KT10*T2>>11) + (int)((long)KT11*T1>>12) + KT12;
	}
	else
	{
		Ttemp=16000;
	}
	wSumCnt++;
	tempSum+=Ttemp;
	if(wSumCnt>=2048)
	{
	    Temperature = tempSum>>11;
	    if(Temperature >0)
	    {
	    	Temperature = (int)((long)Temperature*993>>10);//0.97  //921 0.9
	    }
	    tempSum=0;
	    wSumCnt=0;
	}		
 
}
 
 
/*********************************************END OF FILE**********************/
