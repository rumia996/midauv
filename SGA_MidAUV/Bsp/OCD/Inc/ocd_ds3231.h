#ifndef __OCD_DS3231_H_
#define __OCD_DS3231_H_

#include "drv_hal_conf.h"

/* ds3231��DS1337�õ�ͬһ������ */
#define DS3231_TYPE_ENABLE
//#define DS1337_TYPE _ENABLE

#define DS3231_IIC_ADDR 	0XD0

#define DS3231_WriteAddress 0xD0    /* ����д��ַ */
#define DS3231_ReadAddress  0xD1    /* ��������ַ */

#define DS3231_SECOND       0x00    /* �� */
#define DS3231_MINUTE       0x01    /* �� */
#define DS3231_HOUR         0x02    /* ʱ */
#define DS3231_WEEK         0x03    /* ���� */
#define DS3231_DAY          0x04    /* �� */
#define DS3231_MONTH        0x05    /* �� */
#define DS3231_YEAR         0x06    /* �� */

/* ����1�Ĵ��� */            
#define DS3231_ALARM1SECOND 0x07    /* �� */
#define DS3231_ALARM1MINUTE 0x08    /* �� */
#define DS3231_ALARM1HOUR   0x09    /* ʱ */
#define DS3231_ALARM1WEEK   0x0A    /* ����/�� */

/* ����2�Ĵ��� */
#define DS3231_ALARM2MINUTE 0x0b    /* �� */
#define DS3231_ALARM2HOUR   0x0c    /* ʱ */
#define DS3231_ALARM2WEEK   0x0d    /* ����/�� */

#define DS3231_CONTROL      0x0e    /* ���ƼĴ��� */
#define DS3231_STATUS       0x0f    /* ״̬�Ĵ��� */
#define BSY                 2       /* æ */
#define OSF                 7       /* ����ֹͣ��־ */
#define DS3231_XTAL         0x10    /* �����ϻ��Ĵ��� */
#define DS3231_TEMPERATUREH 0x11    /* �¶ȼĴ������ֽ�(8λ) */
#define DS3231_TEMPERATUREL 0x12    /* �¶ȼĴ������ֽ�(��2λ) */

typedef struct 
{
	uint8_t  ucYear ;  
	uint8_t  ucMonth ; 
	uint8_t  ucDate ; 
	uint8_t  ucHour ; 
	uint8_t  ucMinute ; 
	uint8_t  ucSecond ; 
	uint8_t  ucWeek ; 
}tagDS3231Time_T;

typedef struct 
{
	uint8_t  ucYear[2] ;  
	uint8_t  ucMonth[2] ; 
	uint8_t  ucDate[2] ; 
	uint8_t  ucHour[2] ; 
	uint8_t  ucMinute[2] ; 
	uint8_t  ucSecond[2] ;
	uint8_t  ucWeek[2] ;
}tagDS3231TimeASCII_T;

typedef struct 
{
	tagIICSoft_T	tIICSoft;
}tagDS3231_T;

uint8_t OCD_DS3231_TimeSetASCII(tagDS3231_T *_tIIC, tagDS3231TimeASCII_T *_tTime);
uint8_t OCD_DS3231_TimeSetHex(tagDS3231_T *_tIIC, tagDS3231Time_T *_tTime);
uint8_t OCD_DS3231_TimeGetASCII(tagDS3231_T *_tIIC, tagDS3231TimeASCII_T *_tTime);
uint8_t OCD_DS3231_TimeGetHex(tagDS3231_T *_tIIC, tagDS3231Time_T *_tTime);
void OCD_DS3231_Alarm1ConfigASCII(tagDS3231_T *_tIIC, tagDS3231TimeASCII_T *_tTime);
void OCD_DS3231_Alarm1ConfigHex(tagDS3231_T *_tIIC, tagDS3231Time_T *_tTime);
void OCD_DS3231_Init(tagDS3231_T *_tIIC);

#endif
