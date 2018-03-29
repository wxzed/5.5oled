/**********************************************************
* @ File name -> stmflash.c
* @ Version   -> V1.0
* @ Date      -> 03-08-2014
* @ Brief     -> STM32�ڲ�FLASH��������

 V1.
* @ Revise    ->
**********************************************************/

#include "stmflash.h"

/**********************************************************
* �������� ---> STM32 FLASH��ȡ����
* ��ڲ��� ---> Address��Ҫ��ȡ���ݵĵ�ַ
* ���ز��� ---> ���ض�ȡ���İ������� 
* ����˵�� ---> none
**********************************************************/
uint16_t FLASH_ReadHalfWord(uint32_t Address)
{
	return *(vu16*)Address;
}
/**********************************************************
* �������� ---> STM32 FLASHָ����ַ��ȡָ���������ݣ����֣�
* ��ڲ��� ---> Address�����ݵ���ʼ��ַ
*               *pBuffer�����ݻ���
*               NumReadData����ȡ�����֣�����
* ���ز��� ---> none 
* ����˵�� ---> none
**********************************************************/
void FLASH_Read_HaifWord(uint32_t Address, uint16_t *pBuffer, uint16_t NumReadData)
{
	uint16_t i;

	for(i = 0;i < NumReadData;i++)
	{
		*pBuffer++ = FLASH_ReadHalfWord(Address);	//��������
		Address += 2;	//��ַ��2
	}
}

//=========================================================
#if _STM32_FLASH_WRITEOPEN	//����д����
//=========================================================

/**********************************************************
                     �������ݻ����С
                   ֻ�д�д���ܲ���Ч
**********************************************************/

#if STM32_FLASH_SIZE<256	//С������������оƬ

	#define STM32_FLASH_SECTOR_SIZE			1024	//1K��С
	
#else	//������оƬ

	#define STM32_FLASH_SECTOR_SIZE			2048	//2K��С
	
#endif	//STM32_FLASH_SIZE

uint16_t STM32FLASH_BUFF[STM32_FLASH_SECTOR_SIZE/2];	//�������������ݻ����� 

/**********************************************************
* �������� ---> STM32 FLASHд������
* ��ڲ��� ---> Address��д�����ݵ���ʼ��ַ
*               *pBuffer��д�����ݻ���
*               NumWriteData��д�루���֣�����
* ���ز��� ---> none 
* ����˵�� ---> none
**********************************************************/
void FLASH_Write_HaifWord_SECTOR(uint32_t Address, uint16_t *pBuffer, uint16_t NumWriteData)
{
	uint16_t i;

	for(i = 0;i < NumWriteData;i++)
	{
		FLASH_ProgramHalfWord(Address, pBuffer[i]);	//д���������
		Address += 2;	//��ַ����2
	}
}
/**********************************************************
* �������� ---> STM32 FLASHд������
* ��ڲ��� ---> Address��д�����ݵ���ʼ��ַ
*               *pBuffer��д�����ݻ���
*               NumWriteData��д�루���֣�����
* ���ز��� ---> none 
* ����˵�� ---> none
**********************************************************/
void STM32_FLASH_WriteData(uint32_t Address, uint16_t *pBuffer, uint16_t NumWriteData)
{
	uint32_t sectorpos;	//������ַ
	uint32_t OffAddr;	//��ȥ��ʼ��ַ��ĵ�ַ�ռ�
	uint16_t sectorexc;	//����ƫ����
	uint16_t sectorremain;	//����ʣ��ռ仺��
	uint16_t i;

	/* �ж�����ĵ�ַ�Ƿ�Ϸ� */
	if((Address < STM32_FLASH_STARTAddr) || (Address >= STM32_FLASH_ENDAddr))	return;	//��ַ�Ƿ�
	/* ��ַ�Ϸ������FLASH */
	FLASH_Unlock();	//����

	OffAddr = Address - STM32_FLASH_STARTAddr;	//�õ���ַƫ��
	sectorpos = OffAddr / STM32_FLASH_SECTOR_SIZE;	//�õ�������ַ��0 ~ 255
	sectorexc = (OffAddr % STM32_FLASH_SECTOR_SIZE) / 2;	//�����ڵ�ƫ����
	sectorremain = STM32_FLASH_SECTOR_SIZE / 2 - sectorexc;	//�õ�����ʣ��ռ�  
	if(NumWriteData <= sectorremain)	sectorremain = NumWriteData;	//д�����ݲ�����һ��ҳ��С�ռ䣬��һ��ҳ���ܷ����������
	
	while(1)	//ѭ������д������
	{
		//��ȡ�������ݼ���
		FLASH_Read_HaifWord(sectorpos*STM32_FLASH_SECTOR_SIZE+STM32_FLASH_STARTAddr, STM32FLASH_BUFF, STM32_FLASH_SECTOR_SIZE/2);  
		for(i = 0;i < sectorremain;i++)	//��ʼУ������
		{
			if(STM32FLASH_BUFF[sectorexc+i] != 0xffff)	break;	//δ����
		}
		if(i < sectorremain)	//��Ҫ����
		{
			FLASH_ErasePage(sectorpos*STM32_FLASH_SECTOR_SIZE+STM32_FLASH_STARTAddr);	//����
			for(i = 0;i < sectorremain;i++)	//��������
			{
				STM32FLASH_BUFF[i+sectorexc] = pBuffer[i];
			}
			//д����������
			FLASH_Write_HaifWord_SECTOR(sectorpos*STM32_FLASH_SECTOR_SIZE+STM32_FLASH_STARTAddr, STM32FLASH_BUFF, STM32_FLASH_SECTOR_SIZE/2);
		}
		else	//�Ѿ��������ˣ�ֱ��д������
		{
			FLASH_Write_HaifWord_SECTOR(Address, pBuffer, sectorremain);
		}
		if(NumWriteData == sectorremain)	break;	//д�����
		else	//δ���
		{
			sectorpos++;	//дһ������
			sectorexc = 0;	//��0��ʼд��
			pBuffer += sectorremain;	//����ָ��ƫ��
			Address += sectorremain;	//��ַƫ��
			NumWriteData -= sectorremain;	//��ȥ�Ѿ�д�������
			if(NumWriteData > (STM32_FLASH_SECTOR_SIZE/2))	sectorremain = STM32_FLASH_SECTOR_SIZE/2;	//������������
			else	sectorremain = NumWriteData;	//д�������һ����
		}
	}	//end while
	FLASH_Lock();	//д�����ݣ�����
}

//=========================================================
#endif	//_STM32_FLASH_WRITEOPEN
//=========================================================

	