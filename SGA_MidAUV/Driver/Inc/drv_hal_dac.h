#ifndef __DRV_DAC_H_
#define __DRV_DAC_H_

#include "drv_hal_conf.h"

typedef struct
{
    DAC_HandleTypeDef       tDACHandle;         /* DAC��� */
    DAC_ChannelConfTypeDef  tDACChannel;        /* DACͨ������ */
    tagGPIO_T				tGPIO;              /* GPIO���� */
    uint32_t                ulDACChannelSelect; /* DACͨ��ѡ�� */
    
}tagDAC_T;

void Drv_DAC_Init(tagDAC_T *_tDAC, uint8_t _ucNum);
void Drv_DAC_SetData(tagDAC_T *_tDAC, uint32_t _ulData);
void Drv_DAC_SetValue(tagDAC_T *_tDAC, float _fValue);

#endif
