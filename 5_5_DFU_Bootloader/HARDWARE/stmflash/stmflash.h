/**********************************************************
* @ File name -> stmflash.h
* @ Version   -> V1.0
* @ Date      -> 03-08-2014
* @ Brief     -> STM32�ڲ�FLASH��������ͷ�ļ�

 V1.
* @ Revise    ->
**********************************************************/

#ifndef _stmflash_h_
#define _stmflash_h_

/**********************************************************
                     �ⲿ����ͷ�ļ�
         Ӧ�õ���ͬ������ͷ�ļ����������޸ļ���                        
**********************************************************/

#include "stm32_config.h"

#include "stm32f10x_flash.h"

/**********************************************************
                      �������ܶ���
      ��Ҫ��д���ܡ�оƬflash������С���޸�����
**********************************************************/

#define _STM32_FLASH_WRITEOPEN					1	/* STM32 FLASHд���ܿ��� */
													//0���ر�
													//1������

#define STM32_FLASH_SIZE						512	/* ����оƬ����������λ��KB */
													//�������ҳд�����ݴ�С��С������������оƬΪ1K��������Ϊ2K
													//flash������:64K��128K��256K��512K��1024K��

/**********************************************************
                   ����FLASH������ַ��Χ
                 ��ͬ������оƬ���޸�����
**********************************************************/

#define STM32_FLASH_STARTAddr		(uint32_t)0x08000000	//flash��ʼ��ַ����ͬ��flash��ַ���޸�����

/* flash������ַ���Ǳ�Ҫ���Բ��޸� */ 
#define STM32_FLASH_ENDAddr			(uint32_t)((STM32_FLASH_STARTAddr + STM32_FLASH_SIZE*1024) -1)

/**********************************************************
                       �ⲿ���ܺ���                      
**********************************************************/

uint16_t FLASH_ReadHalfWord(uint32_t Address);	//STM32 FLASH��ȡ����
void FLASH_Read_HaifWord(uint32_t Address, uint16_t *pBuffer, uint16_t NumReadData);	//STM32 FLASHָ����ַ��ȡָ���������ݣ����֣�

#if _STM32_FLASH_WRITEOPEN	//����д����

void FLASH_Write_HaifWord_SECTOR(uint32_t Address, uint16_t *pBuffer, uint16_t NumWriteData);	//STM32 FLASHд������
void STM32_FLASH_WriteData(uint32_t Address, uint16_t *pBuffer, uint16_t NumWriteData);	//STM32 FLASHд������

#endif	//_STM32_FLASH_WRITEOPEN


#endif

