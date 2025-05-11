#include "task_conf.h"
#include "ocd_conf.h"
#include "config.h"

/* 外设初始化函数 */
void Task_UserInit(void)
{
	
	Drv_GPIO_Init(Control_GPIO, 9);
	Drv_GPIO_Init (RS485Ctrl,1);
	
    Drv_Uart_DMAInit(&Uart1);	

	
	//Drv_Uart_DMAInit(&Uart2);//已知有jY901的IM板执行初始化必定会卡住
	OCD_JY901_DMAInit(&JY901S);     /* 初始化JY901S 使用串口2 */
	Drv_Uart_DMAInit(&Uart3);
	Drv_Uart_DMAInit(&Uart4);
	//Drv_Uart_ITInit(&Uart5);//Uart5没有DMA
	
	Drv_PWM_Init(PWM,12);
	Task_MotorSys_Init();//4秒
	
	CRC_Init();
	
	Power_Test();	
	TDmeter_Init();
	
	Drv_IWDG_Init(&demoIWDG);
	//Power_AllOff();
	
}

