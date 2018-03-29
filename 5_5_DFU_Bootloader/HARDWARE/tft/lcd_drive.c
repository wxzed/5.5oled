/**********************************************************
* @ File name -> lcd_drive.c
* @ Version   -> V1.0.2
* @ Date      -> 03-09-2014
* @ Brief     -> TFT�����ײ���������

 V1.0.1
* @ Revise    -> ��д���GPIO����LCD�����ĳ�ʼ������

 V1.0.2
* @ Revise    -> �Ż���ֲʱ�ķ����ԣ���������ֻ���޸������ĺ궨�弴��
**********************************************************/

#include "lcd_drive.h"

/**********************************************************
                 |                        |
                 |  ���������ֲʱ�޸���  |
                \|/                      \|/
**********************************************************/

//=========================================================
#if _USER_GPIO_OR_FSMC      //��������ʹ��FSMC����LCD
//========================================================

/**********************************************************
* �������� ---> FSMC��ʼ��
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void STM32_FSMC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
      
	FSMC_NORSRAMInitTypeDef FSMC_TFTLCD_InitStructure;

	FSMC_NORSRAMTimingInitTypeDef ReadWrite_Time;
	FSMC_NORSRAMTimingInitTypeDef Write_Time;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//ʹ��FSMCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//��������ʱ��

#if _USE_WarShip==1	//��������ʹ�õ���ս��������

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | \
	                        RCC_APB2Periph_GPIOG, ENABLE);	//��������ʱ��

	/*	��ʼ��GPIODΪ�����������	*/
	/*	PD.0ΪFSMC_D2    PD.1ΪFSMC_D3	*/
	/*	PD.4ΪFSMC_NOE    PD.5ΪFSMC_NWE	*/
	/*	PD.8ΪFSMC_D13    PD.9ΪFSMC_D14    PD.10ΪFSMC_D15	*/
	/*	PD.14ΪFSMC_D0    PD.15ΪFSMC_D1	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | \
	                              GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//GPIO��ת�ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//����Ϊ�����������

	GPIO_Init(GPIOD, &GPIO_InitStructure);	//��ʼ��GPIO��ؽṹ��

	/*	��ʼ��GPIOEΪ�����������	*/
	/*	PE.7ΪFSMC_D4    PE.8ΪFSMC_D5	*/
	/*	PE.9ΪFSMC_D6    PE.10ΪFSMC_D7	*/
	/*	PE.11ΪFSMC_D8    PE.12ΪFSMC_D9	*/
	/*	PE.13ΪFSMC_D10    PE.14ΪFSMC_D11	*/
	/*	PE.5ΪFSMC_D12	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | \
	                              GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//GPIO��ת�ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//����Ϊ�����������

	GPIO_Init(GPIOE, &GPIO_InitStructure);	//��ʼ��GPIO��ؽṹ��

	/*	��ʼ��GPIOGΪ�����������	*/
	/*	PG.0ΪFSMC_A10    PG.12ΪFSMC_NE4	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//GPIO��ת�ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//����Ϊ�����������

	GPIO_Init(GPIOG, &GPIO_InitStructure);	//��ʼ��GPIO��ؽṹ��
	
	/*	��ʼ��FSMC����Ĵ���	*/

	/*	��ʼ����дʱ��	*/
	ReadWrite_Time.FSMC_AddressSetupTime = 0x01;	//FSMC��ַ����ʱ��Ϊ2��HCLKʱ�ӣ�1/36MHz = 27ns
	ReadWrite_Time.FSMC_AddressHoldTime = 0x00;	//��ַ����ʱ�䣬ģʽAδ�õ�
	ReadWrite_Time.FSMC_DataSetupTime = 0xff;	//���ݽ���ʱ��Ϊ16��HCLK
	ReadWrite_Time.FSMC_BusTurnAroundDuration = 0x00;	//
	ReadWrite_Time.FSMC_CLKDivision = 0x00;	//
	ReadWrite_Time.FSMC_DataLatency = 0x00;	//
	ReadWrite_Time.FSMC_AccessMode = FSMC_AccessMode_A;	//ѡ��ģʽA

	/*	��ʼ��дʱ��	*/
	Write_Time.FSMC_AddressSetupTime = 0x00;	//��ַ����ʱ��Ϊ1��HCLK
	Write_Time.FSMC_AddressHoldTime = 0x00;	//��ַ����ʱ�䣬ģʽAδ�õ�
	Write_Time.FSMC_DataSetupTime = 0x03;	//���ݽ���ʱ���ڵ��ĸ�HCLKʱ�ӿ�ʼ
	Write_Time.FSMC_BusTurnAroundDuration = 0x00;	//
	Write_Time.FSMC_CLKDivision = 0x00;	//
	Write_Time.FSMC_DataLatency = 0x00;	//
	Write_Time.FSMC_AccessMode = FSMC_AccessMode_A;	//ѡ��ģʽA	
	
	FSMC_TFTLCD_InitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;	//��bank1����4
	FSMC_TFTLCD_InitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;	//��ַ/���ݵ�ַ���ù��ܹر�
	FSMC_TFTLCD_InitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;	//��Һ����ʾ��������SRAM��
	FSMC_TFTLCD_InitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;	//ѡ�����ݿ��Ϊ16bit
	FSMC_TFTLCD_InitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;	//���鴫��ģʽ�ر�
	FSMC_TFTLCD_InitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;	// 
	FSMC_TFTLCD_InitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;	//�͵�ƽ�ı����� 
	FSMC_TFTLCD_InitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;	//
	FSMC_TFTLCD_InitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;	//
	FSMC_TFTLCD_InitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//��д����
	FSMC_TFTLCD_InitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;	//����NWAIT�źţ�������������ȴ��ź�
	FSMC_TFTLCD_InitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;	//����չģʽ�������дʹ�ò�ͬ��ʱ��
	FSMC_TFTLCD_InitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;	//д����ʼ�մ����첽ģʽ

	FSMC_TFTLCD_InitStructure.FSMC_ReadWriteTimingStruct = &ReadWrite_Time;	//��дʱ��
	FSMC_TFTLCD_InitStructure.FSMC_WriteTimingStruct = &Write_Time;	//дʱ��
	
	FSMC_NORSRAMInit(&FSMC_TFTLCD_InitStructure);	//��ʼ��FSMC����

	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);	//ʹ��FSMC��1������
	
#else	//ʹ�õ���STM32F103VET6��Сϵͳ����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);	//��������ʱ��

	/*	��ʼ��GPIODΪ�����������	*/
	/*	PD.0ΪFSMC_D2    PD.1ΪFSMC_D3	*/
	/*	PD.4ΪFSMC_NOE    PD.5ΪFSMC_NWE	*/
	/*	PD.7ΪFSMC_NE1	*/
	/*	PD.8ΪFSMC_D13    PD.9ΪFSMC_D14    PD.10ΪFSMC_D15	*/
	/*	PD.11ΪFSMC_A16	*/
	/*	PD.14ΪFSMC_D0    PD.15ΪFSMC_D1	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8 | \
	                              GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//GPIO��ת�ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//����Ϊ�����������

	GPIO_Init(GPIOD, &GPIO_InitStructure);	//��ʼ��GPIO��ؽṹ��

	/*	��ʼ��GPIOEΪ�����������	*/
	/*	PE.7ΪFSMC_D4    PE.8ΪFSMC_D5	*/
	/*	PE.9ΪFSMC_D6    PE.10ΪFSMC_D7	*/
	/*	PE.11ΪFSMC_D8    PE.12ΪFSMC_D9	*/
	/*	PE.13ΪFSMC_D10    PE.14ΪFSMC_D11	*/
	/*	PE.5ΪFSMC_D12	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | \
	                              GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//GPIO��ת�ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//����Ϊ�����������

	GPIO_Init(GPIOE, &GPIO_InitStructure);	//��ʼ��GPIO��ؽṹ��

	
	/*	��ʼ��FSMC����Ĵ���	*/

	/*	��ʼ����дʱ��	*/
	ReadWrite_Time.FSMC_AddressSetupTime = 0x01;	//FSMC��ַ����ʱ��Ϊ2��HCLKʱ�ӣ�1/36MHz = 27ns
	ReadWrite_Time.FSMC_AddressHoldTime = 0x00;	//��ַ����ʱ�䣬ģʽAδ�õ�
	ReadWrite_Time.FSMC_DataSetupTime = 0xff;	//���ݽ���ʱ��Ϊ16��HCLK
	ReadWrite_Time.FSMC_BusTurnAroundDuration = 0x00;	//
	ReadWrite_Time.FSMC_CLKDivision = 0x00;	//
	ReadWrite_Time.FSMC_DataLatency = 0x00;	//
	ReadWrite_Time.FSMC_AccessMode = FSMC_AccessMode_A;	//ѡ��ģʽA

	/*	��ʼ��дʱ��	*/
	Write_Time.FSMC_AddressSetupTime = 0x00;	//��ַ����ʱ��Ϊ1��HCLK
	Write_Time.FSMC_AddressHoldTime = 0x00;	//��ַ����ʱ�䣬ģʽAδ�õ�
	Write_Time.FSMC_DataSetupTime = 0x03;	//���ݽ���ʱ���ڵ��ĸ�HCLKʱ�ӿ�ʼ
	Write_Time.FSMC_BusTurnAroundDuration = 0x00;	//
	Write_Time.FSMC_CLKDivision = 0x00;	//
	Write_Time.FSMC_DataLatency = 0x00;	//
	Write_Time.FSMC_AccessMode = FSMC_AccessMode_A;	//ѡ��ģʽA	
	
	FSMC_TFTLCD_InitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;	//��bank1����1
	FSMC_TFTLCD_InitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;	//��ַ/���ݵ�ַ���ù��ܹر�
	FSMC_TFTLCD_InitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;	//��Һ����ʾ��������SRAM��
	FSMC_TFTLCD_InitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;	//ѡ�����ݿ��Ϊ16bit
	FSMC_TFTLCD_InitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;	//���鴫��ģʽ�ر�
	FSMC_TFTLCD_InitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;	// 
	FSMC_TFTLCD_InitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;	//�͵�ƽ�ı����� 
	FSMC_TFTLCD_InitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;	//
	FSMC_TFTLCD_InitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;	//
	FSMC_TFTLCD_InitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//��д����
	FSMC_TFTLCD_InitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;	//����NWAIT�źţ�������������ȴ��ź�
	FSMC_TFTLCD_InitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;	//����չģʽ�������дʹ�ò�ͬ��ʱ��
	FSMC_TFTLCD_InitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;	//д����ʼ�մ����첽ģʽ

	FSMC_TFTLCD_InitStructure.FSMC_ReadWriteTimingStruct = &ReadWrite_Time;	//��дʱ��
	FSMC_TFTLCD_InitStructure.FSMC_WriteTimingStruct = &Write_Time;	//дʱ��
	
	FSMC_NORSRAMInit(&FSMC_TFTLCD_InitStructure);	//��ʼ��FSMC����

	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);	//ʹ��FSMC��1��һ��

#endif
}

//=========================================================
#else //����ʹ����ͨGPIO����LCD
//=========================================================

/**********************************************************
* �������� ---> ͨѶ�����ƶ˿ڳ�ʼ��
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void LCD_GPIO_Init(void)
{
	/* ��дGPIO����LCD�����ߵȳ�ʼ������ */
	STM32_GPIOx_Init(LCD_RSInit);	//RS
	STM32_GPIOx_Init(LCD_CSInit);	//CS
	STM32_GPIOx_Init(LCD_RDInit);	//RD
	STM32_GPIOx_Init(LCD_WRInit);	//WR

	/* ��дGPIO����LCD�������߹ܽų�ʼ������ */

#if _USER_8BIT_16BIT  //�����������ʹ�õ���16bit����
	/* ��д16bits���ݿ�ȳ�ʼ������ */

#else	//����ʹ��8bit����
	/* ��д8bits���ݿ�ȳ�ʼ������ */

#endif
}
/**********************************************************
* �������� ---> д���ݵ�����
* ��ڲ��� ---> val��Ҫд�������
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void LCD_Write_Bus(u16 val)
{
#if _USER_8BIT_16BIT  //��������ʹ��16bit����
/* ʹ��16bits�������߿��ʱ���߼�����ʱ�� */

	LCD_BUS_OUT16();	//��������Ϊ�������
	LCD_BUS_DBout16(val);	//��������
	LCD_WR = 0;
	LCD_WR = 1;

#else //����ʹ��8bit����
/* ʹ��8bits�������߿��ʱ���߼�����ʱ�� */

	LCD_BUS_OUT8();	//��������Ϊ�������
	LCD_BUS_DBout8((u8)(val/256));	//�ȷ��͸�8λ
	LCD_WR = 0;
	LCD_WR = 1;
	LCD_BUS_DBout8((u8)(val%256));	//�ٷ��͵�8λ
	LCD_WR = 0;
	LCD_WR = 1;

#endif
}
/**********************************************************
* �������� ---> ��ȡLCD����
* ��ڲ��� ---> none
* ������ֵ ---> ��ȡ��������
* ����˵�� ---> none
**********************************************************/
u16 LCD_Read_Data(void)
{
	u16 temp=0;	//�������ݻ���

#if _USER_8BIT_16BIT  //��������ʹ��16bit����
/* ʹ��16bits�������߿��ʱ���߼�����ʱ�� */

	LCD_BUS_IN16();	//��������Ϊ���뷽��
	LCD_WR = 1;
	LCD_RS = 1;	//��ȡ����
	LCD_RD = 1;
	LCD_RD = 0;	//����ʱ��
	LCD_RD = 1;
	temp = LCD_BUS_DBin16();

#else //����ʹ��8bit����
/* ʹ��8bits�������߿��ʱ���߼�����ʱ��ʵ�ʽ��������������λ */

	LCD_BUS_IN8();	//��������Ϊ���뷽��
	LCD_WR = 1;
	LCD_RS = 1;	//��ȡ����
	LCD_RD = 1;
	LCD_RD = 0;	//����ʱ��
	LCD_RD = 1;
	temp = (LCD_BUS_DBin8() & 0x00ff)<<8;	//���Ƚ��յ����Ǹ�8λ
	LCD_RD = 0;	//����ʱ��
	LCD_RD = 1;
	temp |= (LCD_BUS_DBin8() & 0x00ff);	//�ٽ��յ�8λ	

#endif

	return temp;	//���ض�ȡ��������     
}

//=========================================================
#endif	//end _USER_GPIO_OR_FSMC
//=========================================================

/**********************************************************
                /|\                   /|\
                 |   ������ֲʱ�޸���  |
				 |                     |
**********************************************************/

/**********************************************************
* �������� ---> ���⡢��λ���ƶ˿ڳ�ʼ��
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void LCD_RST_BL_Init(void)
{
	/******����ܽ�******/

	STM32_GPIOx_Init(LCD_BLInit);

	/******************************************************
	         ����������ѡ������Ҫ��д�ĳ�ʼ������
	******************************************************/

	/******��λ�ܽ�******/
      
#if _LCD_RESET_Soft   //��������ʹ��������Ƹ�λ����ʼ���˿�

	/* ��д����LCD_RST�ܽų�ʼ������ */
	STM32_GPIOx_Init(LCD_RSTInit);
	LCD_RST = 1;	//����ߵ�ƽ����ΪLCD�͵�ƽ������λ

#endif

	/******�������߿��ѡ��ܽ�******/

#if _USER_PSB_Soft    //��������ʹ����������������߿��ѡ��

	/* ��д��ʼ��LCD�������߿�ȿ����߳�ʼ������ */
	STM32_GPIOx_Init(LCD_PSBInit);

#if _USER_8BIT_16BIT /* ��������ʹ��16bit */
	LCD_PSB = 0;	//����͵�ƽ��Ĭ��Ϊ16bits���ݿ��

#else	/* ����ʹ��8bit */
	LCD_PSB = 1;	//����ߵ�ƽ

#endif	//end _USER_8BIT_16BIT

#endif	//end _USER_PSB_Soft
}

/**********************************************************
        ��������ص�Ӧ�õ���API�������Ǳ�Ҫ�ɲ��޸�
**********************************************************/


//=========================================================
#if _USER_GPIO_OR_FSMC      //��������ʹ��FSMC����LCD
//=========================================================

/**********************************************************
* �������� ---> д���ݵ�LCD�Ĵ�������Ҫд�����Ĵ�����ַ��
* ��ڲ��� ---> reg_val��Ҫд�������
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void LCD_Write_Register(u16 reg_val)
{
#if _H8bit_To_L8bit	//�����ߵ�8λ

	u16 temp;
	temp = (reg_val%256)<<8;
	reg_val = (reg_val>>8) | temp;

#endif

	LCD_WR_REG(reg_val);
}
/**********************************************************
* �������� ---> д���ݵ�LCD RAM
* ��ڲ��� ---> dat��Ҫд�������
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void LCD_Write_Data(u16 dat)
{
#if _H8bit_To_L8bit	//�����ߵ�8λ

	u16 temp;
	temp = (dat%256)<<8;
	dat = (dat>>8) | temp;

#endif

	LCD_WR_Data(dat);
}
/**********************************************************
* �������� ---> ��ȡLCD����
* ��ڲ��� ---> none
* ������ֵ ---> ��ȡ��������
* ����˵�� ---> none
**********************************************************/
u16 LCD_Read_Data(void)
{
#if _H8bit_To_L8bit	//�����ߵ�8λ

	u16 temp1,temp2;
	temp1 = LCD_RD_Data();
	temp2 = (temp1%256)<<8;
	temp2 = (temp1>>8) | temp2;
	return temp2;

#else

	return (u16)LCD_RD_Data();

#endif
}
/**********************************************************
* �������� ---> ��LCDĳ���Ĵ���д������
* ��ڲ��� ---> reg��Ҫд�����ݵļĴ�����ַ
*               dat��Ҫд�������
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void LCD_WriteRegisterData(u16 reg, u16 dat)
{
	LCD_Write_Register(reg);
	LCD_Write_Data(dat);

//	LCD_WR_REG(reg);
//	LCD_WR_Data(dat);
}
/**********************************************************
* �������� ---> ��ȡLCDĳ���Ĵ�����ֵ
* ��ڲ��� ---> reg��Ҫ��ȡ���ݵļĴ�����ַ
* ������ֵ ---> �Ĵ�����ֵ
* ����˵�� ---> none
**********************************************************/
u16 LCD_ReadRegisterData(u16 reg)
{
	LCD_Write_Register(reg);
	delay_us(5);
	return LCD_Read_Data();
}

//=========================================================
#else //����ʹ����ͨGPIO����LCD
//=========================================================

/**********************************************************
* �������� ---> д���ݵ�LCD�Ĵ�������Ҫд�����Ĵ�����ַ��
* ��ڲ��� ---> reg_val��Ҫд�������
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void LCD_Write_Register(u16 reg_val)
{
	LCD_RS = 0; //���͵���COM Address
	LCD_RD = 1;
	LCD_Write_Bus(reg_val);
	LCD_RS = 1;      
}
/**********************************************************
* �������� ---> д���ݵ�LCD RAM
* ��ڲ��� ---> dat��Ҫд�������
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void LCD_Write_Data(u16 dat)
{
	LCD_RS = 1; //���͵���data
	LCD_RD = 1;
	LCD_Write_Bus(reg_val);
}
/**********************************************************
* �������� ---> ��LCDĳ���Ĵ���д������
* ��ڲ��� ---> reg��Ҫд�����ݵļĴ�����ַ
*               dat��Ҫд�������
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void LCD_WriteRegisterData(u16 reg, u16 dat)
{
	LCD_CS = 0;
	LCD_Write_Register(reg);
	LCD_Write_Data(dat);
	LCD_CS = 1;
}
/**********************************************************
* �������� ---> ��ȡLCDĳ���Ĵ�����ֵ
* ��ڲ��� ---> reg��Ҫ��ȡ���ݵļĴ�����ַ
* ������ֵ ---> �Ĵ�����ֵ
* ����˵�� ---> none
**********************************************************/
u16 LCD_ReadRegisterData(u16 reg)
{
	u16 tem;
      
	LCD_CS = 0;
	LCD_Write_Register(reg);
	delay_us(5);
	tem = LCD_Read_Data();
	LCD_CS = 1;
	return tem;
}

//=========================================================
#endif	//end _USER_GPIO_OR_FSMC
//=========================================================

