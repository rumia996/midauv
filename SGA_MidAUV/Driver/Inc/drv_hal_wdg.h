#ifndef __DRV_WDG_H_
#define __DRV_WDG_H_

#include "drv_hal_conf.h"

/* �������Ź��ṹ�� */
typedef struct
{
    IWDG_HandleTypeDef  tIWDGHandle;
	uint16_t			usResetTime;	/* ��λʱ��(ms)��F1���ֵ26,208ms F4���ֵ32,760ms*/
}tagIWDG_T;

void Drv_IWDG_Init(tagIWDG_T *_tIWDG);
void Drv_IWDG_Feed(tagIWDG_T *_tIWDG);

#endif
