#include "task_motorsys.h"
#include "task_TDmeter.h"
#include "task_altimeter.h"
#include "task_JY901.h"

#define STOP_PWM_HIGHTIME 1500
#define THRUSTER_MAX_PWM_HIGHTIME 2000
#define THRUSTER_MIN_PWM_HIGHTIME 1000
#define MANIPULATOR_OPEN_PWM_HIGHTIME 1900
#define MANIPULATOR_CLOSE_PWM_HIGHTIME 1100
#define SERVORS_MAXANGLE_PWM_HIGHTIME 2500
#define SERVORS_MINANGLE_PWM_HIGHTIME 500
#define SERVORS_ANGLE_RANGE 270
#define SERVORS_DELTA (SERVORS_MAXANGLE_PWM_HIGHTIME-STOP_PWM_HIGHTIME) / (SERVORS_ANGLE_RANGE / 2.0f)
#define SERVORS_0ANGLE_PWM_HIGHTIME 1500
/*推进器高电平时间调整值，用于推力微调平衡 us*/
#define LVT_DELTA 0	//会导致SpeedSet为0档pwm不是1500,尽量不用这个参数调整,如果用这个需确保调整值足够小,0档下推进器不动
#define RVT_DELTA 0
#define BVT_DELTA 0
#define LMT_DELTA 0
#define RMT_DELTA 0

float rudder_ang_left = 0;	//用于add方法累积角度值
float rudder_ang_right = 0;

/**
 * @brief 动力系统初始化
 * @retval null
*/
void Task_MotorSys_Init(void)
{
	Task_MotorSys_AllThruster_Stop();
	//Task_MotorSys_AllSteer_0Angle();
	Task_MotorSys_AllRudder_Angle_Set(0);
	Task_MotorSys_Manipulator_Close();
	Drv_Delay_Ms(4000);//保持停止位4秒让电调初始化
}
/*
 *  舵机信号范围500-2500us(2.5%-12.5%)可推测出1500us(7.5%)是停转
 *   角度270°,即±135°正转定义为面向舵机时舵盘逆时针旋转
 *   PWM计算公式为:占空比=(12.5-2.5)/135*angle+7.5 高电平时间 = (2500-500)/135*angle+1500其中angle是期望的角度,-135°~+135°
 *  10.83%  2167		+90°
 *  9.16%  1833 	+45°
 *  7.5%   1500		0°
 *  5.83%  1167		-45°
 *  4.16% 833 		-90°
 * 	或者 占空比=(12.5-2.5)/270*angle 高电平时间 = (2500-500)/270*angle angle从0-270°
 *	左舵机 -10度对应舵板水平 0度舵板微微向下 20度舵板向下达到最大；-55度舵板向上达到最大 
 *  右舵机 25度对应舵板水平 0度舵板微微向下 -5度舵板向下达到最大；70度舵板向上达到最大
 */

/**
 * @brief 单个舵板角度设置 由于安装问题,即使两个舵机角度相同,舵板角度也不同,需要同步;
 * 经测试,以左舵为基准,右舵机角度 = -左舵机角度+15 时 两个舵板同步
 * @param 舵机编号 左舵机LS 右舵机RS 宏定义在config.h
 * @param 舵板角度值 向上最大22°向下最大-15°舵板角度 = -0.5 × 左舵机角度 - 5 ;左舵机角度 = -2*(舵板角度+5) 
 * @retval true/false
*/
bool Task_MotorSys_Rudder_Angle_Set(uint8_t index ,float ang)
{
	if (ang >= -15 && ang <= 22)
	{
		int16_t steer_ang = 0;
		steer_ang = - 2.0f * (ang + 8);
		if (index == LS)
		{
			rudder_ang_left = ang;
			return Task_MotorSys_Steer_Angle_Set(LS,steer_ang);
		}
		if (index == RS)
		{
			rudder_ang_right = ang;
			return Task_MotorSys_Steer_Angle_Set(RS, -steer_ang + 5);	//修正右舵不齐
		}	
	}
	return false;	
}

/**
 * @brief 所有舵板角度设置
 * @param 舵板角度值 向上最大22°向下最大-15°
 * @retval true/false
*/
bool Task_MotorSys_AllRudder_Angle_Set(float ang)
{
	return Task_MotorSys_Rudder_Angle_Set(LS,ang) && Task_MotorSys_Rudder_Angle_Set(RS,ang);	
}

/**
 * @brief 获取当前舵板角度;读取舵机高电平时间反解出舵板角度，误差较大
 * @param 舵机编号 左舵机LS 右舵机RS 宏定义在config.h
 * @retval 角度值
*/
float Task_MotorSys_GetRudder_Angle(uint8_t index)
{	
	float steer_ang = 0;
	float rudder_ang = 0;
	uint16_t _highTime = Drv_PWM_ReadHighLvTime(&PWM[index]);
	steer_ang = (_highTime - SERVORS_0ANGLE_PWM_HIGHTIME) / (SERVORS_DELTA + 0.7f);
	if (index == LS)
	{
		rudder_ang = (steer_ang / (-2.0f)) - 5;
		return rudder_ang;
	}
	if (index == RS)
	{
		rudder_ang = ((-(steer_ang - 15)) / (-2.0f)) - 5;
		return rudder_ang;
	}
	return 0;
}

/**
 * @brief 舵板角度增量变化 
 * @param 舵机编号 左舵机LS 右舵机RS 宏定义在config.h
 * @param 增量值 不宜太小,至少在2°以上
 * @retval true/false
*/
bool Task_MotorSys_Rudder_Angle_Add(uint8_t index,float ang)
{
	if (index == LS || index == RS || index == LSRS)
	{
		//使用float全局变量存储增量变化,消除舍入误差
		if (index == LS)
		{
			rudder_ang_left += ang;	
			Task_MotorSys_Rudder_Angle_Set(index,rudder_ang_left);			
		}else
		if (index == RS)
		{
			rudder_ang_right += ang;	
			Task_MotorSys_Rudder_Angle_Set(index,rudder_ang_right);				
		}else
		if (index == LSRS)
		{
			rudder_ang_left += ang;
			rudder_ang_right = rudder_ang_left;	//确保角度同步
			Task_MotorSys_Rudder_Angle_Set(LS,rudder_ang_left);
			Task_MotorSys_Rudder_Angle_Set(RS,rudder_ang_right);				
		}
		//↓误差大,会导致两个舵板不同步
		//return Task_MotorSys_Rudder_Angle_Set(index,Task_MotorSys_GetRudder_Angle(index) + ang);
		//保证两个舵板同步，但必须一起动，且不流畅
		//Task_MotorSys_Rudder_Angle_Set(LS,Task_MotorSys_GetRudder_Angle(LS) + ang);
		//Task_MotorSys_Rudder_Angle_Set(RS,Task_MotorSys_GetRudder_Angle(LS) + ang);

	}
	return false;
}

/**
 * @brief 单个舵机角度设置
 * @param 舵机编号 左舵机LS 右舵机RS 宏定义在config.h
 * @param 角度值 -135°~+135°
 * @retval true/false
*/
bool Task_MotorSys_Steer_Angle_Set(uint8_t index,float ang)
{
	if (index == LS)
	{
		if (ang >=-55 && ang <= 20)
		{
			uint16_t _highTime = (SERVORS_DELTA+ 0.7f) * ang + SERVORS_0ANGLE_PWM_HIGHTIME;//ang -135°~+135°0.7f是补偿值，通过测试舵机试出来。
			Drv_PWM_HighLvTimeSet(&PWM[index], _highTime);		
			return true;
		}
	}
	if (index == RS)
	{
		//if (ang >= -5 && ang <= 70)
		if (ang >= -6 && ang <= 70)
		{
			uint16_t _highTime = (SERVORS_DELTA+ 0.7f) * ang + SERVORS_0ANGLE_PWM_HIGHTIME;//ang -135°~+135°0.7f是补偿值，通过测试舵机试出来。
			Drv_PWM_HighLvTimeSet(&PWM[index], _highTime);		
			return true;
		}
	}	
//	if (index == LS || index == RS)
//	{
//		if (ang >= -135 && ang <= 135)
//		{
//			uint16_t _highTime = (SERVORS_DELTA+ 0.7f) * ang + SERVORS_0ANGLE_PWM_HIGHTIME;//ang -135°~+135°0.7f是补偿值，通过测试舵机试出来。
//			//uint16_t _highTime = (SERVORS_MAXANGLE_PWM_HIGHTIME - SERVORS_MINANGLE_PWM_HIGHTIME) / SERVORS_ANGLE_RANGE * ang;//ang 0-270°
//			Drv_PWM_HighLvTimeSet(&PWM[index], _highTime);			
//		}

//		return true;
//	}
	return false;
}

/**
 * @brief 所有舵机角度设置
 * @param 舵机编号 左舵机LS 右舵机RS 宏定义在config.h
 * @param 角度值 
 * @retval true/false
*/
bool Task_MotorSys_AllSteer_Angle_Set(float ang)
{
	if (ang >= -135 && ang <= 135)
	{
		return Task_MotorSys_Steer_Angle_Set(LS,ang) && Task_MotorSys_Steer_Angle_Set(RS,ang);
	}
	return false;
}

/**
 * @brief 舵机复位
 * @param 舵机编号 左舵机LS 右舵机RS 宏定义在config.h
 * @retval true/false
*/
bool Task_MotorSys_Steer_0Angle(uint8_t index)
{
	if (index == LS || index == RS)
	{
		Drv_PWM_HighLvTimeSet(&PWM[index], SERVORS_0ANGLE_PWM_HIGHTIME);
		return true;
	}
	return false;
}

/**
 * @brief 所有舵机复位
 * @retval true/false
*/
bool Task_MotorSys_AllSteer_0Angle(void)
{
	Drv_PWM_HighLvTimeSet(&PWM[LS], SERVORS_0ANGLE_PWM_HIGHTIME);
	Drv_PWM_HighLvTimeSet(&PWM[RS], SERVORS_0ANGLE_PWM_HIGHTIME);
	return true;
}

/**
 * @brief 单个舵机角度增量变化 效果不好,别用
 * @param 舵机编号 左舵机LS 右舵机RS 宏定义在config.h
 * @param 增量值 
 * @retval true/false
*/
bool Task_MotorSys_Steer_Angle_Add(uint8_t index,int16_t ang)
{
	if (index == LS || index == RS)
	{
		uint16_t _highTime = Drv_PWM_ReadHighLvTime(&PWM[index]) + ang * (2000/270);//增量式误差大!但是舵机不是很重要所以没关系:)
		if(_highTime <= 2500 && _highTime > 500)
		{
			Drv_PWM_HighLvTimeSet(&PWM[index], _highTime);
			return true;
		}
	}
	return false;
}

/**
 * @brief 机械手张开 完全张开需要1秒
 * @retval true/false
*/
bool Task_MotorSys_Manipulator_Open(void)
{
	Drv_PWM_HighLvTimeSet(&PWM[MANIPULATOR], MANIPULATOR_OPEN_PWM_HIGHTIME);
	Drv_Delay_Ms(1000);
	Drv_PWM_HighLvTimeSet(&PWM[MANIPULATOR], STOP_PWM_HIGHTIME);
	return true;
}

/**
 * @brief 机械手关闭 完全关闭需要1秒
 * @retval true/false
*/
bool Task_MotorSys_Manipulator_Close(void)
{
	Drv_PWM_HighLvTimeSet(&PWM[MANIPULATOR], MANIPULATOR_CLOSE_PWM_HIGHTIME);
	Drv_Delay_Ms(1000);
	Drv_PWM_DutyFactorSet(&PWM[MANIPULATOR], STOP_PWM_HIGHTIME);
	return true;
}


//推进器控制从单个推进器开始层层封装 底层speedset-中间层start&stop-高级层前进后退上浮下潜左转右转

/**
 * @brief 推进器转速设置子函数 	控制单个推进器 支持推力微调
 * @param index 推进器编号 LVT左垂推 RVT右垂推 BVT后垂推 LMT左主推 RMT右主推 宏定义在config.h中
 * @param _highTime 高电平时间大小	_highTime可设置为1000-2000,1500停止,1000-1500反转,1500-2000正转
 * @retval true/false
*/
bool Task_MotorSys_Thruster_SpeedSet(uint8_t index,uint16_t _highTime)
{
//	if (index == LVT || index == RVT || index == BVT || index == LMT || index == RMT)
//	{
//		Drv_PWM_HighLvTimeSet(&PWM[index], _highTime);
//		return true;
//	}
	if (index == LVT)
	{
		Drv_PWM_HighLvTimeSet(&PWM[index], _highTime + LVT_DELTA);
		return true;	
	}
	else if (index == RVT )
	{
		Drv_PWM_HighLvTimeSet(&PWM[index], _highTime + RVT_DELTA);
		return true;	
	}
	else if (index == BVT )
	{
		Drv_PWM_HighLvTimeSet(&PWM[index], _highTime + BVT_DELTA);
		return true;	
	}
	else if (index == LMT )
	{
		Drv_PWM_HighLvTimeSet(&PWM[index], _highTime + LMT_DELTA);
		return true;	
	}
	else if (index == RMT )
	{
		Drv_PWM_HighLvTimeSet(&PWM[index], _highTime + RMT_DELTA);
		return true;	
	}
	return false;
}
/**
 * @brief 所有推进器相同转速设置函数
 * @param _highTime 高电平时间大小	_highTime可设置为1000-2000,1500停止,1000-1500反转,1500-2000正转
 * @retval true/false
*/
bool Task_MotorSys_AllThruster_SpeedSet(uint16_t _highTime)
{
	Drv_PWM_HighLvTimeSet(&PWM[LVT], _highTime + LVT_DELTA);
	Drv_PWM_HighLvTimeSet(&PWM[RVT], _highTime + RVT_DELTA);
	Drv_PWM_HighLvTimeSet(&PWM[BVT], _highTime + BVT_DELTA);
	Drv_PWM_HighLvTimeSet(&PWM[LMT], _highTime + LMT_DELTA);
	Drv_PWM_HighLvTimeSet(&PWM[RMT], _highTime + RMT_DELTA);
	return true;
}

/**
 * @brief 单个推进器开始工作
 * @param index 推进器编号 LVT左垂推 RVT右垂推 BVT后垂推 LMT左主推 RMT右主推 宏定义在config.h中
 * @param speed 速度档位 -12~+12共25档 负档位即推进器反转
 * @retval true/false
*/
bool Task_MotorSys_Thruster_Start(uint8_t index, float speed)
{
	if (speed > 12)
	{
		speed = 12;
	}
	else if (speed < -12)
	{
		speed = -12;
	}
	uint16_t _highTime = (THRUSTER_MAX_PWM_HIGHTIME - STOP_PWM_HIGHTIME) / 12.0 * speed + STOP_PWM_HIGHTIME;
	return Task_MotorSys_Thruster_SpeedSet(index,_highTime);	
}

int8_t Task_MotorSys_GetThrusterSpeed(uint8_t index)
{
	float speed = 0;
	uint16_t _highTime = Drv_PWM_ReadHighLvTime(&PWM[index]);
	speed = round((_highTime - STOP_PWM_HIGHTIME)/((THRUSTER_MAX_PWM_HIGHTIME - STOP_PWM_HIGHTIME)/ 12.0));
	return speed;
}

/**
 * @brief 垂直推进器开始工作
 * @param speed 速度档位 -12~+12共25档 负档位即推进器反转
 * @retval true/false
*/
bool Task_MotorSys_VerticalThruster_Start(float speed)
{
	/*垂推前后推力平衡;将前推进器降一档*/
	float front_speed = 0;
	float delta = 1 ;
	if (speed > 0)
	{
		front_speed = speed - delta;
		if (front_speed < 0)	/*防止出现负转速*/
		{
			front_speed = 0;
		}
	}
	else if (speed < 0)
	{
		front_speed = speed + delta;
		if (front_speed > 0)		/*防止出现正转速*/
		{
			front_speed = 0;
		}
	}
	else
	{
		front_speed = 0;
	}
	return	Task_MotorSys_Thruster_Start(LVT,front_speed)&&
			Task_MotorSys_Thruster_Start(RVT,front_speed)&&
			Task_MotorSys_Thruster_Start(BVT,speed);
}

/**
 * @brief 主推进器开始工作
 * @param speed 速度档位 -12~+12共25档 负档位即推进器反转
 * @retval true/false
*/
bool Task_MotorSys_MainThruster_Start(float speed)
{
	return 	Task_MotorSys_Thruster_Start(LMT,speed)&&
			Task_MotorSys_Thruster_Start(RMT,speed);
}

/**
 * @brief 所有推进器开始工作
 * @param speed 速度档位 -12~+12共25档 负档位即推进器反转
 * @retval true/false
*/
bool Task_MotorSys_AllThruster_Start(float speed)
{
	if (speed > 12)
	{
		speed = 12;
	}
	else if (speed < -12)
	{
		speed = -12;
	}
	uint16_t _highTime = (THRUSTER_MAX_PWM_HIGHTIME - STOP_PWM_HIGHTIME) / 12.0 * speed + STOP_PWM_HIGHTIME;
	return Task_MotorSys_AllThruster_SpeedSet(_highTime);
}

/**
 * @brief 推进器停止工作
 * @param index 推进器编号 LVT左垂推 RVT右垂推 BVT后垂推 LMT左主推 RMT右主推 宏定义在config.h中
 * @retval true/false
*/
bool Task_MotorSys_Thruster_Stop(uint8_t index)
{
	return Task_MotorSys_Thruster_SpeedSet(index,STOP_PWM_HIGHTIME);
}

/**
 * @brief 垂直推进器停止工作
 * @retval true/false
*/
bool Task_MotorSys_VerticalThruster_Stop(void)
{
	return 	Task_MotorSys_Thruster_Stop(LVT)&&
			Task_MotorSys_Thruster_Stop(RVT)&&
			Task_MotorSys_Thruster_Stop(BVT);
}

/**
 * @brief 主推进器停止工作
 * @retval true/false
*/
bool Task_MotorSys_MainThruster_Stop(void)
{
	return 	Task_MotorSys_Thruster_Stop(LMT)&&
			Task_MotorSys_Thruster_Stop(RMT);
}
/**
 * @brief 所有推进器停止工作
 * @retval true/false
*/
bool Task_MotorSys_AllThruster_Stop(void)
{
	return Task_MotorSys_AllThruster_SpeedSet(STOP_PWM_HIGHTIME);
}

/**
 * @brief AUV前进
 * @param speed 速度档位 0~12共13档
 * @retval true/false
*/
bool Task_MotorSys_GoForward(float speed)
{
	return Task_MotorSys_MainThruster_Start(speed);
}

/**
 * @brief AUV后退
 * @param speed 速度档位 0~12共13档
 * @retval true/false
*/
bool Task_MotorSys_GoBack(float speed)
{
	return Task_MotorSys_MainThruster_Start(-speed);
}

/**
 * @brief AUV上浮
 * @param speed 速度档位 0~12共13档
 * @retval true/false
*/
bool Task_MotorSys_Float(float speed)
{
	return Task_MotorSys_VerticalThruster_Start(speed);
}

/**
 * @brief AUV下潜
 * @param speed 速度档位 0~12共13档
 * @retval true/false
*/
bool Task_MotorSys_Dive(float speed)
{
	return Task_MotorSys_VerticalThruster_Start(-speed);
}

/**
 * @brief AUV左转 
 * @param speed 速度档位 0~12共13档
 * @retval true/false
*/
bool Task_MotorSys_TurnLeft(float speed)
{
	/*平衡转速，将正转的推进器降一档*/
	float front_speed = speed - 2;
	if (front_speed < 0)
	{
		front_speed = 0;
	}
	return 	Task_MotorSys_Thruster_Start(LMT,-speed)&&
			Task_MotorSys_Thruster_Start(RMT,front_speed);
}

/**
 * @brief AUV右转
 * @param speed 速度档位 0~12共13档
 * @retval true/false
*/
bool Task_MotorSys_TurnRight(float speed)
{
	/*平衡转速，将正转的推进器降一档*/
	float front_speed = speed - 2;
	if (front_speed < 0)
	{
		front_speed = 0;
	}	
	return 	Task_MotorSys_Thruster_Start(LMT,front_speed)&&
			Task_MotorSys_Thruster_Start(RMT,-speed);
}

/**
 * @brief AUV前倾
 * @param speed 速度档位 0~12共13档
 * @retval true/false
*/
bool Task_MotorSys_LeanForward(float speed)
{
	return 	Task_MotorSys_Thruster_Start(LVT,-speed)&&
			Task_MotorSys_Thruster_Start(RVT,-speed)&&
			Task_MotorSys_Thruster_Start(BVT,speed);
}

/**
 * @brief AUV后倾
 * @param speed 速度档位 0~12共13档
 * @retval true/false
*/
bool Task_MotorSys_LeanBack(float speed)
{
	return 	Task_MotorSys_Thruster_Start(LVT,speed)&&
			Task_MotorSys_Thruster_Start(RVT,speed)&&
			Task_MotorSys_Thruster_Start(BVT,-speed);
}

/**
 * @brief AUV左倾
 * @param speed 速度档位 0~12共13档
 * @retval true/false
*/
bool Task_MotorSys_LeanLeft(float speed)
{
	return 	Task_MotorSys_Thruster_Start(LVT,-speed)&&
			Task_MotorSys_Thruster_Start(RVT,speed);
}

/**
 * @brief AUV右倾
 * @param speed 速度档位 0~12共13档
 * @retval true/false
*/
bool Task_MotorSys_LeanRight(float speed)
{
	return	Task_MotorSys_Thruster_Start(LVT,speed)&&
			Task_MotorSys_Thruster_Start(RVT,-speed);
}

/**
 * @brief AUV定深
 * @param target_depth目标深度 单位/米
 * @retval true/false
*/
bool Task_MotorSys_SetDepth(float target_depth)
{
//	/*距离预定深度2m以上，全速下潜*/
//	if (target_depth - *p_depth >= 2)
//	{
//		return	Task_MotorSys_Dive(10);		
//	}
//	else if(target_depth - *p_depth > 1)		/*2m~1m，半速下潜*/
//	{
//		return  Task_MotorSys_Dive(8);		
//	}
//	else if (target_depth - *p_depth > 0.5)	/*1m~0.5m，低速下潜*/
//	{
//		return  Task_MotorSys_Dive(6);
//	}
//	else if (target_depth - *p_depth <= 0.5 || target_depth - *p_depth >= -0.5)	/*0.5m~-0.5m，缓慢下潜,尝试维持深度*/
//	{
//		return  Task_MotorSys_Dive(5);	
//	}
//	else									/*当前深度大于目标深度0.5m，停止，用自身浮力上浮*/
//	{
//		return  Task_MotorSys_Dive(0);
//	}
	
	//控制
	//期望值 初始值
	float ExpectValue = target_depth;
	float CurrentValue = *p_depth;
	
	float output = Algo_PID_Calculate(&SetDepthPID, CurrentValue, ExpectValue) * 5;
    
    // 输出限制（根据您的电机系统调整）
    if (output > 10.0f) output = 10.0f;  // 全速下潜
    if (output < 0.0f)   output = 0.0f;   // 停止推进器
    
	//PID系数值
	float PIDparameter[3] = {2, 0,0 };

	//更新PID系数，默认与结构体中相同
	Algo_PID_Update(&SetDepthPID,PIDparameter);
    
	Task_MotorSys_Dive(output);
	
	//舵板控制
//	float output = Algo_PID_Calculate(&PID, CurrentValue, target_depth);
//    
//    // 将PID输出映射到舵角范围：7°为中性点，±范围动态调整
//    float rudder_angle = 7.0f + output;  // 假设PID输出0对应7°（水平）
//    
//    // 发送舵角指令（底层已限制-15~22°）
//    Task_MotorSys_AllRudder_Angle_Set(rudder_angle);
//    
//    // 调试输出（可选）
//    printf("DepthErr=%.2fm, PIDOut=%.2f, Rudder=%.1f°\n", 
//           target_depth - *current_depth, output, rudder_angle);
}

/**
 * @brief AUV前进中定深,只动前2个垂推,当未达到深度时下潜
 * @param target_depth目标深度 单位/米
 * @retval true/false
*/
bool Task_MotorSys_SetDepthAndMove(float target_depth)
{
	float diff_depth = *p_depth - target_depth;
	//float abs_diff = fabsf(diff_depth);
	//int8_t sign = (diff_depth >= 0) ? 1 : -1;	//1上浮 -1下潜

	float pidoutput = Algo_PID_Calculate(&SetDepthAndMovePID,*p_depth,target_depth);
	
    // 输出限制
    if (pidoutput > 10.0f) pidoutput = 10.0f;
	if (pidoutput < -10.0f) pidoutput = -10.0f;
    //if (pidoutput < 4.0f)   pidoutput = 4.0f;	//维持auv深度的最小输出
    
	//PID系数值
	float PIDparameter[3] = {2, 0,0 };

	//更新PID系数，默认与结构体中相同
	Algo_PID_Update(&SetDepthAndMovePID,PIDparameter);
    
	//const float DEPTH_TOLERANCE = 0.1f;	//误差0.1m内认为方向正确
	
	Task_MotorSys_Thruster_Start(LVT,pidoutput);
	Task_MotorSys_Thruster_Start(RVT,pidoutput);

}
/**
 * @brief AUV定高
 * @param target_height目标深度 单位/米 高度计支持最大50m，最小0.5m；建议最小值1m
 * @retval true/false
*/
bool Task_MotorSys_SetHeight(float target_height)
{
	/*距离预定高度2m以上，全速下潜*/
	if (*p_height - target_height >= 2)
	{
		return	Task_MotorSys_Dive(10);		
	}
	else if(*p_height - target_height >= 1)	/*2m~1m，半速下潜*/
	{
		return  Task_MotorSys_Dive(8);		
	}
	else if (*p_height - target_height >= 0.5)	/*1m~0.5m，低速下潜*/
	{
		return  Task_MotorSys_Dive(6);
	}
	else if (*p_height - target_height <= 0.5 || *p_height - target_height >= -0.5)	/*0.5m~-0.5m，缓慢下潜,尝试维持高度*/
	{
		return  Task_MotorSys_Dive(5);	
	}
	else									/*当前高度小于目标高度0.5m，无操作，用自身浮力上浮*/
	{
		return Task_MotorSys_Dive(0);
	}
}

/**
 * @brief AUV定姿 维持俯仰角pitch和横滚角roll 说实话没啥用，全速前后左右倾都无法让AUV翻转；
 * @param 
 * @retval true/false
*/
//还没写




/**
 * @brief AUV原地定艏 让船头指向指定方位 原地转向
 * @param target_yaw 目标方位 按手柄摇杆定义，0度为东，90度为北，180为西，-90度为南
 * @retval true/false 方向正确时返回true,否则返回false
*/
bool Task_MotorSys_SetYaw(float target_yaw)
{
	/*将手柄摇杆定义的坐标系转换为传感器东北天坐标系*/
	float cov_target_yaw = transform_angle(target_yaw);
	if ((*p_yaw - cov_target_yaw >= 0)  && (*p_yaw - cov_target_yaw <= 180))
	{
		/*右转弯*/
		if (*p_yaw-cov_target_yaw > 5)
		{
			Task_MotorSys_TurnRight(5);
			return false;
		}
		else
		{	
			Task_MotorSys_TurnRight(0);
			return true;
		}
	}
	else
	{
		/*左转弯*/
		if (*p_yaw-cov_target_yaw < -5)
		{
			Task_MotorSys_TurnLeft(5);
			return false;
		}
		else
		{	
			Task_MotorSys_TurnLeft(0);
			return true;
		}		
	}

}



/**
 * @brief AUV定艏航行 让船头指向指定方位 并前进
 * @param target_yaw 目标方位 直接按传感器来
 * @param speed 速度档位 0~12共13档
 * @retval true/false 方向正确时返回true,否则返回false
*/
bool Task_MototSys_SetYawAndMove(float target_yaw,float speed)
{
	//float cov_target_yaw = transform_angle(target_yaw);
	//Task_MotorSys_SetDepthAndMove(target_depth);	//结合前进定深
	target_yaw = normalize_angle(target_yaw);
	float diff_yaw = normalize_angle(*p_yaw - target_yaw);
	float abs_diff = fabsf(diff_yaw);
	int8_t sign = (diff_yaw >= 0) ? 1 : -1;	//1右转弯 -1左转弯

	float pidoutput = Algo_PID_Calculate(&SetYawAndMovePID, 0.0f,abs_diff);
    //float pidoutput = Algo_PID_Calculate(&SetYawAndMovePID, *p_yaw,target_yaw);
	
    // 输出限制
    if (pidoutput > speed) pidoutput = speed;
    if (pidoutput < 0.0f)   pidoutput = 0.0f;
    
	//PID系数值
	float PIDparameter[3] = {2, 0,0 };

	//更新PID系数，默认与结构体中相同
	Algo_PID_Update(&SetYawAndMovePID,PIDparameter);
    
	/*将手柄摇杆定义的坐标系转换为传感器东北天坐标系*/

//	float BaseForce = speed * 0.5;	//基准前进力,为了给转向力留出差速空间,乘比例系数
//	float turnForce = pidoutput * 0.5;		//转向力	由pid得出
	float BaseForce = speed * 0.5f;  // 基准前进力

	// 转向力系数，速度越高越小，限制最低为0.2
	float k = 1.0f - (BaseForce / 10.0f);
	if (k < 0.2f) k = 0.2f;

	float turnForce = pidoutput * k;  // 自适应转向力

	/*p_yaw - cov_target_yaw 值用于判断转向 */

	
	//最大前进力10 最大转向力10,合成力=前进力±转向力 当前进力大于转向力时两个推进器都正转,当前进力等于转向力时有一个推进器正好停止,当前进力小于转向力时有一个推进器倒转
	const float YAW_TOLERANCE = 1.0f;	//误差2度内认为方向正确
	
    if (abs_diff < YAW_TOLERANCE) {
        // 方向正确，保持前进
        Task_MotorSys_Thruster_Start(LMT, BaseForce);
        Task_MotorSys_Thruster_Start(RMT, BaseForce);
        return true;
    } else {
        // 调整方向
        Task_MotorSys_Thruster_Start(LMT, BaseForce + sign * turnForce);
        Task_MotorSys_Thruster_Start(RMT, BaseForce - sign * turnForce);
        return true;
    }
}
/*推进器测试*/
void Task_MotorSys_Thruster_Test()
{
		//初始化
		//Task_MotorSys_Init();
		
		//单独测试
		Task_MotorSys_Thruster_Start(LVT,10);
		Drv_Delay_Ms(5000);
		Task_MotorSys_Thruster_Stop(LVT);
		Task_MotorSys_Thruster_Start(RVT,10);
		Drv_Delay_Ms(5000);
		Task_MotorSys_Thruster_Stop(RVT);
		Task_MotorSys_Thruster_Start(BVT,10);
		Drv_Delay_Ms(5000);
		Task_MotorSys_Thruster_Stop(BVT);
		Task_MotorSys_Thruster_Start(LMT,10);
		Drv_Delay_Ms(5000);
		Task_MotorSys_Thruster_Stop(LMT);
		Task_MotorSys_Thruster_Start(RMT,10);
		Drv_Delay_Ms(5000);
		Task_MotorSys_Thruster_Stop(RMT);
		Drv_Delay_Ms(2000);
		
		Task_MotorSys_AllThruster_Stop();
//		
//		//编组测试
//		Task_MotorSys_AllThruster_Start(10);
//		Drv_Delay_Ms(5000);
//		Task_MotorSys_AllThruster_Stop();
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_AllThruster_Start(-10);//反转测试
//		Drv_Delay_Ms(5000);
//		Task_MotorSys_AllThruster_Stop();
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_VerticalThruster_Start(10);
//		Drv_Delay_Ms(5000);
//		Task_MotorSys_VerticalThruster_Stop();
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_MainThruster_Start(10);
//		Drv_Delay_Ms(5000);
//		Task_MotorSys_MainThruster_Stop();
//		Drv_Delay_Ms(2000);
//		//转弯测试
//		Task_MotorSys_TurnLeft(10);
//		Drv_Delay_Ms(5000);
//		Task_MotorSys_MainThruster_Stop();
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_TurnRight(10);
//		Drv_Delay_Ms(5000);
//		Task_MotorSys_MainThruster_Stop();
//		Drv_Delay_Ms(2000);		
//		//上浮下潜
//		Task_MotorSys_Float(10);
//		Drv_Delay_Ms(5000);
//		Task_MotorSys_VerticalThruster_Stop();
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_Dive(10);
//		Drv_Delay_Ms(5000);
//		Task_MotorSys_VerticalThruster_Stop();
//		Drv_Delay_Ms(2000);
//		//前倾后倾
//		Task_MotorSys_LeanForward(10);
//		Drv_Delay_Ms(5000);
//		Task_MotorSys_VerticalThruster_Stop();
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_LeanBack(10);
//		Drv_Delay_Ms(5000);
//		Task_MotorSys_VerticalThruster_Stop();
//		Drv_Delay_Ms(2000);
//		//变速测试
//		Task_MotorSys_AllThruster_Start(1);
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_AllThruster_Start(2);
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_AllThruster_Start(3);
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_AllThruster_Start(4);
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_AllThruster_Start(5);
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_AllThruster_Start(6);
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_AllThruster_Start(7);
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_AllThruster_Start(8);
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_AllThruster_Start(9);
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_AllThruster_Start(10);
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_AllThruster_Start(11);
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_AllThruster_Start(12);	
//		Drv_Delay_Ms(2000);
		
//		Task_MotorSys_AllThruster_Stop();
//		Drv_Delay_Ms(2000);

//		//停止
//		Task_MotorSys_AllThruster_Stop();
}
/*机械手测试*/
void Task_MotorSys_Manipulator_Test()
{
		Task_MotorSys_Manipulator_Open();
		Drv_Delay_Ms(2000);//保持张开状态2s
		Task_MotorSys_Manipulator_Close();
		Drv_Delay_Ms(2000);
}
/*舵机测试*/
void Task_MotorSys_Servos_Test()
{
//		//左舵向上最大-55 对应右舵向上最大为70
//		Task_MotorSys_Steer_Angle_Set(LS,-55);
//		Task_MotorSys_Steer_Angle_Set(RS,+70);
//		Drv_Delay_Ms(2000);
//		//平舵
//		Task_MotorSys_Steer_Angle_Set(LS,-10);
//		Task_MotorSys_Steer_Angle_Set(RS,25);
//		Drv_Delay_Ms(2000);
//		//左舵向下最大20 对应的右舵向下最大为-5
//		Task_MotorSys_Steer_Angle_Set(LS,20);
//		Task_MotorSys_Steer_Angle_Set(RS,-5);
//		Drv_Delay_Ms(2000);
//		//平舵 左舵-10 右舵25
//		Task_MotorSys_Steer_Angle_Set(LS,-10);
//		Task_MotorSys_Steer_Angle_Set(RS,25);
//		Drv_Delay_Ms(2000);
//		//任意角度测试 右舵机角度 = -左舵机角度+15
//		Task_MotorSys_Steer_Angle_Set(LS,-30);
//		Task_MotorSys_Steer_Angle_Set(RS,45);
//		Drv_Delay_Ms(2000);

//		Task_MotorSys_AllRudder_Angle_Set(0);
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_AllRudder_Angle_Set(-15);
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_AllRudder_Angle_Set(-8);
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_AllRudder_Angle_Set(0);
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_AllRudder_Angle_Set(11);
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_AllRudder_Angle_Set(22);
//		Drv_Delay_Ms(2000);
		
		//慢速摆动 只能从指定角度开始
//		Task_MotorSys_Rudder_Angle_Set(LS,-15);
//		Task_MotorSys_Rudder_Angle_Set(RS,-15);
//		Drv_Delay_Ms(2000);	
//		for (int8_t i=-15;i<22;i++)
//		{
//			Task_MotorSys_AllRudder_Angle_Set(i);
//			Drv_Delay_Ms(100);
//		}
//		for (int8_t i=22;i>-15;i--)
//		{
//			Task_MotorSys_AllRudder_Angle_Set(i);
//			Drv_Delay_Ms(100);
//		}

		//增量式运动,较为精准,且两舵板同步
		Task_MotorSys_Rudder_Angle_Set(LS,-15);
		Task_MotorSys_Rudder_Angle_Set(RS,-15);
		Drv_Delay_Ms(2000);	
		while (rudder_ang_left<22)
		{
			Task_MotorSys_Rudder_Angle_Add(LS,1.5);
			Task_MotorSys_Rudder_Angle_Add(RS,1.5);
			Drv_Delay_Ms(100);
		}
		Task_MotorSys_Rudder_Angle_Set(LS,22);
		Task_MotorSys_Rudder_Angle_Set(RS,22);
		while (rudder_ang_left>-15)
		{
			Task_MotorSys_Rudder_Angle_Add(LS,-1.5);
			Task_MotorSys_Rudder_Angle_Add(RS,-1.5);
			Drv_Delay_Ms(100);
		}		
//		for (int8_t i=-15;i<22;i++)
//		{
//			Task_MotorSys_Rudder_Angle_Add(LS,1);
//			Task_MotorSys_Rudder_Angle_Add(RS,1);
//			Drv_Delay_Ms(100);
//		}
//		for (int8_t i=22;i>-15;i--)
//		{
//			Task_MotorSys_Rudder_Angle_Add(LS,-1);
//			Task_MotorSys_Rudder_Angle_Add(RS,-1);
//			Drv_Delay_Ms(100);
//		}		
		
}


/*动力系统句柄*/
void Task_MotorSys_Handle(void)
{	
	//Task_MotorSys_Manipulator_Test();
	//Task_MotorSys_Thruster_Test();
	//Task_MotorSys_Servos_Test();	
	//Task_MotorSys_SetDepth(1);
	//Task_MototSys_SetYawAndMove(-9,6);
	//Task_MotorSys_SetDepthAndMove(0);
}
