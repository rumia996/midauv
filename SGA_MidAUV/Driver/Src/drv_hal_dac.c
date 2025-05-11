/****************************************************************************

* Sigma�Ŷ�

* �ļ���: drv_hal_dac.c

* ���ݼ�����DAC�����ļ�

* �ļ���ʷ��

* �汾��	����		����		˵��
*  3.0 	 2024-01-26	  �����	����STM32F4ϵ��

*  2.5 	 2023-06-01	  �����	�������ļ�

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_DAC_ENABLE

/**
 * @brief DACʱ��ʹ�ܺ���
 * @param _tDAC-DAC�ṹ��ָ��
 * @retval Null
*/
static void S_DAC_CLKEnable(tagDAC_T *_tDAC)
{
    /* ʹ��DACʱ�� */
    __HAL_RCC_DAC_CLK_ENABLE();
}

/**
 * @brief DAC�������ú���
 * @param _tDAC-DAC�ṹ��ָ��
 * @retval Null
*/
static void S_DAC_ParamConfig(tagDAC_T *_tDAC)
{
    _tDAC->tDACHandle.Instance = DAC;

    /* DAC�����ʼ�� */
    if(HAL_DAC_Init(&_tDAC->tDACHandle) != HAL_OK)
    {
        Drv_HAL_Error(__FILE__, __LINE__);
    }

    /* DACͨ�����ܳ�ʼ�� */
    if(HAL_DAC_ConfigChannel(&_tDAC->tDACHandle,&_tDAC->tDACChannel,_tDAC->ulDACChannelSelect) != HAL_OK)
    {
        Drv_HAL_Error(__FILE__, __LINE__);
    }
}

/**
 * @brief DAC GPIO���ú���
 * @param _tDAC-DAC�ṹ��ָ��
 * @retval Null
*/
static void S_DAC_GPIOConfig(tagDAC_T *_tDAC)
{
    /* �����ͨ��1����PA4�������� */
    if(_tDAC->ulDACChannelSelect == DAC_CHANNEL_1)
    {
        _tDAC->tGPIO.tGPIOInit.Pin  = GPIO_PIN_4;
        _tDAC->tGPIO.tGPIOInit.Mode = GPIO_MODE_ANALOG;
        _tDAC->tGPIO.tGPIOInit.Pull = GPIO_NOPULL;
        _tDAC->tGPIO.tGPIOPort      = GPIOA;
    }

    /* �����ͨ��2����PA5�������� */
    if(_tDAC->ulDACChannelSelect == DAC_CHANNEL_2)
    {
        _tDAC->tGPIO.tGPIOInit.Pin  = GPIO_PIN_5;
        _tDAC->tGPIO.tGPIOInit.Mode = GPIO_MODE_ANALOG;
        _tDAC->tGPIO.tGPIOInit.Pull = GPIO_NOPULL;
        _tDAC->tGPIO.tGPIOPort      = GPIOA;
    }

	Drv_GPIO_Init(&_tDAC->tGPIO, 1);
}

/**
 * @brief DAC��ʼ������
 * @param _tDAC-DAC�ṹ��ָ��
 * @param _ucNum-��ʼ��DAC����,��������Ϊ2
 * @retval Null
*/
void Drv_DAC_Init(tagDAC_T *_tDAC, uint8_t _ucNum)
{
	uint8_t index;

    /* ��ΪDACֻ����������ʼ����������2���� */
	if(_ucNum > 2)
    {
        Drv_HAL_Error(__FILE__, __LINE__);
        while(1);
    }

	for(index = 0; index < _ucNum; index++)
	{
		S_DAC_CLKEnable(&_tDAC[index]);		/* DACʱ��ʹ�� */
		S_DAC_GPIOConfig(&_tDAC[index]);	/* DAC��GPIO���� */
		S_DAC_ParamConfig(&_tDAC[index]);	/* DAC�������� */
	}
}

/**
 * @brief DAC����ֱ��ֵ����
 * @param _tDAC-DAC�ṹ��ָ��
 * @param _ulData-����ֱ��ֵ����Χ[0,4095]
 * @retval Null
*/
void Drv_DAC_SetData(tagDAC_T *_tDAC, uint32_t _ulData)
{
    if(_ulData > 4095)
    {
        Drv_HAL_Error(__FILE__, __LINE__);
        while(1);
    }

    /* ����DAC��� */
    HAL_DAC_Start(&_tDAC->tDACHandle,_tDAC->ulDACChannelSelect);

    /* ����DAC���ֵ */
    HAL_DAC_SetValue(&_tDAC->tDACHandle,_tDAC->ulDACChannelSelect,DAC_ALIGN_12B_R,_ulData);
}

/**
 * @brief DAC����ʵ�ʵ�ѹֵ����
 * @param _tDAC-DAC�ṹ��ָ��
 * @param _ulValue-����ʵ�ʵ�ѹֵ����Χ[0,3.3]V
 * @retval Null
*/
void Drv_DAC_SetValue(tagDAC_T *_tDAC, float _fValue)
{
    uint32_t ulDACData = 0;

    if(_fValue < 0.0f || _fValue > 3.3f)
    {
        Drv_HAL_Error(__FILE__, __LINE__);
        while(1);
    }

    /* ת��Ϊֱ��ֵ */
    ulDACData = (uint32_t)(_fValue / 3.3f * 4095);

    /* ����DAC��� */
    HAL_DAC_Start(&_tDAC->tDACHandle,_tDAC->ulDACChannelSelect);

    /* ����DAC���ֵ */
    HAL_DAC_SetValue(&_tDAC->tDACHandle,_tDAC->ulDACChannelSelect,DAC_ALIGN_12B_R,ulDACData);
}

#endif
