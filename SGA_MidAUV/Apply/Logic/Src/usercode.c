#include "usercode.h"		/* usercode头文件 */
#include "drv_hal_conf.h"   /* SGA库头文件配置 */
#include "task_conf.h"      /* task层头文件配置 */
#include "ocd_conf.h"       /* OCD层头文件配置 */
#include "dev_conf.h"		/* Dev层头文件配置 */
#include "algo_conf.h"		/* Algo层头文件配置 */
#include "config.h"			/* I/O配置头文件配置 */

/* 线程句柄 */
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

/* 信号量句柄 */
rt_sem_t CmdFromIPC_Sem = RT_NULL;			//上位机命令信号量
rt_sem_t MessageFromTDmeter_Sem = RT_NULL;	//温深仪信息信息量
rt_sem_t MessageFromAltimeter_Sem = RT_NULL;//深度计信息信号量
rt_sem_t MessageFromHWT9073_Sem = RT_NULL;	//姿态传感器信息信号量
rt_sem_t ManualCmd_Sem = RT_NULL;			//手柄命令信号量
rt_sem_t JY901S_Sem = RT_NULL;				//JY901信号量

/* 用户逻辑代码 */
void UserLogic_Code(void)
{
	/* 创建信号量 */
    CmdFromIPC_Sem 				= rt_sem_create("CmdFromIPC_Sem",0,RT_IPC_FLAG_FIFO);
	MessageFromTDmeter_Sem		= rt_sem_create("MessageFromTDmeter_Sem",0,RT_IPC_FLAG_FIFO);
	MessageFromAltimeter_Sem 	= rt_sem_create("MessageFromAltimeter_Sem",0,RT_IPC_FLAG_FIFO);
	MessageFromHWT9073_Sem 		= rt_sem_create("MessageFromHWT9073_Sem",0,RT_IPC_FLAG_FIFO);
    ManualCmd_Sem 				= rt_sem_create("ManualCmd_Sem",0,RT_IPC_FLAG_FIFO);
	JY901S_Sem					= rt_sem_create("JY901S_Sem",0,RT_IPC_FLAG_FIFO);
	
    /* 创建线程 */
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
	
	/*启动线程*/
	rt_thread_startup(ReportDataThread_t);		//报告数据线程
	rt_thread_startup(IPCcmdThread_t);			//上位机命令线程
	rt_thread_startup(AltimeterThread_t);		//高度计线程
	rt_thread_startup(MotorSysThread_t);		//动力线程
	//rt_thread_startup(HWT9073Thread_t);		//姿态传感器线程
	//rt_thread_startup(HWT9073Thread_send_t);	//姿态传感器发送请求线程;
	rt_thread_startup(JY901Thread_t);			//JY901线程
	rt_thread_startup(TDmeterThread_t);			//温深仪线程
	rt_thread_startup(PowerSysThread_t);		//电源系统仪线程
	//rt_thread_startup(TestThread_t);			//测试线程
	rt_thread_startup(AutoModeThread_t);		//自动线程
	rt_thread_startup(HandleModeThread_t);		//手动线程
	rt_thread_startup(AD4111Thread_t);			//磁力仪采集线程
}

/*公用函数*/

/**
 * @brief 提取2个字符之间的字符串
 * @param 输入字符串
 * @param 存放结果的字符串
 * @param 起始字符
 * @param 结束字符
*/
void extract_str_between_2char(const uint8_t *input, uint8_t *output, uint8_t startchr, uint8_t endchr)
{
    const uint8_t *start = (uint8_t *)strchr((const char *)input, startchr); //字符1
    const uint8_t *end = (uint8_t *)strchr((const char *)input, endchr);   //字符2
    
    if (start && end && start < end) { // 确保字符1和字符2存在且顺序正确
        start++; // 跳过字符1
        size_t len = end - start; // 计算要提取字符串的长度
        strncpy((char *)output, (const char *)start, len);
        output[len] = '\0'; // 添加字符串结束符
    } else {
        output[0] = '\0'; // 解析失败返回空字符串
		#ifdef DEBUG_MODE
		printf("字符不存在或顺序不正确,请检查输入:%s,起始字符:%c,结束字符%c\r\n",input,startchr,endchr);
		#endif
    }
}

/**
 * @brief 提取从某一位到某个字符间的字符串
 * @param 输入字符串
 * @param 存放结果的字符串
 * @param 起始位,从0开始数 例如@RSCOOCCCO$ 要提取S后到$之前的字符,则起始位为3,结束字符为'$'
 * @param 结束字符
*/
void extract_char_between_indexandchar(const uint8_t *input, uint8_t *output, uint8_t startindex, uint8_t endchr)
{
    const uint8_t *start = &input[startindex]; // 查找字符串第startindex个字符
    const uint8_t *end = (uint8_t *)strchr((const char *)input, endchr);   // 查找结束字符
    
    if (start && end && start < end) { 
        size_t len = end - start; // 计算要提取字符串的长度
        strncpy((char *)output, (const char *)start, len);
        output[len] = '\0'; // 添加字符串结束符
    } else {
        output[0] = '\0'; // 解析失败返回空字符串
		#ifdef DEBUG_MODE
		printf("Command format error,cmd=%s\r\n",input);
		#endif
    }
}


/**
 * @brief 开启串口4 RS485发送模式 高电平发送 低电平接收
*/
void RS485_Send_On()
{
	Drv_GPIO_Set(&RS485Ctrl[0]);
	Drv_Delay_Ms(1);
}

/**
 * @brief 开启串口4 RS485接收模式 高电平发送 低电平接收
*/
void RS485_Receive_On()
{	
	Drv_GPIO_Reset(&RS485Ctrl[0]);
	Drv_Delay_Ms(1);
}

/**
 * @brief 模式切换 切换到不同模式时触发一次停止
 * @param tartget_MODE 要切换到的模式
 * @retval 成功切换返回true,失败返回false
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
	if( MODE != target_MODE)//模式变更
	{
		//退出旧模式 后处理
		//退出自动模式时,重置自动模式结构体
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
		//停止

		
		//切换模式
		MODE = target_MODE;
		
		//进入新模式 前处理	
		if (target_MODE == DEFAULT_MODE)		
		{
			#ifdef DEBUG_MODE
			printf("Switch to Default Mode\r\n");
			#endif
		}
		else if (target_MODE == MANUAL_MODE)
		{
			ClearManualSem();
			rt_sem_release(ManualCmd_Sem);//释放信号量,重置手柄计时器
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
	//模式未变更
	else
	{
		if (target_MODE == MANUAL_MODE)
		{
			//ClearManualSem();
			rt_sem_release(ManualCmd_Sem);//释放信号量,重置手柄计时器
		}
	}
	return true;
	
}

void ClearManualSem()
{
    // 清空旧信号量
    while (rt_sem_trytake(ManualCmd_Sem) == RT_EOK);
}
	



