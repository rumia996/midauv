#include "task_test.h"

void Task_Test_Handle()
{	
	/*��֪���̻߳ᵼ�³��򱨴���,�ʲ����в���ʱ�����usercode.cע�͵�����߳�,��дһ����ѭ��������ʱ*/
//	Power_Test();
//	Task_MotorSys_Servos_Test();
	//����ϵͳ
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

	//ˮ�µƲ���
//	while(1)
//	{
//		Power_On(POW_LIGHT);
//		Drv_Delay_Ms(3000);
//		Power_Off(POW_LIGHT);
//		Drv_Delay_Ms(3000);
//	}

	//
	/*CRC����*/
//	uint32_t buffer[20] = {0x12345678}; 
//	uint32_t crc_result = HAL_CRC_Calculate(&hcrc, (uint32_t *)buffer, 1);	
//	printf("0x%08X",crc_result);
//	Drv_Delay_Ms(1000);
	
	//printf("�������\r\n");
	//Drv_Delay_Ms(1000);
//	uint8_t data[] = "���";
	//Drv_Uart_Transmit(&Uart2,data,sizeof(data));
//	Drv_Uart_Transmit_DMA(&Uart2,data,sizeof(data));
//	Drv_Delay_Ms(10);
//	Drv_Uart_Transmit_DMA(&Uart3,data,sizeof(data));
//	Drv_Delay_Ms(10);
	//Drv_Uart_Transmit(&Uart3,data,sizeof(data));
	//printf("\r\nDMA�������\r\n");
//	Drv_Delay_Ms(1000);
}
