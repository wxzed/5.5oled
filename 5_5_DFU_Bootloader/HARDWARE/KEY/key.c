/**********************************************************
* @ File name -> key.c
* @ Version   -> V1.0
* @ Date      -> 01-18-2014
* @ Brief     -> ������������

 V1.*
* @ Revise    ->
**********************************************************/

#include "key.h"

/**********************************************************
* �������� ---> KEY�ӿڳ�ʼ��
* ��ڲ��� ---> none
* ���ز��� ---> none 
* ����˵�� ---> none
**********************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);	//����GPIOC����ʱ��	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;	//��ʼ��GPIOx.2---------->KEY0---->PC.2
																						//��ʼ��GPIOx.3---------->KEY1---->PC.3
																						//��ʼ��GPIOx.4---------->KEY2---->PC.4
																						//��ʼ��GPIOx.5---------->KEY3---->PC.5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//����Ϊ��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	//��ʼ��GPIO��ؽṹ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//��ʼ��GPIOx.0---------->KEY_UP---->PA.0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//����Ϊ��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��GPIO��ؽṹ��
}

/**********************************************************
* �������� ---> ������ֵ��ȡ
* ��ڲ��� ---> mode���Ƿ�֧������
*                    0����֧������
*                    1��֧�����������������޸����޸ĺ궨�����ز�����
* ���ز��� ---> ���ذ���ֵ 
* ����˵�� ---> none
**********************************************************/
//u8 KEY_Scan(u8 mode)
//{
//	static u8 KeyStatus = 0;	//��̬���������水��״̬
//	static u8 KeyReturn = 0;	//��̬���������浱ǰ�����ļ�ֵ
//           u8 KeyPress;			//��ȡ��������ֵ

////	KeyPress = (u8)((((GPIOC->IDR & 0x0000003c)) >> 1) | (~(GPIOA->IDR & 0x00000001)));	//��ȡ����ֵ
//	
//	KeyPress = KeyInput & 0x1f;	//�õ�����ֵ
//  
//    switch(KeyStatus)
//    {
//		case 0:	//������ʼ״̬
//				if(KeyPress != 0x1f)	KeyStatus = 1;	//����а������£����밴��ȷ��״̬
//				break;

//		case 1:	//����ȷ��״̬
//				if(KeyPress == (KeyInput & 0x1f))	//����������
//				{
//					     if(KeyPress == 0x1d)	KeyReturn = KEY_LEFT;
//					else if(KeyPress == 0x1b)	KeyReturn = KEY_UP;
//					else if(KeyPress == 0x17)	KeyReturn = KEY_DOWN;
//					else if(KeyPress == 0x0f)	KeyReturn = KEY_RIGHT;
//					else if(KeyPress == 0x1e)	KeyReturn = KEY_WKUP;
//				
//					KeyStatus = 2; //������һ��״̬Ϊ�ͷź������ж�״̬
//				}
//				else	KeyStatus = 0;	//�����Ѿ�����
//				
//				break;

//	    case 2:	//�����ͷ�״̬
//				if(KeyPress == 0x1f)         //�����ͷ�
//				{
//					KeyStatus = 0;  //������һ��״̬Ϊ�ͷ�״̬
//				}
//				
//				/******************************************
//				              ��������ȷ�ϻ���
//				******************************************/
//				
//				if(mode)	//֧������
//				{
//					KeyReturn = KeyPress | Key_ContPress;	//���������İ���ֵ
//				}
//				
//				/*****************************************/
//				break; 
//				
//        default: break;
//    }

//	return KeyReturn;  //���ص�ǰ����
//}



u8 KEY_Scan(u8 mode)
{
	static u8 key_loo=1;	//�����ɿ���־
	if(mode)	key_loo = 1;	//�����ɿ�
	if(key_loo && (KEY0 == 0 || KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 1))	//��������
	{					
		delay_ms(10);	//��ʱ������
        key_loo = 0;
             if(KEY0 == 0)	return KEY_LEFT;
        else if(KEY1 == 0)	return KEY_UP;
        else if(KEY2 == 0)  return KEY_DOWN;
        else if(KEY3 == 0)  return KEY_RIGHT;
		else if(KEY4 == 1)	return KEY_WKUP;
	}
	else if(KEY0 == 1 && KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 0)	key_loo = 1;	//�����Ѿ��ͷ�	
		
	return 0;

}
 
