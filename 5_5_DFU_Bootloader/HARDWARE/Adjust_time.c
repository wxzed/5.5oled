/**********************************************************
* @ File name -> Adjust_time.c
* @ Version   -> V1.0
* @ Date      -> 01-19-2014
* @ Brief     -> ����ʱ�亯��

 V1.
* @ Revise    -> 
**********************************************************/

#include "Adjust_time.h"

/**********************************************************
                        �������
**********************************************************/

u8 dis_mode;	//��ʾ�л�״̬����

u8 Time_Variable;
u8 Display_Value[5];	//����ʱ���õ�����

//u8 time_display[13];

/**********************************************************
* �������� ---> ʱ��������ʾˢ�º���
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> ÿ����һ��ʱ����������ʱ����Ҫ�����ı�������ɫ�ı�����ɫ��ʾ
**********************************************************/
void Refresh_Display_Time(void)
{	
	switch(dis_mode)
	{
		case 0:	//������ʾʱ��
				Time_Handle();	//��ȡʱ��������Ϣ
				LCD_Display_String_Num(64, 271, Display_Date, 16, 13, 13, RED, WHITH);	//��ʾ����
				LCD_Display_String_Num(88, 287, Display_Time, 16, 8, 8, RED, WHITH);	//��ʾʱ��
				break;
		
		case 1:	//������
				Display_Value[0] = 0x32;
				Display_Value[1] = 0x30;
				Display_Value[2] = (TimeValue.year >> 4) + 0x30;
				Display_Value[3] = (TimeValue.year & 0x0f) + 0x30;
//				LCD_Display_String_Num(64, 271, (u8*)Display_Value, 4);
				LCD_Display_String_Num(64, 271, (u8*)Display_Value, 16, 5, 4, RED, WHITH);
				break;
		
		case 2:	//������
				Display_Value[0] = (TimeValue.month >> 4) + 0x30;
				Display_Value[1] = (TimeValue.month & 0x0f) + 0x30;
//				LCD_Display_String_Num(104, 271, (u8*)Display_Value, 2);
				LCD_Display_String_Num(104, 271, (u8*)Display_Value, 16, 5, 2, RED, WHITH);
				break;
		
		case 3:	//������
				Display_Value[0] = (TimeValue.date >> 4) + 0x30;
				Display_Value[1] = (TimeValue.date & 0x0f) + 0x30;
//				LCD_Display_String_Num(128, 271, (u8*)Display_Value, 2);
				LCD_Display_String_Num(128, 271, (u8*)Display_Value, 16, 5, 2, RED, WHITH);
				break;
		
		case 4:	//��������
				Display_Value[0] = (TimeValue.week >> 4) + 0x30;
				Display_Value[1] = (TimeValue.week & 0x0f) + 0x30;
//				LCD_Display_String_Num(152, 271, (u8*)Display_Value, 2);
				LCD_Display_String_Num(152, 271, (u8*)Display_Value, 16, 5, 2, RED, WHITH);
				break;
		
		case 5:	//����Сʱ
				Display_Value[0] = (TimeValue.hour >> 4) + 0x30;
				Display_Value[1] = (TimeValue.hour & 0x0f) + 0x30;
//				LCD_Display_String_Num(88, 287, (u8*)Display_Value, 2);
				LCD_Display_String_Num(88, 271, (u8*)Display_Value, 16, 5, 2, RED, WHITH);
				break;
		
		case 6:	//��������
				Display_Value[0] = (TimeValue.minute >> 4) + 0x30;
				Display_Value[1] = (TimeValue.minute & 0x0f) + 0x30;
//				LCD_Display_String_Num(112, 287, (u8*)Display_Value, 2);
				LCD_Display_String_Num(112, 271, (u8*)Display_Value, 16, 5, 2, RED, WHITH);
				break;
		
		default:	//ȱʡֵ
				break;
	}
}
/**********************************************************
* �������� ---> ����������
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void Key_Handle(void)
{
	u8 Key_Value;	//��������
	
	Key_Value = KEY_Scan(0);	//��ȡ����ֵ
//	if(Key_Value == 0)	return;
	
	//�˵�����
	if(Key_Value == KEY_LEFT)
	{
		if(dis_mode == 6)	//����ʱ�����
		{
			dis_mode = 0;	//�ص���ʼ��ʾ
//			Point_color = RED;	//������ɫ
//			LCD_Draw_Char(10, 287, " ", 0);
			LCD_Display_String_BK(10, 0, "  ", 16, WHITH, WHITH);
			TimeValue.second = 0x00;	//����Ϊ0�����¿�ʼ����
			PCF8563_ReadWrite_Time(0);	//����ʱ����Ϣ
		}
		else
		{
//			Point_color = BLUE;	//������ɫ
//			LCD_Draw_Char(10, 287, "A", 0);
			LCD_Display_String_BK(10, 0, "Ad", 16, BLUE, WHITH);
			dis_mode++;
		}
	}
	
	//ȡ������
	if(Key_Value == KEY_RIGHT)
	{
		dis_mode = 0;
	}
	
	//�Ӱ���
	if(Key_Value == KEY_UP)
	{
		switch(dis_mode)
		{
			case 1: //������
					Time_Variable = HEX_to_BCD(TimeValue.year);	//ת����BCD
					if(Time_Variable == 99)	Time_Variable = 0;
					else Time_Variable++;

					TimeValue.year = BCD_to_HEX(Time_Variable);	//��������
					break;
			
			case 2:	//������
					Time_Variable = HEX_to_BCD(TimeValue.month);	//ת����BCD
					if(Time_Variable == 12)	Time_Variable = 1;
					else Time_Variable++;

					TimeValue.month = BCD_to_HEX(Time_Variable);	//��������
					break;
			
			case 3:	//������
					Time_Variable = HEX_to_BCD(TimeValue.date);	//ת����BCD
					if(Time_Variable == 31)	Time_Variable = 1;
					else Time_Variable++;

					TimeValue.date = BCD_to_HEX(Time_Variable);	//��������
					break;
			
			case 4:	//��������
					Time_Variable = HEX_to_BCD(TimeValue.week);	//ת����BCD
					if(Time_Variable == 7)	Time_Variable = 1;
					else Time_Variable++;

					TimeValue.week = BCD_to_HEX(Time_Variable);	//��������
					break;
			
			case 5:	//����Сʱ
					Time_Variable = HEX_to_BCD(TimeValue.hour);	//ת����BCD
					if(Time_Variable == 23)	Time_Variable = 0;
					else Time_Variable++;

					TimeValue.hour = BCD_to_HEX(Time_Variable);	//��������
					break;
			
			case 6:	//��������
					Time_Variable = HEX_to_BCD(TimeValue.minute);	//ת����BCD
					if(Time_Variable == 59)	Time_Variable = 0;
					else Time_Variable++;

					TimeValue.minute = BCD_to_HEX(Time_Variable);	//��������
					break;
			
			default:
					break;
		}
	}
	
	//������
	if(Key_Value ==KEY_DOWN)
	{
		switch(dis_mode)
		{
			case 1:	//������
					Time_Variable = HEX_to_BCD(TimeValue.year);	//ת����BCD
					if(Time_Variable == 0)	Time_Variable = 99;
					else Time_Variable--;

					TimeValue.year = BCD_to_HEX(Time_Variable);	//��������
					break;
			
			case 2:	//������
					Time_Variable = HEX_to_BCD(TimeValue.month);	//ת����BCD
					if(Time_Variable == 1)	Time_Variable = 12;
					else Time_Variable--;

					TimeValue.month = BCD_to_HEX(Time_Variable);	//��������
					break;
			
			case 3:	//������
					Time_Variable = HEX_to_BCD(TimeValue.date);	//ת����BCD
					if(Time_Variable == 1)	Time_Variable = 31;
					else Time_Variable--;

					TimeValue.date = BCD_to_HEX(Time_Variable);	//��������
					break;
			
			case 4:	//��������
					Time_Variable = HEX_to_BCD(TimeValue.week);	//ת����BCD
					if(Time_Variable == 1)	Time_Variable = 7;
					else Time_Variable--;

					TimeValue.week = BCD_to_HEX(Time_Variable);	//��������
					break;
			
			case 5:	//����Сʱ
					Time_Variable = HEX_to_BCD(TimeValue.hour);	//ת����BCD
					if(Time_Variable == 0)	Time_Variable = 23;
					else Time_Variable--;

					TimeValue.hour = BCD_to_HEX(Time_Variable);	//��������
					break;
			
			case 6:	//��������
					Time_Variable = HEX_to_BCD(TimeValue.minute);	//ת����BCD
					if(Time_Variable == 0)	Time_Variable = 59;
					else Time_Variable--;

					TimeValue.minute = BCD_to_HEX(Time_Variable);	//��������
					break;
			
			default:
					break;
		}
	}
}
