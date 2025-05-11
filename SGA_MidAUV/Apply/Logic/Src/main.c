#include "main.h"
#include "drv_hal_conf.h"	/* �����ض����Լ�RTOS���� */ 
#include "task_userinit.h"
#include "usercode.h"       

#if !defined(RTT_ENABLE)
#include "task_sysinit.h"
#endif

int main()
{
#if !defined(RTT_ENABLE)
    Task_Sys_Init();		/* ϵͳ��ʼ�������� */
#endif
    
    Task_UserInit();		/* �û������ʼ�� */	
    
#ifdef RTT_ENABLE
	rt_kprintf("RTT Start!\r\n");
	/*
     * ������Ӳ����ʼ���� RT-Thread ϵͳ��ʼ���Ѿ��� main ����֮ǰ���
     * ���� component.c �ļ��е� rtthread_startup()����������ˡ� 
     * 1.�� usercode �У������̺߳������̡߳�
     * 2.�� threadpool �У�д���߳���ں�����
    */
#endif

#ifdef FREERTOS_ENABLE
    printf("FreeRTOS Start!\r\n");
	/*
     * 1.�� usercode �У������̺߳������̡߳�
     * 2.�� threadpool �У�д���߳���ں�����
     * 3.�ֶ�����osKernelStart()�����������ȡ�
    */
#endif

    UserLogic_Code();		/* �û��߼�ִ�� */
}

