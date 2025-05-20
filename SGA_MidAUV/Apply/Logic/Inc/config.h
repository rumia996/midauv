#ifndef __CONFIG_H_
#define __CONFIG_H_

#include "drv_hal_conf.h"
#include "ocd_conf.h"
#include "algo_conf.h"
#include "dev_conf.h"


//#define DEBUG_MODE//DEBUG模式开关,注释此行关闭DEBUG
/*运行模式宏定义*/
#define TEST_MODE 0
#define MANUAL_MODE 1
#define AUTO_MODE 2

extern uint8_t MODE;

/*运行模式宏定义*/
#define TEST_MODE 0		//测试模式
#define MANUAL_MODE 1	//手柄模式
#define AUTO_MODE 2		//自动模式
#define SLEEP_MODE 3	//低功耗模式
extern uint8_t MODE;	//当前模式

/*继电器控制宏定义*/
#define POW_MAG 0		//磁力仪
#define POW_TDM 1		//温深仪
#define POW_HEIGHT 2	//高度计
#define POW_LIGHT 3		//水下灯
#define POW_BDGPS 4		//北斗GPS
#define POW_RADIO 5		//无线电台
#define POW_UACM 6		//声通机
#define POW_TEST_1 7	//备用24V
#define POW_TEST_2 8	//备用24V

/* PWM宏定义 */
#define LVT 0 	//左垂推 left vertical thruster
#define RVT 1	//右垂推 right vertical thruster
#define BVT 2	//后垂推 back vertical thruster
#define LMT 3	//左主推 left main thruster
#define RMT 4	//右主推 right main thruster
#define LS	5	//左舵机 left Servos
#define RS	6	//右舵机 right Servos
#define LIGHT 7	//水下灯
#define MANIPULATOR 8 //机械臂
#define PWM_TEST_1 9	//备用
#define PWM_TEST_2 10	//备用
#define PWM_TEST_3 11	//备用

/* 用户句柄声明包含区 */
extern tagGPIO_T RS485Ctrl[];
extern tagGPIO_T Control_GPIO[];
extern tagGPIO_T SYNC;

extern tagAD24BIT_T AD4111_1;
extern tagAD24BIT_T AD4111_2;

extern tagUART_T Uart1;
extern tagUART_T Uart2;
extern tagJY901_T JY901S;
extern tagUART_T Uart3;
extern tagUART_T Uart4;
//extern tagUART_T Uart5;

extern tagPWM_T PWM[];
extern tagIWDG_T demoIWDG;

extern CRC_HandleTypeDef hcrc;
void CRC_Init(void);
void CRC_update(void);
void PWM_update(void);

#endif
