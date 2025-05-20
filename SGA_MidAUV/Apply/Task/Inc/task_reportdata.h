#ifndef __TASK_REPORTDATA_H__
#define __TASK_REPORTDATA_H__

#include "drv_hal_conf.h"
#include "config.h"

//数据类型占用空间
#define FLOAT_SIZE                  sizeof(float)
#define UINT16_T_SIZE               sizeof(uint16_t)
#define UINT8_T_SIZE                sizeof(uint8_t)
#define INT16_T_SIZE				sizeof(int16_t)
#define UINT32_T_SIZE				sizeof(uint32_t)
	
/*各种数据类型在数据帧的中的起始位置*/
#define FRAMESTART_BASE             0
#define DATA_BASE                   3//帧头长度
#define RPY_ATTITUED_BASE			FRAMESTART_BASE + DATA_BASE
#define TEMPERATURE_BASE			RPY_ATTITUED_BASE + 3 * FLOAT_SIZE		
#define DEPTH_BASE					TEMPERATURE_BASE + FLOAT_SIZE
#define HEIGHT_BASE					DEPTH_BASE + FLOAT_SIZE
#define GRADIENTMAGNET_FRONT_BASE	HEIGHT_BASE	 + FLOAT_SIZE
#define GRADIENTMAGNET_LEFT_BASE	GRADIENTMAGNET_FRONT_BASE + 3 * FLOAT_SIZE
#define GRADIENTMAGNET_RIGHT_BASE	GRADIENTMAGNET_LEFT_BASE + 3 * FLOAT_SIZE
#define PWM_BASE					GRADIENTMAGNET_RIGHT_BASE + 3 * FLOAT_SIZE
#define BATTARY_BASE				PWM_BASE + 7 * UINT16_T_SIZE
#define MYCRC_BASE					BATTARY_BASE + 2 * UINT16_T_SIZE
#define FRAMEEND_BASE               MYCRC_BASE + UINT32_T_SIZE
//共86字节


extern uint8_t ReportDataBuffer[FRAMEEND_BASE + 1];

void Task_ReportData_Handle(void);

#endif // !__TASK_REPORTDATA_H__
