#ifndef __TASK_JY901_H__
#define __TASK_JY901_H__

#include "drv_hal_conf.h"
#include "config.h"
#include "usercode.h"
#include "task_reportdata.h"

void Task_JY901_Handle(void);

extern float Roll;	//ºá¹ö
extern float Pitch;	//¸©Ñö
extern float Yaw;	//º½Ïò

float normalize_angle(float angle);
float transform_angle(float target_yaw);

#endif // !__TASK_JY901_H__
