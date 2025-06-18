#include "task_handlemode.h"
#include "task_motorsys.h"
/**
 * @brief �ֶ�ģʽ������
 * @param AMInfo �ֶ�ģʽ��Ϣ�ṹ��ָ��
 */
void Task_HandleMode_Process()
{
	/*�ֱ�ģʽ��ȫ��������;Ӧ�����߳����ȼ���Ϊ����������������߳�!*/
	/*700�����ղ�����һ���ֱ�����,�ر����ж���*/
    rt_tick_t timeout_tick = rt_tick_from_millisecond(600);
	while(1)
	{
		rt_err_t result = rt_sem_take(ManualCmd_Sem, timeout_tick);	
		if (MODE == MANUAL_MODE)
		{
			if (result == RT_EOK)
			{
				continue;
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
	}	
}


