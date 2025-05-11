#ifndef __OCD_MS5837_H_
#define __OCD_MS5837_H_

#include "drv_hal_conf.h"

/* У׼���� 
    C1  ѹ�������� SENS|T1
    C2  ѹ������  OFF|T1
    C3	�¶�ѹ��������ϵ�� TCS
    C4	�¶�ϵ����ѹ������ TCO
    C5	�ο��¶� T|REF
    C6 	�¶�ϵ�����¶� TEMPSENS
*/
#define C1 _tMS5837->usaCaliData[1]
#define C2 _tMS5837->usaCaliData[2]
#define C3 _tMS5837->usaCaliData[3]
#define C4 _tMS5837->usaCaliData[4]
#define C5 _tMS5837->usaCaliData[5]
#define C6 _tMS5837->usaCaliData[6]

/* �������ٶ�(m/s^2) */
#define G   9.8f
/* ��׼����ѹֵ��mBar�� */
#define ATM 1013

/* MS5837��д��ַ */
#define MS5837_WRITEADDR            0xEC
#define MS5837_READADDR             0xED

#define MS5837_RESET                0X1E
#define MS5837_ADC_READ             0X00
#define MS5837_PROM_READ            0XA0

/* �ֱ��ʼĴ��� */
#define MS5837_D1_OSR_256			0x40
#define MS5837_D2_OSR_256			0x50
#define MS5837_D1_OSR_512			0x42
#define MS5837_D2_OSR_512			0x52
#define MS5837_D1_OSR_1024			0x44
#define MS5837_D2_OSR_1024			0x54
#define MS5837_D1_OSR_2048			0x46
#define MS5837_D2_OSR_2048			0x56
#define MS5837_D1_OSR_4096			0x48
#define MS5837_D2_OSR_4096			0x58
#define	MS5837_D1_OSR_8192          0x4A
#define	MS5837_D2_OSR_8192          0x5A

/* ���������ʿ�ѡֵ */
typedef enum{
    MS5837_OSR256,
    MS5837_OSR512,
    MS5837_OSR1024,
    MS5837_OSR2048,
    MS5837_OSR4096,
    MS5837_OSR8192
}MS5837OSR_E;

typedef struct
{
    /* һ�ײ���ֵ */
    float fTemp1;
    float fPress1;

    /* ���ײ���ֵ */
    float fTemp2;
    float fPress2;

    /* ʵ�����(cm) */
    float fDepth;

    /* ʵ���¶�(C) */
    float fTemperature;

    /* ���������� */
    MS5837OSR_E setOSR;

    /* У׼���� */
    uint32_t usaCaliData[7];    

    tagIICSoft_T tIIC;
}tagMS5837_T;

void OCD_MS5837_GetData(tagMS5837_T *_tMS5837);
uint8_t OCD_MS5837_Init(tagMS5837_T *_tMS5837);

#endif
