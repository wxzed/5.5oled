/**********************************************************
* @ File name -> spi.c
* @ Version   -> V1.1
* @ Date      -> 11-05-2013
* @ Brief     -> STM32Ӳ��SPI��������

 V1.
* @ Revise    -> 
**********************************************************/

#include "spi.h"

/**********************************************************
* �������� ---> SPI_CS��ʼ��
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void SPIx_CSPin_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PB,D,G�˿�ʱ��
    
	//W25Q32Ƭѡ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //PA4���� ��ֹW25X�ĸ���
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��ָ��IO
 	GPIO_SetBits(GPIOA, GPIO_Pin_4);//�øߵ�ƽ				
 	
	//SD��SPI����ʱ��Ƭѡ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 //PD2�����������   ��ֹSD���ĸ���
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	
	GPIO_Init(GPIOD, &GPIO_InitStructure);	//��ʼ��ָ��IO
 	GPIO_SetBits(GPIOD, GPIO_Pin_2);//�øߵ�ƽ
	
	//����ģ���Ƭѡ��ʹ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	//PC6/7 ���죬����ģ���CE��CSN	  
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��ָ��IO
	GPIO_SetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7);//�øߵ�ƽ

	//����ģ����жϼ������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;	//����ģ���IRQ   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA8 ����
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_8);//�øߵ�ƽ
}
/**********************************************************
* �������� ---> GPIO��ʼ��
* ��ڲ��� ---> none
* ������ֵ ---> none
* ����˵�� ---> none
**********************************************************/
void SPI_GPIO_Init(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//����GPIOA����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);	//����SPI1����ʱ��
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);	//����SPI2����ʱ��
	
	SPIx_CSPin_Init();	//��ʼ����ص�CS�ܽ�

	/*	��ʼ��GPIOA	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PA5/6/7����������� 

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);	//PA5.6.7����
	
//	SPI_Cmd(SPI1, DISABLE); // SPI���費ʹ��

	/*	��ʼ��SPI1	*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//����SPI�������˫�������ģʽ������SPIΪ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	//����SPI����ģʽ������SPIΪ���豸
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	//����SPI����֡�ṹ������SPI����֡�ṹΪ8bitģʽ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;	//����SPI����ʱ�ӵ���̬������SPIʱ������Ϊ��
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����SPI���ݲ���ʽ������SPI���ݲ���ʽΪ�ڶ���ʱ����
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;	//����SPI����ʱ�ӵ���̬������SPIʱ������Ϊ��
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//����SPI���ݲ���ʽ������SPI���ݲ���ʽΪ��һ��ʱ����      
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	//����SPI NSS��Ƭѡ�źţ����Ʒ�ʽ������SPI NSS���Ʒ�ʽΪ�������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;	//����SPI������Ԥ��Ƶֵ������SPIԤ��ƵֵΪ4��Ƶ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ��SPI���ݴ����Ǵ�MSB��ʼ����LSB��ʼ������SPI�������ݴ�MSB��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//����SPI CRCֵ����Ķ���ʽ������SPI CRC�������ʽ7

	SPI_Init(SPI1, &SPI_InitStructure);	//��ʼ��SPI1����

	SPI_Cmd(SPI1, ENABLE);	//ʹ��SPI1����

	SPIx_WriteRead_Data(0xff);
}
/**********************************************************
* �������� ---> ����SPIx�����ٶ�
* ��ڲ��� ---> SPI_Speed��Ҫ�ı���ٶ�ֵ
* ������ֵ ---> none
* ����˵�� ---> SPI_BaudRatePrescaler_2		2��Ƶ
*               SPI_BaudRatePrescaler_4		4��Ƶ
*               SPI_BaudRatePrescaler_8		8��Ƶ
*               SPI_BaudRatePrescaler_16	16��Ƶ
*               SPI_BaudRatePrescaler_32	32��Ƶ
*               SPI_BaudRatePrescaler_64	64��Ƶ
*               SPI_BaudRatePrescaler_128	128��Ƶ
*               SPI_BaudRatePrescaler_256	256��Ƶ
**********************************************************/
void SPIx_SetSpeed(u8 SPI_Speed)
{
//	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_Speed));
	SPI1->CR1 &= 0xffc7; 
	SPI1->CR1 |= SPI_Speed;	//д���Ƶֵ
	SPI_Cmd(SPI1, ENABLE);	//ʹ��SPI1����
}
/**********************************************************
* �������� ---> SPIx���ݶ�д
* ��ڲ��� ---> д������ݻ�������
* ���ز��� ---> MOSI���յ�������
*               1����ʾ���ͻ��߽��ճ�����߳�ʱ 
* ����˵�� ---> None
**********************************************************/
u8 SPIx_WriteRead_Data(u8 write_dat)
{
	u8 time_count;

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)	//DR Register���ͻ���ձ�־
	{
		time_count++;
		if(time_count > 200)	return 1;	//��ʱ�˳�����ֹ������ѭ��������
	}	

	SPI_I2S_SendData(SPI1, write_dat);	//��SPI�豸����һ���ֽ����ݻ�������

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)	//�ȴ�������ɣ����շǿձ�־
	{
		time_count++;
		if(time_count > 200)	return 1;	//��ʱ�˳�����ֹ������ѭ��������
	}

	return SPI_I2S_ReceiveData(SPI1);	//���ش�SPIx�豸�Ϸ���������
}

