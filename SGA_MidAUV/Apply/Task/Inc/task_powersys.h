#ifndef __TASK_POWERSYS_H__
#define __TASK_POWERSYS_H__

#include "drv_hal_conf.h"
#include "config.h"
#include "usercode.h"
#include "task_reportdata.h"

void Power_On(uint8_t index);
void Power_Off(uint8_t index);
void Power_AllOff(void);
void Power_AllOn(void);
void Power_SensorOn(void);
void Power_SensorOff(void);
void Power_CommunicationOn(void);
void Power_CommunicationOff(void);
void Power_Test(void);

uint8_t Power_GetState(void);

void Task_PowerSys_Handle(void);
extern uint8_t powerstate;


#endif
