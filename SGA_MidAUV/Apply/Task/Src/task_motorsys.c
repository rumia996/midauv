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
/*�ƽ����ߵ�ƽʱ�����ֵ����������ƽ�� us*/
#define LVT_DELTA 0
#define RVT_DELTA 0
#define BVT_DELTA 0
#define LMT_DELTA 0
#define RMT_DELTA 0

/**
 * @brief ����ϵͳ��ʼ��
 * @retval null
*/
void Task_MotorSys_Init(void)
{
	Task_MotorSys_AllThruster_Stop();
	Task_MotorSys_AllSteer_0Angle();
	Task_MotorSys_Manipulator_Close();
	Drv_Delay_Ms(4000);//����ֹͣλ4���õ����ʼ��
}
/*
 *  ����źŷ�Χ500-2500us(2.5%-12.5%)���Ʋ��1500us(7.5%)��ͣת
 *   �Ƕ�270��,����135����ת����Ϊ������ʱ������ʱ����ת
 *   PWM���㹫ʽΪ:ռ�ձ�=(12.5-2.5)/135*angle+7.5 �ߵ�ƽʱ�� = (2500-500)/135*angle+1500����angle�������ĽǶ�,-135��~+135��
 *  10.83%  2167		+90��
 *  9.16%  1833 	+45��
 *  7.5%   1500		0��
 *  5.83%  1167		-45��
 *  4.16% 833 		-90��
 * 	���� ռ�ձ�=(12.5-2.5)/270*angle �ߵ�ƽʱ�� = (2500-500)/270*angle angle��0-270��
 *	���� -15�ȶ�Ӧ���ˮƽ 0�ȶ��΢΢���� 10�ȶ�����´ﵽ���-90�ȶ�����ϴﵽ��� 
 *  �Ҷ�� 15�ȶ�Ӧ���ˮƽ 0�ȶ��΢΢���� -10�ȶ�����´ﵽ���90�ȶ�����ϴﵽ���
 */
 
/**
 * @brief ��������Ƕ�����
 * @param ������ ����LS �Ҷ��RS �궨����config.h
 * @param �Ƕ�ֵ -135��~+135��
 * @retval true/false
*/
bool Task_MotorSys_Steer_Angle_Set(uint8_t index,int16_t ang)
{
	if (index == LS)
	{
		if (ang >=-90 && ang <= 10)
		{
			uint16_t _highTime = (SERVORS_DELTA+ 0.7f) * ang + SERVORS_0ANGLE_PWM_HIGHTIME;//ang -135��~+135��0.7f�ǲ���ֵ��ͨ�����Զ���Գ�����
			Drv_PWM_HighLvTimeSet(&PWM[index], _highTime);		
			return true;
		}
	}
	if (index == RS)
	{
		if (ang >= -10 && ang <= 90)
		{
			uint16_t _highTime = (SERVORS_DELTA+ 0.7f) * ang + SERVORS_0ANGLE_PWM_HIGHTIME;//ang -135��~+135��0.7f�ǲ���ֵ��ͨ�����Զ���Գ�����
			Drv_PWM_HighLvTimeSet(&PWM[index], _highTime);		
			return true;
		}
	}	
//	if (index == LS || index == RS)
//	{
//		if (ang >= -135 && ang <= 135)
//		{
//			uint16_t _highTime = (SERVORS_DELTA+ 0.7f) * ang + SERVORS_0ANGLE_PWM_HIGHTIME;//ang -135��~+135��0.7f�ǲ���ֵ��ͨ�����Զ���Գ�����
//			//uint16_t _highTime = (SERVORS_MAXANGLE_PWM_HIGHTIME - SERVORS_MINANGLE_PWM_HIGHTIME) / SERVORS_ANGLE_RANGE * ang;//ang 0-270��
//			Drv_PWM_HighLvTimeSet(&PWM[index], _highTime);			
//		}

//		return true;
//	}
	return false;
}

/**
 * @brief ���ж���Ƕ�����
 * @param ������ ����LS �Ҷ��RS �궨����config.h
 * @param �Ƕ�ֵ 
 * @retval true/false
*/
bool Task_MotorSys_AllSteer_Angle_Set(int16_t ang)
{
	if (ang >= -135 && ang <= 135)
	{
		return Task_MotorSys_Steer_Angle_Set(LS,ang) && Task_MotorSys_Steer_Angle_Set(RS,ang);
	}
	return false;
}

/**
 * @brief �����λ
 * @param ������ ����LS �Ҷ��RS �궨����config.h
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
 * @brief ���ж����λ
 * @retval true/false
*/
bool Task_MotorSys_AllSteer_0Angle(void)
{
	Drv_PWM_HighLvTimeSet(&PWM[LS], SERVORS_0ANGLE_PWM_HIGHTIME);
	Drv_PWM_HighLvTimeSet(&PWM[RS], SERVORS_0ANGLE_PWM_HIGHTIME);
	return true;
}

/**
 * @brief ��������Ƕ������仯
 * @param ������ ����LS �Ҷ��RS �궨����config.h
 * @param ����ֵ 
 * @retval true/false
*/
bool Task_MotorSys_Steer_Angle_Add(uint8_t index,int16_t ang)
{
	if (index == LS || index == RS)
	{
		uint16_t _highTime = Drv_PWM_ReadHighLvTime(&PWM[index]) + ang * (2000/270);//����ʽ����!���Ƕ�����Ǻ���Ҫ����û��ϵ:)
		if(_highTime <= 2500 && _highTime > 500)
		{
			Drv_PWM_HighLvTimeSet(&PWM[index], _highTime);
			return true;
		}
	}
	return false;
}

/**
 * @brief ��е���ſ� ��ȫ�ſ���Ҫ1��
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
 * @brief ��е�ֹر� ��ȫ�ر���Ҫ1��
 * @retval true/false
*/
bool Task_MotorSys_Manipulator_Close(void)
{
	Drv_PWM_HighLvTimeSet(&PWM[MANIPULATOR], MANIPULATOR_CLOSE_PWM_HIGHTIME);
	Drv_Delay_Ms(1000);
	Drv_PWM_DutyFactorSet(&PWM[MANIPULATOR], STOP_PWM_HIGHTIME);
	return true;
}


//�ƽ������ƴӵ����ƽ�����ʼ����װ �ײ�speedset-�м��start&stop-�߼���ǰ�������ϸ���Ǳ��ת��ת

/**
 * @brief �ƽ���ת�������Ӻ��� 	���Ƶ����ƽ��� ֧������΢��
 * @param index �ƽ������ LVT���� RVT�Ҵ��� BVT���� LMT������ RMT������ �궨����config.h��
 * @param _highTime �ߵ�ƽʱ���С	_highTime������Ϊ1000-2000,1500ֹͣ,1000-1500��ת,1500-2000��ת
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
 * @brief �����ƽ�����ͬת�����ú���
 * @param _highTime �ߵ�ƽʱ���С	_highTime������Ϊ1000-2000,1500ֹͣ,1000-1500��ת,1500-2000��ת
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
 * @brief �����ƽ�����ʼ����
 * @param index �ƽ������ LVT���� RVT�Ҵ��� BVT���� LMT������ RMT������ �궨����config.h��
 * @param speed �ٶȵ�λ -12~+12��25�� ����λ���ƽ�����ת
 * @retval true/false
*/
bool Task_MotorSys_Thruster_Start(uint8_t index,int8_t speed)
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

/**
 * @brief ��ֱ�ƽ�����ʼ����
 * @param speed �ٶȵ�λ -12~+12��25�� ����λ���ƽ�����ת
 * @retval true/false
*/
bool Task_MotorSys_VerticalThruster_Start(int8_t speed)
{
	/*����ǰ������ƽ��;��ǰ�ƽ�����һ��*/
	int8_t front_speed = 0;
	int8_t delta = 2;
	if (speed > 0)
	{
		front_speed = speed - delta;
		if (front_speed < 0)	/*��ֹ���ָ�ת��*/
		{
			front_speed = 0;
		}
	}
	else if (speed < 0)
	{
		front_speed = speed + delta;
		if (front_speed > 0)		/*��ֹ������ת��*/
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
 * @brief ���ƽ�����ʼ����
 * @param speed �ٶȵ�λ -12~+12��25�� ����λ���ƽ�����ת
 * @retval true/false
*/
bool Task_MotorSys_MainThruster_Start(int8_t speed)
{
	return 	Task_MotorSys_Thruster_Start(LMT,speed)&&
			Task_MotorSys_Thruster_Start(RMT,speed);
}

/**
 * @brief �����ƽ�����ʼ����
 * @param speed �ٶȵ�λ -12~+12��25�� ����λ���ƽ�����ת
 * @retval true/false
*/
bool Task_MotorSys_AllThruster_Start(int8_t speed)
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
 * @brief �ƽ���ֹͣ����
 * @param index �ƽ������ LVT���� RVT�Ҵ��� BVT���� LMT������ RMT������ �궨����config.h��
 * @retval true/false
*/
bool Task_MotorSys_Thruster_Stop(uint8_t index)
{
	return Task_MotorSys_Thruster_SpeedSet(index,STOP_PWM_HIGHTIME);
}

/**
 * @brief ��ֱ�ƽ���ֹͣ����
 * @retval true/false
*/
bool Task_MotorSys_VerticalThruster_Stop(void)
{
	return 	Task_MotorSys_Thruster_Stop(LVT)&&
			Task_MotorSys_Thruster_Stop(RVT)&&
			Task_MotorSys_Thruster_Stop(BVT);
}

/**
 * @brief ���ƽ���ֹͣ����
 * @retval true/false
*/
bool Task_MotorSys_MainThruster_Stop(void)
{
	return 	Task_MotorSys_Thruster_Stop(LMT)&&
			Task_MotorSys_Thruster_Stop(RMT);
}
/**
 * @brief �����ƽ���ֹͣ����
 * @retval true/false
*/
bool Task_MotorSys_AllThruster_Stop(void)
{
	return Task_MotorSys_AllThruster_SpeedSet(STOP_PWM_HIGHTIME);
}

/**
 * @brief AUVǰ��
 * @param speed �ٶȵ�λ 0~12��13��
 * @retval true/false
*/
bool Task_MotorSys_GoForward(uint8_t speed)
{
	return Task_MotorSys_MainThruster_Start(speed);
}

/**
 * @brief AUV����
 * @param speed �ٶȵ�λ 0~12��13��
 * @retval true/false
*/
bool Task_MotorSys_GoBack(uint8_t speed)
{
	return Task_MotorSys_MainThruster_Start(-speed);
}

/**
 * @brief AUV�ϸ�
 * @param speed �ٶȵ�λ 0~12��13��
 * @retval true/false
*/
bool Task_MotorSys_Float(uint8_t speed)
{
	return Task_MotorSys_VerticalThruster_Start(speed);
}

/**
 * @brief AUV��Ǳ
 * @param speed �ٶȵ�λ 0~12��13��
 * @retval true/false
*/
bool Task_MotorSys_Dive(uint8_t speed)
{
	return Task_MotorSys_VerticalThruster_Start(-speed);
}

/**
 * @brief AUV��ת 
 * @param speed �ٶȵ�λ 0~12��13��
 * @retval true/false
*/
bool Task_MotorSys_TurnLeft(uint8_t speed)
{
	/*ƽ��ת�٣�����ת���ƽ�����һ��*/
	int8_t front_speed = speed - 2;
	if (front_speed < 0)
	{
		front_speed = 0;
	}
	return 	Task_MotorSys_Thruster_Start(LMT,-speed)&&
			Task_MotorSys_Thruster_Start(RMT,front_speed);
}

/**
 * @brief AUV��ת
 * @param speed �ٶȵ�λ 0~12��13��
 * @retval true/false
*/
bool Task_MotorSys_TurnRight(uint8_t speed)
{
	/*ƽ��ת�٣�����ת���ƽ�����һ��*/
	int8_t front_speed = speed - 2;
	if (front_speed < 0)
	{
		front_speed = 0;
	}	
	return 	Task_MotorSys_Thruster_Start(LMT,front_speed)&&
			Task_MotorSys_Thruster_Start(RMT,-speed);
}

/**
 * @brief AUVǰ��
 * @param speed �ٶȵ�λ 0~12��13��
 * @retval true/false
*/
bool Task_MotorSys_LeanForward(uint8_t speed)
{
	return 	Task_MotorSys_Thruster_Start(LVT,-speed)&&
			Task_MotorSys_Thruster_Start(RVT,-speed)&&
			Task_MotorSys_Thruster_Start(BVT,speed);
}

/**
 * @brief AUV����
 * @param speed �ٶȵ�λ 0~12��13��
 * @retval true/false
*/
bool Task_MotorSys_LeanBack(uint8_t speed)
{
	return 	Task_MotorSys_Thruster_Start(LVT,speed)&&
			Task_MotorSys_Thruster_Start(RVT,speed)&&
			Task_MotorSys_Thruster_Start(BVT,-speed);
}

/**
 * @brief AUV����
 * @param speed �ٶȵ�λ 0~12��13��
 * @retval true/false
*/
bool Task_MotorSys_LeanLeft(uint8_t speed)
{
	return 	Task_MotorSys_Thruster_Start(LVT,-speed)&&
			Task_MotorSys_Thruster_Start(RVT,speed);
}

/**
 * @brief AUV����
 * @param speed �ٶȵ�λ 0~12��13��
 * @retval true/false
*/
bool Task_MotorSys_LeanRight(uint8_t speed)
{
	return	Task_MotorSys_Thruster_Start(LVT,speed)&&
			Task_MotorSys_Thruster_Start(RVT,-speed);
}

/**
 * @brief AUV����
 * @param target_depthĿ����� ��λ/��
 * @retval true/false
*/
bool Task_MotorSys_SetDepth(float target_depth)
{
	/*����Ԥ�����2m���ϣ�ȫ����Ǳ*/
	if (target_depth - *p_depth >= 2)
	{
		return	Task_MotorSys_Dive(10);		
	}
	else if(target_depth - *p_depth > 1)		/*2m~1m��������Ǳ*/
	{
		return  Task_MotorSys_Dive(8);		
	}
	else if (target_depth - *p_depth > 0.5)	/*1m~0.5m��������Ǳ*/
	{
		return  Task_MotorSys_Dive(6);
	}
	else if (target_depth - *p_depth <= 0.5 || target_depth - *p_depth >= -0.5)	/*0.5m~-0.5m��������Ǳ,����ά�����*/
	{
		return  Task_MotorSys_Dive(5);	
	}
	else									/*��ǰ��ȴ���Ŀ�����0.5m��ֹͣ�����������ϸ�*/
	{
		return  Task_MotorSys_Dive(0);
	}
}

/**
 * @brief AUV����
 * @param target_heightĿ����� ��λ/�� �߶ȼ�֧�����50m����С0.5m��������Сֵ1m
 * @retval true/false
*/
bool Task_MotorSys_SetHeight(float target_height)
{
	/*����Ԥ���߶�2m���ϣ�ȫ����Ǳ*/
	if (*p_height - target_height >= 2)
	{
		return	Task_MotorSys_Dive(10);		
	}
	else if(*p_height - target_height >= 1)	/*2m~1m��������Ǳ*/
	{
		return  Task_MotorSys_Dive(8);		
	}
	else if (*p_height - target_height >= 0.5)	/*1m~0.5m��������Ǳ*/
	{
		return  Task_MotorSys_Dive(6);
	}
	else if (*p_height - target_height <= 0.5 || *p_height - target_height >= -0.5)	/*0.5m~-0.5m��������Ǳ,����ά�ָ߶�*/
	{
		return  Task_MotorSys_Dive(5);	
	}
	else									/*��ǰ�߶�С��Ŀ��߶�0.5m���޲��������������ϸ�*/
	{
		return Task_MotorSys_Dive(0);
	}
}

/**
 * @brief AUV���� ά�ָ�����pitch�ͺ����roll ˵ʵ��ûɶ�ã�ȫ��ǰ�������㶼�޷���AUV��ת��
 * @param 
 * @retval true/false
*/
//��ûд




/**
 * @brief AUV���� �ô�ͷָ��ָ����λ
 * @param target_yaw Ŀ�귽λ ���ֱ�ҡ�˶��壬0��Ϊ����90��Ϊ����180Ϊ����-90��Ϊ��
 * @retval true/false ������ȷʱ����true,���򷵻�false
*/
bool Task_MotorSys_SetYaw(float target_yaw)
{
	/*���ֱ�ҡ�˶��������ϵת��Ϊ����������������ϵ*/
	float cov_target_yaw = transform_angle(target_yaw);
	if ((*p_yaw - cov_target_yaw >= 0)  && (*p_yaw - cov_target_yaw <= 180))
	{
		/*��ת��*/
		if (*p_yaw-cov_target_yaw > 45)
		{
			Task_MotorSys_TurnRight(5);
			return false;
		}
		else if (*p_yaw-cov_target_yaw > 30)
		{
			Task_MotorSys_TurnRight(5);
			return false;
		}
		else if (*p_yaw-cov_target_yaw > 15)
		{
			Task_MotorSys_TurnRight(5);
			return false;
		}
		else if (*p_yaw-cov_target_yaw > 5)
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
		/*��ת��*/
		if (*p_yaw-cov_target_yaw < -45)
		{
			Task_MotorSys_TurnLeft(5);
			return false;
		}
		else if (*p_yaw-cov_target_yaw < -30)
		{
			Task_MotorSys_TurnLeft(5);
			return false;
		}
		else if (*p_yaw-cov_target_yaw < -15)
		{
			Task_MotorSys_TurnLeft(5);
			return false;
		}
		else if (*p_yaw-cov_target_yaw < -5)
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


/*�ƽ�������*/
void Task_MotorSys_Thruster_Test()
{
		//��ʼ��
		//Task_MotorSys_Init();
		
		//��������
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
		Drv_Delay_Ms(2000);
//		
//		//�������
//		Task_MotorSys_AllThruster_Start(10);
//		Drv_Delay_Ms(5000);
//		Task_MotorSys_AllThruster_Stop();
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_AllThruster_Start(-10);//��ת����
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
//		//ת�����
//		Task_MotorSys_TurnLeft(10);
//		Drv_Delay_Ms(5000);
//		Task_MotorSys_MainThruster_Stop();
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_TurnRight(10);
//		Drv_Delay_Ms(5000);
//		Task_MotorSys_MainThruster_Stop();
//		Drv_Delay_Ms(2000);		
//		//�ϸ���Ǳ
//		Task_MotorSys_Float(10);
//		Drv_Delay_Ms(5000);
//		Task_MotorSys_VerticalThruster_Stop();
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_Dive(10);
//		Drv_Delay_Ms(5000);
//		Task_MotorSys_VerticalThruster_Stop();
//		Drv_Delay_Ms(2000);
//		//ǰ�����
//		Task_MotorSys_LeanForward(10);
//		Drv_Delay_Ms(5000);
//		Task_MotorSys_VerticalThruster_Stop();
//		Drv_Delay_Ms(2000);
//		Task_MotorSys_LeanBack(10);
//		Drv_Delay_Ms(5000);
//		Task_MotorSys_VerticalThruster_Stop();
//		Drv_Delay_Ms(2000);
//		//���ٲ���
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

//		//ֹͣ
//		Task_MotorSys_AllThruster_Stop();
}
/*��е�ֲ���*/
void Task_MotorSys_Manipulator_Test()
{
		Task_MotorSys_Manipulator_Open();
		Drv_Delay_Ms(2000);//�����ſ�״̬2s
		Task_MotorSys_Manipulator_Close();
		Drv_Delay_Ms(2000);
}
/*�������*/
void Task_MotorSys_Servos_Test()
{
	while(1)
	{
		Task_MotorSys_Steer_Angle_Set(LS,-90);
		Drv_Delay_Ms(2000);
		Task_MotorSys_Steer_Angle_Set(LS,10);
		Drv_Delay_Ms(2000);
		Task_MotorSys_Steer_Angle_Set(RS,90);
		Drv_Delay_Ms(2000);
		Task_MotorSys_Steer_Angle_Set(RS,-10);
		Drv_Delay_Ms(2000);
		Task_MotorSys_AllSteer_0Angle();
		Drv_Delay_Ms(2000);
	}
}


/*����ϵͳ���*/
void Task_MotorSys_Handle(void)
{	
	//Task_MotorSys_Servos_Test();
	//Task_MotorSys_Thruster_Test();
}
