/**********************************************************
* @ File name -> hw_iic.h
* @ Version   -> V1.0
* @ Date      -> 02-19-2014
* @ Brief     -> STM32Ӳ��IIC��������ͷ�ļ�

 V1.
* @ Revise    -> 
**********************************************************/

#ifndef _hw_iic_h_
#define _hw_iic_h_

/**********************************************************
                     �ⲿ����ͷ�ļ�                        
**********************************************************/

#include "stm32f10x_i2c.h"

#include "STM32_config.h"

/**********************************************************
               IIC�������̵��д�����Ϣ����
**********************************************************/

extern u8 IIC_RES_STA;

#define IIC_START_ERROR					0x01	//������ʼλ����
#define IIC_SLAVE_ERROR					0x02	//�豸��ַ + д�������
#define IIC_READ_ERROR					0x04	//�豸��ַ + ���������
#define IIC_ADD_H_ERROR					0x08	//16λ��ַʱ���͸�8λ����
#define IIC_ADD_L_ERROR					0x10	//16λ��ַʱ���͵�8λ����
#define IIC_DATA_ERROR					0x20	//����/�������ݴ���
#define IIC_ACK_ERROR					0x40	//Ӧ�����

#define IIC_NoBUSY						(u8)0	//���߿���
#define IIC_BUSYING						(u8)1	//����æ

/**********************************************************
                     �ⲿ���ܺ���
**********************************************************/

u8 IIC_Check_Busy(void);	//�������״̬

void STM32hwIIC_Init(u8 Slave_Add);	//IIC�ӿڳ�ʼ��

u8 STM32hw_Write_nByte(u8 Slave_Add, u16 Data_Add, u8 *Bytebuff, u8 len, u8 mode);	//IICд������

u8 STM32hw_Read_nByte(u8 Slave_Add, u16 Data_Add, u8 *Bytebuff, u8 len, u8 mode);	//IIC��ȡ����


#endif

