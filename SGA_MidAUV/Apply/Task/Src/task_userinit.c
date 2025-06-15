#include "task_conf.h"
#include "ocd_conf.h"
#include "config.h"

/* 外设初始化函数 */
void Task_UserInit(void)
{
	uint8_t ret = 0;
	Drv_GPIO_Init(&SYNC,1);			/* 初始化AD4111的SYNC引脚 */
	
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
	//Power_AllOn();
	TDmeter_Init();
	
	
	//Power_AllOff();
	
	ret = OCD_AD24BIT_Init(&AD4111_1);  /* 24位AD初始化 */
	if (ret != 0)
	{
		printf("##AD init fail, error code:%d!!!\r\n",ret);
		while(1);  //程序报错stop	
	}
    ret = OCD_AD24BIT_Init(&AD4111_2);  /* 24位AD初始化 */
	if (ret != 0)
	{
		printf("##AD init fail, error code:%d!!!\r\n",ret);
		while(1);  //程序报错stop	
	}
	
	Drv_IWDG_Init(&demoIWDG);//看门狗放最后初始化
	
	#ifdef DEBUG_MODE
	printf("Initialize success\r\n");
	#endif
}

