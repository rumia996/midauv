/****************************************************************************

* Sigma�Ŷ�

* �ļ���: drv_hal_wdg.c

* ���ݼ�����WDG���Ź������ļ�

* �ļ���ʷ��

* �汾��	����		����		˵��
*  3.0	 2024-01-26	  �����	����STM32F4ϵ��

*  2.5   2023-06-03	  �����	�������ļ�

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_WDG_ENABLE

/**
 * @brief �������Ź����ú���
 * @param _tIWDG-�������Ź��ṹ��ָ��
 * @note  F1���ʱ����㹫ʽ Tout = Prescaler / 40 * Reload (ms)
 * 		  F4���ʱ����㹫ʽ Tout = Prescaler / 32 * Reload (ms)
 *        ÿ������ֵ���ǰ���2,048�����ֵ��һ�룩�������
 * @retval Null 
*/
static void S_IWDG_Config(tagIWDG_T *_tIWDG)
{
    /* �������ʱ��ѡ����ʵ�Ԥ��Ƶֵ������ֵ */
#ifdef STM32F1_SGA_ENABLE
    if(_tIWDG->usResetTime > 13107)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_256;
		_tIWDG->tIWDGHandle.Init.Reload = (float)_tIWDG->usResetTime / 6.4f;
	}
	else if(_tIWDG->usResetTime > 6553)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_128;
		_tIWDG->tIWDGHandle.Init.Reload = (float)_tIWDG->usResetTime / 3.2f;
	}
	else if(_tIWDG->usResetTime > 3276)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_64;
		_tIWDG->tIWDGHandle.Init.Reload = (float)_tIWDG->usResetTime / 1.6f;
	}
	else if(_tIWDG->usResetTime > 1638)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_32;
		_tIWDG->tIWDGHandle.Init.Reload = (float)_tIWDG->usResetTime / 0.8f;
	}
	else if(_tIWDG->usResetTime > 819)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_16;
		_tIWDG->tIWDGHandle.Init.Reload = (float)_tIWDG->usResetTime / 0.4f;
	}
	else if(_tIWDG->usResetTime > 409)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_8;
		_tIWDG->tIWDGHandle.Init.Reload = (float)_tIWDG->usResetTime / 0.2f;
	}
	else
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_4;
		_tIWDG->tIWDGHandle.Init.Reload = (float)_tIWDG->usResetTime / 0.1f;
	}

#endif

#ifdef STM32F4_SGA_ENABLE
	if(_tIWDG->usResetTime > 16384)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_256;
		_tIWDG->tIWDGHandle.Init.Reload = _tIWDG->usResetTime / 8;		
	}
	else if(_tIWDG->usResetTime > 8192)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_128;
		_tIWDG->tIWDGHandle.Init.Reload = _tIWDG->usResetTime / 4;
	}
	else if(_tIWDG->usResetTime > 4096)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_64;
		_tIWDG->tIWDGHandle.Init.Reload = _tIWDG->usResetTime / 2;
	}
	else if(_tIWDG->usResetTime > 2048)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_32;
		_tIWDG->tIWDGHandle.Init.Reload = _tIWDG->usResetTime;
	}
	else if(_tIWDG->usResetTime > 1024)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_16;
		_tIWDG->tIWDGHandle.Init.Reload = _tIWDG->usResetTime * 2;
	}
	else if(_tIWDG->usResetTime > 512)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_8;
		_tIWDG->tIWDGHandle.Init.Reload = _tIWDG->usResetTime * 4;
	}
	else
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_4;
		_tIWDG->tIWDGHandle.Init.Reload = _tIWDG->usResetTime * 8;		
	}

#endif
    _tIWDG->tIWDGHandle.Instance = IWDG;
}

/**
 * @brief �������Ź���ʼ��
 * @param _tIWDG-�������Ź��ṹ��ָ��
 * @retval Null 
*/
void Drv_IWDG_Init(tagIWDG_T *_tIWDG)
{
	S_IWDG_Config(_tIWDG);

    HAL_IWDG_Init(&_tIWDG->tIWDGHandle);
}

/**
 * @brief �������Ź�ι��
 * @param _tIWDG-�������Ź��ṹ��ָ��
 * @retval Null 
*/
void Drv_IWDG_Feed(tagIWDG_T *_tIWDG)
{
	HAL_IWDG_Refresh(&_tIWDG->tIWDGHandle);
}

#endif
