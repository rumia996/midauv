#include "task_test.h"

void Task_Test_Handle()
{	
	/*已知空线程会导致程序报错卡死,故不进行测试时最好在usercode.c注释掉这个线程,或写一个死循环里面延时*/
//	Power_Test();
//	Task_MotorSys_Servos_Test();
	//动力系统
//	while(1)
//	{
//		//Task_MotorSys_Manipulator_Test();	
//		Drv_PWM_HighLvTimeSet(&PWM[MANIPULATOR], 1500);		
//		Drv_Delay_Ms(1000);
//		Drv_PWM_HighLvTimeSet(&PWM[MANIPULATOR], 1900);	
//		Drv_Delay_Ms(1000);
//		Drv_PWM_HighLvTimeSet(&PWM[MANIPULATOR], 1500);	
//		Drv_Delay_Ms(1000);
//		Drv_PWM_HighLvTimeSet(&PWM[MANIPULATOR], 1100);	
//		Drv_Delay_Ms(1000);
//	}
	 

	//Task_MotorSys_AllThruster_Stop();
	while(1)
	{
		Drv_Delay_Ms(1000);
	}
//	while(1)
//	{
//		//Task_MotorSys_Thruster_Test();		
//	}

	//水下灯测试
//	while(1)
//	{
//		Power_On(POW_LIGHT);
//		Drv_Delay_Ms(3000);
//		Power_Off(POW_LIGHT);
//		Drv_Delay_Ms(3000);
//	}

	//
	/*CRC测试*/
//	uint32_t buffer[20] = {0x12345678}; 
//	uint32_t crc_result = HAL_CRC_Calculate(&hcrc, (uint32_t *)buffer, 1);	
//	printf("0x%08X",crc_result);
//	Drv_Delay_Ms(1000);
	
	//printf("进入测试\r\n");
	//Drv_Delay_Ms(1000);
//	uint8_t data[] = "你好";
	//Drv_Uart_Transmit(&Uart2,data,sizeof(data));
//	Drv_Uart_Transmit_DMA(&Uart2,data,sizeof(data));
//	Drv_Delay_Ms(10);
//	Drv_Uart_Transmit_DMA(&Uart3,data,sizeof(data));
//	Drv_Delay_Ms(10);
	//Drv_Uart_Transmit(&Uart3,data,sizeof(data));
	//printf("\r\nDMA传输完成\r\n");
//	Drv_Delay_Ms(1000);
}
