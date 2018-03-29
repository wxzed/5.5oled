/**********************************************************
* @ File name -> flash.c
* @ Version   -> V1.0
* @ Date      -> 06-03-2014
* @ Brief     -> SPI Flash��������
**********************************************************/

#include "flash.h"

/**********************************************************
                       �����������
**********************************************************/

static u8 Flash_Data_Buff[4096];	//�������ݻ�������һ�����ٲ���һ������

u16 SPI_FLASH_TYPE = W25Q64;    //Ĭ��ΪW25Q64
u16 SPI_Flash_ID;	//�豸ID����

/**********************************************************
                        �ӿڶ���
                    Ӳ����ֲ�޸�����
**********************************************************/

/**********************************************************
* �������� ---> GPIO��ʼ��
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> ��Ҫ�Ǵ��豸��Ƭѡ���ų�ʼ��
**********************************************************/
void FlashSPI_CS_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG, ENABLE );//GPIOʱ��ʹ�� 

	/*	GPIOB12��ʼ����FlashƬѡ����	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);

	/*	GPIOD2��ʼ����SD��Ƭѡ	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  // PD2 ���� 

	GPIO_Init(GPIOD, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOD,GPIO_Pin_2);

	/*	GPIOG7��ʼ��������ģ��Ƭѡ	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  // PG7 ����
 
 	GPIO_Init(GPIOG, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOG,GPIO_Pin_7);

	SPI_GPIO_Init();		   	//��ʼ��SPI
	SPIx_SetSpeed(SPI_BaudRatePrescaler_4);//����Ϊ9Mʱ��,����ģʽ
	SPI_Flash_ID = Flash_Read_ID();//��ȡFLASH ID.  
}
/**********************************************************
* �������� ---> flashоƬSPI��ȡ����
* ��ڲ��� ---> dat��Ҫд�������
* ������ֵ ---> �����豸����
* ����˵�� ---> none
**********************************************************/
static u8 Flash_SPI_WriteRead_Data(u8 dat)
{
	return SPIx_WriteRead_Data(dat);
}

/**********************************************************
                  ����Ӳ����ֲ�޸�����
**********************************************************/

/**********************************************************
* �������� ---> ��ȡW25Qxx���кţ�ID��
* ��ڲ��� ---> none
* ������ֵ ---> ID������16λ����
* ����˵�� ---> none
**********************************************************/
u16 Flash_Read_ID(void)
{
	u16 IDtemp=0;
	SPI_FLASH_CS = 0;	//ѡ��Flash
	Flash_SPI_WriteRead_Data(W25Qxx_Manufacturer);	//���Ͷ�ȡ����
	Flash_SPI_WriteRead_Data(0x00);	//��ȡ24bits��ַ�ռ�
	Flash_SPI_WriteRead_Data(0x00);
	Flash_SPI_WriteRead_Data(0x00);
	IDtemp |= Flash_SPI_WriteRead_Data(0xff) << 8;	//��ȡ��8bit
	IDtemp |= Flash_SPI_WriteRead_Data(0xff);	//��ȡ��8bit
	SPI_FLASH_CS = 1;	//�ͷ�оƬ
	return IDtemp;	//����ID��
}
/**********************************************************
* �������� ---> W25Qxx�������ģʽ
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> �ڵ�ع���ϵͳ�У����õ�Flashʱ�������˯����ʡ��
**********************************************************/
void Flash_Enter_Power_Down(void)
{
	SPI_FLASH_CS = 0;	//ѡ��Flash
	Flash_SPI_WriteRead_Data(W25Qxx_Power_Down);	//����power down����0xb9
	SPI_FLASH_CS = 1;	//�ͷ�оƬ
	delay_us(3);
}
/**********************************************************
* �������� ---> ����W25Qxx
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> ��Flash��
**********************************************************/
void Flash_Exit_Power_Down(void)
{
	SPI_FLASH_CS = 0;	//ѡ��Flash
	Flash_SPI_WriteRead_Data(W25Qxx_Release_Power_Down);	//���ͻ�������0xab
	SPI_FLASH_CS = 1;	//�ͷ�оƬ
	delay_us(3);
}
/**********************************************************
* �������� ---> W25Qxx��ȡ״̬�Ĵ���
* ��ڲ��� ---> REG_CMD: �Ĵ�����ȡ����
* ������ֵ ---> ����״̬�Ĵ�������
* ����˵�� ---> Bit 15	Bit 14	Bit 13	Bit 12	Bit 11	Bit 10	Bit 9	Bit 8
*                   R      R       R      R       R        R      QE     SRP1
*
*               Bit 7	Bit 6	Bit 5	Bit 4	Bit 3	Bit 2	Bit 1	Bit 0    Ĭ��ֵΪ0x00
*                SRP0   SEC     TB      BP2     BP1     BP0     WEL     BUSY
**********************************************************/
u8 Flash_Read_Register_Status(u8 REG_CMD)
{
	u8 RS_byte=0;
	SPI_FLASH_CS = 0;	//ѡ��Flash
	Flash_SPI_WriteRead_Data(REG_CMD);	//���Ͷ�ȡ����
	RS_byte = Flash_SPI_WriteRead_Data(0xff);	//��ȡһ���ֽ����� 
	SPI_FLASH_CS = 1;	//�ͷ�оƬ
	return RS_byte;	//���ض��������� 
}
/**********************************************************
* �������� ---> �ȴ�W25Qxxæ��
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void Flash_Wait_Busy(void)
{
//      u8 temp,Bitbusy;

//      do
//      {
//            temp++;
//            if(temp > 200)    return;     //��ʱ�˳�

//            Bitbusy = Flash_Read_Register_Status(W25Qxx_Read_Status_Register);  //��ȡ״̬�Ĵ���ֵ
//      }while(Bitbusy & 0x01);
	while((Flash_Read_Register_Status(W25Qxx_Read_Status_Register) & 0x01) == 0x01);	//�ȴ�æ��־λ����
}
/**********************************************************
* �������� ---> W25Qxxд״̬�Ĵ���
* ��ڲ��� ---> RS_dat��д���ֵ
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void Flash_Write_Register_Status(u8 RS_dat)
{
	SPI_FLASH_CS = 0;	//ѡ��Flash
	Flash_SPI_WriteRead_Data(W25Qxx_Write_Status_Register);	//��������
	Flash_SPI_WriteRead_Data(RS_dat);	//д������
	SPI_FLASH_CS = 1;	//�ͷ�оƬ
}
/**********************************************************
* �������� ---> W25Qxx��д����
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void Flash_WriteEnable(void)
{
	SPI_FLASH_CS = 0;	//ѡ��Flash
	Flash_SPI_WriteRead_Data(W25Qxx_Write_Enable);	//��������
	SPI_FLASH_CS = 1;	//�ͷ�оƬ
}
/**********************************************************
* �������� ---> W25Qxx��ֹд����
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void Flash_WriteDisable(void)
{
	SPI_FLASH_CS = 0;	//ѡ��Flash
	Flash_SPI_WriteRead_Data(W25Qxx_Write_Disable);	//��������
	SPI_FLASH_CS = 1;	//�ͷ�оƬ
}
/**********************************************************
* �������� ---> W25Qxx����һ������4K�����߲���һ����
* ��ڲ��� ---> Sectoradd��������ַ
*               ORD����������ѡ��0x00������һ��������0x01������һ����
* ������ֵ ---> none
* ����˵�� ---> Ҫ����һ���鷢�Ͳ�����ָ��W25Qxx_Erase_Block64Kb/W25Qxx_Erase_Block32Kb���Ӳ�ͬ��оƬ����  
**********************************************************/
void Flash_EraseSector(u32 Sectoradd,u8 ORD)
{
	u8 ORD_temp;

	if(ORD == 0x00)
	{
		Sectoradd *= 4096;	//�õ����������Ĵ�С��4K
		ORD_temp = W25Qxx_Erase_Sector4Kb;
	} 
	if(ORD == 0x01)
	{
		Sectoradd *= 65536;	//�õ�������Ĵ�С��32K/64K
		ORD_temp = W25Qxx_Erase_Block64Kb;
	}

	Flash_WriteEnable();	//����оƬд����
	Flash_Wait_Busy();	//оƬ���ڿ���״̬�²�ִ�в���
	SPI_FLASH_CS = 0;	//ѡ��Flash
	Flash_SPI_WriteRead_Data(ORD_temp);	//���Ͳ���ָ��
	Flash_SPI_WriteRead_Data((u8)(Sectoradd >> 16));	//����24bits��ַ
	Flash_SPI_WriteRead_Data((u8)(Sectoradd >> 8));
	Flash_SPI_WriteRead_Data((u8)Sectoradd);
	SPI_FLASH_CS = 1;	//�ͷ�оƬ
	Flash_Wait_Busy();	//�ȴ������������
}
/**********************************************************
* �������� ---> W25Qxx��������оƬ
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> ����Խ�󣬲���ʱ���Խ�����Ӳ�ͬ��оƬ����  
**********************************************************/
void Flash_EraseChip(void)
{
	Flash_WriteEnable();	//����оƬд����
	Flash_Wait_Busy();	//оƬ���ڿ���״̬�²�ִ�в���
	SPI_FLASH_CS = 0;	//ѡ��Flash
	Flash_SPI_WriteRead_Data(W25Qxx_Erase_Chip);	//���Ͳ���ָ��
	SPI_FLASH_CS = 1;	//�ͷ�оƬ
	Flash_Wait_Busy();	//�ȴ��������
}
/**********************************************************
* �������� ---> W25Qxx��һ��ҳ��д������
* ��ڲ��� ---> *datBuff��������洢��
*               startadd��д��������ʼ��ַ
*               num��д�����ݳ��ȣ���󲻳���256���ֽڣ�
* ������ֵ ---> none
* ����˵�� ---> ÿ��������16ҳ����ַ��0x0000 ~ 0x00ff����ܹ���4K   
**********************************************************/
void Flash_Write_Page(u32 startadd,u8* wbuffer,u16 num)
{
	u16 n;

	Flash_WriteEnable();	//����оƬд����
	SPI_FLASH_CS = 0;	//ѡ��Flash
	Flash_SPI_WriteRead_Data(W25Qxx_Page_Program);	//����ҳдָ��
	Flash_SPI_WriteRead_Data((u8)(startadd >> 16));	//����24bits��ַ
	Flash_SPI_WriteRead_Data((u8)(startadd >> 8));
	Flash_SPI_WriteRead_Data((u8)startadd);
	for(n = 0;n < num;n++)	Flash_SPI_WriteRead_Data(wbuffer[n]);	//��ʼд������
	SPI_FLASH_CS = 1;	//�ͷ�оƬ
	Flash_Wait_Busy();	//�ȴ�����д�����
}
/**********************************************************
* �������� ---> W25Qxxд��������
* ��ڲ��� ---> *datBuff��������洢��
*               startadd��д��������ʼ��ַ
*               num��д�����ݳ��ȣ���󲻳���65535���ֽڣ���������һ������4K
* ������ֵ ---> none
* ����˵�� ---> ÿ��������16ҳ����ַ��0x0000 ~ 0x00ff����ܹ���4K     
**********************************************************/
void Flash_Write_Sector(u32 writeadd,u8* wbuffer,u16 sum)
{
	u16 page_remain;	//ҳʣ������

//	if((sum + writeadd%256) >= 256)     page_remain = 256 - writeadd%256;
//	else  page_remain = sum;
	
	page_remain = 256 - writeadd%256;	//�õ��״�д�����ݶ���
	if(page_remain >= sum)	page_remain = sum;	//���ݴ�С����ҳ���洢��

	while(1)	//ѭ��д�����ݵ�����
	{
		Flash_Write_Page(writeadd,wbuffer,page_remain);	//��ʼд������
		if(sum == page_remain)	break;	//����д����ϣ��˳�
		else
		{
			writeadd += page_remain;	//д��һҳ
			wbuffer += page_remain;	//д��һҳ����
			
			sum -= page_remain;	//��ȥ�Ѿ�д����ɵ�ҳ��
			
			if(sum > 256)	page_remain = 256;	//����ҳ��Χ��256�ֽ�
			else	page_remain = sum;	//û����
		}
	}
}
/**********************************************************
* �������� ---> W25Qxxд����
* ��ڲ��� ---> *pdBuff��������洢��
*               writeadd��д��������ʼ��ַ
*               sum��д�����ݳ��ȣ���󲻳���65535���ֽڣ���������һ������4K
* ������ֵ ---> none
* ����˵�� ---> ÿ��������16ҳ��ÿҳ��256�ֽ�   
**********************************************************/
void Flash_WriteData(u8* pdbuffer,u32 writeadd,u16 sum)
{
	u16 i;
	u16 sectorremain;	//����ʣ��ռ仺��
	u32 sectorpos;	//������ַ
	u16 sectorexc;	//����ƫ����
	
	sectorpos = writeadd / 4096;	//������ַ��0 ~ 15
	sectorexc = writeadd % 4096;	//����ƫ������0 ~ 255
	sectorremain = 4096 - sectorexc;	//����ʣ��ռ��С��һ��д��256���ֽ�
	
	if(sum <= sectorremain) sectorremain = sum;	//д�����ݲ�����һ��ҳ��С�ռ�256����һ��ҳ���ܷ����������

	while(1)	//ѭ������
	{
		Flash_ReadData(Flash_Data_Buff,sectorpos*4096,4096);	//��ȡ������������
		for(i = 0;i < sectorremain;i++)	//У�������Ƿ�Ϊ0xff�������������Ƿ������
		{
			if(Flash_Data_Buff[sectorexc+i] != 0xff)	break;	//ֻҪ��һ�����ݲ���0xff��Ҫʵ�ֲ�������	
		}
		if(i < sectorremain)	//��Ҫ��������
		{
			Flash_EraseSector(sectorpos,0x00);	//������������
			for(i = 0;i < sectorremain;i++)	//���ƽ�Ҫд�������
			{
				Flash_Data_Buff[i+sectorexc] = pdbuffer[i];	//�ڶ�����������ʣ�¿ռ��в���д�������
			}
			Flash_Write_Sector(sectorpos*4096,Flash_Data_Buff,4096);	//����д����������
		}
		else	//�����Ѿ��ǲ����˵�
		{
			Flash_Write_Sector(writeadd,pdbuffer,sectorremain);	//ֱ��д������
		}

		if(sum == sectorremain)	break;	//����д�����
		else
		{
			sectorpos++;	//��һ����
			sectorexc = 0;	//��һ������0��ַд��
			
			pdbuffer += sectorremain;	//����ָ��ƫ��
			writeadd += sectorremain;	//д��ַҲ����ƫ��
			
			sum -= sectorremain;	//��ֵ�ݼ�
			
			if(sum > 4096)	sectorremain = 4096;	//�����Ų���������
			else	sectorremain = sum;	//д�걾����������д��һ����
		}
	}
}
/**********************************************************
* �������� ---> W25Qxx������
* ��ڲ��� ---> *datBuff��������洢��
*               startadd����ȡ������ʼ��ַ
*               num����ȡ���ݳ��ȣ���󲻳���65535���ֽڣ���������һ������4K
* ������ֵ ---> none
* ����˵�� ---> W25Qxx��ָ����ַ��ȡָ�����ȵ����ݣ�ÿ��������16������������ַ��0x0000 ~ 0x00ff����ܹ���4K    
**********************************************************/
void Flash_ReadData(u8* rbuffer,u32 startadd,u16 num)
{
	u16 i;
	
	SPI_FLASH_CS = 0;	//ѡ��Flash
	Flash_SPI_WriteRead_Data(W25Qxx_Read_Data);	//���Ͷ�ȡָ��
//	Flash_SPI_WriteRead_Data(W25Qxx_Fast_Data);	//���Ͷ�ȡָ��
	Flash_SPI_WriteRead_Data((u8)(startadd >> 16));	//����24bits��ַ
	Flash_SPI_WriteRead_Data((u8)(startadd >> 8));
	Flash_SPI_WriteRead_Data((u8)startadd);
//	Flash_SPI_WriteRead_Data(0xff);	//�ٶ�һ��
	for(i = 0;i < num;i++)
	{
		rbuffer[i] = Flash_SPI_WriteRead_Data(0xff);	//��ȡ����
	}
	SPI_FLASH_CS = 1;	//�ͷ�оƬ
}

