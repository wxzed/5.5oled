#ifndef __GT1151_H
#define __GT1151_H	
#include "sys.h"	


//IO��������	 
#define GT_RST    		PBout(0)	//GT1151��λ����
#define GT_INT    		PBin(1)		//GT1151�ж�����	
   	
 
//I2C��д����	
#define GT_CMD_WR 		0X28     	//д����
#define GT_CMD_RD 		0X29		//������
  
//GT1151 ���ּĴ������� 
#define GT_CTRL_REG 	0X8040   	//GT1151���ƼĴ���
#define GT_CFGS_REG 	0X8050   	//GT1151������ʼ��ַ�Ĵ���
#define GT_CHECK_REG 	0X813C   	//GT1151У��ͼĴ���
#define GT_PID_REG 		0X8140   	//GT1151��ƷID�Ĵ���

#define GT_GSTID_REG 	0X814E   	//GT1151��ǰ��⵽�Ĵ������
#define GT_TP1_REG 		0X814F  	//��һ�����������ݵ�ַ
#define GT_TP2_REG 		0X8157		//�ڶ������������ݵ�ַ
#define GT_TP3_REG 		0X815F		//���������������ݵ�ַ
#define GT_TP4_REG 		0X8167		//���ĸ����������ݵ�ַ
#define GT_TP5_REG 		0X816F		//��������������ݵ�ַ  
#define GT_TP6_REG 		0X8177
#define GT_TP7_REG 		0X817F
#define GT_TP8_REG 		0X8187
#define GT_TP9_REG 		0X818F
#define GT_TP10_REG 	0X8197
 
u8 GT1151_Send_Cfg(u8 mode);
u8 GT1151_WR_Reg(u16 reg,u8 *buf,u8 len);
void GT1151_RD_Reg(u16 reg,u8 *buf,u8 len); 
u8 GT1151_Init(void);
u8 GT1151_Scan(void); 
#endif













