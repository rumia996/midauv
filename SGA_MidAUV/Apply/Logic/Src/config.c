#include "config.h"

uint8_t MODE = TEST_MODE;//����Ĭ��ģʽ

/* �������Ź� */
tagIWDG_T demoIWDG = 
{
	.usResetTime = 1000,			/* 1Sι�����ʱ�� */
};

tagGPIO_T RS485Ctrl[] =
{
		[0] = 
	{
		.tGPIOInit.Pin 		= GPIO_PIN_8,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
		.tGPIOPort 			= GPIOA,					/* GPIO���� */		
	}
};

/* GPIO���Ƽ̵��� */
tagGPIO_T Control_GPIO[] =
{
		[POW_MAG]=	//ctrl_1 PA6 12V ������ר�� 12VJ1
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_6,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_PULLUP,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */ //Ĭ������,���м̵����ر�
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
		.tGPIOPort 			= GPIOA,					/* GPIO���� */
	},
		[POW_TDM]=	//ctrl_2 PA5 12V ������ 12VJ2
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_5,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_PULLUP,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
		.tGPIOPort 			= GPIOA,					/* GPIO���� */
	},
		[POW_HEIGHT]=	//ctrl_3 PC4 12V �߶ȼ� 12VJ3
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_4,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_PULLUP,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
		.tGPIOPort 			= GPIOC,					/* GPIO���� */
	},
		[POW_LIGHT]=	//ctrl_4 PA7 12V ˮ�µ� 12VJ4
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_7,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
		.tGPIOPort 			= GPIOA,					/* GPIO���� */
	},

		[POW_BDGPS]=	//ctrl_5 PC5 12V ����GPS 12VJ5
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_5,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_PULLUP,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
		.tGPIOPort 			= GPIOC,					/* GPIO���� */
	},
		[POW_RADIO]=	//ctrl_6 PE7 12V ���ߵ�̨ 12VJ6
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_7,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_PULLUP,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
		.tGPIOPort 			= GPIOE,					/* GPIO���� */
	},
		[POW_UACM]=	//ctrl_7 PE8 24V ��ͨ�� 24VJ1
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_8,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_PULLUP,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
		.tGPIOPort 			= GPIOE,					/* GPIO���� */
	},
		[POW_TEST_1]=	//ctrl_8 PE10 24V 
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_10,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_PULLUP,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
		.tGPIOPort 			= GPIOE,					/* GPIO���� */
	},	 
		[POW_TEST_2]=	//ctrl_9 PE12 24V 
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_12,				/* GPIO���� */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
		.tGPIOInit.Pull 	= GPIO_PULLUP,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
		.tGPIOPort 			= GPIOE,					/* GPIO���� */
	},
	//ctrl_10 PE15 ����,����Ӽ̵���
};

/* ����1��ݮ�� */
tagUART_T Uart1 = 
{
	//���ڹ���ģʽ����
	.tUARTHandle.Instance 						= USART1,					/* STM32 �����豸 */
	.tUARTHandle.Init.BaudRate   				= 9600,						/* ���ڲ����� */

	.ucPriority									= 1,						/* �ж����ȼ� */
	.ucSubPriority								= 3,						/* �ж������ȼ� */
	
	//����DMA���ղ�������
	.tUartDMA.bRxEnable							= true,						/* DMA����ʹ�� */
	.tRxInfo.usDMARxMAXSize             		= 100,              		/* DMA���ջ�������С ��С������Э����ֽ�*2���ϣ�ȷ�������һ���ܹ��ȶ�����һ������������֡*/

	//����DMA���Ͳ�������
	.tUartDMA.bTxEnable							= true,						/* DMA����ʹ�� */
	.tTxInfo.usDMATxMAXSize						= 150,						/* DMA���ͻ�������С */
};

/*����2������(����)*/
tagUART_T Uart2 = 
{
	//���ڹ���ģʽ����
	.tUARTHandle.Instance 						= USART2,					/* STM32 �����豸 */
	.tUARTHandle.Init.BaudRate   				= 9600,						/* ���ڲ����� */

	.ucPriority									= 1,						/* �ж����ȼ� */
	.ucSubPriority								= 3,						/* �ж������ȼ� */
	
	//����DMA���ղ�������
	.tUartDMA.bRxEnable							= true,						/* DMA����ʹ�� */
	.tRxInfo.usDMARxMAXSize             		= 100,              		/* DMA���ջ�������С ��С������Э����ֽ�*2���ϣ�ȷ�������һ���ܹ��ȶ�����һ������������֡*/

	//����DMA���Ͳ�������
	.tUartDMA.bTxEnable							= true,						/* DMA����ʹ�� */
	.tTxInfo.usDMATxMAXSize						= 150,						/* DMA���ͻ�������С */
};

/* ����2 JY901S�������� */
tagJY901_T JY901S = 
{
	.tConfig.ucBaud 	= JY901_RXBAUD_9600,
	.tConfig.ucRate		= JY901_RX_2HZ,
	.tConfig.usType		= JY901_OUTPUT_ANGLE | JY901_OUTPUT_MAG | JY901_OUTPUT_ACCEL | JY901_OUTPUT_GYRO,

	.tUART.tRxInfo.usDMARxMAXSize             	= 200,                 /* �������ݳ��� ���ȱ�����Э����ֽ�*2���ϣ�ȷ�������һ���ܹ��ȶ�����һ������������֡*/

    .tUART.tUartDMA.bRxEnable					= true,					/* DMA����ʹ�� */
};

/*����3�߶ȼ�*/
tagUART_T Uart3 = 
{
	//���ڹ���ģʽ����
	.tUARTHandle.Instance 						= USART3,					/* STM32 �����豸 */
	.tUARTHandle.Init.BaudRate   				= 9600,						/* ���ڲ����� */

	.ucPriority									= 1,						/* �ж����ȼ� */
	.ucSubPriority								= 3,						/* �ж������ȼ� */
	
	//����DMA���ղ�������
	.tUartDMA.bRxEnable							= true,						/* DMA����ʹ�� */
	.tRxInfo.usDMARxMAXSize             		= 150,              		/* DMA���ջ�������С ��С������Э����ֽ�*2���ϣ�ȷ�������һ���ܹ��ȶ�����һ������������֡*/

	//����DMA���Ͳ�������
	.tUartDMA.bTxEnable							= true,						/* DMA����ʹ�� */
	.tTxInfo.usDMATxMAXSize						= 150,						/* DMA���ͻ�������С */
};
/*����4������*/
tagUART_T Uart4 = 
{
	//���ڹ���ģʽ����
	.tUARTHandle.Instance 						= UART4,					/* STM32 �����豸 */
	.tUARTHandle.Init.BaudRate   				= 115200,					/* ���ڲ����� */

	.ucPriority									= 1,						/* �ж����ȼ� */
	.ucSubPriority								= 3,						/* �ж������ȼ� */
	
	//����DMA���ղ�������
	.tUartDMA.bRxEnable							= true,						/* DMA����ʹ�� */
	.tRxInfo.usDMARxMAXSize             		= 150,              		/* DMA���ջ�������С ��С������Э����ֽ�*2���ϣ�ȷ�������һ���ܹ��ȶ�����һ������������֡*/

	//����DMA���Ͳ�������
	.tUartDMA.bTxEnable							= true,						/* DMA����ʹ�� */
	.tTxInfo.usDMATxMAXSize						= 150,						/* DMA���ͻ�������С */
};
/*����5����*/

/*PWM��12·*/
tagPWM_T PWM[] =
{
	[LVT] =       //PC7 TIM8_CH2 ����
	{
		.fDuty					= 5,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_7,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOC,			/* IO��ӳ�� */
	},
	[RVT] =       //PC6 TIM8_CH1 �Ҵ���
	{
		.fDuty					= 6,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_6,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOC,			/* IO��ӳ�� */
	},
	[BVT] =       //PD15 TIM4_CH4 ����
	{
		.fDuty					= 7.5,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_15,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOD,			/* IO��ӳ�� */
	},
	[LMT] =       //PD13 TIM4_CH2 ������
	{
		.fDuty					= 7.5,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_13,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOD,			/* IO��ӳ�� */
	},
	[RMT] =       //PD14 TIM4_CH3 ������
	{
		.fDuty					= 7.5,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_14,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOD,			/* IO��ӳ�� */
	},
	[LS] =       //PD12 TIM4_CH1 ����
	{
		.fDuty					= 7.5,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_12,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOD,			/* IO��ӳ�� */
	},
	[RS] =       //PE13 TIM1_CH3 �Ҷ��
	{
		.fDuty					= 7.5,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_13,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOE,			/* IO��ӳ�� */
	},
	[LIGHT] =     //PE14 TIM1_CH4 ˮ�µ�
	{
		.fDuty					= 7.5,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_14,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOE,			/* IO��ӳ�� */
	},
	[MANIPULATOR] =       //PB1 TIM3_CH4 ��е��
	{
		.fDuty					= 7.5,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_1,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOB,			/* IO��ӳ�� */
	},
	[PWM_TEST_1] =       //PB0 TIM3_CH3 ���� ����
	{
		.fDuty					= 7.5,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_0,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOB,			/* IO��ӳ�� */
	},
	[PWM_TEST_2] =       //PC9 TIM8_CH4 ���� ����
	{
		.fDuty					= 9,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_9,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOC,			/* IO��ӳ�� */
	},
	[PWM_TEST_3] =       //PC8 TIM8_CH3 ���� ����
	{
		.fDuty					= 7.5,				/* ��ʼռ�ձȣ�%�� */
		.ulFreq					= 50,				/* Ƶ�ʣ�Hz�� */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_8,		/* IOӳ�� */
		.tGPIO.tGPIOPort		= GPIOC,			/* IO��ӳ�� */
	},
};

CRC_HandleTypeDef hcrc;
void CRC_Init()
{
    __HAL_RCC_CRC_CLK_ENABLE();  // ʹ�� CRC ʱ��
    hcrc.Instance = CRC;
    HAL_CRC_Init(&hcrc);
}
