#ifndef	__USERCODE_H_
#define	__USERCODE_H_

#include "drv_hal_conf.h"

void UserLogic_Code(void);

extern rt_sem_t CmdFromIPC_Sem;
extern rt_sem_t MessageFromAltimeter_Sem;
extern rt_sem_t MessageFromHWT9073_Sem;
extern rt_sem_t JY901S_Sem;
extern rt_sem_t MessageFromTDmeter_Sem;
extern rt_sem_t ManualCmd_Sem;


extern rt_thread_t ReportDataThread_t;
extern rt_thread_t IPCcmdThread_t;
extern rt_thread_t AltimeterThread_t;
extern rt_thread_t MotorSysThread_t;
extern rt_thread_t HWT9073Thread_t;
extern rt_thread_t HWT9073Thread_send_t;
extern rt_thread_t JY901Thread_t;
extern rt_thread_t TDmeterThread_t;
extern rt_thread_t PowerSysThread_t; 
extern rt_thread_t TestThread_t;
extern rt_thread_t AutoModeThread_t;
extern rt_thread_t HandleModeThread_t;
extern rt_thread_t AD4111Thread_t;

void extract_str_between_2char(const uint8_t *input, uint8_t *output, uint8_t startchr, uint8_t endchr);
void extract_char_between_indexandchar(const uint8_t *input, uint8_t *output, uint8_t startindex, uint8_t endchr);
bool MODE_Switch(uint8_t target_MODE);

void RS485_Send_On(void);
void RS485_Receive_On(void);
#endif
