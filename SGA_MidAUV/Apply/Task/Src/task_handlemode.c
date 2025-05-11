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
				printf("�˳��ֱ�ģʽ!�������ģʽ\r\n");
				#endif
				MODE = TEST_MODE;
				Task_MotorSys_AllThruster_Stop();
				Task_MotorSys_AllSteer_0Angle();				
			}			
		}
	}	
}
