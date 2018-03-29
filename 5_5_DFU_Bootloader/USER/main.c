
#include "stm32_config.h"

#include "led.h"
#include "key.h"
#include "lcd_tft.h"
#include "stmflash.h"


#include "hw_config.h"	//USB���ͷ�ļ�
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_pwr.h"
#include "dfu_mal.h"

//#define GPIO_Remap_SWJ_JTAGDisable  ((uint32_t)0x00300200)
u8 Dis_buffer[16];	//��ʾ����

typedef  void (*pFunction)(void);

uint8_t DeviceState;
uint8_t DeviceStatus[6];
pFunction Jump_To_Application;
uint32_t JumpAddress;

void SGPIO_Init()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
 	
 //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PA,PD�˿�ʱ��
	
 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 //LED0-->PA.8 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
	GPIO_SetBits(GPIOB,GPIO_Pin_3);
	
}
/**********************************************************
                           ������
**********************************************************/
int main(void)
{
	u8 i=0;

	delay_init(72);	//��ʼ����ʱ����
	USART1_Init(9600);	//��ʼ������1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); 	
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);      /*ʹ��SWD ����JTAG*/

	DFU_Button_Config();	//��ʼ����תAPP���򰴼�
  GPIO_SetBits(GPIOA,GPIO_Pin_14);
	//����Ƿ����DFUģʽ����������û�а�������ת��APP������ִ��
	if(DFU_Button_Read() == 1)
	{
		if(((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)	//���APP��ַ�Ƿ�Ϸ�
		{
			//��ת��APP��ַ��ʼִ�У���ַ+4λ���Ǹ�λ�ж����
			JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
			Jump_To_Application = (pFunction) JumpAddress;

			//����APP�����ջָ��
			__set_MSP(*(__IO uint32_t*) ApplicationAddress);
			
			//��ת��APP������ִ��
			Jump_To_Application();
		}
	}

    SGPIO_Init();
	//�����Ѿ����£������APP����ģʽ
	DeviceState = STATE_dfuERROR;
	DeviceStatus[0] = STATUS_ERRFIRMWARE;
	DeviceStatus[4] = DeviceState;
	
	Set_System();
	Set_USBClock();
	USB_Init();
	
	while(1)
	{
		i++;
		delay_ms(10);
		if(i == 20)
		{
//			LED0 = ~LED0;
			i = 0;
		}
	}
} 
