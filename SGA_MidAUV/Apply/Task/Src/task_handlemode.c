#include "task_handlemode.h"
#include "task_motorsys.h"
/**
 * @brief 手动模式处理函数
 * @param AMInfo 手动模式信息结构体指针
 */
void Task_HandleMode_Process()
{
	/*手柄模式安全保护程序;应将此线程优先级设为最低以免阻塞其他线程!*/
	/*600毫秒收不到下一条手柄命令,关闭所有动力*/
    rt_tick_t timeout_tick = rt_tick_from_millisecond(600);
//	while(1)
//	{
		rt_err_t result = rt_sem_take(ManualCmd_Sem, timeout_tick);	
		if (MODE == MANUAL_MODE)
		{
			if (result == RT_EOK)
			{
				ClearManualSem();
//				continue;
			}
			else
			//else if (result == -RT_ETIMEOUT)
			{
				#ifdef DEBUG_MODE
				printf("Timeout! Exit Manual mode\r\n");
				#endif
				MODE_Switch(DEFAULT_MODE);			
			}			
		}
		else if (MODE == DEFAULT_MODE)	//终极安全保护,默认模式下始终尝试停止推进器
		{
			Task_MotorSys_AllThruster_Stop();
		}
		else if (MODE == AUTO_MODE)	//自动模式下什么也不做
		{
			//do nothing
		}
		else
		{
			Task_MotorSys_AllThruster_Stop();
		}
//	}	
}


