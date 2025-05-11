#include "task_conf.h"
#include "ocd_conf.h"
#include "config.h"

/* �����ʼ������ */
void Task_UserInit(void)
{
	
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
	TDmeter_Init();
	
	Drv_IWDG_Init(&demoIWDG);
	//Power_AllOff();
	
}

