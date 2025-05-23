/****************************************************************************

* Sigma团队

* 文件名: drv_hal_pwm.c

* 内容简述：PWM驱动文件

* 文件历史：

* 版本号	   日期		  	作者		  说明
*  3.0	 	2024-01-26	鲍程璐		适配STM32F4系列

*  2.7		2023-08-09   鲍程璐		减少初始化结构体必要参数，现仅需I/O即可输出

*  2.6     	2023-06-07   鲍程璐		更改部分函数命名

* 2.3.1 	2023-05-05   鲍程璐		格式优化

* 1.1.7 	2022-10-11   鲍程璐		优化执行顺序

* 1.1.2 	2022-08-08	 鲍程璐		新增定时器引脚重映射代码

* 1.0.0a 	2020-02-22	 李环宇		创建该文件

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_PWM_ENABLE

/**
 * @brief PWM波时钟使能
 * @param _tPWM-PWM结构体指针
 * @retval Null
*/
static void S_PWM_CLKEnable(tagPWM_T *_tPWM)
{
	if(_tPWM->tPWMHandle.Instance == TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();	/* 使能定时器1 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();	/* 使能定时器2 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();	/* 使能定时器3 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();	/* 使能定时器4 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();	/* 使能定时器5 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM8)
	{
		__HAL_RCC_TIM8_CLK_ENABLE();	/* 使能定时器8 */
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
 * @brief PWM波参数设置
 * @param _tPWM-PWM结构体指针
 * @retval Null
*/
static void S_PWM_ParamConfig(tagPWM_T *_tPWM)
{	
	_tPWM->tPWMHandle.Init.Prescaler			= SYSTEM_CLOCK - 1;         		/* 定时器分频 */
	_tPWM->tPWMHandle.Init.CounterMode			= TIM_COUNTERMODE_UP;			    /* 向上计数模式 */
	_tPWM->tPWMHandle.Init.Period				= MAX_RELOAD /_tPWM->ulFreq - 1;	/* 自动重装载值 */
	_tPWM->tPWMHandle.Init.ClockDivision		= TIM_CLOCKDIVISION_DIV1;
	_tPWM->tPWMHandle.Init.AutoReloadPreload 	= TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_PWM_Init(&_tPWM->tPWMHandle) != HAL_OK)
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
    
	/* 模式选择PWM1 */
	_tPWM->tPWMChannel.OCMode		= TIM_OCMODE_PWM1;
    
 	/* 设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50% */    
	_tPWM->tPWMChannel.Pulse		= (MAX_RELOAD / _tPWM->ulFreq -1) * _tPWM->fDuty / 100;
    
    /* 输出比较极性为高 */
	_tPWM->tPWMChannel.OCPolarity	= TIM_OCPOLARITY_HIGH;
	
	if(HAL_TIM_PWM_ConfigChannel(&_tPWM->tPWMHandle,&_tPWM->tPWMChannel,_tPWM->ucChannel))	/* 配置通道 */
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}				
	if(HAL_TIM_PWM_Start(&_tPWM->tPWMHandle,_tPWM->ucChannel))	/* 开启通道 */
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
}

/**
 * @brief PWM对应GPIO配置
 * @param _tPWM-PWM结构体指针
 * @retval Null
*/
static void S_PWM_GPIOConfig(tagPWM_T *_tPWM)
{
	/*PWM生成默认I/O配置*/
	_tPWM->tGPIO.tGPIOInit.Mode	 = GPIO_MODE_AF_PP;			/* 复用推挽输出 */
	_tPWM->tGPIO.tGPIOInit.Pull	 = GPIO_PULLUP;				/* 上拉 */
	_tPWM->tGPIO.tGPIOInit.Speed = GPIO_SPEED_FREQ_HIGH;	/* 高速模式 */

#ifdef STM32F1_SGA_ENABLE

	/* 开启复用模式时钟 */
	__HAL_RCC_AFIO_CLK_ENABLE();

	/* 根据不同定时器的ucAFMode开启对应的重映射，重映射表在drv_hal_pwm.h中 */
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
	/* 配置GPIO复用 */
	if(_tPWM->tPWMHandle.Instance == TIM1)			_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF1_TIM1;
	else if(_tPWM->tPWMHandle.Instance == TIM2)		_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF1_TIM2;
	else if(_tPWM->tPWMHandle.Instance == TIM3)		_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF2_TIM3;
	else if(_tPWM->tPWMHandle.Instance == TIM4)		_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF2_TIM4;
	else if(_tPWM->tPWMHandle.Instance == TIM5)		_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF2_TIM5;
	/* TIM6和TIM7为基本定时器，不具备输出PWM功能 */
	else if(_tPWM->tPWMHandle.Instance == TIM8)		_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF3_TIM8;
	else if(_tPWM->tPWMHandle.Instance == TIM9)		_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF3_TIM9;
	else if(_tPWM->tPWMHandle.Instance == TIM10)	_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF3_TIM10;
	else if(_tPWM->tPWMHandle.Instance == TIM11)	_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF3_TIM11;
	else if(_tPWM->tPWMHandle.Instance == TIM12)	_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF9_TIM12;
	else if(_tPWM->tPWMHandle.Instance == TIM13)	_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF9_TIM13;
	else if(_tPWM->tPWMHandle.Instance == TIM14)	_tPWM->tGPIO.tGPIOInit.Alternate = GPIO_AF9_TIM14;

#endif
	Drv_GPIO_Init(&_tPWM->tGPIO, 1);		/* GPIO初始化 */
}

/**
 * @brief PWM波参数匹配函数
 * @param _tPWM-PWM结构体指针
 * @retval Null
*/
static void S_PWM_ParamMatch(tagPWM_T *_tPWM)
{
	/* PWM波参数 */
	DEFAULT(_tPWM->fDuty,7.5);
	DEFAULT(_tPWM->ulFreq,50);

	/* 如果指定定时器则退出 */
	if(_tPWM->tPWMHandle.Instance != NULL)
		return;
#ifdef STM32F1_SGA_ENABLE
	/* 根据选择的GPIO来配置定时器 */
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
	/* 根据选择的GPIO来配置定时器 */
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
 * @brief PWM波占空比设置
 * @param _tPWM-PWM结构体指针
 * @param _fDuty-占空比(单位：%)
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
 * @brief PWM波高电平时间设置
 * @param _tPWM-PWM结构体指针
 * @param _usTime-高电平时间(单位：us)
 * @note 在PWM波频率为50HZ的前提下
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
 * @brief 读取当前设置的PWM高电平时间
 * @param _tPWM-PWM结构体指针
 * @retval _usTime PWM高电平时间
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
 * @brief PWM初始化
 * @param _tPWM-PWM结构体指针
 * @param _ucNum-PWM数量
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
