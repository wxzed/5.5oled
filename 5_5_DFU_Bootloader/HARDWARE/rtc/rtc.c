/**********************************************************
* @ File name -> rtc.c
* @ Version   -> V1.0
* @ Date      -> 03-02-2014
* @ Brief     -> STM32�ڲ�RTC��������

 V1.
* @ Revise    -> 
**********************************************************/

#include "rtc.h"

/**********************************************************
                   ������ʾʱ���ʽ
             Ҫ�ı���ʾ�ĸ�ʽ���޸Ĵ�����
**********************************************************/

uint8_t RTC_Display_Time[8] = {0x30,0x30,0x3a,0x30,0x30,0x3a,0x30,0x30};	
					//ʱ����ʾ����   ��ʽ  00:00:00

uint8_t RTC_Display_Date[13] = {0x32,0x30,0x31,0x33,0x2f,0x31,0x30,0x2f,0x32,0x30,0x20,0x37,0x57};
					//������ʾ����   ��ʽ  2013/10/20 7W

/**********************************************************
                       ������ر���
**********************************************************/

uint8_t const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�
const uint8_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};	//ƽ����·����ڱ�

STMRTC_Typedef RTC_TimeValue;	//����ʱ�����ݻ���

/**********************************************************
* �������� ---> ����RTCĬ��ʱ��ֵ
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void RTC_Default(void)
{
	/* ����Ĭ��ʱ����Ϣ */
	RTC_TimeValue.year = 2014;
	RTC_TimeValue.month = 3;
	RTC_TimeValue.date = 3;
	RTC_TimeValue.week = 1;
	RTC_TimeValue.hour = 22;
	RTC_TimeValue.minute = 8;
	RTC_TimeValue.second = 0;
}

/**********************************************************
* �������� ---> ����RTC�ж����ȼ�
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
static void RTC_SetNVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/**********************************************************
* �������� ---> �ж�����ǲ�������
* ��ڲ��� ---> years�����
* ������ֵ ---> 0����������
*               1������
* ����˵�� ---> ������366�죬2��Ϊ29��
*               ƽ����365�죬2��Ϊ28��
*               1�¹�31�죬3�¹�31�죬4�¹�30�죬5�¹�31��
*               6�¹�30�죬7�¹�31�죬8�¹�31�죬9�¹�30��
*               10�¹�31�죬11�¹�30�죬12�¹�31��
**********************************************************/
uint8_t RTC_Check_Year(uint16_t years)
{
	if(years % 4 == 0)	//�����ܱ�4���������������귶Χ����Ҫ��һ���ж�
	{
		if(years % 100 == 0)	//�ܱ�100��������Ҫ�ܱ�400�����Ĳ�������
		{
			if(years % 400 == 0)	return 1;	//��Ҫ�ܱ�400����
			else	return 0;	//ƽ����
		}
		else	return 1;	//���ܱ�100���������Ա�4�����ľ�������
	}
	else	return 0;	//ƽ��
}
/**********************************************************
* �������� ---> ����RTCʱ��
* ��ڲ��� ---> none
* ������ֵ ---> 1��ʧ��
*               ������������
* ����˵�� ---> none
**********************************************************/
uint32_t RTC_SetTime(void)
{	
	uint16_t k;
	uint32_t seconds;

	if((RTC_TimeValue.year < 2001) || (RTC_TimeValue.year > 2099))	return 1;	//���Ϸ�������

	for(k = 2001;k < RTC_TimeValue.year;k++)
	{
		if(RTC_Check_Year(k))	seconds += 31622400;	//�����������
		else	seconds += 31536000;
	}
	RTC_TimeValue.month -= 1;	//��Ϊ0 ~ 11
	for(k = 0;k < RTC_TimeValue.month;k++)	//�·����������
	{
		seconds += (uint32_t)mon_table[k] * 86400;	//�·ݵ����������
		if(RTC_Check_Year(RTC_TimeValue.year) && k==1)	seconds += 86400;	//����2�·�����һ�������
	}
	seconds += (uint32_t)(RTC_TimeValue.date - 1) * 86400;	//���յ���������
	seconds += (uint32_t)RTC_TimeValue.hour * 3600;	//��Сʱ����������
	seconds += (uint32_t)RTC_TimeValue.minute * 60;	//�����ӵ���������
	seconds += RTC_TimeValue.second;	//������

	return seconds;
}
/**********************************************************
* �������� ---> ��ȡRTCʱ��������Ϣ
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> ��ȡ����ʱ����Ϣֱ�ӱ����ڽṹ���У��������������
**********************************************************/
void RTC_GetTime(void)
{
	static uint16_t daycnt=0;
	uint8_t yearH,yearL;
	uint16_t temp2;
	uint16_t temp1=0;
	uint32_t temp=0;
	uint32_t timecount=0;

	timecount = RTC_GetCounter();	//�õ�RTC������ֵ

	temp = timecount/86400;   //�õ�����
	if(daycnt != temp)//����һ����
	{	  
		daycnt = temp;
		temp1 = 2001;	//��2001�꿪ʼ
		while(temp >= 365)
		{				 
			if(RTC_Check_Year(temp1))	//������
			{
				if(temp >= 366)	temp -= 366;	//�����������
				else	
				{
					temp1++;
					break;
				}  
			}
			else	temp -= 365;	//ƽ�� 
			temp1++;  
		}
		RTC_TimeValue.year = temp1;	//�õ����   
		temp1 = 0;
		while(temp >= 28)	//������һ����
		{
			if(RTC_Check_Year(RTC_TimeValue.year)&&temp1==1)	//�����ǲ�������/2�·�
			{
				if(temp >= 29)	temp -= 29;	//�����������
				else	break; 
			}
			else 
			{
				if(temp >= mon_table[temp1])	temp -= mon_table[temp1];	//ƽ��
				else break;
			}
			temp1++;  
		}
		RTC_TimeValue.month = temp1 + 1;	//�õ���
		RTC_TimeValue.date = temp + 1;	//�õ��� 
	}
	temp = timecount%86400;	//�õ�������
	RTC_TimeValue.hour = temp/3600;	//�õ�Сʱ
	RTC_TimeValue.minute = (temp%3600)/60;	//�õ�����
	RTC_TimeValue.second = (temp%3600)%60;	//�õ�����

	//��ȡ����
	yearH = RTC_TimeValue.year/100;
	yearL = RTC_TimeValue.year%100;
	if(yearH > 19)	yearL += 100;	//21���ͣ���ݼ�100

	temp2 = yearL + yearL/4;
	temp2 = temp2%7;
	temp2 = temp2 + RTC_TimeValue.date + table_week[RTC_TimeValue.month-1]; 
	if(yearL%4 == 0 && RTC_TimeValue.month<3)	temp2--;
	RTC_TimeValue.week = temp2%7;	//�õ�����	
}
/**********************************************************
* �������� ---> ��ʼ��RTC_Time
* ��ڲ��� ---> none
* ������ֵ ---> ���������д������
*               0���ɹ�
*               1��LSEʧ��
*               2��LSIʧ��
*               3����������
* ����˵�� ---> none
**********************************************************/
uint8_t RTC_Init(void)
{
	uint8_t temp=0;
	
	/* ����ǲ��ǵ�һ������ */
	if(BKP_Read_Byte(BKP_DR2) != RTC_Init_FlagData)	//��һ������ 
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//��ʱ��
		PWR_BackupAccessCmd(ENABLE);	//����д��RTC�ͺ󱸼Ĵ���
		BKP_DeInit();	//��λ

		/* ѡ��RTCʱ��Դ */
#if RTC_CRYSTAL_SELECT==1	/* ʹ��LSE */
		RCC_LSEConfig(RCC_LSE_ON);	//�ⲿ32kHz��������
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//�ȴ�LSE׼����
		{
			temp++;
			delay_ms(10);
			if(temp >= 230)	return 1;	//LSEδ����
		}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);	//����LSEΪRTCʱ��Դ
#elif RTC_CRYSTAL_SELECT==2	/* ʹ���ڲ�LSI */
		RCC_LSICmd(ENABLE);	//�ڲ�40kHz RC��������
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)	//�ȴ�LSI׼����
		{
			temp++;
			delay_ms(10);
			if(temp >= 230)	return 2;	//LSIδ����
		}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);	//����LSIΪRTCʱ��Դ
#elif RTC_CRYSTAL_SELECT==3	/* ʹ��HSE/128 */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);	//����HSE/128ΪRTCʱ��Դ
#else
		return 3;	//��������
#endif	
		/* ����RTCʱ��Դѡ�� */

		RCC_RTCCLKCmd(ENABLE);	//RTCʱ�ӿ���
		RTC_WaitForSynchro();	//�ȴ�RTC��BKPͬ��
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ����������
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//ʹ��RTC���ж�
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ����������
		RTC_EnterConfigMode();	//��������
#if ((RTC_CRYSTAL_SELECT==1) || (RTC_CRYSTAL_SELECT==2))	/* ѡ��LSE����LSIʱ��Դ */
		RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
#elif RTC_CRYSTAL_SELECT==3	/* ѡ��HSE/128 */
		RTC_SetPrescaler(62499);
#else
		return 3;
#endif
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ����������
		RTC_Default();	//����Ĭ��ʱ��
		RTC_SetCounter(RTC_SetTime());	//����RTCʱ��
		RTC_ExitConfigMode();	//�˳�����ģʽ
		BKP_Write_Byte(BKP_DR2, RTC_Init_FlagData);	//д����
	}
	else	//���ǵ�һ�����ã�������ʱ
	{
		RTC_WaitForSynchro();	//�ȴ�RTC��BKPͬ��
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//ʹ��RTC���ж�
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ����������
	}
	RTC_SetNVIC();	//����RTC�ж�
	RTC_GetTime();	//��ȡʱ��

	return 0;	//�ɹ�
}
/**********************************************************
* �������� ---> ʱ���������ݴ�����
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> ����ȡ����ʱ��������Ϣת����ASCII�󱣴浽ʱ���ʽ������
**********************************************************/
void RTC_Time_Handle(void)
{
	/******************************************************
	            ʱ����Ϣת��ΪASCII������ַ�
	******************************************************/
	
	RTC_Display_Time[6] = (RTC_TimeValue.second%100/10) + 0x30;
	RTC_Display_Time[7] = (RTC_TimeValue.second%10) + 0x30;	//Second

	RTC_Display_Time[3] = (RTC_TimeValue.minute%100/10) + 0x30;
	RTC_Display_Time[4] = (RTC_TimeValue.minute%10) + 0x30;	//Minute

	RTC_Display_Time[0] = (RTC_TimeValue.hour%100/10) + 0x30;
	RTC_Display_Time[1] = (RTC_TimeValue.hour%10) + 0x30;	//Hour 

	RTC_Display_Date[8] = (RTC_TimeValue.date%100/10) + 0x30;
	RTC_Display_Date[9] = (RTC_TimeValue.date%10) + 0x30;	//Date

	RTC_Display_Date[5] = (RTC_TimeValue.month%100/10) + 0x30;
	RTC_Display_Date[6] = (RTC_TimeValue.month%10) + 0x30;	//Month

	RTC_Display_Date[0] = (u8)(RTC_TimeValue.year%10000/1000) + 0x30;	//2
	RTC_Display_Date[1] = (u8)(RTC_TimeValue.year%1000/100) + 0x30;	//0
	RTC_Display_Date[2] = (RTC_TimeValue.year%100/10) + 0x30;
	RTC_Display_Date[3] = (RTC_TimeValue.year%10) + 0x30;	//Year

	if(RTC_TimeValue.week == 0)	//ת��Ϊ7 
	{
		RTC_Display_Date[11] = 0x37;
	}
	else	RTC_Display_Date[11] = (RTC_TimeValue.week%10) + 0x30;	//week
}
/**********************************************************
* �������� ---> RTC�жϷ�����
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void RTC_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_SEC) != RESET)	//�����ж�
	{
		RTC_ClearITPendingBit(RTC_IT_SEC);	//������жϱ�־
		RTC_WaitForLastTask();	//�ȴ��������
		RTC_GetTime();	//��ȡʱ��
	}
	if(RTC_GetITStatus(RTC_IT_OW) != RESET)	//����ж�
	{
		RTC_ClearITPendingBit(RTC_IT_OW);	//������жϱ�־
		RTC_WaitForLastTask();	//�ȴ��������
	}
	if(RTC_GetITStatus(RTC_IT_ALR) != RESET)	//�����ж�
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);	//�������жϱ�־
		RTC_WaitForLastTask();	//�ȴ��������
	}
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_ALR|RTC_IT_OW);	//���жϱ�־
	RTC_WaitForLastTask();	//�ȴ��������
}

