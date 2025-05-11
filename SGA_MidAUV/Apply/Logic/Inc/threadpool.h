#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_

/* 线程入口函数声明 */

void ReportDataThread(void* paramenter);
void IPCcmdThread(void* paramenter);
void AltimeterThread(void* paramenter);
void MotorSysThread(void* pramenter);
//void HWT9073Thread(void* pramenter);
//void HWT9073Thread_send(void* pramenter);
void JY901Thread(void* pramenter);
void TDmeterThread(void* pramenter);
void PowerSysThread(void* pramenter);
void TestThread(void* pramenter);
void AutoModeThread(void* pramenter);
void HandleModeThread(void* pramenter);

#endif
