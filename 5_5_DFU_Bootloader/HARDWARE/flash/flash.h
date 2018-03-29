/**********************************************************
* @ File name -> flash.h
* @ Version   -> V1.0
* @ Date      -> 06-03-2014
* @ Brief     -> SPI Flash��������ͷ�ļ�
**********************************************************/

#ifndef _flash_h_
#define _flash_h_

/**********************************************************
                     �ⲿ����ͷ�ļ�
         Ӧ�õ���ͬ������ͷ�ļ����������޸ļ���                        
**********************************************************/

#include "STM32_config.h"

#include "spi.h"

/**********************************************************
                      ����ģ��ӿ�
                ���ӵ�IO��һ�����޸�����
    ����IO�ķ������޸�������ڲ��Ǿ���׼˫��IO��MCU
**********************************************************/

#define	SPI_FLASH_CS			PBout(12)  //FLASHƬѡ����

/**********************************************************
                        оƬID�궨��
**********************************************************/

//W25Qxxϵ��FlashоƬID
#define W25Q80					0xef13 	
#define W25Q16					0xef14
#define W25Q32					0xef15
#define W25Q64					0xef16
#define W25Q128					0xef17

extern u16 SPI_FLASH_TYPE;  //SPI Flash����
extern u16 SPI_Flash_ID;	//�豸ID����

/**********************************************************
                     ����Ͳ����궨��
**********************************************************/

//ָ��궨��
#define W25Qxx_Write_Enable					0x06
#define W25Qxx_Write_Disable				0x04
#define W25Qxx_Read_Status_Register			0x05
#define W25Qxx_Read_Status_Register2		0x35
#define W25Qxx_Write_Status_Register		0x01
#define W25Qxx_Page_Program					0x02
#define W25Qxx_Quad_Page_Program			0x32
#define W25Qxx_Erase_Block64Kb				0xd8	//����һ���飬64Kb
#define W25Qxx_Erase_Block32Kb				0x52	//����һ���飬32Kb
#define W25Qxx_Erase_Sector4Kb				0x20	//����һ��������4Kb
#define W25Qxx_Erase_Chip					0xc7	//or 0x60
#define W25Qxx_Erase_Suspend				0x75
#define W25Qxx_Erase_Resume					0x7a
#define W25Qxx_Power_Down					0xb9	//�������ģʽ
#define W25Qxx_High_Performance_Mode		0xa3	//����
#define W25Qxx_Continuous_Read_Mode			0xff	//Reset
#define W25Qxx_Release_Power_Down			0xab	//or HPM/Device ID��ID7 ~ ID0
#define W25Qxx_Manufacturer					0x90	//or Device ID��ID7 ~ ID0
#define W25Qxx_Read_Unique_ID				0x4b
#define W25Qxx_Jedec_ID						0x9f	//ID15 ~ ID0

#define W25Qxx_Read_Data					0x03
#define W25Qxx_Fast_Data					0x0b
#define W25Qxx_Fast_Read_Dual_Output		0x3b
#define W25Qxx_Fast_Read_Dual_IO			0xbb
#define W25Qxx_Fast_Read_Quad_Output		0x6b
#define W25Qxx_Fast_Read_Quad_IO			0xeb
#define W25Qxx_Octal_Word_Read_Quad_IO		0xe3

/**********************************************************
                        �ⲿ���ܺ���
**********************************************************/

void FlashSPI_CS_Init(void);	//GPIO��ʼ��

u16 Flash_Read_ID(void);	//��ȡW25Qxx���кţ�ID��

void Flash_Enter_Power_Down(void);	//W25Qxx�������ģʽ
void Flash_Exit_Power_Down(void);	//����W25Qxx

void Flash_Wait_Busy(void);	//�ȴ�W25Qxxæ��

u8 Flash_Read_Register_Status(u8 REG_CMD);	//W25Qxx��ȡ״̬�Ĵ���
void Flash_Write_Register_Status(u8 RS_dat);	//W25Qxxд״̬�Ĵ���

void Flash_EraseSector(u32 Sectoradd,u8 ORD);	//W25Qxx����һ������4K�����߲���һ����
void Flash_EraseChip(void);	//W25Qxx��������оƬ

void Flash_Write_Sector(u32 writeadd,u8* wbuffer,u16 sum);	//W25Qxxд��������
void Flash_Write_Page(u32 startadd,u8* wbuffer,u16 num);	//W25Qxx��һ��ҳ��д������

void Flash_ReadData(u8* rbuffer,u32 startadd,u16 num);	//W25Qxx������
void Flash_WriteData(u8* pdbuffer,u32 writeadd,u16 sum);	//W25Qxxд����


#endif

/*****************************************************
����W25Q64BVоƬ�������ṹͼ

8M�ֽڷ�Ϊ128����-->16����/��-->16ҳ/������ҳ�Ĵ�СΪ256

*****************************************************/

