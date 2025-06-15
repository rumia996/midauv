#include "task_TDmeter.h"

uint8_t TDmeter_ReceBuf[100] = {0};	//存储上位机命令字符串,无符号字符类型数组
uint8_t TDmeter_ReceNum = 0;	//命令字符串长度
//float Temperature = 0;		//温度
//float Depth = 0;			//深度

/*温深仪句柄*/
void Task_TDmeter_Handle(void)
{
	if(rt_sem_take(MessageFromTDmeter_Sem,RT_WAITING_FOREVER) == RT_EOK)//监听串口4
	{		
		TDmeter_ReceNum = Drv_Uart_Receive_DMA(&Uart4,TDmeter_ReceBuf);//若收到信息,通过DMA接收字符串并存储,并记录字符串长度
        if(TDmeter_ReceNum != 0)
		{	
			#ifdef DEBUG_MODE
			//Drv_Uart_Transmit(&Uart4,TDmeter_ReceBuf,sizeof(TDmeter_ReceBuf));	
			#endif		
			if (TDmeter_ReceBuf[0] =='T' && TDmeter_ReceBuf[10] =='P' && TDmeter_ReceBuf[TDmeter_ReceNum-1] == '\n')
			{
				uint8_t Buf[30] = {0};
				uint8_t TemperatureBuf[30] = {0};
				uint8_t DepthBuf[30] = {0};
				/*原始数据 T:23.2909,P:10.5725\r\n */
				/*这里,要注意提取函数extract_str_between_2char使用strchr只会找到字符串中第一个匹配的字符*/
				/*若用提取函数提取':'和',' , ':'和'\r' 之间的数据,在提取P时会出错*/
				/*可以先提取':'和'\n'之间的数据,得到23.2909,P:10.5725\r */
				/*再使用extract_char_between_indexandchar提取T,使用extract_str_between_2char提取P*/
//				extract_str_between_2char(TDmeter_ReceBuf,TemperatureBuf,':',',');
//				printf("%s\r\n",TemperatureBuf);
//				extract_str_between_2char(TDmeter_ReceBuf,DepthBuf,':','\r');
//				printf("%s\r\n",DepthBuf);
				extract_str_between_2char(TDmeter_ReceBuf,Buf,':','\n');
				extract_char_between_indexandchar(Buf,TemperatureBuf,0,',');
				extract_str_between_2char(Buf,DepthBuf,':','\r');
				if (TemperatureBuf[0] != '\0' )
				{
					float Temperature = 0.0f;
					float Depth = 0.0f;
					Temperature = atof((const char*)TemperatureBuf);
					Depth = atof((const char*)DepthBuf) - 10.0;	/*压力dbar-10就是深度值/米*/
					memcpy(&ReportDataBuffer[TEMPERATURE_BASE],&Temperature,FLOAT_SIZE);					
					memcpy(&ReportDataBuffer[DEPTH_BASE],&Depth,FLOAT_SIZE);	
					#ifdef DEBUG_MODE
					printf("Temperature:%.4f,Depth:%.4f\r\n",Temperature,Depth);
					#endif							
				}
			}
		}
		TDmeter_ReceNum = 0;
	}
}

/*测试温深仪程序,请在单独的测试线程中运行*/
void Task_TDmeter_Test(void)
{
	Task_TDmeter_Setcycle(2);
	Task_TDmeter_AutoMode();
	Drv_Delay_Ms(6000);
	Task_TDmeter_Getmessage();
	Task_TDmeter_SleepMode();
	Drv_Delay_Ms(3000);
	Task_TDmeter_Setcycle(3);
	Task_TDmeter_Getmessage();
	Task_TDmeter_AutoMode();
	Drv_Delay_Ms(9000);
	Task_TDmeter_Reset();
	Drv_Delay_Ms(100);
	Task_TDmeter_OnceMode();
	Drv_Delay_Ms(2000);
	Task_TDmeter_OnceMode();
	Drv_Delay_Ms(2000);
	Task_TDmeter_Reset();
	Drv_Delay_Ms(100);
}



/**
 * @brief 初始化温深仪,设置波特率,采样频率;以自动模式运行
 * @retval 
*/
void TDmeter_Init()
{
	//Task_TDmeter_Reset();		/*上电后自动复位,无需手动复位*/
	//Drv_Delay_Ms(2000);
	//Task_TDmeter_Setbaud(115200);
	//Drv_Delay_Ms(10);
	Task_TDmeter_Setcycle(2);
	Drv_Delay_Ms(100);
	Task_TDmeter_AutoMode();	/*重复,防止通信问题导致温深仪初始化失败*/
	Drv_Delay_Ms(100);
	Task_TDmeter_AutoMode();
	Drv_Delay_Ms(100);
}

/**
 * @brief 复位温深仪		
 * @retval 
*/
void Task_TDmeter_Reset()
{
	uint8_t SendBuffer[30] = {0};//存储发送数据字符串,以<CR><LF>结尾
	sprintf((char *)SendBuffer, "reset\r\n");
	Drv_Uart_Transmit(&Uart4,SendBuffer,strlen((const char *)SendBuffer));
}

/**
 * @brief 设置波特率波;无实际意义,修改波特率后就无法通信了!	
 * @param 波特率 只能是9600、14400、19200、38400、57600、115200;默认为115200;
*/
void Task_TDmeter_Setbaud(uint32_t baudrate)
{
	uint8_t SendBuffer[30] = {0};//存储发送数据字符串,以<CR><LF>结尾	
	if (baudrate == 9600 || baudrate == 14400 || baudrate == 19200 || baudrate == 38400 ||baudrate == 57600 || baudrate == 115200)
	{
		sprintf((char *)SendBuffer, "setbaud %d\r\n",baudrate);
		Drv_Uart_Transmit(&Uart4,SendBuffer,strlen((const char *)SendBuffer));
	}else 
	{
		#ifdef DEBUG_MODE
		printf("仅支持波特率9600、14400、19200、38400、57600、115200!\r\n");
		#endif
	}
}

/**
 * @brief 设置采样频率
 * @param 采样频率 运行模式下每隔多久进行一次输出.可以是-4,-2,-1,1,2,3...负数代表Hz,-4即4Hz,0.25s;正数代表秒,3即3秒;这里设置2s发送一次	
*/
void Task_TDmeter_Setcycle(int8_t cycle)
{
	uint8_t SendBuffer[30] = {0};//存储发送数据字符串,以<CR><LF>结尾	
	sprintf((char *)SendBuffer, "setcycle %d\r\n",cycle);
	Drv_Uart_Transmit(&Uart4,SendBuffer,strlen((const char *)SendBuffer));
}

/**
 * @brief 温深仪以自动模式运行,每隔一段时间发送温度和压力值;
*/
void Task_TDmeter_AutoMode()
{
	uint8_t SendBuffer[30] = {0};//存储发送数据字符串,以<CR><LF>结尾	
	sprintf((char *)SendBuffer, "run\r\n");
	Drv_Uart_Transmit(&Uart4,SendBuffer,strlen((const char *)SendBuffer));
}

/**
 * @brief 单次采样模式,每执行一次采样一次
*/
void Task_TDmeter_OnceMode()
{
	uint8_t SendBuffer[30] = {0};	
	sprintf((char *)SendBuffer, "once\r\n");
	Drv_Uart_Transmit(&Uart4,SendBuffer,strlen((const char *)SendBuffer));
}

/**
 * @brief 温深仪进入低功耗待机模式;上电/复位后运行在此模式
*/
void Task_TDmeter_SleepMode()
{
	uint8_t SendBuffer[30] = {0};	
	sprintf((char *)SendBuffer, "stop\r\n");
	Drv_Uart_Transmit(&Uart4,SendBuffer,strlen((const char *)SendBuffer));
}

/**
 * @brief 获取温深仪信息,不完整,还需要在句柄接收处添加处理以在串口1打印信息
*/
void Task_TDmeter_Getmessage()
{
	uint8_t SendBuffer[30] = {0};	
	sprintf((char *)SendBuffer, "getmessage\r\n");
	Drv_Uart_Transmit(&Uart4,SendBuffer,strlen((const char *)SendBuffer));
}
