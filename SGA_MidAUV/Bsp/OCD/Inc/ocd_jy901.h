#ifndef __OCD_JY901_H_
#define __OCD_JY901_H_

#include "drv_hal_conf.h"

/** @defgroup SaveConfig _ucSet
  * @{
  */
#define SAVE_NOW		0x00
#define SAVE_DEFAULT	0x01
#define	SAVE_RESET		0xff
/**
  * @}
  */

 /** @defgroup Data Flag
  * @{
  */
#define JY901_TIME			0x50
#define JY901_ACCEL			0x51
#define	JY901_GYRO			0x52
#define	JY901_ANGLE			0x53
#define	JY901_MAG			0x54
#define	JY901_HEAD			0x55
#define JY901_QUATER		0x59
/**
  * @}
  */

/** @defgroup RxTypeConfig _ucType
  * @{
  */
#define JY901_OUTPUT_TIME			0x0100	/* ʱ�� */
#define JY901_OUTPUT_ACCEL			0x0200	/* ���ٶ� */
#define JY901_OUTPUT_GYRO			0x0400	/* ���ٶ� */
#define JY901_OUTPUT_ANGLE			0x0800	/* �Ƕ� */
#define	JY901_OUTPUT_MAG			0x1000	/* �ų� */	
#define	JY901_OUTPUT_PORT 			0x2000	/* �˿�״̬ */
#define	JY901_OUTPUT_PRESS 			0x4000	/* ��ѹ�͸߶� */
#define	JY901_OUTPUT_GPS 			0x8000	/* ��γ�� */
#define	JY901_OUTPUT_VELOCITY 		0x0001	/* ���� */
#define	JY901_OUTPUT_QUATER 		0x0002	/* ��Ԫ�� */
#define	JY901_OUTPUT_GSA 			0x0004	/* ���Ƕ�λ���� */
/**
  * @}
  */

/** @defgroup Correct _ucMode
  * @{
  */
#define JY901_CALSW_NORMAL			0x00 	/* ��������ģʽ */
#define	JY901_CALSW_ACCEL			0x01	/* �Զ��Ӽ�У׼ģʽ */
#define	JY901_CALSW_HIGH			0x03	/* �߶�����ģʽ */
#define	JY901_CALSW_YAW				0x04	/* ���������ģʽ */
#define	JY901_CALSW_MAG_SPHERE		0x07	/* �ų�У׼��������Ϸ��� */
#define	JY901_CALSW_ANGLE			0x08	/* ���ýǶȲο� */
#define	JY901_CALSW_MAG_BIPLANE		0x09	/* �ų�У׼��˫ƽ��ģʽ�� */
/**
  * @}
  */

/** @defgroup RxSpeedConfig	_ucFreq
  * @{
  */
#define JY901_RX_0_2HZ		0x01
#define JY901_RX_0_5HZ		0x02
#define JY901_RX_1HZ		0x03
#define JY901_RX_2HZ		0x04
#define JY901_RX_5HZ		0x05
#define JY901_RX_10HZ		0x06
#define JY901_RX_20HZ		0x07
#define JY901_RX_50HZ		0x08
#define JY901_RX_100HZ		0x09
#define JY901_RX_NULL		0x0d
#define JY901_RX_200HZ		0x0b
#define JY901_RX_ONCE		0x0c
/**
  * @}
  */

/** @defgroup RxBaudConfig	_ucBaud
  * @{
  */
#define JY901_RXBAUD_2400		0x00
#define JY901_RXBAUD_4800		0x01
#define JY901_RXBAUD_9600		0x02
#define JY901_RXBAUD_19200		0x03
#define JY901_RXBAUD_38400		0x04
#define JY901_RXBAUD_57600		0x05
#define JY901_RXBAUD_115200		0x06
#define JY901_RXBAUD_230400		0x07
#define JY901_RXBAUD_460800		0x08
#define JY901_RXBAUD_921600		0x09
/**
  * @}
  */

/** @defgroup OrientConfig	_ucOrient
  * @{
  */
#define JY901_ORIENT_HORIZONTAL		0x00
#define JY901_ORIENT_VERTICAL		0x01
/**
  * @}
  */

/** @defgroup AxisConfig	_ucAxis
  * @{
  */
#define JY901_AXIS_9		0x00
#define JY901_AXIS_6		0x01
/**
  * @}
  */

/* ʱ�����ݽṹ�� */
typedef struct 
{
	/* ԭʼ���� */
	uint8_t Year;
	uint8_t Month;
	uint8_t Day;
	uint8_t Hour;
	uint8_t Min;
	uint8_t Sec;
	short	mSec;
}tagJY901Time;

/* ���ٶ����ݽṹ�� */
typedef struct 
{
	/* ԭʼ���� */
	short AccX;
	short AccY;
	short AccZ;
	short T;

	/* ת��������� */
	float ConAccX;
	float ConAccY;
	float ConAccZ;
}tagJY901Acc;

/* ���ٶ����ݽṹ�� */
typedef struct 
{
	/* ԭʼ���� */
	short GyroX;
	short GyroY;
	short GyroZ;
	
	/* ת��������� */
	float ConGyroX;
	float ConGyroY;
	float ConGyroZ;
}tagJY901Gyro;

/* ŷ�������ݽṹ�� */
typedef struct 
{
	/* ԭʼ���� */
	short Roll;
	short Pitch;
	short Yaw;

	/* ת��������� */
	float ConRoll;
	float ConPitch;
	float ConYaw;
}tagJY901Angle;

/* �ų����ݽṹ�� */
typedef struct 
{
	/* ԭʼ���� */
	short MagX;
	short MagY;
	short MagZ;
	short T;

	/* ת��������� */
	float ConMagX;
	float ConMagY;
	float ConMagZ;
}tagJY901Mag;

/* ��Ԫ�����ݽṹ�� */
typedef struct 
{
	/* ԭʼ���� */
	short q0;
	short q1;
	short q2;
	short q3;

	/* ת��������� */
	float Conq0;
	float Conq1;
	float Conq2;
	float Conq3;
}tagJY901Quater;

/* JY901�������ýṹ�� */
typedef struct 
{
	uint8_t 	ucBaud;		/* ���������� */
	uint8_t 	ucRate;		/* �������� */
	uint16_t 	usType; 	/* ����������� */
	uint8_t		ucOrient;	/* ��װ�������� */
	uint8_t		ucAxis;		/* �����㷨���� */
}tagJY901config;

/* JY901�ṹ�� */
typedef struct
{
	tagUART_T 			tUART;
	tagJY901config		tConfig;
	tagJY901Time		stcTime;
	tagJY901Acc			stcAcc;
	tagJY901Gyro 		stcGyro;	
	tagJY901Angle 		stcAngle;
	tagJY901Mag			stcMag;
	tagJY901Quater		stcQuater;
}tagJY901_T;


void OCD_JY901_DMAInit(tagJY901_T *_tJY901);
void OCD_JY901_RxTypeConfig(tagJY901_T *_tJY901);
void OCD_JY901_RxSpeedConfig(tagJY901_T *_tJY901);
void OCD_JY901_RxBaudConfig(tagJY901_T *_tJY901);
void OCD_JY901_OrientConfig(tagJY901_T *_tJY901);
void OCD_JY901_AxisConfig(tagJY901_T *_tJY901);
void OCD_JY901_GyroAutoCorrect(tagJY901_T *_tJY901, uint8_t _ucMode);
void OCD_JY901_Sleep(tagJY901_T *_tJY901);
void OCD_JY901_Correct(tagJY901_T *_tJY901, uint8_t _ucMode);
void OCD_JY901_OutputOnce(tagJY901_T *_tJY901);
uint8_t OCD_JY901_DataProcess(tagJY901_T *_tJY901);
void OCD_JY901_DataConversion(tagJY901_T *_tJY901);
void OCD_JY901_Printf(tagJY901_T *_tJY901);

#endif
