#ifndef __TASK_AUTOMODE_H_
#define __TASK_AUTOMODE_H_

#include "drv_hal_conf.h"

#define AUTO_STOP		0
#define AUTO_GOFORWARD 	1
#define AUTO_GOBACK 	2
#define AUTO_TURNLEFT 	3
#define AUTO_TURNRIGHT 	4
#define AUTO_FLOAT 		5
#define AUTO_DIVE 		6
#define AUTO_SETDEPTH 	7
#define AUTO_SETHEIGHT 	8
#define AUTO_SETYAW 	9


/* �Զ�ģʽ��Ϣ�ṹ�� */
typedef struct 
{
    uint8_t Auto_Horizontal_Mode;             	//ƽ��ģʽ��־ ǰ������ת����ת
	uint8_t Auto_Vertical_Mode;		//��ֱ�˶�ģʽ��־ �ϸ���Ǳ
    float target_yaw;               //Ŀ�꺽���
    float target_depth;             //Ŀ�����
	float target_height;			//Ŀ��߶�
	uint8_t target_speed;			//�趨�ٶ�	
}AutoModeInfo;
/* �Զ�ģʽ��Ϣ�ṹ��ʵ������ */
extern AutoModeInfo AMInfo;

void AutoModeInfo_Init(void);
void Task_AutoMode_Process(AutoModeInfo *pAMInfo);

#endif
