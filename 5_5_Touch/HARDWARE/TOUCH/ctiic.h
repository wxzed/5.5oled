#ifndef __MYCT_IIC_H
#define __MYCT_IIC_H
#include "sys.h"	    




  	   		   
//IO��������
#define CT_SDA_IN()  {GPIOB->CRH&=0XFFFFF0FF;GPIOB->CRH|=8<<4*2;}
#define CT_SDA_OUT() {GPIOB->CRH&=0XFFFFF0FF;GPIOB->CRH|=3<<4*2;}

//IO��������	 
#define CT_IIC_SCL    PBout(11) 			//SCL     
#define CT_IIC_SDA    PBout(10) 			//SDA	 
#define CT_READ_SDA   PBin(10)  			//����SDA 

//IIC���в�������
void CT_IIC_Init(void);                	//��ʼ��IIC��IO��				 
void CT_IIC_Start(void);				//����IIC��ʼ�ź�
void CT_IIC_Stop(void);	  				//����IICֹͣ�ź�
void CT_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 CT_IIC_Read_Byte(unsigned char ack);	//IIC��ȡһ���ֽ�
u8 CT_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void CT_IIC_Ack(void);					//IIC����ACK�ź�
void CT_IIC_NAck(void);					//IIC������ACK�ź�

#endif







