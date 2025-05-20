#ifndef __CONFIG_H_
#define __CONFIG_H_

#include "drv_hal_conf.h"
#include "ocd_conf.h"
#include "algo_conf.h"
#include "dev_conf.h"


//#define DEBUG_MODE//DEBUGģʽ����,ע�ʹ��йر�DEBUG
/*����ģʽ�궨��*/
#define TEST_MODE 0
#define MANUAL_MODE 1
#define AUTO_MODE 2

extern uint8_t MODE;

/*����ģʽ�궨��*/
#define TEST_MODE 0		//����ģʽ
#define MANUAL_MODE 1	//�ֱ�ģʽ
#define AUTO_MODE 2		//�Զ�ģʽ
#define SLEEP_MODE 3	//�͹���ģʽ
extern uint8_t MODE;	//��ǰģʽ

/*�̵������ƺ궨��*/
#define POW_MAG 0		//������
#define POW_TDM 1		//������
#define POW_HEIGHT 2	//�߶ȼ�
#define POW_LIGHT 3		//ˮ�µ�
#define POW_BDGPS 4		//����GPS
#define POW_RADIO 5		//���ߵ�̨
#define POW_UACM 6		//��ͨ��
#define POW_TEST_1 7	//����24V
#define POW_TEST_2 8	//����24V

/* PWM�궨�� */
#define LVT 0 	//���� left vertical thruster
#define RVT 1	//�Ҵ��� right vertical thruster
#define BVT 2	//���� back vertical thruster
#define LMT 3	//������ left main thruster
#define RMT 4	//������ right main thruster
#define LS	5	//���� left Servos
#define RS	6	//�Ҷ�� right Servos
#define LIGHT 7	//ˮ�µ�
#define MANIPULATOR 8 //��е��
#define PWM_TEST_1 9	//����
#define PWM_TEST_2 10	//����
#define PWM_TEST_3 11	//����

/* �û�������������� */
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
