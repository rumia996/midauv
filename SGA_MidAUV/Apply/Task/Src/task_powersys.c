#include "task_powersys.h"
#include "task_altimeter.h"
#include "task_TDmeter.h"
uint8_t PowerSys_ReceBuf[100];	//存储上位机命令字符串,无符号字符类型数组
uint8_t PowerSys_ReceNum = 0;	//命令字符串长度
uint8_t powerstate = 0;
uint8_t counter = 0;
/**
 * @brief 打开外设电源
 * @param 继电器编号 定义在config.h中
 * @retval null
*/
void Power_On(uint8_t index)
{

	Drv_GPIO_Reset(&Control_GPIO[index]);
//	if (index == POW_TDM)		/*温深仪上电需要初始化*/ /*使温深仪不经继电器直连12V电源,避免了复位问题*/
//	{
//		Drv_Delay_Ms(2000);		/*延时2秒等待温深仪上电复位*/
//		TDmeter_Init();
//	}
}

/**
 * @brief 关闭外设电源
 * @param 继电器编号 定义在config.h中
 * @retval null
*/
void Power_Off(uint8_t index)
{
	Drv_GPIO_Set(&Control_GPIO[index]);
}
/**
 * @brief 电源全部关闭
 * @retval null
*/
void Power_AllOff()
{
	Power_Off(POW_MAG);
	Power_Off(POW_TDM);
	Power_Off(POW_HEIGHT);
	Power_Off(POW_LIGHT);
	Power_Off(POW_BDGPS);
	Power_Off(POW_RADIO);
	Power_Off(POW_UACM);
	Power_Off(POW_TEST_1);
	Power_Off(POW_TEST_2);
}

/**
 * @brief 电源全部开启 备用除外
 * @retval null
*/
void Power_AllOn()
{
	Power_On(POW_MAG);
	Power_On(POW_TDM);
	Power_On(POW_HEIGHT);
	Power_On(POW_LIGHT);
	Power_On(POW_BDGPS);
	Power_On(POW_RADIO);
	Power_On(POW_UACM);
	//Power_On(POW_TEST_1);
	//Power_On(POW_TEST_2);
}

/**
 * @brief 开传感器电源
 * @retval null
*/
void Power_SensorOn()
{
	Power_On(POW_MAG);
	Power_On(POW_TDM);
	Power_On(POW_HEIGHT);
}

/**
 * @brief 关传感器电源
 * @retval null
*/
void Power_SensorOff()
{
	Power_Off(POW_MAG);
	Power_Off(POW_TDM);
	Power_Off(POW_HEIGHT);
}

/**
 * @brief 开通信电源
 * @retval null
*/
void Power_CommunicationOn()
{
	Power_On(POW_BDGPS);
	Power_On(POW_RADIO);
	Power_On(POW_UACM);
}

/**
 * @brief 关通信电源
 * @retval null
*/
void Power_CommunicationOff()
{
	Power_Off(POW_BDGPS);
	Power_Off(POW_RADIO);
	Power_Off(POW_UACM);
}

/**
 * @brief 测试电源方案 
 * @retval null
*/
void Power_Test()
{
	Power_On(POW_MAG);
	Power_On(POW_TDM);
	Power_On(POW_HEIGHT);
	Power_On(POW_BDGPS);
	Power_Off(POW_LIGHT);
	Power_On(POW_RADIO);
	Power_On(POW_UACM);	
	Power_Off(POW_TEST_1);
	Power_Off(POW_TEST_2);	
}
/**
 * @brief 获取当前继电器开关状态 
 * @retval 继电器状态 从0-7位分别为 温深仪 高度计 北斗GPS 声通机 水下灯 磁力仪 无线电台
 * 继电器是低电平0开高电平1关,按照协议要求,将引脚状态取反,1表示开,0表示关;
*/
uint8_t Power_GetState()
{
	uint8_t powerstate= 0;
	powerstate = 1;	//第7位 暂时留空
	powerstate |= Drv_GPIO_Read(&Control_GPIO[POW_RADIO]);	//第6位 此处与协议不同,没有水下灯
	powerstate <<=1;
	powerstate |= Drv_GPIO_Read(&Control_GPIO[POW_MAG]);	//第5位
	powerstate <<=1;
	powerstate |= Drv_GPIO_Read(&Control_GPIO[POW_LIGHT]);	//第4位
	powerstate <<=1;
	powerstate |= Drv_GPIO_Read(&Control_GPIO[POW_UACM]);	//第3位
	powerstate <<=1;
	powerstate |= Drv_GPIO_Read(&Control_GPIO[POW_BDGPS]);	//第2位
	powerstate <<=1;
	powerstate |= Drv_GPIO_Read(&Control_GPIO[POW_HEIGHT]);	//第1位
	powerstate <<=1;
	powerstate |= Drv_GPIO_Read(&Control_GPIO[POW_TDM]);	//第0位
	
	return ~powerstate;

//	Drv_GPIO_Read(&Control_GPIO[POW_TEST_1]);
//	Drv_GPIO_Read(&Control_GPIO[POW_TEST_2]);
}

void Task_PowerSys_Handle()
{

	/*深度小于1m且声通机为开启状态，自动关闭声通机电源*/
	if(*p_height < 1 && !Drv_GPIO_Read(&Control_GPIO[POW_UACM]))
	{
		/*每100ms counter+1 计时3秒后关闭*/
		counter+=1;
		//printf("counter=%d\r\n",counter);
		if(counter>= 30)
		{			
			counter = 0;
			//Power_Off(POW_UACM);
			#ifdef DEBUG_MODE
			printf("当前深度:%.3f 声通机已关闭\r\n",*p_height);
			#endif
		}
	}

	
	/*每100ms喂狗*/
	Drv_IWDG_Feed(&demoIWDG);
	#ifdef DEBUG_MODE
	//printf("喂狗!\r\n");
	#endif
	Drv_Delay_Ms(100);
}
