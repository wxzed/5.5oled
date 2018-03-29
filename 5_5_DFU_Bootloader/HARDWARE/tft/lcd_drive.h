/**********************************************************
* @ File name -> lcd_drive.h
* @ Version   -> V1.0.2
* @ Date      -> 03-09-2014
* @ Brief     -> TFT�����ײ���������ͷ�ļ�

 V1.0.1
* @ Revise    -> ��д���GPIO����LCD�����ĳ�ʼ������

 V1.0.2
* @ Revise    -> �Ż���ֲʱ�ķ����ԣ���������ֻ���޸������ĺ궨�弴��
**********************************************************/

#ifndef _lcd_drive_h_
#define _lcd_drive_h_

/**********************************************************
                     �ⲿ����ͷ�ļ�
         Ӧ�õ���ͬ������ͷ�ļ����������޸ļ���                        
**********************************************************/

#include "STM32_config.h"

/**********************************************************
           ������ͬSTM32���������л����ض���
**********************************************************/

#define _USE_WarShip				1	//1��ʹ��ս��������
                                        //0��ʹ��STM32F103VET6��Сϵͳ��

//ʹ�ò�ͬҺ��ʱ��ʹ�õĿ���
#define _H8bit_To_L8bit				0	//0����������8λ�͵�8λ
                                        //1������8λ�͵�8λ����

/**********************************************************
                    ����LCD��صĺ궨��
**********************************************************/

#define _USER_GPIO_OR_FSMC           1     /* ���Ʒ�ʽ��FSMC or GPIO */
                                          //0: ʹ����ͨ��GPIO����
                                          //1: ʹ��FSMC���ƣ����ƿ���NOR SRAMһ��

#define _LCD_RESET_Soft              0     /* LCD��λ����������ƻ���Ӳ�����ƶ��� */
                                          //0: ʹ��Ӳ�����Ƹ�λ
                                          //1: ʹ��������Ƹ�λ

/**********************************************************
                    ���屳��ӿ���Ϣ
               ��ͬ�Ŀ��ƽӿ����޸�����                       
**********************************************************/

#if _USE_WarShip	//��������ʹ�õ���ս��������

#define LCD_BLControl               PBout(0)    //�������

//                                      GPIOʱ��        GPIO��   ���ź�     ��ת�ٶ�       �������ģʽ
#define LCD_BLInit					RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_0,GPIO_Speed_2MHz,GPIO_Mode_Out_PP
/*
ȡֵ��Χ������Ӧ�궨��˳��
RCC_APB2Periph_GPIOA ~ RCC_APB2Periph_GPIOG,
GPIOA ~ GPIOG,
GPIO_Pin_0 ~ GPIO_Pin_15 and GPIO_Pin_All,
GPIO_Speed_2MHz��GPIO_Speed_10MHz��GPIO_Speed_50MHz,
GPIOMode_TypeDef�ṹ�巶Χ
*/

#else	//����ʹ��STM32F103VET6��Сϵͳ��

#define LCD_BLControl               PBout(1)    //�������

//                                      GPIOʱ��        GPIO��   ���ź�     ��ת�ٶ�       �������ģʽ
#define LCD_BLInit					RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_1,GPIO_Speed_2MHz,GPIO_Mode_Out_PP

#endif

/**********************************************************
             �����������λʱ�ܽŶ�����Ϣ
**********************************************************/
#if _LCD_RESET_Soft	//��������ʹ��IO����LCD��λ����

	#define LCD_RST							PBout(0)	/* ������Ƹ�λ�ܽŶ��塣����PAout(0) */

	//                                      GPIOʱ��        GPIO��   ���ź�     ��ת�ٶ�       �������ģʽ
	#define LCD_RSTInit					RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_0,GPIO_Speed_2MHz,GPIO_Mode_Out_PP
/*
ȡֵ��Χ������Ӧ�궨��˳��
RCC_APB2Periph_GPIOA ~ RCC_APB2Periph_GPIOG,
GPIOA ~ GPIOG,
GPIO_Pin_0 ~ GPIO_Pin_15 and GPIO_Pin_All,
GPIO_Speed_2MHz��GPIO_Speed_10MHz��GPIO_Speed_50MHz,
GPIOMode_TypeDef�ṹ�巶Χ
*/

#endif	//end _LCD_RESET_Soft
/* �����������LCD��λ�ܽ���Ϣ���� */

//=========================================================
#if !_USER_GPIO_OR_FSMC	//ֻ�п�����GPIO����ʱ��Ҫ�����������
//=========================================================
/* ���������궨��ֻ�п�����GPIO����LCD�ӿڲ�Ҫ���ã�Ӳ������FSMC�����Ƶ�ʱ�򲻹� */

	#define _USER_PSB_Soft               0     /* �Ƿ����������������λ��ѡ�񡾾��������ƽ��GPIO��ʼ��������ɡ� */
                                               //0: ʹ��Ӳ������
                                               //1: ʹ���������

	#define _USER_8BIT_16BIT             1     /* 8 or 16λ���ݽӿ�ѡ��FSMC�������߿���ɳ�ʼ���Ǿ����� */
                                               //0: ʹ��8bit��������
                                               //1: ʹ��16bit�������ߣ�Ĭ��ֵ

/* �����Ƕ����������ѡ���������߿�ȵ�IO��ʼ����Ϣ */
//=========================================================
#if _USER_PSB_Soft    //��������ʹ����������������߿��
//=========================================================
	
	#define LCD_PSB							PBout(1)	/* ������Ƹ�λ�ܽŶ��塣����PAout(0) */

	//                                      GPIOʱ��        GPIO��   ���ź�     ��ת�ٶ�       �������ģʽ
	#define LCD_PSBInit					RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_1,GPIO_Speed_2MHz,GPIO_Mode_Out_PP
/*
ȡֵ��Χ������Ӧ�궨��˳��
RCC_APB2Periph_GPIOA ~ RCC_APB2Periph_GPIOG,
GPIOA ~ GPIOG,
GPIO_Pin_0 ~ GPIO_Pin_15 and GPIO_Pin_All,
GPIO_Speed_2MHz��GPIO_Speed_10MHz��GPIO_Speed_50MHz,
GPIOMode_TypeDef�ṹ�巶Χ
*/

//=========================================================
#endif	//end _USER_PSB_Soft
//=========================================================

//=========================================================
#endif	//end _USER_GPIO_OR_FSMC
//=========================================================

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/

//=========================================================
#if _USER_GPIO_OR_FSMC    //������ʹ��FSMC����LCD
//========================================================= 

/**********************************************************
              FSMC Bank1�����������˵��
**********************************************************/
/*
 _______________________________________________________________________________
|  ��ѡ��Bank   | Ƭѡ�ź� |          ��ַ��Χ         | [27:26] |    [25:0]    |
|_______________|__________|___________________________|_________|______________|
| Bank1.region1 | FSMC_NE1 | 0x6000,0000 ~ 0x63ff,ffff |    00   |              |
|_______________|__________|___________________________|_________|              |
| Bank1.region2 | FSMC_NE2 | 0x6400,0000 ~ 0x67ff,ffff |    01   | FSMC_A[25:0] |
|_______________|__________|___________________________|_________|              |
| Bank1.region3 | FSMC_NE3 | 0x6800,0000 ~ 0x6bff,ffff |    10   |              |
|_______________|__________|___________________________|_________|              |
| Bank1.region4 | FSMC_NE4 | 0x6c00,0000 ~ 0x6fff,ffff |    11   |              |
|_______________|__________|___________________________|_________|______________|

*/ 
//���ӵĵ�ַ�ߵ�ַ����ڿ�������ַ��ƫ��������
//RAM��ַ = Bank(x).region(y) + 2 * (2^n)
//������˵��x������
//          y����������
//          n���ⲿ���ӵĵ�ַ�ߺ�

/* ���岻ͬ�Ŀ����޸�lcd_drive.c�����STM32_FSMC_Init()���������ɶ�FSMC��ʼ������ */

#if _USE_WarShip==1	//ʹ��ս��������
                                 
	/* ʹ�ò�ͬ�Ŀ��ƿ����ַ���޸����� */
	#define LCD_FSMC_REGION_BASE	(u32)0x6c000000	//FSMC����ƻ���ַ
	/* ����LCD_RS��ַ�߲�һ�����޸����� */
	#define LCD_RAM_ADDR_LINE		(u32)0x00000800	//LCD_RS����������FSMC��ַ��ƫ����

	/* �����������LCD��صĶ��壬�����޸� */
	#define LCD_REG_ADD				(u32)LCD_FSMC_REGION_BASE //�Ĵ�����ַ������,RS = 0
	#define LCD_DAT_ADD				(u32)(LCD_FSMC_REGION_BASE | LCD_RAM_ADDR_LINE) //��ʾ���ݵ�ַ����ʾRAM,RS = 1

	#define LCD_WR_REG(index)		((*(__IO u16 *)(LCD_REG_ADD)) = ((u16)index))	//дLCD�Ĵ���д����ֵindex
	#define LCD_WR_Data(val)		((*(__IO u16 *)(LCD_DAT_ADD)) = ((u16)val))		//��LCD��RAMд������val
	#define LCD_RD_Data()			(*(__IO u16 *)(LCD_DAT_ADD))	//��LCD��ȡ����

/* �����ǽṹ���ʵ�ַ��� */

//	typedef struct
//	{
//		u16 LCD_REG;	/* ��ַΪ��LCD_BASE */
//		u16 LCD_RAM;	/* ��ַΪ��LCD_BASE + 2 */
//	}LCD_TypeDef;
//
//	#define LCD_BASE				((u32)(0x6c000000 | 0x000007fe))	//FSMC����ƻ���ַ + LCD_RS����������FSMC��ַ��ƫ����
//	#define LCD						((LCD_TypeDef *)LCD_BASE)
//
//	#define LCD_WR_REG(index)		(LCD->LCD_REG = (u16)index)	//дLCD�Ĵ���д����ֵindex
//	#define LCD_WR_Data(val)		(LCD->LCD_RAM = (u16)val)	//��LCD��RAMд������val
//	#define LCD_RD_Data()			(u16)LCD->LCD_RAM			//��LCD��ȡ����

#else

	/* ʹ�ò�ͬ�Ŀ��ƿ����ַ���޸����� */
	#define LCD_FSMC_REGION_BASE	(u32)0x60000000	//FSMC����ƻ���ַ
	/* ����LCD_RS��ַ�߲�һ�����޸����� */
	#define LCD_RAM_ADDR_LINE		(u32)0x00020000	//LCD_RS����������FSMC��ַ��ƫ����

	/* �����������LCD��صĶ��壬�����޸� */
	#define LCD_REG_ADD				(u32)LCD_FSMC_REGION_BASE //�Ĵ�����ַ������,RS = 0
	#define LCD_DAT_ADD				(u32)(LCD_FSMC_REGION_BASE | LCD_RAM_ADDR_LINE) //��ʾ���ݵ�ַ����ʾRAM,RS = 1

	#define LCD_WR_REG(index)		((*(__IO u16 *)(LCD_REG_ADD)) = ((u16)index))	//дLCD�Ĵ���д����ֵindex
	#define LCD_WR_Data(val)		((*(__IO u16 *)(LCD_DAT_ADD)) = ((u16)val))		//��LCD��RAMд������val
	#define LCD_RD_Data()			(*(__IO u16 *)(LCD_DAT_ADD))	//��LCD��ȡ����

/* �����ǽṹ���ʵ�ַ��� */

//	typedef struct
//	{
//		u16 LCD_REG;	/* ��ַΪ��LCD_BASE */
//		u16 LCD_RAM;	/* ��ַΪ��LCD_BASE + 2 */
//	}LCD_TypeDef;
//
//	#define LCD_BASE				((u32)(0x60000000 | 0x0001fffe))	//FSMC����ƻ���ַ + LCD_RS����������FSMC��ַ��ƫ����
//	#define LCD						((LCD_TypeDef *)LCD_BASE)
//
//	#define LCD_WR_REG(index)		(LCD->LCD_REG = (u16)index)	//дLCD�Ĵ���д����ֵindex
//	#define LCD_WR_Data(val)		(LCD->LCD_RAM = (u16)val)	//��LCD��RAMд������val
//	#define LCD_RD_Data()			(u16)LCD->LCD_RAM			//��LCD��ȡ����

#endif

//=========================================================
#else //����ʹ��GPIO����
//=========================================================

	/* ������Ҫ��дlcd_drive.c�����LCD_GPIO_Init()����������IO�ĳ�ʼ������ */

	/* ����ʵ�����ӽ��и�д����궨�� */
	//                                      GPIOʱ��        GPIO��   ���ź�     ��ת�ٶ�       �������ģʽ
	#define LCD_RSInit					RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_0,GPIO_Speed_50MHz,GPIO_Mode_Out_PP
	#define LCD_CSInit					RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_0,GPIO_Speed_50MHz,GPIO_Mode_Out_PP
	#define LCD_RDInit					RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_0,GPIO_Speed_50MHz,GPIO_Mode_Out_PP
	#define LCD_WRInit					RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_0,GPIO_Speed_50MHz,GPIO_Mode_Out_PP

	#define LCD_RS						PAout(0)	/* LCD���� <---> ����ѡ������߶��壬����PAout(0) */
	#define LCD_CS						PAout(0)	/* LCDƬѡ�����߶��壬����PAout(0) */
	#define LCD_RD						PAout(0)	/* LCD�������߶��壬����PAout(0) */
	#define LCD_WR						PAout(0)	/* LCDд�����߶��壬����PAout(0) */

	//===================================================
	#if _USER_8BIT_16BIT  //�����������ʹ�õ���16bit����
	//===================================================

	/* ����ʵ�����ӵ��������߽�������궨��ĸ�д */

	#define LCD_BUS_DBout16(val)    /* LCDд�����ݺ��������磺GPIO_Write(GPIOB, (uint16_t)val) */
	#define LCD_BUS_DBin16()        /* LCD��ȡ���ݺ��������磺(uint16_t)GPIO_ReadInputData(GPIOB) */

	/* ��ȡ����ʱ�����������߷���Ϊ���� */
	/* ���磺#define LCD_BUS_IN16()	{GPIOB->CRL &= 0xffffffff;GPIOB->CRL |= 0x88888888;GPIOB->CRH &= 0xffffffff;GPIOB->CRH |= 0x88888888;} */
	#define LCD_BUS_IN16()

	/* д������ʱ�����������߷���Ϊ��� */
	/* ���磺#define LCD_BUS_OUT16()	{GPIOB->CRL &= 0xffffffff;GPIOB->CRL |= 0x33333333;GPIOB->CRH &= 0xffffffff;GPIOB->CRH |= 0x33333333;} */
	#define LCD_BUS_OUT16()

	//===================================================
	#else //����ʹ��8bit����
	//===================================================

	/* ����ʵ�����ӵ��������߽�������궨��ĸ�д */

	#define LCD_BUS_DBout8(val)    /* LCDд�����ݺ��������磺GPIO_Write(GPIOB, (uint8_t)val) */
	#define LCD_BUS_DBin8()        /* LCD��ȡ���ݺ��������磺(uint8_t)GPIO_ReadInputData(GPIOB) */

	/* ��ȡ����ʱ�����������߷���Ϊ���� */
	/* ���磺#define LCD_BUS_IN8()	{GPIOB->CRL &= 0xffffffff;GPIOB->CRL |= 0x88888888;} */
	#define LCD_BUS_IN8()
	 
	/* д������ʱ�����������߷���Ϊ��� */
	/* ���磺#define LCD_BUS_OUT8()	{GPIOB->CRL &= 0xffffffff;GPIOB->CRL |= 0x33333333;} */
	#define LCD_BUS_OUT8()

	//===================================================
	#endif	//end _USER_8BIT_16BIT
	//===================================================

//=========================================================
#endif	//end _USER_GPIO_OR_FSMC
//=========================================================

/**********************************************************
                       �ⲿ���ܺ���                      
**********************************************************/

#if _USER_GPIO_OR_FSMC      //��������ʹ��FSMC����

void STM32_FSMC_Init(void);   //FSMC��ʼ��

#else	//ʹ��GPIO����ʱ�������溯��

void LCD_GPIO_Init(void);     //ͨѶ�����ƶ˿ڳ�ʼ��

#endif	//end _USER_GPIO_OR_FSMC

/* ����ͨ��������� */
void LCD_RST_BL_Init(void);   //���⡢��λ���ƶ˿ڳ�ʼ��     
void LCD_Write_Register(u16 reg_val);     //д���ݵ�LCD�Ĵ�������Ҫд�����Ĵ�����ַ��
void LCD_Write_Data(u16 dat); //д���ݵ�LCD RAM
u16 LCD_Read_Data(void);      //��ȡLCD����
void LCD_WriteRegisterData(u16 reg, u16 dat);   //��LCDĳ���Ĵ���д������
u16 LCD_ReadRegisterData(u16 reg);  //��ȡLCDĳ���Ĵ�����ֵ


#endif

