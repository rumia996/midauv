#ifndef __TASK_TDmeter_H__
#define __TASK_TDmeter_H__

#include "drv_hal_conf.h"
#include "config.h"
#include "usercode.h"
#include "task_reportdata.h"

void Task_TDmeter_Handle(void);
void Task_TDmeter_Test(void);

void TDmeter_Init(void);
void Task_TDmeter_Reset(void);
void Task_TDmeter_Setbaud(uint32_t baudrate);
void Task_TDmeter_Setcycle(int8_t cycle);
void Task_TDmeter_AutoMode(void);
void Task_TDmeter_OnceMode(void);
void Task_TDmeter_SleepMode(void);
void Task_TDmeter_Getmessage(void);

//extern float Temperature;
//extern float Depth;
#endif
