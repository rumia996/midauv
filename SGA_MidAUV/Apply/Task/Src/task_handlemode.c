#include "task_handlemode.h"
#include "task_motorsys.h"
/**
 * @brief 手动模式处理函数
 * @param AMInfo 手动模式信息结构体指针
 */
void Task_HandleMode_Process()
{
	/*手柄模式安全保护程序;应将此线程优先级设为最低以免阻塞其他线程!*/
	/*700毫秒收不到下一条手柄命令,关闭所有动力*/
    rt_tick_t timeout_tick = rt_tick_from_millisecond(700);
	while(1)
	{
		rt_err_t result = rt_sem_take(ManualCmd_Sem, timeout_tick);	
		if (MODE == MANUAL_MODE)
		{
			if (result == RT_EOK)
			{
				continue;
			}
			else if (result == -RT_ETIMEOUT)
			{
				#ifdef DEBUG_MODE
				printf("退出手柄模式!进入测试模式\r\n");
				#endif
				MODE = TEST_MODE;
				Task_MotorSys_AllThruster_Stop();
				Task_MotorSys_AllSteer_0Angle();				
			}			
		}
	}	
}
