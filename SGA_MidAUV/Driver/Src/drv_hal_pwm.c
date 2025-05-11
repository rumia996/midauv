/****************************************************************************

* Sigma�Ŷ�

* �ļ���: drv_hal_pwm.c

* ���ݼ�����PWM�����ļ�

* �ļ���ʷ��

* �汾��	   ����		  	����		  ˵��
*  3.0	 	2024-01-26	�����		����STM32F4ϵ��

*  2.7		2023-08-09   �����		���ٳ�ʼ���ṹ���Ҫ�������ֽ���I/O�������

*  2.6     	2023-06-07   �����		���Ĳ��ֺ�������

* 2.3.1 	2023-05-05   �����		��ʽ�Ż�

* 1.1.7 	2022-10-11   �����		�Ż�ִ��˳��

* 1.1.2 	2022-08-08	 �����		������ʱ��������ӳ�����

* 1.0.0a 	2020-02-22	 ���		�������ļ�

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_PWM_ENABLE

/**
 * @brief PWM��ʱ��ʹ��
 * @param _tPWM-PWM�ṹ��ָ��
 * @retval Null
*/
static void S_PWM_CLKEnable(tagPWM_T *_tPWM)
{
	if(_tPWM->tPWMHandle.Instance == TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();	/* ʹ�ܶ�ʱ��1 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();	/* ʹ�ܶ�ʱ��2 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();	/* ʹ�ܶ�ʱ��3 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();	/* ʹ�ܶ�ʱ��4 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();	/* ʹ�ܶ�ʱ��5 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM8)
	{
		__HAL_RCC_TIM8_CLK_ENABLE();	/* ʹ�ܶ�ʱ��8 */
	}
#ifdef STM32F4_SGA_ENABLE
	else if(_tPWM->tPWMHandle.Instance == TIM9)
	{
		__HAL_RCC_TIM9_CLK_ENABLE();
	}
	else if(_tPWM->tPWMHandle.Instance == TIM10)
	{
		__HAL_RCC_TIM10_CLK_ENABLE();
	}
	else if(_tPWM->tPWMHandle.Instance == TIM11)
	{
		__HAL_RCC_TIM11_CLK_ENABLE();
	}
	else if(_tPWM->tPWMHandle.Instance == TIM12)
	{
		__HAL_RCC_TIM12_CLK_ENABLE();
	}
	else if(_tPWM->tPWMHandle.Instance == TIM13)
	{
		__HAL_RCC_TIM13_CLK_ENABLE();
	}
	else if(_tPWM->tPWMHandle.Instance == TIM14)
	{
		__HAL_RCC_TIM14_CLK_ENABLE();
	}
#endif
}

/**
 * @brief PWM����������
 * @param _tPWM-PWM�ṹ��ָ��
 * @retval Null
*/
static void S_PWM_ParamConfig(tagPWM_T *_tPWM)
{	
	_tPWM->tPWMHandle.Init.Prescaler			= SYSTEM_CLOCK - 1;         		/* ��ʱ����Ƶ */
	_tPWM->tPWMHandle.Init.CounterMode			= TIM_COUNTERMODE_UP;			    /* ���ϼ���ģʽ */
	_tPWM->tPWMHandle.Init.Period				= MAX_RELOAD /_tPWM->ulFreq - 1;	/* �Զ���װ��ֵ */
	_tPWM->tPWMHandle.Init.ClockDivision		= TIM_CLOCKDIVISION_DIV1;
	_tPWM->tPWMHandle.Init.AutoReloadPreload 	= TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_PWM_Init(&_tPWM->tPWMHandle) != HAL_OK)
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
    
	/* ģʽѡ��PWM1 */
	_tPWM->tPWMChannel.OCMode		= TIM_OCMODE_PWM1;
    
 	/* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50% */    
	_tPWM->tPWMChannel.Pulse		= (MAX_RELOAD / _tPWM->ulFreq -1) * _tPWM->fDuty / 100;
    
    /* ����Ƚϼ���Ϊ�� */
	_tPWM->tPWMChannel.OCPolarity	= TIM_OCPOLARITY_HIGH;
	
	if(HAL_TIM_PWM_ConfigChannel(&_tPWM->tPWMHandle,&_tPWM->tPWMChannel,_tPWM->ucChannel))	/* ����ͨ�� */
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}				
	if(HAL_TIM_PWM_Start(&_tPWM->tPWMHandle,_tPWM->ucChannel))	/* ����ͨ�� */
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
}

/**
 * @brief PWM��ӦGPIO����
 * @param _tPWM-PWM�ṹ��ָ��
 * @retval Null
*/
static void S_PWM_GPIOConfig(tagPWM_T *_tPWM)
{
	/*PWM����Ĭ��I/O����*/
	_tPWM->tGPIO.tGPIOInit.Mode	 = GPIO_MODE_AF_PP;			/* ����������� */
	_tPWM->tGPIO.tGPIOInit.Pull	 = GPIO_PULLUP;				/* ���� */
	_tPWM->tGPIO.tGPIOInit.Speed = GPIO_SPEED_FREQ_HIGH;	/* ����ģʽ */

#ifdef STM32F1_SGA_ENABLE

	/* ��������ģʽʱ�� */
	__HAL_RCC_AFIO_CLK_ENABLE();

	/* ���ݲ�ͬ��ʱ����ucAFMode������Ӧ����ӳ�䣬��ӳ�����drv_hal_pwm.h�� */
	if(_tPWM->tPWMHandle.Instance == TIM1)
	{
		if(_tPWM->tGPIO.ucAFMode == NO_REMAP)				__HAL_AFIO_REMAP_TIM1_DISABLE();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP)	    __HAL_AFIO_REMAP_TIM1_PARTIAL();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP2)	while(1);
		else if(_tPWM->tGPIO.ucAFMode == FULL_REMAP)		__HAL_AFIO_REMAP_TIM1_ENABLE();
	}
	else if(_tPWM->tPWMHandle.Instance == TIM2)
	{
		if(_tPWM->tGPIO.ucAFMode == NO_REMAP)				__HAL_AFIO_REMAP_TIM2_DISABLE();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP)	    __HAL_AFIO_REMAP_TIM2_PARTIAL_1();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP2)	__HAL_AFIO_REMAP_TIM2_PARTIAL_2();
		else if(_tPWM->tGPIO.ucAFMode == FULL_REMAP)		__HAL_AFIO_REMAP_TIM2_ENABLE();
	}
	else if(_tPWM->tPWMHandle.Instance == TIM3)
	{
		if(_tPWM->tGPIO.ucAFMode == NO_REMAP)				__HAL_AFIO_REMAP_TIM3_DISABLE();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP)	    __HAL_AFIO_REMAP_TIM3_PARTIAL();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP2)	while(1);
		else if(_tPWM->tGPIO.ucAFMode == FULL_REMAP)		__HAL_AFIO_REMAP_TIM3_ENABLE();
	}
	else if(_tPWM->tPWMHandle.Instance == TIM4)
	{
		if(_tPWM->tGPIO.ucAFMode == NO_REMAP)				__HAL_AFIO_REMAP_TIM4_DISABLE();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP)	    while(1);
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP2)	while(1);
		else if(_tPWM->tGPIO.ucAFMode == FULL_REMAP)		__HAL_AFIO_REMAP_TIM4_ENABLE();
	}
	else if(_tPWM->tPWMHandle.Instance == TIM5 && _tPWM->ucChannel == TIM_CHANNEL_4)
	{
		if(_tPWM->tGPIO.ucAFMode == NO_REMAP)				__HAL_AFIO_REMAP_TIM5CH4_DISABLE();
		else if(_tPWM->tGPIO.ucAFMode == FULL_REMAP) 		__HAL_AFIO_REMAP_TIM5CH4_ENABLE();
	}

#endif

#ifdef STM32F4_SGA_ENABLE
	/* ����GPIO���� */
	if(_tPWM->tPWMHandle.Instance == TIM1)			_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF1_TIM1;
	else if(_tPWM->tPWMHandle.Instance == TIM2)		_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF1_TIM2;
	else if(_tPWM->tPWMHandle.Instance == TIM3)		_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF2_TIM3;
	else if(_tPWM->tPWMHandle.Instance == TIM4)		_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF2_TIM4;
	else if(_tPWM->tPWMHandle.Instance == TIM5)		_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF2_TIM5;
	/* TIM6��TIM7Ϊ������ʱ�������߱����PWM���� */
	else if(_tPWM->tPWMHandle.Instance == TIM8)		_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF3_TIM8;
	else if(_tPWM->tPWMHandle.Instance == TIM9)		_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF3_TIM9;
	else if(_tPWM->tPWMHandle.Instance == TIM10)	_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF3_TIM10;
	else if(_tPWM->tPWMHandle.Instance == TIM11)	_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF3_TIM11;
	else if(_tPWM->tPWMHandle.Instance == TIM12)	_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF9_TIM12;
	else if(_tPWM->tPWMHandle.Instance == TIM13)	_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF9_TIM13;
	else if(_tPWM->tPWMHandle.Instance == TIM14)	_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF9_TIM14;

#endif
	Drv_GPIO_Init(&_tPWM->tGPIO, 1);		/* GPIO��ʼ�� */
}

/**
 * @brief PWM������ƥ�亯��
 * @param _tPWM-PWM�ṹ��ָ��
 * @retval Null
*/
static void S_PWM_ParamMatch(tagPWM_T *_tPWM)
{
	/* PWM������ */
	DEFAULT(_tPWM->fDuty,7.5);
	DEFAULT(_tPWM->ulFreq,50);

	/* ���ָ����ʱ�����˳� */
	if(_tPWM->tPWMHandle.Instance != NULL)
		return;
#ifdef STM32F1_SGA_ENABLE
	/* ����ѡ���GPIO�����ö�ʱ�� */
	if(_tPWM->tGPIO.tGPIOPort == GPIOA)
	{
		switch(_tPWM->tGPIO.tGPIOInit.Pin)
		{
			case GPIO_PIN_0:
				_tPWM->tPWMHandle.Instance = TIM5;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM5_CH1))
					printf("TIM5_CH1 is only used by PA0\r\n");
				*(s_ucpCheckCache + TIM5_CH1) = 1;
				break;
			case GPIO_PIN_1:
				_tPWM->tPWMHandle.Instance = TIM5;
				_tPWM->ucChannel = TIM_CHANNEL_2;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM5_CH2))
					printf("TIM5_CH2 is only used by PA1\r\n");
				*(s_ucpCheckCache + TIM5_CH2) = 1;
				break;
			case GPIO_PIN_2:
				_tPWM->tPWMHandle.Instance = TIM5;
				_tPWM->ucChannel = TIM_CHANNEL_3;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM5_CH3))
					printf("TIM5_CH3 is only used by PA2\r\n");
				*(s_ucpCheckCache + TIM5_CH3) = 1;
				break;				
			case GPIO_PIN_3:
				_tPWM->tPWMHandle.Instance = TIM2;
				_tPWM->ucChannel = TIM_CHANNEL_4;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM2_CH4))
					printf("TIM2_CH4 is only used by PA3\r\n");
				*(s_ucpCheckCache + TIM2_CH4) = 1;
				break;
			case GPIO_PIN_6:
				_tPWM->tPWMHandle.Instance = TIM3;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM3_CH1))
					printf("TIM3_CH1 is only used by PA6\r\n");
				*(s_ucpCheckCache + TIM3_CH1) = 1;
				break;
			case GPIO_PIN_7:
				_tPWM->tPWMHandle.Instance = TIM3;
				_tPWM->ucChannel = TIM_CHANNEL_2;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM3_CH2))
					printf("TIM3_CH2 is only used by PA7\r\n");
				*(s_ucpCheckCache + TIM3_CH2) = 1;
				break;
			case GPIO_PIN_8:
				_tPWM->tPWMHandle.Instance = TIM1;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM1_CH1))	
					printf("TIM1_CH1 is only used by PA8\r\n");
				*(s_ucpCheckCache + TIM1_CH1) = 1;
				break;	
			case GPIO_PIN_9:
				_tPWM->tPWMHandle.Instance = TIM1;
				_tPWM->ucChannel = TIM_CHANNEL_2;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM1_CH2))
					printf("TIM1_CH2 is only used by PA9\r\n");
				*(s_ucpCheckCache + TIM1_CH2) = 1;
				break;
			case GPIO_PIN_10:
				_tPWM->tPWMHandle.Instance = TIM1;
				_tPWM->ucChannel = TIM_CHANNEL_3;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM1_CH3))
					printf("TIM1_CH3 is only used by PA10\r\n");
				*(s_ucpCheckCache + TIM1_CH3) = 1;
				break;	
			case GPIO_PIN_11:
				_tPWM->tPWMHandle.Instance = TIM1;
				_tPWM->ucChannel = TIM_CHANNEL_4;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM1_CH4))
					printf("TIM1_CH4 is only used by PA11\r\n");
				*(s_ucpCheckCache + TIM1_CH4) = 1;
				break;
			case GPIO_PIN_15:
				_tPWM->tPWMHandle.Instance = TIM2;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				_tPWM->tGPIO.ucAFMode = FULL_REMAP;
				if(*(s_ucpCheckCache + TIM2_CH1))
					printf("TIM2_CH1 is only used by PA15\r\n");
				*(s_ucpCheckCache + TIM2_CH1) = 1;
				break;
			default:
				printf("The I/O port cannot output PWM");					
		}
	}
	else if(_tPWM->tGPIO.tGPIOPort == GPIOB)
	{
		switch(_tPWM->tGPIO.tGPIOInit.Pin)
		{
			case GPIO_PIN_0:
				_tPWM->tPWMHandle.Instance = TIM3;
				_tPWM->ucChannel = TIM_CHANNEL_3;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM3_CH3))
					printf("TIM3_CH3 is only used by PB0\r\n");
				*(s_ucpCheckCache + TIM3_CH3) = 1;
				break;
			case GPIO_PIN_1:
				_tPWM->tPWMHandle.Instance = TIM3;
				_tPWM->ucChannel = TIM_CHANNEL_4;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM3_CH4))
					printf("TIM3_CH4 is only used by PB1\r\n");
				*(s_ucpCheckCache + TIM3_CH4) = 1;
				break;
			case GPIO_PIN_3:
				_tPWM->tPWMHandle.Instance = TIM2;
				_tPWM->ucChannel = TIM_CHANNEL_2;
				_tPWM->tGPIO.ucAFMode = FULL_REMAP;
				if(*(s_ucpCheckCache + TIM2_CH2))
					printf("TIM2_CH2 is only used by PB3\r\n");
				*(s_ucpCheckCache + TIM2_CH2) = 1;
				break;
			case GPIO_PIN_4:
				_tPWM->tPWMHandle.Instance = TIM3;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				_tPWM->tGPIO.ucAFMode = PARTIAL_REMAP;
				if(*(s_ucpCheckCache + TIM3_CH1))
					printf("TIM3_CH1 is only used by PB4\r\n");
				*(s_ucpCheckCache + TIM3_CH1) = 1;
				break;
			case GPIO_PIN_5:
				_tPWM->tPWMHandle.Instance = TIM3;
				_tPWM->ucChannel = TIM_CHANNEL_2;
				_tPWM->tGPIO.ucAFMode = PARTIAL_REMAP;
				if(*(s_ucpCheckCache + TIM3_CH2))
					printf("TIM3_CH2 is only used by PB5\r\n");
				*(s_ucpCheckCache + TIM3_CH2) = 1;
				break;
			case GPIO_PIN_6:
				_tPWM->tPWMHandle.Instance = TIM4;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM4_CH1))
					printf("TIM4_CH1 is only used by PB6\r\n");
				*(s_ucpCheckCache + TIM4_CH1) = 1;
				break;
			case GPIO_PIN_7:
				_tPWM->tPWMHandle.Instance = TIM4;
				_tPWM->ucChannel = TIM_CHANNEL_2;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM4_CH2))
					printf("TIM4_CH2 is only used by PB7\r\n");
				*(s_ucpCheckCache + TIM4_CH2) = 1;
				break;
			case GPIO_PIN_8:
				_tPWM->tPWMHandle.Instance = TIM4;
				_tPWM->ucChannel = TIM_CHANNEL_3;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM4_CH3))
					printf("TIM4_CH3 is only used by PB8\r\n");
				*(s_ucpCheckCache + TIM4_CH3) = 1;
				break;
			case GPIO_PIN_9:
				_tPWM->tPWMHandle.Instance = TIM4;
				_tPWM->ucChannel = TIM_CHANNEL_4;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM4_CH4))
					printf("TIM4_CH4 is only used by PB9\r\n");
				*(s_ucpCheckCache + TIM4_CH4) = 1;
				break;
			case GPIO_PIN_10:
				_tPWM->tPWMHandle.Instance = TIM2;
				_tPWM->ucChannel = TIM_CHANNEL_3;
				_tPWM->tGPIO.ucAFMode = PARTIAL_REMAP2;
				if(*(s_ucpCheckCache + TIM2_CH3))
					printf("TIM2_CH3 is only used by PB10\r\n");
				*(s_ucpCheckCache + TIM2_CH3) = 1;
				break;
			case GPIO_PIN_11:
				_tPWM->tPWMHandle.Instance = TIM2;
				_tPWM->ucChannel = TIM_CHANNEL_4;
				_tPWM->tGPIO.ucAFMode = PARTIAL_REMAP2;
				if(*(s_ucpCheckCache + TIM2_CH4))
					printf("TIM2_CH4 is only used by PB11\r\n");
				*(s_ucpCheckCache + TIM2_CH4) = 1;
				break;
			default:
				printf("The I/O port cannot output PWM");	
		}
	}
	else if(_tPWM->tGPIO.tGPIOPort == GPIOC)
	{
		switch(_tPWM->tGPIO.tGPIOInit.Pin)
		{
			case GPIO_PIN_6:
				_tPWM->tPWMHandle.Instance = TIM8;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM8_CH1))
					printf("TIM8_CH1 is only used by PC6\r\n");
				*(s_ucpCheckCache + TIM8_CH1) = 1;
				break;
			case GPIO_PIN_7:
				_tPWM->tPWMHandle.Instance = TIM8;
				_tPWM->ucChannel = TIM_CHANNEL_2;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM8_CH2))
					printf("TIM8_CH2 is only used by PC7\r\n");
				*(s_ucpCheckCache + TIM8_CH2) = 1;
				break;
			case GPIO_PIN_8:
				_tPWM->tPWMHandle.Instance = TIM8;
				_tPWM->ucChannel = TIM_CHANNEL_3;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM8_CH3))
					printf("TIM8_CH3 is only used by PC8\r\n");
				*(s_ucpCheckCache + TIM8_CH3) = 1;
				break;
			case GPIO_PIN_9:
				_tPWM->tPWMHandle.Instance = TIM8;
				_tPWM->ucChannel = TIM_CHANNEL_4;
				_tPWM->tGPIO.ucAFMode = NO_REMAP;
				if(*(s_ucpCheckCache + TIM8_CH4))
					printf("TIM8_CH4 is only used by PC9\r\n");
				*(s_ucpCheckCache + TIM8_CH4) = 1;
				break;
			default:
				printf("The I/O port cannot output PWM");	
		}
	}
	else if(_tPWM->tGPIO.tGPIOPort == GPIOD)
	{
		switch(_tPWM->tGPIO.tGPIOInit.Pin)
		{
			case GPIO_PIN_12:
				_tPWM->tPWMHandle.Instance = TIM4;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				_tPWM->tGPIO.ucAFMode = FULL_REMAP;
				if(*(s_ucpCheckCache + TIM4_CH1))
					printf("TIM4_CH1 is only used by PD12\r\n");
				*(s_ucpCheckCache + TIM4_CH1) = 1;
				break;
			case GPIO_PIN_13:
				_tPWM->tPWMHandle.Instance = TIM4;
				_tPWM->ucChannel = TIM_CHANNEL_2;
				_tPWM->tGPIO.ucAFMode = FULL_REMAP;
				if(*(s_ucpCheckCache + TIM4_CH2))
					printf("TIM4_CH2 is only used by PD13\r\n");
				*(s_ucpCheckCache + TIM4_CH2) = 1;
				break;
			case GPIO_PIN_14:
				_tPWM->tPWMHandle.Instance = TIM4;
				_tPWM->ucChannel = TIM_CHANNEL_3;
				_tPWM->tGPIO.ucAFMode = FULL_REMAP;
				if(*(s_ucpCheckCache + TIM4_CH3))
					printf("TIM4_CH3 is only used by PD14\r\n");
				*(s_ucpCheckCache + TIM4_CH3) = 1;
				break;
			case GPIO_PIN_15:
				_tPWM->tPWMHandle.Instance = TIM4;
				_tPWM->ucChannel = TIM_CHANNEL_4;
				_tPWM->tGPIO.ucAFMode = FULL_REMAP;
				if(*(s_ucpCheckCache + TIM4_CH4))
					printf("TIM4_CH4 is only used by PD15\r\n");
				*(s_ucpCheckCache + TIM4_CH4) = 1;
				break;
			default:
				printf("The I/O port cannot output PWM");	
		}
	}
	else if(_tPWM->tGPIO.tGPIOPort == GPIOE)
	{
		switch(_tPWM->tGPIO.tGPIOInit.Pin)
		{
			case GPIO_PIN_9:
				_tPWM->tPWMHandle.Instance = TIM1;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				_tPWM->tGPIO.ucAFMode = FULL_REMAP;
				if(*(s_ucpCheckCache + TIM1_CH1))
					printf("TIM1_CH1 is only used by PE9\r\n");
				*(s_ucpCheckCache + TIM1_CH1) = 1;
				break;
			case GPIO_PIN_11:
				_tPWM->tPWMHandle.Instance = TIM1;
				_tPWM->ucChannel = TIM_CHANNEL_2;
				_tPWM->tGPIO.ucAFMode = FULL_REMAP;
				if(*(s_ucpCheckCache + TIM1_CH2))
					printf("TIM1_CH2 is only used by PE11\r\n");
				*(s_ucpCheckCache + TIM1_CH2) = 1;
				break;
			case GPIO_PIN_13:
				_tPWM->tPWMHandle.Instance = TIM1;
				_tPWM->ucChannel = TIM_CHANNEL_3;
				_tPWM->tGPIO.ucAFMode = FULL_REMAP;
				if(*(s_ucpCheckCache + TIM1_CH3))
					printf("TIM1_CH3 is only used by PE13\r\n");
				*(s_ucpCheckCache + TIM1_CH3) = 1;
				break;	
			case GPIO_PIN_14:
				_tPWM->tPWMHandle.Instance = TIM1;
				_tPWM->ucChannel = TIM_CHANNEL_4;
				_tPWM->tGPIO.ucAFMode = FULL_REMAP;
				if(*(s_ucpCheckCache + TIM1_CH4))
					printf("TIM1_CH4 is only used by PE14\r\n");
				*(s_ucpCheckCache + TIM1_CH4) = 1;
				break;
			default:
				printf("The I/O port cannot output PWM");	
		}
	}
#endif

#ifdef STM32F4_SGA_ENABLE
	/* ����ѡ���GPIO�����ö�ʱ�� */
	if(_tPWM->tGPIO.tGPIOPort == GPIOA)
	{
		switch(_tPWM->tGPIO.tGPIOInit.Pin)
		{
			case GPIO_PIN_0:
				_tPWM->tPWMHandle.Instance = TIM5;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				if(*(s_ucpCheckCache + TIM5_CH1))
					printf("TIM5_CH1 is only used by PA0\r\n");
				*(s_ucpCheckCache + TIM5_CH1) = 1;
				break;
			case GPIO_PIN_1:
				_tPWM->tPWMHandle.Instance = TIM5;
				_tPWM->ucChannel = TIM_CHANNEL_2;
				if(*(s_ucpCheckCache + TIM5_CH2))
					printf("TIM5_CH2 is only used by PA1\r\n");
				*(s_ucpCheckCache + TIM5_CH2) = 1;
				break;
			case GPIO_PIN_2:
				_tPWM->tPWMHandle.Instance = TIM5;
				_tPWM->ucChannel = TIM_CHANNEL_3;
				if(*(s_ucpCheckCache + TIM5_CH3))
					printf("TIM5_CH3 is only used by PA2\r\n");
				*(s_ucpCheckCache + TIM5_CH3) = 1;
				break;				
			case GPIO_PIN_3:
				_tPWM->tPWMHandle.Instance = TIM5;
				_tPWM->ucChannel = TIM_CHANNEL_4;
				if(*(s_ucpCheckCache + TIM5_CH4))
					printf("TIM5_CH4 is only used by PA3\r\n");
				*(s_ucpCheckCache + TIM5_CH4) = 1;
				break;
			case GPIO_PIN_5:
				_tPWM->tPWMHandle.Instance = TIM2;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				if(*(s_ucpCheckCache + TIM2_CH1))
					printf("TIM2_CH1 is only used by PA5\r\n");
				*(s_ucpCheckCache + TIM2_CH1) = 1;
				break;
			case GPIO_PIN_6:
				_tPWM->tPWMHandle.Instance = TIM3;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				if(*(s_ucpCheckCache + TIM3_CH1))
					printf("TIM3_CH1 is only used by PA6\r\n");
				*(s_ucpCheckCache + TIM3_CH1) = 1;
				break;
			case GPIO_PIN_7:
				_tPWM->tPWMHandle.Instance = TIM3;
				_tPWM->ucChannel = TIM_CHANNEL_2;
				if(*(s_ucpCheckCache + TIM3_CH2))
					printf("TIM3_CH2 is only used by PA7\r\n");
				*(s_ucpCheckCache + TIM3_CH2) = 1;
				break;
			default:
				printf("The I/O port cannot output PWM");					
		}
	}
	else if(_tPWM->tGPIO.tGPIOPort == GPIOB)
	{
		switch(_tPWM->tGPIO.tGPIOInit.Pin)
		{
			case GPIO_PIN_0:
				_tPWM->tPWMHandle.Instance = TIM3;
				_tPWM->ucChannel = TIM_CHANNEL_3;
				if(*(s_ucpCheckCache + TIM3_CH3))
					printf("TIM3_CH3 is only used by PB0\r\n");
				*(s_ucpCheckCache + TIM3_CH3) = 1;
				break;
			case GPIO_PIN_1:
				_tPWM->tPWMHandle.Instance = TIM3;
				_tPWM->ucChannel = TIM_CHANNEL_4;
				if(*(s_ucpCheckCache + TIM3_CH4))
					printf("TIM3_CH4 is only used by PB1\r\n");
				*(s_ucpCheckCache + TIM3_CH4) = 1;
				break;
			case GPIO_PIN_3:
				_tPWM->tPWMHandle.Instance = TIM2;
				_tPWM->ucChannel = TIM_CHANNEL_2;
				if(*(s_ucpCheckCache + TIM2_CH2))
					printf("TIM2_CH2 is only used by PB3\r\n");
				*(s_ucpCheckCache + TIM2_CH2) = 1;
				break;
			case GPIO_PIN_10:
				_tPWM->tPWMHandle.Instance = TIM2;
				_tPWM->ucChannel = TIM_CHANNEL_3;
				if(*(s_ucpCheckCache + TIM2_CH3))
					printf("TIM2_CH3 is only used by PB10\r\n");
				*(s_ucpCheckCache + TIM2_CH3) = 1;
				break;
			case GPIO_PIN_11:
				_tPWM->tPWMHandle.Instance = TIM2;
				_tPWM->ucChannel = TIM_CHANNEL_4;
				if(*(s_ucpCheckCache + TIM2_CH4))
					printf("TIM2_CH4 is only used by PB11\r\n");
				*(s_ucpCheckCache + TIM2_CH4) = 1;
				break;
			case GPIO_PIN_14:
				_tPWM->tPWMHandle.Instance = TIM12;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				if(*(s_ucpCheckCache + TIM12_CH1))
					printf("TIM12_CH1 is only used by PB14\r\n");
				*(s_ucpCheckCache + TIM12_CH1) = 1;
				break;
			case GPIO_PIN_15:
				_tPWM->tPWMHandle.Instance = TIM12;
				_tPWM->ucChannel = TIM_CHANNEL_2;
				if(*(s_ucpCheckCache + TIM12_CH2))
					printf("TIM12_CH2 is only used by PB15\r\n");
				*(s_ucpCheckCache + TIM12_CH2) = 1;
				break;
			default:
				printf("The I/O port cannot output PWM");	
		}
	}
	else if(_tPWM->tGPIO.tGPIOPort == GPIOC)
	{
		switch(_tPWM->tGPIO.tGPIOInit.Pin)
		{
			case GPIO_PIN_6:
				_tPWM->tPWMHandle.Instance = TIM8;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				if(*(s_ucpCheckCache + TIM8_CH1))
					printf("TIM8_CH1 is only used by PC6\r\n");
				*(s_ucpCheckCache + TIM8_CH1) = 1;
				break;
			case GPIO_PIN_7:
				_tPWM->tPWMHandle.Instance = TIM8;
				_tPWM->ucChannel = TIM_CHANNEL_2;
				if(*(s_ucpCheckCache + TIM8_CH2))
					printf("TIM8_CH2 is only used by PC7\r\n");
				*(s_ucpCheckCache + TIM8_CH2) = 1;
				break;
			case GPIO_PIN_8:
				_tPWM->tPWMHandle.Instance = TIM8;
				_tPWM->ucChannel = TIM_CHANNEL_3;
				if(*(s_ucpCheckCache + TIM8_CH3))
					printf("TIM8_CH3 is only used by PC8\r\n");
				*(s_ucpCheckCache + TIM8_CH3) = 1;
				break;
			case GPIO_PIN_9:
				_tPWM->tPWMHandle.Instance = TIM8;
				_tPWM->ucChannel = TIM_CHANNEL_4;
				if(*(s_ucpCheckCache + TIM8_CH4))
					printf("TIM8_CH4 is only used by PC9\r\n");
				*(s_ucpCheckCache + TIM8_CH4) = 1;
				break;
			default:
				printf("The I/O port cannot output PWM");	
		}
	}
	else if(_tPWM->tGPIO.tGPIOPort == GPIOD)
	{
		switch(_tPWM->tGPIO.tGPIOInit.Pin)
		{
			case GPIO_PIN_12:
				_tPWM->tPWMHandle.Instance = TIM4;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				if(*(s_ucpCheckCache + TIM4_CH1))
					printf("TIM4_CH1 is only used by PD12\r\n");
				*(s_ucpCheckCache + TIM4_CH1) = 1;
				break;
			case GPIO_PIN_13:
				_tPWM->tPWMHandle.Instance = TIM4;
				_tPWM->ucChannel = TIM_CHANNEL_2;
				if(*(s_ucpCheckCache + TIM4_CH2))
					printf("TIM4_CH2 is only used by PD13\r\n");
				*(s_ucpCheckCache + TIM4_CH2) = 1;
				break;
			case GPIO_PIN_14:
				_tPWM->tPWMHandle.Instance = TIM4;
				_tPWM->ucChannel = TIM_CHANNEL_3;
				if(*(s_ucpCheckCache + TIM4_CH3))
					printf("TIM4_CH3 is only used by PD14\r\n");
				*(s_ucpCheckCache + TIM4_CH3) = 1;
				break;
			case GPIO_PIN_15:
				_tPWM->tPWMHandle.Instance = TIM4;
				_tPWM->ucChannel = TIM_CHANNEL_4;
				if(*(s_ucpCheckCache + TIM4_CH4))
					printf("TIM4_CH4 is only used by PD15\r\n");
				*(s_ucpCheckCache + TIM4_CH4) = 1;
				break;
			default:
				printf("The I/O port cannot output PWM");	
		}
	}
	else if(_tPWM->tGPIO.tGPIOPort == GPIOE)
	{
		switch(_tPWM->tGPIO.tGPIOInit.Pin)
		{
			case GPIO_PIN_5:
				_tPWM->tPWMHandle.Instance = TIM9;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				if(*(s_ucpCheckCache + TIM9_CH1))
					printf("TIM9_CH1 is only used by PA5\r\n");
				*(s_ucpCheckCache + TIM9_CH1) = 1;
				break;
			case GPIO_PIN_6:
				_tPWM->tPWMHandle.Instance = TIM9;
				_tPWM->ucChannel = TIM_CHANNEL_2;
				if(*(s_ucpCheckCache + TIM9_CH2))
					printf("TIM9_CH2 is only used by PA6\r\n");
				*(s_ucpCheckCache + TIM9_CH2) = 1;
				break;
			case GPIO_PIN_9:
				_tPWM->tPWMHandle.Instance = TIM1;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				if(*(s_ucpCheckCache + TIM1_CH1))
					printf("TIM1_CH1 is only used by PE9\r\n");
				*(s_ucpCheckCache + TIM1_CH1) = 1;
				break;
			case GPIO_PIN_11:
				_tPWM->tPWMHandle.Instance = TIM1;
				_tPWM->ucChannel = TIM_CHANNEL_2;
				if(*(s_ucpCheckCache + TIM1_CH2))
					printf("TIM1_CH2 is only used by PE11\r\n");
				*(s_ucpCheckCache + TIM1_CH2) = 1;
				break;	
			case GPIO_PIN_13:
				_tPWM->tPWMHandle.Instance = TIM1;
				_tPWM->ucChannel = TIM_CHANNEL_3;
				if(*(s_ucpCheckCache + TIM1_CH3))
					printf("TIM1_CH3 is only used by PE13\r\n");
				*(s_ucpCheckCache + TIM1_CH3) = 1;
				break;
			case GPIO_PIN_14:
				_tPWM->tPWMHandle.Instance = TIM1;
				_tPWM->ucChannel = TIM_CHANNEL_4;
				if(*(s_ucpCheckCache + TIM1_CH4))
					printf("TIM1_CH4 is only used by PE14\r\n");
				*(s_ucpCheckCache + TIM1_CH4) = 1;
				break;
			default:
				printf("The I/O port cannot output PWM");					
		}
	}
	else if(_tPWM->tGPIO.tGPIOPort == GPIOF)
	{
		switch(_tPWM->tGPIO.tGPIOInit.Pin)
		{
			case GPIO_PIN_6:
				_tPWM->tPWMHandle.Instance = TIM10;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				if(*(s_ucpCheckCache + TIM10_CH1))
					printf("TIM10_CH1 is only used by PF6\r\n");
				*(s_ucpCheckCache + TIM10_CH1) = 1;
				break;
			case GPIO_PIN_7:
				_tPWM->tPWMHandle.Instance = TIM11;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				if(*(s_ucpCheckCache + TIM11_CH1))
					printf("TIM11_CH1 is only used by PF7\r\n");
				*(s_ucpCheckCache + TIM11_CH1) = 1;
				break;
			case GPIO_PIN_8:
				_tPWM->tPWMHandle.Instance = TIM13;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				if(*(s_ucpCheckCache + TIM13_CH1))
					printf("TIM13_CH1 is only used by PF8\r\n");
				*(s_ucpCheckCache + TIM13_CH1) = 1;
				break;
			case GPIO_PIN_9:
				_tPWM->tPWMHandle.Instance = TIM14;
				_tPWM->ucChannel = TIM_CHANNEL_1;
				if(*(s_ucpCheckCache + TIM14_CH1))
					printf("TIM14_CH1 is only used by PF9\r\n");
				*(s_ucpCheckCache + TIM14_CH1) = 1;
				break;
			default:
				printf("The I/O port cannot output PWM");					
		}
	}
#endif
}

/**
 * @brief PWM��ռ�ձ�����
 * @param _tPWM-PWM�ṹ��ָ��
 * @param _fDuty-ռ�ձ�(��λ��%)
 * @retval Null
*/
void Drv_PWM_DutyFactorSet(tagPWM_T *_tPWM, float _fDuty)
{
	switch(_tPWM->ucChannel)
	{
		case TIM_CHANNEL_1:
			_tPWM->tPWMHandle.Instance->CCR1 = (MAX_RELOAD /_tPWM->ulFreq - 1) * _fDuty / 100;
		break;
		
		case TIM_CHANNEL_2:
			_tPWM->tPWMHandle.Instance->CCR2 = (MAX_RELOAD /_tPWM->ulFreq - 1) * _fDuty / 100;
		break;
		
		case TIM_CHANNEL_3:
			_tPWM->tPWMHandle.Instance->CCR3 = (MAX_RELOAD /_tPWM->ulFreq - 1) * _fDuty / 100;
		break;
		
		case TIM_CHANNEL_4:
			_tPWM->tPWMHandle.Instance->CCR4 = (MAX_RELOAD /_tPWM->ulFreq - 1) * _fDuty / 100;
		break;
		
		default:
			Drv_HAL_Error(__FILE__, __LINE__);
		break;
	}
}

/**
 * @brief PWM���ߵ�ƽʱ������
 * @param _tPWM-PWM�ṹ��ָ��
 * @param _usTime-�ߵ�ƽʱ��(��λ��us)
 * @note ��PWM��Ƶ��Ϊ50HZ��ǰ����
 * @retval Null
*/
void Drv_PWM_HighLvTimeSet(tagPWM_T *_tPWM, uint16_t _usTime)
{
	if(_tPWM->ulFreq == 50)
	{
		switch(_tPWM->ucChannel)
		{
			case TIM_CHANNEL_1:
				_tPWM->tPWMHandle.Instance->CCR1 = _usTime;
			break;
			
			case TIM_CHANNEL_2:
				_tPWM->tPWMHandle.Instance->CCR2 = _usTime;
			break;
			
			case TIM_CHANNEL_3:
				_tPWM->tPWMHandle.Instance->CCR3 = _usTime;
			break;
			
			case TIM_CHANNEL_4:
				_tPWM->tPWMHandle.Instance->CCR4 = _usTime;
			break;
			
			default:
				Drv_HAL_Error(__FILE__, __LINE__);
			break;
		}
	}
}

/**
 * @brief ��ȡ��ǰ���õ�PWM�ߵ�ƽʱ��
 * @param _tPWM-PWM�ṹ��ָ��
 * @retval _usTime PWM�ߵ�ƽʱ��
*/
uint16_t Drv_PWM_ReadHighLvTime(tagPWM_T *_tPWM)
{
		switch(_tPWM->ucChannel)
		{
			case TIM_CHANNEL_1:
				return _tPWM->tPWMHandle.Instance->CCR1;
			
			case TIM_CHANNEL_2:
				return _tPWM->tPWMHandle.Instance->CCR2;
			
			case TIM_CHANNEL_3:
				return _tPWM->tPWMHandle.Instance->CCR3;
			
			case TIM_CHANNEL_4:
				return _tPWM->tPWMHandle.Instance->CCR4;
			
			default:
				Drv_HAL_Error(__FILE__, __LINE__);
				return 0;
		}

}
/**
 * @brief PWM��ʼ��
 * @param _tPWM-PWM�ṹ��ָ��
 * @param _ucNum-PWM����
 * @retval Null
*/
void Drv_PWM_Init(tagPWM_T *_tPWM, uint8_t _ucNum)
{
	uint8_t index;

    s_ucpCheckCache = (uint8_t *)calloc(0,TIM_CHANNEL_COUNT);

	for(index = 0;index < _ucNum;index++)
	{
		S_PWM_ParamMatch(&_tPWM[index]);
		S_PWM_CLKEnable(&_tPWM[index]);
		S_PWM_GPIOConfig(&_tPWM[index]);
		S_PWM_ParamConfig(&_tPWM[index]);
	}

	free(s_ucpCheckCache);
}
#endif
