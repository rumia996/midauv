#ifndef __TASK_HANDLEMODE_H_
#define __TASK_HANDLEMODE_H_

#include "drv_hal_conf.h"

/* �ֶ�ģʽ��Ϣ�ṹ�� δʹ�� */
typedef struct 
{
    uint8_t ModeChange;             //ģʽ�л���־
}HandleModeInfo;

//void Task_HandleMode_Process(HandleModeInfo *HMInfo);
void Task_HandleMode_Process(void);
#endif
