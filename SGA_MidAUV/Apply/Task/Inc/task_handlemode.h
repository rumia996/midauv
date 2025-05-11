#ifndef __TASK_HANDLEMODE_H_
#define __TASK_HANDLEMODE_H_

#include "drv_hal_conf.h"

/* 手动模式信息结构体 未使用 */
typedef struct 
{
    uint8_t ModeChange;             //模式切换标志
}HandleModeInfo;

//void Task_HandleMode_Process(HandleModeInfo *HMInfo);
void Task_HandleMode_Process(void);
#endif
