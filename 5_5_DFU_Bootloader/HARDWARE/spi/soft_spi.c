/**********************************************************
* @ File name -> soft_spi.c
* @ Version   -> V1.0
* @ Date      -> 11-15-2013
* @ Brief     -> GPIOģ��SPI�ӿں���

 V1.
* @ Revise    -> 
**********************************************************/

#include "soft_spi.h"

/**********************************************************
* �������� ---> ģ��SPI��ʼ��GPIO
* ��ڲ��� ---> none
* ���ز��� ---> none
* ����˵�� ---> none 
**********************************************************/
void Soft_SPI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��   
    	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;  //CLK and MOSI
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    
 	GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��ָ��IO��CLK and MOSI
 	
 	GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);    //���õ͵�ƽ

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;   //MISO
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //��������

    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��ָ��IO��MISO
}
/**********************************************************
* �������� ---> ģ��SPI�������ݺ���
* ��ڲ��� ---> send_data: Ҫ���͵�����
* ���ز��� ---> ���յ�������
* ����˵�� ---> ���ȷ���MSB����Ҫ�ȷ���LSB�����޸Ĵ˺���
**********************************************************/
u8 Soft_SPI_ReadWrite_Byte(u8 send_data)
{
    u8 i;

//    Soft_SPI_MISO_IN(); //����MISOΪ����ģʽ

    for(i = 0;i < 8;i++)
    {
        if(send_data & 0x80)    Soft_SPI_MOSI = 1;  //�������ݣ����ȷ���LSB���޸�Ϊ0x01
        else    Soft_SPI_MOSI = 0;

        send_data <<= 1;    //���ȷ���LSB���Ϊ����

        Soft_SPI_CLK = 0;
        Soft_SPI_CLK = 1;   //�����ط�������
//        Soft_SPI_CLK = 0;   //�½��ش��豸�����������ϴ˾�

        if(Soft_SPI_MISO)   send_data |= 0x01;  //����������"1"������LSBʱ���޸�Ϊ0x80
    }
    Soft_SPI_CLK = 0;
    return send_data;   //���ؽ��յ�������
}

