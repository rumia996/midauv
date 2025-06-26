#include "task_automode.h"
#include "task_motorsys.h"

/* �Զ�ģʽ��Ϣ�ṹ��ʵ������ */
AutoModeInfo AMInfo =
{
	.Auto_Horizontal_Mode = 	AUTO_STOP,
	.Auto_Vertical_Mode =		AUTO_STOP,
	.target_yaw = 		0,
	.target_depth = 	0,
	.target_height = 	0,
	.target_speed = 	0,
};

void AutoModeInfo_Init(void)
{
	AMInfo.Auto_Horizontal_Mode = 	AUTO_STOP;
	AMInfo.Auto_Vertical_Mode = 	AUTO_STOP;
	AMInfo.target_yaw = 	0;
	AMInfo.target_depth = 	0;
	AMInfo.target_height = 	0;
	AMInfo.target_speed = 	0;
}
/**
 * @brief �Զ�ģʽ������
 * @param AMInfo �Զ�ģʽ��Ϣ�ṹ��ָ��
 */
void Task_AutoMode_Process(AutoModeInfo *pAMInfo)
{
	/*ˮƽ�˶�*/
	if (pAMInfo->Auto_Horizontal_Mode == AUTO_GOFORWARD)
	{
		Task_MotorSys_GoForward(pAMInfo->target_speed);
	}
	else if (pAMInfo->Auto_Horizontal_Mode == AUTO_GOBACK)
	{
		Task_MotorSys_GoBack(pAMInfo->target_speed);
	}
	else if (pAMInfo->Auto_Horizontal_Mode == AUTO_TURNLEFT)
	{
		Task_MotorSys_TurnLeft(pAMInfo->target_speed);
	}
	else if (pAMInfo->Auto_Horizontal_Mode == AUTO_TURNRIGHT)
	{
		Task_MotorSys_TurnRight(pAMInfo->target_speed);
	}
	else if (pAMInfo->Auto_Horizontal_Mode == AUTO_SETYAW)
	{
		//Task_MotorSys_SetYaw(pAMInfo->target_yaw);
		Task_MototSys_SetYawAndMove(pAMInfo->target_yaw,pAMInfo->target_speed);
	}
	else
	{
		Task_MotorSys_MainThruster_Stop();
	}
	
	/*��ֱ�˶�*/
	if (pAMInfo->Auto_Vertical_Mode == AUTO_FLOAT)
	{
		Task_MotorSys_Float(pAMInfo->target_speed);
	}
	else if (pAMInfo->Auto_Vertical_Mode == AUTO_DIVE)
	{
		Task_MotorSys_Dive(pAMInfo->target_speed);
	}
	else if (pAMInfo->Auto_Vertical_Mode == AUTO_SETDEPTH)
	{
		Task_MotorSys_SetDepth(pAMInfo->target_depth);
	}
	else if (pAMInfo->Auto_Vertical_Mode == AUTO_SETHEIGHT)
	{
		Task_MotorSys_SetHeight(pAMInfo->target_height);
	}	
	else
	{
		Task_MotorSys_VerticalThruster_Stop();
	}
}
