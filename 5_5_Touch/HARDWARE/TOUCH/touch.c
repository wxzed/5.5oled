#include "touch.h" 
#include "delay.h"
#include "stdlib.h"
#include "math.h"
    


_m_tp_dev tp_dev=
{
	TP_Init,
	GT1151_Scan,
	0,0,0,0,0,
	0,0,0,0,0,
  0,0,0,0,0,	
	0,	
  0	
};

//��������ʼ��  		    
//����ֵ:0,û�н���У׼
//       1,���й�У׼
u8 TP_Init(void)
{		
	
	GT1151_Init();		
	tp_dev.scan=GT1151_Scan;	//ɨ�躯��ָ��GT1151������ɨ��

	return 1; 									 
}






