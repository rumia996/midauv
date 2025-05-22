#include "config.h"

uint8_t MODE = DEFAULT_MODE;//����Ĭ��ģʽ

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

/* SYNC���� */
tagGPIO_T SYNC =
{
	.tGPIOInit.Pin 		= GPIO_PIN_0,				/* GPIO���� */
	.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
	.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tGPIOPort 			= GPIOE,					/* GPIO���� */
};

/* ADоƬ���� */
tagAD24BIT_T AD4111_1 =
{
	.tSPI.tSPIHandle.Instance 		    = SPI2,		
    .tSPI.tGPIO[0].tGPIOInit.Pin 		= GPIO_PIN_13,				/* GPIO���� ,SPI2_SCK*/
	.tSPI.tGPIO[0].tGPIOInit.Mode 		= GPIO_MODE_AF_PP,			/* GPIOģʽ */
	.tSPI.tGPIO[0].tGPIOInit.Pull 		= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tSPI.tGPIO[0].tGPIOInit.Speed 		= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tSPI.tGPIO[0].tGPIOPort 			= GPIOB,					/* GPIO���� */	
	.tSPI.tGPIO[1].tGPIOInit.Pin 		= GPIO_PIN_15,				/* GPIO���� ,SPI2_MOSI*/
	.tSPI.tGPIO[1].tGPIOInit.Mode 		= GPIO_MODE_AF_PP,			/* GPIOģʽ */
	.tSPI.tGPIO[1].tGPIOInit.Pull 		= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tSPI.tGPIO[1].tGPIOInit.Speed 		= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tSPI.tGPIO[1].tGPIOPort 			= GPIOB,					/* GPIO���� */	
	.tSPI.tGPIO[2].tGPIOInit.Pin 		= GPIO_PIN_14,				/* GPIO���� ,SPI2_MISO*/
	.tSPI.tGPIO[2].tGPIOInit.Mode 		= GPIO_MODE_AF_PP,			/* GPIOģʽ */
	.tSPI.tGPIO[2].tGPIOInit.Pull 		= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tSPI.tGPIO[2].tGPIOInit.Speed 		= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tSPI.tGPIO[2].tGPIOPort 			= GPIOB,					/* GPIO���� */		
    .tSPI.tGPIO[3].tGPIOInit.Pin 		= GPIO_PIN_12,				/* GPIO���� ,CS*/
	.tSPI.tGPIO[3].tGPIOInit.Mode 		= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
	.tSPI.tGPIO[3].tGPIOInit.Pull 		= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tSPI.tGPIO[3].tGPIOInit.Speed 		= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tSPI.tGPIO[3].tGPIOPort 			= GPIOB,					/* GPIO���� */	
	.port_num = 8,  //�����Ķ˿��������������õĶ˿ڶ�Ӧ����
	.port[0].enable = PORT_ENABLE,//�˿�ʹ��
	.port[0].num = VIN0,  //�ɼ��źŵ�,����ģʽ���źŵĵؽ�Vincom�˿�;���ģʽ���źŽӶ�Ӧ�˿�
	.port[0].gain = 1.001648338,  //У����������Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[0].offset= 0.000936992, //У������ƫ����Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[1].enable = PORT_ENABLE,
	.port[1].num = VIN1,	//�ɼ��źŵ�,����ģʽ���źŵĵؽ�Vincom�˿�;���ģʽ���źŽӶ�Ӧ�˿�
	.port[1].gain = 1.00148387,  //У����������Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[1].offset= 0.002501957,//У������ƫ����Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;	
	.port[2].enable = PORT_ENABLE,
	.port[2].num = VIN2,	//�ɼ��źŵ�,����ģʽ���źŵĵؽ�Vincom�˿�;���ģʽ���źŽӶ�Ӧ�˿�
	.port[2].gain = 1.0,  //У����������Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[2].offset= 0.0,//У������ƫ����Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[3].enable = PORT_ENABLE,//�˿�ʹ��
	.port[3].num = VIN3,  //�ɼ��źŵ�,����ģʽ���źŵĵؽ�Vincom�˿�;���ģʽ���źŽӶ�Ӧ�˿�
	.port[3].gain = 1.0,  //У����������Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[3].offset= 0.0,//У������ƫ����Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[4].enable = PORT_ENABLE,
	.port[4].num = VIN4,	//�ɼ��źŵ�,����ģʽ���źŵĵؽ�Vincom�˿�;���ģʽ���źŽӶ�Ӧ�˿�
	.port[4].gain = 1.001571984,  //У����������Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[4].offset= 0.001703874,//У������ƫ����Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;	
	.port[5].enable = PORT_ENABLE,
	.port[5].num = VIN5,	//�ɼ��źŵ�,����ģʽ���źŵĵؽ�Vincom�˿�;���ģʽ���źŽӶ�Ӧ�˿�
	.port[5].gain = 1.001649837,  //У����������Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[5].offset= 0.001140979,//У������ƫ����Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
    .port[6].enable = PORT_ENABLE,
	.port[6].num = VIN6,	//�ɼ��źŵ�,����ģʽ���źŵĵؽ�Vincom�˿�;���ģʽ���źŽӶ�Ӧ�˿�
	.port[6].gain = 1.001620507,  //У����������Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[6].offset= 0.001374424,//У������ƫ����Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
    .port[7].enable = PORT_ENABLE,
	.port[7].num = VIN7,	//�ɼ��źŵ�,����ģʽ���źŵĵؽ�Vincom�˿�;���ģʽ���źŽӶ�Ӧ�˿�
	.port[7].gain = 1.001448878,  //У����������Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[7].offset= 0.00172139,//У������ƫ����Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
};

/* ADоƬ���� */
tagAD24BIT_T AD4111_2 =
{
	.tSPI.tSPIHandle.Instance 		    = SPI3,		
    .tSPI.tGPIO[0].tGPIOInit.Pin 		= GPIO_PIN_3,				/* GPIO���� ,SPI3_SCK*/
	.tSPI.tGPIO[0].tGPIOInit.Mode 		= GPIO_MODE_AF_PP,			/* GPIOģʽ */
	.tSPI.tGPIO[0].tGPIOInit.Pull 		= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tSPI.tGPIO[0].tGPIOInit.Speed 		= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tSPI.tGPIO[0].tGPIOPort 			= GPIOB,					/* GPIO���� */	
	.tSPI.tGPIO[1].tGPIOInit.Pin 		= GPIO_PIN_5,				/* GPIO���� ,SPI3_MOSI*/
	.tSPI.tGPIO[1].tGPIOInit.Mode 		= GPIO_MODE_AF_PP,			/* GPIOģʽ */
	.tSPI.tGPIO[1].tGPIOInit.Pull 		= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tSPI.tGPIO[1].tGPIOInit.Speed 		= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tSPI.tGPIO[1].tGPIOPort 			= GPIOB,					/* GPIO���� */	
	.tSPI.tGPIO[2].tGPIOInit.Pin 		= GPIO_PIN_4,				/* GPIO���� ,SPI3_MISO*/
	.tSPI.tGPIO[2].tGPIOInit.Mode 		= GPIO_MODE_AF_PP,			/* GPIOģʽ */
	.tSPI.tGPIO[2].tGPIOInit.Pull 		= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tSPI.tGPIO[2].tGPIOInit.Speed 		= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tSPI.tGPIO[2].tGPIOPort 			= GPIOB,					/* GPIO���� */		
    .tSPI.tGPIO[3].tGPIOInit.Pin 		= GPIO_PIN_7,				/* GPIO���� ,SPI3_CS*/
	.tSPI.tGPIO[3].tGPIOInit.Mode 		= GPIO_MODE_OUTPUT_PP,		/* GPIOģʽ */
	.tSPI.tGPIO[3].tGPIOInit.Pull 		= GPIO_NOPULL,				/* GPIO���������ã��Ƿ���Ҫ��������Ӳ�� */
	.tSPI.tGPIO[3].tGPIOInit.Speed 		= GPIO_SPEED_FREQ_HIGH,		/* GPIO�ٶ� */	
	.tSPI.tGPIO[3].tGPIOPort 			= GPIOD,					/* GPIO���� */	
	.port_num = 8,  //�����Ķ˿��������������õĶ˿ڶ�Ӧ����
	.port[0].enable = PORT_ENABLE,//�˿�ʹ��
	.port[0].num = VIN0,  //�ɼ��źŵ�,����ģʽ���źŵĵؽ�Vincom�˿�;���ģʽ���źŽӶ�Ӧ�˿�
	.port[0].gain = 1.001566898,  //У����������Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[0].offset= 0.002822938,//У������ƫ����Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[1].enable = PORT_ENABLE,
	.port[1].num = VIN1,	//�ɼ��źŵ�,����ģʽ���źŵĵؽ�Vincom�˿�;���ģʽ���źŽӶ�Ӧ�˿�
	.port[1].gain = 1.001477404,  //У����������Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[1].offset= 0.002462209,//У������ƫ����Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;	
	.port[2].enable = PORT_ENABLE,
	.port[2].num = VIN2,	//�ɼ��źŵ�,����ģʽ���źŵĵؽ�Vincom�˿�;���ģʽ���źŽӶ�Ӧ�˿�
	.port[2].gain = 1.0,  //У����������Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[2].offset= 0.0,//У������ƫ����Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[3].enable = PORT_ENABLE,//�˿�ʹ��
	.port[3].num = VIN3,  //�ɼ��źŵ�,����ģʽ���źŵĵؽ�Vincom�˿�;���ģʽ���źŽӶ�Ӧ�˿�
	.port[3].gain = 1.0,  //У����������Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[3].offset= 0.0,//У������ƫ����Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[4].enable = PORT_ENABLE,
	.port[4].num = VIN4,	//�ɼ��źŵ�,����ģʽ���źŵĵؽ�Vincom�˿�;���ģʽ���źŽӶ�Ӧ�˿�
	.port[4].gain = 1.001607121,  //У����������Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[4].offset= 0.002711971,//У������ƫ����Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;	
	.port[5].enable = PORT_ENABLE,
	.port[5].num = VIN5,	//�ɼ��źŵ�,����ģʽ���źŵĵؽ�Vincom�˿�;���ģʽ���źŽӶ�Ӧ�˿�
	.port[5].gain = 1.001525822,  //У����������Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[5].offset= 0.00250732,//У������ƫ����Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
    .port[6].enable = PORT_ENABLE,
	.port[6].num = VIN6,	//�ɼ��źŵ�,����ģʽ���źŵĵؽ�Vincom�˿�;���ģʽ���źŽӶ�Ӧ�˿�
	.port[6].gain = 1.001581494,  //У����������Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[6].offset= 0.003163044,//У������ƫ����Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
    .port[7].enable = PORT_ENABLE,
	.port[7].num = VIN7,	//�ɼ��źŵ�,����ģʽ���źŵĵؽ�Vincom�˿�;���ģʽ���źŽӶ�Ӧ�˿�
	.port[7].gain = 1.00152087,  //У����������Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
	.port[7].offset= 0.002486376,//У������ƫ����Y =  gain*X+offset,������ҪУ����Ĭ��gain = 1.0,offset = 0.0;
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


