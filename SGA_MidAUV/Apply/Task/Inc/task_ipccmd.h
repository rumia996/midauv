#ifndef __TASK_IPCCMD_H__
#define __TASK_IPCCMD_H__

#include "drv_hal_conf.h"
#include "config.h"
#include "usercode.h"
#include "task_conf.h"

void Task_IPCcmd_Handle(void);

void ReportCmd(uint8_t cmdtype_1,uint8_t cmdtype_2,uint8_t cmdtype_3);
void Report_Z(bool reportflag);
void param_check_0to12(int16_t cmd,bool * reportflag);
#endif // !__TASK_IPCCMD_H__
