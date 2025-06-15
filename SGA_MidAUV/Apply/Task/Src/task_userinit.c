#include "task_conf.h"
#include "ocd_conf.h"
#include "config.h"

/* �����ʼ������ */
void Task_UserInit(void)
{
	uint8_t ret = 0;
	Drv_GPIO_Init(&SYNC,1);			/* ��ʼ��AD4111��SYNC���� */
	
	Drv_GPIO_Init(Control_GPIO, 9);
	Drv_GPIO_Init (RS485Ctrl,1);
	
    Drv_Uart_DMAInit(&Uart1);	

	
	//Drv_Uart_DMAInit(&Uart2);//��֪��jY901��IM��ִ�г�ʼ���ض��Ῠס
	OCD_JY901_DMAInit(&JY901S);     /* ��ʼ��JY901S ʹ�ô���2 */
	Drv_Uart_DMAInit(&Uart3);
	Drv_Uart_DMAInit(&Uart4);
	//Drv_Uart_ITInit(&Uart5);//Uart5û��DMA
	
	Drv_PWM_Init(PWM,12);
	Task_MotorSys_Init();//4��
	CRC_Init();
	
	Power_Test();
	//Power_AllOn();
	TDmeter_Init();
	
	
	//Power_AllOff();
	
	ret = OCD_AD24BIT_Init(&AD4111_1);  /* 24λAD��ʼ�� */
	if (ret != 0)
	{
		printf("##AD init fail, error code:%d!!!\r\n",ret);
		while(1);  //���򱨴�stop	
	}
    ret = OCD_AD24BIT_Init(&AD4111_2);  /* 24λAD��ʼ�� */
	if (ret != 0)
	{
		printf("##AD init fail, error code:%d!!!\r\n",ret);
		while(1);  //���򱨴�stop	
	}
	
	Drv_IWDG_Init(&demoIWDG);//���Ź�������ʼ��
	
	#ifdef DEBUG_MODE
	printf("Initialize success\r\n");
	#endif
}

