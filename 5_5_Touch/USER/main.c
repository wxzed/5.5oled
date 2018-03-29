#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"  
#include "24cxx.h" 
#include "myiic.h"
#include "touch.h" 
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h" 
#include "ctiic.h"
 //#define GPIO_Remap_SWJ_JTAGDisable  ((uint32_t)0x00300200)  /*!< JTAG-DP Disabled and SW-DP Enabled */
void SGPIO_Init()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
 	
//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PA,PD�˿�ʱ��
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); 
 GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);      /*ʹ��SWD ����JTAG*/
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 //LED0-->PA.8 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
	
}
 int main(void)
 {
	u8 num,num_last,i=0,j=0,m=0;
	u16 x_last[10]={0},y_last[10]={0};
	u16 x0[10]={0},y0[10]={0};
	u8 touch_sta[10]={0};
	u8 pre_id[10]={0};
	u8 id_send[10]={0};
	
	SCB->VTOR = FLASH_BASE | 0X8000; 
	delay_init();	//��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������	 
	uart_init(9600);//���ڳ�ʼ��Ϊ9600
  SGPIO_Init();
	
	
	USB_Port_Set(0); 	//USB�ȶϿ�
	delay_ms(300);
	USB_Port_Set(1);	//USB�ٴ�����
	//USB����
	USB_Interrupts_Config();    
	Set_USBClock();   
	USB_Init();	
	
	TP_Init();		//��ʼ��������      						    	   


 	while(1)
	{
		num=tp_dev.scan(); 	
		if(num!=0)	//������������
		{
			if(num<num_last)  //�д����㱻�ͷ�
			{
				u8 n=0;
				for(i=0;i<num_last;i++)
				{
					u8 flag=0;
					for(j=0;j<num;j++)         //������ǰid
					{
						if(tp_dev.tp_id[j]==pre_id[i])
						{
							flag=1;
							break;
						}
					}
					if(flag==0)            //���ͷ�
					{
						x0[n]=x_last[i];
						y0[n]=y_last[i];
						touch_sta[n]=0x06;
						id_send[n]=pre_id[i];
						printf("11111x:%d  y:%d \r\n",x0[n],y0[n]);
						n++;
					}
				}
				if(n>0&&bDeviceState==CONFIGURED)
			{
				MulTouch_Send(y0,x0,touch_sta,id_send,n);
				//printf("11111x:%d  y:%d \r\n",x0[0],y0[0]);
			}
				
			}
			
			for(i=0;i<num;i++)
			{
				pre_id[i]=tp_dev.tp_id[i];
				x0[i]=tp_dev.x[i]*4095/1920;
				x_last[i]=x0[i];
				y0[i]=(tp_dev.y[i]*4096/1080);
				y_last[i]=y0[i];
				touch_sta[i]=0x07;
				printf("22222x:%d  y:%d \r\n",tp_dev.x[i],tp_dev.y[i]);
			}	
			num_last=num;
			if(bDeviceState==CONFIGURED)
			{
				MulTouch_Send(y0,x0,touch_sta,tp_dev.tp_id,num);
				
			}
		}else if(num_last!=0)    //������û�����£��ͷ��ϴδ���
		{
			
			m++;
			if(m>10)
			{
				for(i=0;i<num_last;i++)
			{
				touch_sta[i]=0x06;
				printf(" 3333333  x:%d  y:%d \r\n",x_last[i],y_last[i]);
			}
			if(bDeviceState==CONFIGURED)
			{
				MulTouch_Send(y_last,x_last,touch_sta,pre_id,num_last);
				
			}	
			num_last=0;
			m=0;
			}
			
		}
		
		
	
}									    			    
}

