#ifndef __TASK_MOTORSYS_H__
#define __TASK_MOTORSYS_H__

#include "drv_hal_conf.h"
#include "config.h"
#include "usercode.h"

void Task_MotorSys_Init(void);

bool Task_MotorSys_Rudder_Angle_Set(uint8_t index ,float ang);
bool Task_MotorSys_AllRudder_Angle_Set(float ang);
bool Task_MotorSys_Rudder_Angle_Add(uint8_t index,float ang);
float Task_MotorSys_GetRudder_Angle(uint8_t index);

bool Task_MotorSys_Steer_Angle_Set(uint8_t index,float ang);
bool Task_MotorSys_AllSteer_Angle_Set(float ang);
bool Task_MotorSys_Steer_0Angle(uint8_t index);
bool Task_MotorSys_AllSteer_0Angle(void);
bool Task_MotorSys_Steer_Angle_Add(uint8_t index,int16_t ang);

bool Task_MotorSys_Manipulator_Open(void);
bool Task_MotorSys_Manipulator_Close(void);

bool Task_MotorSys_Thruster_SpeedSet(uint8_t index,uint16_t _highTime);
bool Task_MotorSys_AllThruster_SpeedSet(uint16_t _highTime);

int8_t Task_MotorSys_GetThrusterSpeed(uint8_t index);
bool Task_MotorSys_Thruster_Start(uint8_t index,float speed);
bool Task_MotorSys_VerticalThruster_Start(float speed);
bool Task_MotorSys_MainThruster_Start(float speed);
bool Task_MotorSys_AllThruster_Start(float speed);

bool Task_MotorSys_Thruster_Stop(uint8_t index);
bool Task_MotorSys_VerticalThruster_Stop(void);
bool Task_MotorSys_MainThruster_Stop(void);
bool Task_MotorSys_AllThruster_Stop(void);

bool Task_MotorSys_GoForward(float speed);
bool Task_MotorSys_GoBack(float speed);
bool Task_MotorSys_Float(float speed);
bool Task_MotorSys_Dive(float speed);
bool Task_MotorSys_TurnLeft(float speed);
bool Task_MotorSys_TurnRight(float speed);
bool Task_MotorSys_LeanForward(float speed);
bool Task_MotorSys_LeanBack(float speed);
bool Task_MotorSys_LeanLeft(float speed);
bool Task_MotorSys_LeanRight(float speed);

bool Task_MotorSys_SetDepth(float target_depth);
bool Task_MotorSys_SetDepthAndMove(float target_depth);
bool Task_MotorSys_SetHeight(float target_height);
bool Task_MotorSys_SetYaw(float target_yaw);
bool Task_MototSys_SetYawAndMove(float target_yaw, float speed);

void Task_MotorSys_Manipulator_Test(void);
void Task_MotorSys_Thruster_Test(void);
void Task_MotorSys_Servos_Test(void);
void Task_MotorSys_Handle(void);

#endif // !__TASK_MOTORSYS_H__
