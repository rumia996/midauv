#include "usercode.h"		/* usercodeͷ�ļ� */
#include "threadpool.h"		/* threadpoolͷ�ļ� */
#include "drv_hal_conf.h"   /* SGA��ͷ�ļ����� */
#include "task_conf.h"      /* task��ͷ�ļ����� */
#include "ocd_conf.h"       /* OCD��ͷ�ļ����� */
#include "dev_conf.h"		/* Dev��ͷ�ļ����� */
#include "algo_conf.h"		/* Algo��ͷ�ļ����� */
#include "config.h"			/* I/O����ͷ�ļ����� */

/* �߳���ں�����ʹ��������Դ��ļ��� */

/* ��λ����������߳� */
//�յ���λ�������ر�

void IPCcmdThread(void* paramenter)
{
    while(1)
    {
		Task_IPCcmd_Handle();
        Drv_Delay_Ms(1);     
    }
}

/* ���������߳� */
void ReportDataThread(void* paramenter)
{
	Task_ReportData_Handle();
}

/* �߶ȼ��߳� */
void AltimeterThread(void* paramenter)
{
	while(1)
	{
		Task_Altimeter_Handle();
		Drv_Delay_Ms(1); 
	}
}

/*�����߳�*/
void MotorSysThread(void* pramenter)
{
	while(1)
	{
		Task_MotorSys_Handle();
		Drv_Delay_Ms(1); 
	}
}

///*��̬�������߳�*/
//void HWT9073Thread(void* pramenter)
//{
//	while(1)
//	{
//		Task_HWT9073_Handle();
//		Drv_Delay_Ms(1); 
//	}
//}

/* ��ȡJY901S�����߳� */
void JY901Thread(void* pramenter)
{
	while(1)
	{	 
		Task_JY901_Handle();
		Drv_Delay_Ms(1); 
	}
}

///*��̬�����������߳�*/
//void HWT9073Thread_send(void* pramenter)
//{
//	while(1)
//	{
//		Task_HWT9073_send_Handle();
//		Drv_Delay_Ms(1); 
//	}
//}

/*�������߳�*/
void TDmeterThread(void* pramenter)
{
	while(1)
	{
		Task_TDmeter_Handle();
		Drv_Delay_Ms(1); 
	}
}

/*��Դϵͳ�߳�*/
void PowerSysThread(void* pramenter)
{
	while(1)
	{
		Task_PowerSys_Handle();
		Drv_Delay_Ms(1); 
	}
}

/*�����߳�*/
void TestThread(void* pramenter)
{
	while(1)
	{
		Task_Test_Handle();
		Drv_Delay_Ms(1); 
	}
}


/*�Զ�ģʽ�߳�*/
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

/*�ֶ�ģʽ�߳�*/
void HandleModeThread(void* pramenter)
{
	while(1)
	{
		Task_HandleMode_Process();		
		Drv_Delay_Ms(1); 
	}
}
