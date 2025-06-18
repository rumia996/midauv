#include "usercode.h"		/* usercodeͷ�ļ� */
#include "drv_hal_conf.h"   /* SGA��ͷ�ļ����� */
#include "task_conf.h"      /* task��ͷ�ļ����� */
#include "ocd_conf.h"       /* OCD��ͷ�ļ����� */
#include "dev_conf.h"		/* Dev��ͷ�ļ����� */
#include "algo_conf.h"		/* Algo��ͷ�ļ����� */
#include "config.h"			/* I/O����ͷ�ļ����� */

/* �߳̾�� */
rt_thread_t ReportDataThread_t = RT_NULL;
rt_thread_t IPCcmdThread_t = RT_NULL;
rt_thread_t AltimeterThread_t = RT_NULL;
rt_thread_t MotorSysThread_t = RT_NULL;
rt_thread_t HWT9073Thread_t = RT_NULL;
rt_thread_t HWT9073Thread_send_t = RT_NULL;
rt_thread_t JY901Thread_t = RT_NULL;
rt_thread_t TDmeterThread_t = RT_NULL;
rt_thread_t PowerSysThread_t = RT_NULL;
rt_thread_t TestThread_t = RT_NULL;
rt_thread_t AutoModeThread_t = RT_NULL;
rt_thread_t HandleModeThread_t = RT_NULL;
rt_thread_t AD4111Thread_t = RT_NULL;

/* �ź������ */
rt_sem_t CmdFromIPC_Sem = RT_NULL;			//��λ�������ź���
rt_sem_t MessageFromTDmeter_Sem = RT_NULL;	//��������Ϣ��Ϣ��
rt_sem_t MessageFromAltimeter_Sem = RT_NULL;//��ȼ���Ϣ�ź���
rt_sem_t MessageFromHWT9073_Sem = RT_NULL;	//��̬��������Ϣ�ź���
rt_sem_t ManualCmd_Sem = RT_NULL;			//�ֱ������ź���
rt_sem_t JY901S_Sem = RT_NULL;				//JY901�ź���

/* �û��߼����� */
void UserLogic_Code(void)
{
	/* �����ź��� */
    CmdFromIPC_Sem 				= rt_sem_create("CmdFromIPC_Sem",0,RT_IPC_FLAG_FIFO);
	MessageFromTDmeter_Sem		= rt_sem_create("MessageFromTDmeter_Sem",0,RT_IPC_FLAG_FIFO);
	MessageFromAltimeter_Sem 	= rt_sem_create("MessageFromAltimeter_Sem",0,RT_IPC_FLAG_FIFO);
	MessageFromHWT9073_Sem 		= rt_sem_create("MessageFromHWT9073_Sem",0,RT_IPC_FLAG_FIFO);
    ManualCmd_Sem 				= rt_sem_create("ManualCmd_Sem",0,RT_IPC_FLAG_FIFO);
	JY901S_Sem					= rt_sem_create("JY901S_Sem",0,RT_IPC_FLAG_FIFO);
	
    /* �����߳� */
	ReportDataThread_t  = rt_thread_create("ReportDataThread",ReportDataThread,NULL,512,2,20);
	IPCcmdThread_t 		= rt_thread_create("IPCcmdThread",IPCcmdThread,NULL,512,1,20);
    AltimeterThread_t 	= rt_thread_create("AltimeterThread",AltimeterThread,NULL,512,8,20); 
	MotorSysThread_t	= rt_thread_create("MotorsysThread",MotorSysThread,NULL,512,10,20); 
	//HWT9073Thread_t		= rt_thread_create("HWT9073Thread",HWT9073Thread,NULL,512,9,20);
	//HWT9073Thread_send_t= rt_thread_create("HWT9073Thread_send",HWT9073Thread_send,NULL,256,9,20);
	JY901Thread_t 		= rt_thread_create("JY901Thread",JY901Thread,NULL,512,9,20);
	TDmeterThread_t		= rt_thread_create("TDmeterThread",TDmeterThread,NULL,512,7,20); 
	PowerSysThread_t	= rt_thread_create("PowerSysThread",PowerSysThread,NULL,512,4,20);
	TestThread_t		= rt_thread_create("TestThread",TestThread,NULL,512,3,20);
	AutoModeThread_t	= rt_thread_create("AutoModeThread",AutoModeThread,NULL,512,11,20);
	HandleModeThread_t  = rt_thread_create("HandleModeThread",HandleModeThread,NULL,512,11,20);
	AD4111Thread_t 		= rt_thread_create("AD4111Thread",AD4111Thread,NULL,512,6,20);
	
	/*�����߳�*/
	rt_thread_startup(ReportDataThread_t);		//���������߳�
	rt_thread_startup(IPCcmdThread_t);			//��λ�������߳�
	rt_thread_startup(AltimeterThread_t);		//�߶ȼ��߳�
	rt_thread_startup(MotorSysThread_t);		//�����߳�
	//rt_thread_startup(HWT9073Thread_t);		//��̬�������߳�
	//rt_thread_startup(HWT9073Thread_send_t);	//��̬���������������߳�;
	rt_thread_startup(JY901Thread_t);			//JY901�߳�
	rt_thread_startup(TDmeterThread_t);			//�������߳�
	rt_thread_startup(PowerSysThread_t);		//��Դϵͳ���߳�
	//rt_thread_startup(TestThread_t);			//�����߳�
	rt_thread_startup(AutoModeThread_t);		//�Զ��߳�
	rt_thread_startup(HandleModeThread_t);		//�ֶ��߳�
	rt_thread_startup(AD4111Thread_t);			//�����ǲɼ��߳�
}

/*���ú���*/

/**
 * @brief ��ȡ2���ַ�֮����ַ���
 * @param �����ַ���
 * @param ��Ž�����ַ���
 * @param ��ʼ�ַ�
 * @param �����ַ�
*/
void extract_str_between_2char(const uint8_t *input, uint8_t *output, uint8_t startchr, uint8_t endchr)
{
    const uint8_t *start = (uint8_t *)strchr((const char *)input, startchr); //�ַ�1
    const uint8_t *end = (uint8_t *)strchr((const char *)input, endchr);   //�ַ�2
    
    if (start && end && start < end) { // ȷ���ַ�1���ַ�2������˳����ȷ
        start++; // �����ַ�1
        size_t len = end - start; // ����Ҫ��ȡ�ַ����ĳ���
        strncpy((char *)output, (const char *)start, len);
        output[len] = '\0'; // ����ַ���������
    } else {
        output[0] = '\0'; // ����ʧ�ܷ��ؿ��ַ���
		#ifdef DEBUG_MODE
		printf("�ַ������ڻ�˳����ȷ,��������:%s,��ʼ�ַ�:%c,�����ַ�%c\r\n",input,startchr,endchr);
		#endif
    }
}

/**
 * @brief ��ȡ��ĳһλ��ĳ���ַ�����ַ���
 * @param �����ַ���
 * @param ��Ž�����ַ���
 * @param ��ʼλ,��0��ʼ�� ����@RSCOOCCCO$ Ҫ��ȡS��$֮ǰ���ַ�,����ʼλΪ3,�����ַ�Ϊ'$'
 * @param �����ַ�
*/
void extract_char_between_indexandchar(const uint8_t *input, uint8_t *output, uint8_t startindex, uint8_t endchr)
{
    const uint8_t *start = &input[startindex]; // �����ַ�����startindex���ַ�
    const uint8_t *end = (uint8_t *)strchr((const char *)input, endchr);   // ���ҽ����ַ�
    
    if (start && end && start < end) { 
        size_t len = end - start; // ����Ҫ��ȡ�ַ����ĳ���
        strncpy((char *)output, (const char *)start, len);
        output[len] = '\0'; // ����ַ���������
    } else {
        output[0] = '\0'; // ����ʧ�ܷ��ؿ��ַ���
		#ifdef DEBUG_MODE
		printf("Command format error,cmd=%s\r\n",input);
		#endif
    }
}


/**
 * @brief ��������4 RS485����ģʽ �ߵ�ƽ���� �͵�ƽ����
*/
void RS485_Send_On()
{
	Drv_GPIO_Set(&RS485Ctrl[0]);
	Drv_Delay_Ms(1);
}

/**
 * @brief ��������4 RS485����ģʽ �ߵ�ƽ���� �͵�ƽ����
*/
void RS485_Receive_On()
{	
	Drv_GPIO_Reset(&RS485Ctrl[0]);
	Drv_Delay_Ms(1);
}

/**
 * @brief ģʽ�л� �л�����ͬģʽʱ����һ��ֹͣ
 * @param tartget_MODE Ҫ�л�����ģʽ
 * @retval �ɹ��л�����true,ʧ�ܷ���false
*/
bool MODE_Switch(uint8_t target_MODE)
{
	if(target_MODE != DEFAULT_MODE && target_MODE != MANUAL_MODE && target_MODE != AUTO_MODE)
	{
		#ifdef DEBUG_MODE
		printf("Fail to switch mode:undefined mode %d\r\n",target_MODE);
		#endif
		return false;
	}
	if( MODE != target_MODE)//ģʽ���
	{
		//�˳���ģʽ ����
		//�˳��Զ�ģʽʱ,�����Զ�ģʽ�ṹ��
		if (MODE == AUTO_MODE)
		{
			AutoModeInfo_Init();
			Task_MotorSys_AllThruster_Stop();
			Task_MotorSys_Manipulator_Close();
		}
		if (MODE == MANUAL_MODE)
		{
			Task_MotorSys_AllThruster_Stop();
			Task_MotorSys_Manipulator_Close();
		}
		//ֹͣ

		
		//�л�ģʽ
		MODE = target_MODE;
		
		//������ģʽ ǰ����	
		if (target_MODE == DEFAULT_MODE)		
		{
			#ifdef DEBUG_MODE
			printf("Switch to Default Mode\r\n");
			#endif
		}
		else if (target_MODE == MANUAL_MODE)
		{
			ClearManualSem();
			rt_sem_release(ManualCmd_Sem);//�ͷ��ź���,�����ֱ���ʱ��
			#ifdef DEBUG_MODE
			printf("Switch to Manual Mode\r\n");
			#endif
		}
		else if (target_MODE == AUTO_MODE)
		{
			#ifdef DEBUG_MODE
			printf("Switch to Auto Mode\r\n");
			#endif
		}
	}
	//ģʽδ���
	else
	{
		if (target_MODE == MANUAL_MODE)
		{
			//ClearManualSem();
			rt_sem_release(ManualCmd_Sem);//�ͷ��ź���,�����ֱ���ʱ��
		}
	}
	return true;
	
}

void ClearManualSem()
{
    // ��վ��ź���
    while (rt_sem_trytake(ManualCmd_Sem) == RT_EOK);
}
	



