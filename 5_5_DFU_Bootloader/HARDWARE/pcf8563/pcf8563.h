/**********************************************************
* @ File name -> pcf8563.h
* @ Version   -> V1.0.3
* @ Date      -> 03-09-2014
* @ Brief     -> ʱ��оƬPCF8563��������ͷ�ļ�

 V1.0.1
* @ Revise    -> A��������ʾʱ�����ڸ�ʽ����
* @           -> B�����Ӷ�ȡʱ�䴦��������ȡ����ʱ��������Ϣֱ��ת����ASCII���浽ʱ���ʽ������
* @           -> C������ʱ�����ڴ���������ʾ���ߴ�ӡ�����ڵĻ�ֱ����ʾ���ߴ�ӡʱ���ʽ���鼴��

 V1.0.2
* @ Revise    -> ������ṹΪ16λ��ֵ����ֵλ������20xx��19xx

 V1.0.3
* @ Revise    -> ����PCF8563�Ƿ���ڼ�⺯��
**********************************************************/

#ifndef _pcf8563_h_ 
#define _pcf8563_h_

/**********************************************************
                     �ⲿ����ͷ�ļ�
         Ӧ�õ���ͬ������ͷ�ļ����������޸ļ���                        
**********************************************************/

#include "iic.h"

#include "STM32_config.h"

/**********************************************************
                  PCF8563�Ĵ����ṹ����                          
**********************************************************/
typedef struct
{
	u16 year;	//��
	u8 month;	//��
	u8 week;	//����
	u8 date;	//��
	u8 hour;	//Сʱ
	u8 minute;	//����
	u8 second;	//����

	u8 cont1;	//����/״̬�Ĵ���1
	u8 cont2;	//����/״̬�Ĵ���2
	
	u8 alarmmin;	//���ӱ���
	u8 alarmhour;	//Сʱ����
	u8 alarmdate;	//�ձ���
	u8 alarmweek;	//���ڱ���
	
	u8 CLKcont;	//Ƶ���������
	u8 timecont;	//��ʱ������
	u8 timeconut;	//��ʱ������ʱ�Ĵ���
}Time_Typedef;

extern Time_Typedef TimeValue;	//����ʱ�����ݻ���

extern u8 Time_Register[16];	//����ʱ����Ϣ

extern u8 Display_Time[8];	//��ʾʱ���ʽ

extern u8 Display_Date[13];	//��ʾ���ڸ�ʽ

/**********************************************************
                        �����궨��                       
**********************************************************/

#define PCF8563_Check_Data		0x55	//����ã�����������ֵ

#define PCF8563_YEARDATA		(u16)0x2000	//��ṹ�������19xx�ꡢ21xx��Ļ����޸������ֵ����

#define PCF8563_Write			0xa2	//д����
#define PCF8563_Read			0xa3	//����������ã�PCF8563_Write + 1��

#define Clear_Register			0x00	//����Ĵ���ֵ�������

/**********************************************************
                   �����Ĵ�����ַ�궨��                    
**********************************************************/

#define Address_control1		0x00	//����/״̬�Ĵ���1
#define Address_control2		0x01	//����/״̬�Ĵ���2

#define Address_CLKOUT			0x0d	//CLKOUTƵ�ʼĴ���
#define Address_Time			0x0e	//��ʱ�����ƼĴ���
#define Address_Time_VAL		0x0f	//��ʱ���������Ĵ���

#define Address_year			0x08	//��
#define Address_month			0x07	//��
#define Address_date			0x05	//��
#define Address_week			0x06	//����
#define Address_hour			0x04	//Сʱ
#define Address_minute			0x03	//����
#define Address_second			0x02	//��

#define Alarm_minute			0x09	//���ӱ���
#define Alarm_hour				0x0a	//Сʱ����
#define Alarm_date				0x0b	//�ձ���
#define Alarm_week				0x0c	//���ڱ���

/**********************************************************
                ʱ�����������Чλ�궨��                 
**********************************************************/

#define Shield_secondBit		0x7f
#define Shield_minuteBit		0x7f
#define Shield_hourBit			0x3f
#define Shield_weekBit			0x07
#define Shield_dateBit			0x3f
#define Shield_monthBit			0x1f	//bit7λΪ����Ϊ��0��ָ��������Ϊ20xx
											//                1��ָ��������Ϊ19xx
											//����Ĵ����е�ֵ��99��Ϊ00ʱ������λ��ı�

/**********************************************************
                       ���������                      
**********************************************************/

#define Set_Year19xx			(1<<7)	//����Ϊ19xx��ģʽ
#define Set_Year20xx			(0<<7)	//����Ϊ20xx��ģʽ

//����/״̬�Ĵ���1
#define Control_Test1_Common	(0<<7)	//��ͨģʽ
#define Control_Test1_EXT_CLK	(1<<7)	//EXT_CLK����ģʽ

#define Control_Chip_Run		(0<<5)	//оƬ����
#define Control_Chip_Stop		(1<<5)	//оƬֹͣ���У�����оƬ��Ƶ���첽���߼�0

#define Control_TESTC_Close		(0<<3)	//��Դ��λ����ʧЧ����ͨģʽʱ���߼�0��
#define Control_TESTC_Open		(1<<3)	//��Դ��λ������Ч

//����/״̬�Ĵ���2
#define Control_TI_TF1			(0<<4)	//��TF��ЧʱINT��Ч����ȡ����TIE��״̬��
#define Control_TI_TF2			(1<<4)	//INT������Ч����ȡ����TIE��״̬��
										//ע�⣺��AF��AIE����Чʱ����INTһֱ��Ч

#define Control_clear_AF		(0<<3)	//�������
#define Control_clear_TF		(0<<2)
										//����������ʱ��AF��ֵ�߼�1���ڶ�ʱ������������ʱ��
										//TF��ֵ�߼�1�������ڱ������дǰһֱ����ԭ��ֵ��
										//����ʱ���ͱ����ж϶�����ʱ���ж�Դ��AF��TF������
										//��Ҫʹ���һ����־λ����ֹ��һ��־λ����д��Ӧ�����߼�
										//ָ��AND
#define Alarm_INT_Open			(1<<1)	//�����ж���Ч
#define Alarm_INT_Close			(0<<1)	//�����ж���Ч

#define Time_INT_Open			(1<<0)	//��ʱ���ж���Ч
#define Time_INT_Close			(0<<0)	//��ʱ���ж���Ч

//��/VL�Ĵ���
#define Accuracy_Clock_Yes		(0<<7)	//��֤׼ȷ��ʱ��/��������
#define Accuracy_Clock_No		(1<<7)	//����֤׼ȷ��ʱ��/��������

//���ӱ����Ĵ���
#define Alarm_minute_Open		(0<<7)	//���ӱ�����Ч
#define Alarm_minute_Close		(1<<7)	//���ӱ�����Ч

//Сʱ�����Ĵ���
#define Alarm_hour_Open			(0<<7)	//Сʱ������Ч
#define Alarm_hour_Close		(1<<7)	//Сʱ������Ч

//�ձ����Ĵ���
#define Alarm_date_Open			(0<<7)	//�ձ�����Ч
#define Alarm_date_Close		(1<<7)	//�ձ�����Ч

//���ڱ����Ĵ���
#define Alarm_week_Open			(0<<7)	//���ڱ�����Ч
#define Alarm_week_Close		(1<<7)	//���ڱ�����Ч

//CLKOUTƵ�ʼĴ���
#define CLKOUT_Open				(1<<7)	//CLKOUT�����Ч
#define CLKOUT_Close			(0<<7)	//CLKOUT�������ֹ����ɸ��迹

#define CLKOUT_f32768			0x00	//���32.768KHz
#define CLKOUT_f1024			0x01	//���1024Hz
#define CLKOUT_f32				0x02	//���32Hz
#define CLKOUT_f1				0x03	//���1Hz

//��ʱ���������Ĵ���
#define Time_Open				(1<<7)	//��ʱ����Ч
#define Time_Close				(0<<7)	//��ʱ����Ч

#define Time_f4096				0x00	//��ʱ��ʱ��Ƶ��Ϊ4096Hz
#define Time_f64				0x01	//��ʱ��ʱ��Ƶ��Ϊ64Hz
#define Time_f1					0x02	//��ʱ��ʱ��Ƶ��Ϊ1Hz
#define Time_f160				0x03	//��ʱ��ʱ��Ƶ��Ϊ1/60Hz

/**********************************************************
                       �ⲿ���ܺ���                      
**********************************************************/

u8 PCF8563_Check(void);	//PCF8563����Ƿ����

void PCF8563_Write_Byte(u8 REG_ADD,u8 dat);	//PCF8563ĳ�Ĵ���д��һ���ֽ�����

u8 PCF8563_Read_Byte(u8 REG_ADD);	//PCF8563ĳ�Ĵ�����ȡһ���ֽ�����

void PCF8563_Operate_Register(u8 REG_ADD,u8 *pBuff,u8 num,u8 mode);	//PCF8563��ʱ�������Ĵ���������д�����ݻ��߶�ȡ����

void PCF8563_Set_Alarm(Time_Typedef *Alarm);	//��������

void PCF8563_ReadWrite_Time(u8 mode);	//PCF8563��ȡ����д��ʱ����Ϣ

void PCF8563_Time_Init(Time_Typedef *TimeVAL);	//ʱ��������ʼ��

void Time_Handle(void);	//ʱ���������ݴ�����


#endif
 
