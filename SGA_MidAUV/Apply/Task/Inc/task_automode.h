#ifndef __TASK_AUTOMODE_H_
#define __TASK_AUTOMODE_H_

#include "drv_hal_conf.h"

#define AUTO_STOP		0
#define AUTO_GOFORWARD 	1
#define AUTO_GOBACK 	2
#define AUTO_TURNLEFT 	3
#define AUTO_TURNRIGHT 	4
#define AUTO_FLOAT 		5
#define AUTO_DIVE 		6
#define AUTO_SETDEPTH 	7
#define AUTO_SETHEIGHT 	8
#define AUTO_SETYAW 	9


/* 自动模式信息结构体 */
typedef struct 
{
    uint8_t Auto_Horizontal_Mode;             	//平动模式标志 前，后，左转，右转
	uint8_t Auto_Vertical_Mode;		//垂直运动模式标志 上浮下潜
    float target_yaw;               //目标航向角
    float target_depth;             //目标深度
	float target_height;			//目标高度
	uint8_t target_speed;			//设定速度	
}AutoModeInfo;
/* 自动模式信息结构体实例声明 */
extern AutoModeInfo AMInfo;

void AutoModeInfo_Init(void);
void Task_AutoMode_Process(AutoModeInfo *pAMInfo);

#endif
