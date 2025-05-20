#ifndef __TASK_AD4111_H__
#define __TASK_AD4111_H__

#include "drv_hal_conf.h"
#include "config.h"
#include "usercode.h"
#include "task_reportdata.h"

void AD_Acquisition(tagAD24BIT_T* device);
void Task_AD4111_Sync(void);
void Task_AD4111_Handle(void);

#endif // !__TASK_AD4111_H__




