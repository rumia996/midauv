#include "usercode.h"		/* usercode头文件 */
#include "threadpool.h"		/* threadpool头文件 */
#include "drv_hal_conf.h"   /* SGA库头文件配置 */
#include "task_conf.h"      /* task层头文件配置 */
#include "ocd_conf.h"       /* OCD层头文件配置 */
#include "dev_conf.h"		/* Dev层头文件配置 */
#include "algo_conf.h"		/* Algo层头文件配置 */
#include "config.h"			/* I/O配置头文件配置 */

/* 线程入口函数（使用裸机忽略此文件） */

/* 上位机命令解析线程 */
//收到上位机命令后回报

void IPCcmdThread(void* paramenter)
{
    while(1)
    {
		Task_IPCcmd_Handle();
        Drv_Delay_Ms(1);     
    }
}

/* 报告数据线程 */
void ReportDataThread(void* paramenter)
{
	Task_ReportData_Handle();
}

/* 高度计线程 */
void AltimeterThread(void* paramenter)
{
	while(1)
	{
		Task_Altimeter_Handle();
		Drv_Delay_Ms(1); 
	}
}

/*动力线程*/
void MotorSysThread(void* pramenter)
{
	while(1)
	{
		Task_MotorSys_Handle();
		Drv_Delay_Ms(1); 
	}
}

///*姿态传感器线程*/
//void HWT9073Thread(void* pramenter)
//{
//	while(1)
//	{
//		Task_HWT9073_Handle();
//		Drv_Delay_Ms(1); 
//	}
//}

/* 读取JY901S数据线程 */
void JY901Thread(void* pramenter)
{
	while(1)
	{	 
		Task_JY901_Handle();
		Drv_Delay_Ms(1); 
	}
}

///*姿态传感器发送线程*/
//void HWT9073Thread_send(void* pramenter)
//{
//	while(1)
//	{
//		Task_HWT9073_send_Handle();
//		Drv_Delay_Ms(1); 
//	}
//}

/*温深仪线程*/
void TDmeterThread(void* pramenter)
{
	while(1)
	{
		Task_TDmeter_Handle();
		Drv_Delay_Ms(1); 
	}
}

/*电源系统线程*/
void PowerSysThread(void* pramenter)
{
	while(1)
	{
		Task_PowerSys_Handle();
		Drv_Delay_Ms(1); 
	}
}

/*测试线程*/
void TestThread(void* pramenter)
{
	while(1)
	{
		Task_Test_Handle();
		Drv_Delay_Ms(1); 
	}
}


/*自动模式线程*/
void AutoModeThread(void* pramenter)
{
	while(1)
	{
		if (MODE == AUTO_MODE)
		{
			Task_AutoMode_Process(&AMInfo);		
		}
		else
		{
			AutoModeInfo_Init();
		}
		Drv_Delay_Ms(1); 
	}
}

/*手动模式线程*/
void HandleModeThread(void* pramenter)
{
	while(1)
	{
		Task_HandleMode_Process();		
		Drv_Delay_Ms(1); 
	}
}
