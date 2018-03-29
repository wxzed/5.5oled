/**********************************************************
* @ File name -> pcf8563.c
* @ Version   -> V1.0.3
* @ Date      -> 03-09-2014
* @ Brief     -> ʱ��оƬPCF8563��������

 V1.0.1
* @ Revise    -> A��������ʾʱ�����ڸ�ʽ����
* @           -> B�����Ӷ�ȡʱ�䴦��������ȡ����ʱ��������Ϣֱ��ת����ASCII���浽ʱ���ʽ������
* @           -> C������ʱ�����ڴ���������ʾ���ߴ�ӡ�����ڵĻ�ֱ����ʾ���ߴ�ӡʱ���ʽ���鼴��

 V1.0.2
* @ Revise    -> ������ṹΪ16λ��ֵ����ֵλ������20xx��19xx

 V1.0.3
* @ Revise    -> ����PCF8563�Ƿ���ڼ�⺯��
**********************************************************/

#include "pcf8563.h"

/**********************************************************
                   ������ʾʱ���ʽ
             Ҫ�ı���ʾ�ĸ�ʽ���޸Ĵ�����
**********************************************************/

u8 Display_Time[8] = {0x30,0x30,0x3a,0x30,0x30,0x3a,0x30,0x30};	
					//ʱ����ʾ����   ��ʽ  00:00:00

u8 Display_Date[13] = {0x32,0x30,0x31,0x33,0x2f,0x31,0x30,0x2f,0x32,0x30,0x20,0x37,0x57};
					//������ʾ����   ��ʽ  2013/10/20 7W

/**********************************************************
                    ������صı�������
**********************************************************/

Time_Typedef TimeValue;	//����ʱ������ָ��

u8 Time_Buffer[16];	//ʱ���������ݻ���

/**********************************************************
* �������� ---> PCF8563ĳ�Ĵ���д��һ���ֽ�����
* ��ڲ��� ---> REG_ADD��Ҫ�����Ĵ�����ַ
*               dat��Ҫд�������
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void PCF8563_Write_Byte(u8 REG_ADD,u8 dat)
{
	IIC_Start();
	if(!(IIC_Write_Byte(PCF8563_Write)))	//����д������Ӧ��λ
	{
		IIC_Write_Byte(REG_ADD);
		IIC_Write_Byte(dat);	//��������
	}
	IIC_Stop();
} 
/**********************************************************
* �������� ---> PCF8563ĳ�Ĵ�����ȡһ���ֽ�����
* ��ڲ��� ---> REG_ADD��Ҫ�����Ĵ�����ַ
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
u8 PCF8563_Read_Byte(u8 REG_ADD)
{
	u8 ReData;
	IIC_Start();
	if(!(IIC_Write_Byte(PCF8563_Write)))	//����д������Ӧ��λ
	{
		IIC_Write_Byte(REG_ADD);	//ȷ��Ҫ�����ļĴ���
		IIC_Start();	//��������
		IIC_Write_Byte(PCF8563_Read);	//���Ͷ�ȡ����
		ReData = IIC_Read_Byte();	//��ȡ����
		IIC_Ack(1);	//���ͷ�Ӧ���źŽ������ݴ���
	}
	IIC_Stop();
	return ReData;
}
/**********************************************************
* �������� ---> PCF8563����Ƿ����
* ��ڲ��� ---> none
* ������ֵ ---> 0������
*               1��PCF8563���������
* ����˵�� ---> ��ʱ������ʱ�Ĵ���д��һ����ֵ�ٶ�ȡ�������Աȣ���ͬ��ȷ����ͬ�����
**********************************************************/
u8 PCF8563_Check(void)
{
	u8 test_value=0;
	u8 Time_Count=0;	//��ʱ������ʱ���ݻ���

	if(PCF8563_Read_Byte(Address_Time) & 0x80)	//������˶�ʱ�������ȹر�
	{
		PCF8563_Write_Byte(Address_Time, Time_Close);	//�ȹرն�ʱ��
		Time_Count = PCF8563_Read_Byte(Address_Time_VAL);	//�ȱ������ֵ
	}

	PCF8563_Write_Byte(Address_Time_VAL, PCF8563_Check_Data);	//д����ֵ
	for(test_value = 0;test_value < 250;test_value++)	{}	//��ʱһ��ʱ���ٶ�ȡ
	test_value = PCF8563_Read_Byte(Address_Time_VAL);	//�ٶ�ȡ����

	if(Time_Count != 0)	//�����˶�ʱ�����ܣ���ָ�
	{
		PCF8563_Write_Byte(Address_Time_VAL, Time_Count);	//�ָ��ֳ�
		PCF8563_Write_Byte(Address_Time, Time_Open);	//������ʱ��
	}

	if(test_value != PCF8563_Check_Data)	return 1;	//�������������
	
	return 0;	//����
}
/**********************************************************
* �������� ---> PCF8563��ʱ�������Ĵ���������д�����ݻ��߶�ȡ����
* ��ڲ��� ---> REG_ADD��Ҫ�����Ĵ�����ʼ��ַ
*               *WBuff��д�����ݻ���
*               num��д����������
*               mode������ģʽ��0��д�����ݲ�����1����ȡ���ݲ���
* ������ֵ ---> none
* ����˵�� ---> ����д��n�ֽڻ���������ȡn�ֽ�����
**********************************************************/	
void PCF8563_Operate_Register(u8 REG_ADD,u8 *pBuff,u8 num,u8 mode)
{
	u8 i;
	if(mode)	//��ȡ����
	{
		IIC_Start();
		if(!(IIC_Write_Byte(PCF8563_Write)))	//����д������Ӧ��λ
		{
			IIC_Write_Byte(REG_ADD);	//��λ��ʼ�Ĵ�����ַ
			IIC_Start();	//��������
			IIC_Write_Byte(PCF8563_Read);	//���Ͷ�ȡ����
			for(i = 0;i < num;i++)
			{
				*pBuff = IIC_Read_Byte();	//��ȡ����
				if(i == (num - 1))	IIC_Ack(1);	//���ͷ�Ӧ���ź�
				else IIC_Ack(0);	//����Ӧ���ź�
				pBuff++;
			}
		}
		IIC_Stop();	
	}
	else	//д������
	{		 	
		IIC_Start();
		if(!(IIC_Write_Byte(PCF8563_Write)))	//����д������Ӧ��λ
		{
			IIC_Write_Byte(REG_ADD);	//��λ��ʼ�Ĵ�����ַ
			for(i = 0;i < num;i++)
			{
				IIC_Write_Byte(*pBuff);	//д������
				pBuff++;
			}
		}
		IIC_Stop();
	}
}
/**********************************************************
* �������� ---> PCF8563��ȡ����д��ʱ����Ϣ
* ��ڲ��� ---> *pBuff��д�����ݻ���
*               mode������ģʽ��0��д�����ݲ�����1����ȡ���ݲ���
* ������ֵ ---> none
* ����˵�� ---> ����д��n�ֽڻ���������ȡn�ֽ�����
**********************************************************/
void PCF8563_ReadWrite_Time(u8 mode)
{
	u8 Time_Register[8];	//����ʱ�仺��
	
	if(mode)	//��ȡʱ����Ϣ
	{
		PCF8563_Operate_Register(Address_second,Time_Register,7,1);	//�����ַ��0x02����ʼ��ȡʱ����������
		
		/******�����ݸ��Ƶ�ʱ��ṹ���У��������������******/
		TimeValue.second = Time_Register[0] & Shield_secondBit;	//������
		TimeValue.minute = Time_Register[1] & Shield_minuteBit;	//��������
		TimeValue.hour   = Time_Register[2] & Shield_hourBit;	//Сʱ����
		TimeValue.date   = Time_Register[3] & Shield_dateBit;	//������
		TimeValue.week   = Time_Register[4] & Shield_weekBit;	//��������
		TimeValue.month  = Time_Register[5] & Shield_monthBit;	//������
		TimeValue.year   = Time_Register[6] | PCF8563_YEARDATA;	//������
	}
	else
	{
		if(TimeValue.week == 0x07)	TimeValue.week = 0x00;	//�Ĵ�����ֵΪ0 ~ 6

		/******��ʱ��ṹ���и������ݽ���******/
		Time_Register[0] = TimeValue.second | Accuracy_Clock_Yes;	//�룬��֤׼ȷʱ��
		Time_Register[1] = TimeValue.minute;						//����
		Time_Register[2] = TimeValue.hour;							//Сʱ
		Time_Register[3] = TimeValue.date;							//��
		Time_Register[4] = TimeValue.week;							//����
		Time_Register[5] = TimeValue.month | (0<<7);				//�£�����������λ20xx��
		Time_Register[6] = (u8)TimeValue.year;						//��
		
//		PCF8563_Write_Byte(Address_control1,Control_Chip_Stop);	//оƬֹͣ����
		PCF8563_Operate_Register(Address_second,Time_Register,7,0);	//�����ַ��0x00����ʼд��ʱ����������
//		PCF8563_Write_Byte(Address_control1,Control_Chip_Run);	//����оƬ
	}
}

/**********************************************************
* �������� ---> ʱ��������ʼ��
* ��ڲ��� ---> *TimeVAL��RTCоƬ�Ĵ���ֵָ��
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void PCF8563_Time_Init(Time_Typedef *TimeVAL)
{
	if(TimeVAL->week == 0x07)	TimeVAL->week = 0x00;	//�Ĵ�����ֵΪ0 ~ 6
	
	//����/״̬�Ĵ���
	Time_Buffer[0]  = TimeVAL->cont1;	//����/״̬�Ĵ���1
	Time_Buffer[1]  = TimeVAL->cont2;	//����/״̬�Ĵ���2
	//ʱ����������
	Time_Buffer[2]  = TimeVAL->second | Accuracy_Clock_Yes;	//��֤׼ȷʱ��
	Time_Buffer[3]  = TimeVAL->minute;
	Time_Buffer[4]  = TimeVAL->hour;
	Time_Buffer[5]  = TimeVAL->date;
	Time_Buffer[6]  = TimeVAL->week;
	Time_Buffer[7]  = TimeVAL->month | Set_Year20xx;	//����Ϊ20xx��
	Time_Buffer[8]  = (u8)TimeVAL->year;
	//���屨��
	Time_Buffer[9]  = TimeVAL->alarmmin | Alarm_minute_Close;	//���ӱ���
	Time_Buffer[10] = TimeVAL->alarmhour | Alarm_hour_Close;	//Сʱ����
	Time_Buffer[11] = TimeVAL->alarmdate | Alarm_date_Close;	//�ձ���
	Time_Buffer[12] = TimeVAL->alarmweek | Alarm_week_Close;	//���ڱ���
	//Ƶ��������ã���ʱ������
	Time_Buffer[13] = TimeVAL->CLKcont;	//Ƶ���������
	Time_Buffer[14] = TimeVAL->timecont;	//��ʱ������
	Time_Buffer[15] = TimeVAL->timeconut;	//��ʱ������ʱ
	
//	PCF8563_Write_Byte(Address_control1,Control_Chip_Stop);	//оƬֹͣ����
	PCF8563_Operate_Register(Address_control1,Time_Buffer,16,0);	//�ӿ���/״̬�Ĵ���1��0x00����ʼд��16������
//	PCF8563_Write_Byte(Address_control1,Control_Chip_Run);	//����оƬ
}
/**********************************************************
* �������� ---> ʱ���������ݴ�����
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> ����ȡ����ʱ��������Ϣת����ASCII�󱣴浽ʱ���ʽ������
**********************************************************/
void Time_Handle(void)
{
	/******************************************************
	                   ��ȡʱ��������Ϣ
	******************************************************/
	
	PCF8563_ReadWrite_Time(1);	//��ȡʱ����������
	
	/******************************************************
	            ʱ����Ϣת��ΪASCII������ַ�
	******************************************************/
	
	Display_Time[6] = (TimeValue.second >> 4) + 0x30;
	Display_Time[7] = (TimeValue.second & 0x0f) + 0x30;	//Second

	Display_Time[3] = (TimeValue.minute >> 4) + 0x30;
	Display_Time[4] = (TimeValue.minute & 0x0f) + 0x30;	//Minute

	Display_Time[0] = (TimeValue.hour >> 4) + 0x30;
	Display_Time[1] = (TimeValue.hour & 0x0f) + 0x30;	//Hour 

	Display_Date[8] = (TimeValue.date >> 4) + 0x30;
	Display_Date[9] = (TimeValue.date & 0x0f) + 0x30;	//Date

	Display_Date[5] = (TimeValue.month >> 4) + 0x30;
	Display_Date[6] = (TimeValue.month & 0x0f) + 0x30;	//Month

	Display_Date[0] = (u8)(TimeValue.year >> 12) + 0x30;	//2
	Display_Date[1] = (u8)((TimeValue.year & 0x0f00) >> 8) + 0x30;	//0
	Display_Date[2] = (TimeValue.year >> 4) + 0x30;
	Display_Date[3] = (TimeValue.year & 0x0f) + 0x30;	//Year

	/******************************************************
	   ��������ʾ����7�������ľ��Ƕ�Ӧ��������ʾ����0ת��Ϊ7
	******************************************************/
	
	if(TimeValue.week == 0x00)
	{	Display_Date[11] = 0x37;	}	//week
	else
	{	Display_Date[11] = (TimeValue.week & 0x0f) + 0x30;	}

}

