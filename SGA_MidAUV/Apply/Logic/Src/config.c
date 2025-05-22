#include "config.h"

uint8_t MODE = DEFAULT_MODE;//设置默认模式

/* 独立看门狗 */
tagIWDG_T demoIWDG = 
{
	.usResetTime = 1000,			/* 1S喂狗溢出时间 */
};

tagGPIO_T RS485Ctrl[] =
{
		[0] = 
	{
		.tGPIOInit.Pin 		= GPIO_PIN_8,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
		.tGPIOPort 			= GPIOA,					/* GPIO分组 */		
	}
};

/* GPIO控制继电器 */
tagGPIO_T Control_GPIO[] =
{
		[POW_MAG]=	//ctrl_1 PA6 12V 磁力仪专用 12VJ1
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_6,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_PULLUP,				/* GPIO上下拉设置，是否需要上下拉看硬件 */ //默认上拉,所有继电器关闭
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
		.tGPIOPort 			= GPIOA,					/* GPIO分组 */
	},
		[POW_TDM]=	//ctrl_2 PA5 12V 温深仪 12VJ2
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_5,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_PULLUP,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
		.tGPIOPort 			= GPIOA,					/* GPIO分组 */
	},
		[POW_HEIGHT]=	//ctrl_3 PC4 12V 高度计 12VJ3
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_4,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_PULLUP,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
		.tGPIOPort 			= GPIOC,					/* GPIO分组 */
	},
		[POW_LIGHT]=	//ctrl_4 PA7 12V 水下灯 12VJ4
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_7,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
		.tGPIOPort 			= GPIOA,					/* GPIO分组 */
	},

		[POW_BDGPS]=	//ctrl_5 PC5 12V 北斗GPS 12VJ5
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_5,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_PULLUP,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
		.tGPIOPort 			= GPIOC,					/* GPIO分组 */
	},
		[POW_RADIO]=	//ctrl_6 PE7 12V 无线电台 12VJ6
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_7,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_PULLUP,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
		.tGPIOPort 			= GPIOE,					/* GPIO分组 */
	},
		[POW_UACM]=	//ctrl_7 PE8 24V 声通机 24VJ1
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_8,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_PULLUP,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
		.tGPIOPort 			= GPIOE,					/* GPIO分组 */
	},
		[POW_TEST_1]=	//ctrl_8 PE10 24V 
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_10,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_PULLUP,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
		.tGPIOPort 			= GPIOE,					/* GPIO分组 */
	},	 
		[POW_TEST_2]=	//ctrl_9 PE12 24V 
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_12,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_PULLUP,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
		.tGPIOPort 			= GPIOE,					/* GPIO分组 */
	},
	//ctrl_10 PE15 空置,可外接继电器
};

/* SYNC引脚 */
tagGPIO_T SYNC =
{
	.tGPIOInit.Pin 		= GPIO_PIN_0,				/* GPIO引脚 */
	.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
	.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tGPIOPort 			= GPIOE,					/* GPIO分组 */
};

/* AD芯片配置 */
tagAD24BIT_T AD4111_1 =
{
	.tSPI.tSPIHandle.Instance 		    = SPI2,		
    .tSPI.tGPIO[0].tGPIOInit.Pin 		= GPIO_PIN_13,				/* GPIO引脚 ,SPI2_SCK*/
	.tSPI.tGPIO[0].tGPIOInit.Mode 		= GPIO_MODE_AF_PP,			/* GPIO模式 */
	.tSPI.tGPIO[0].tGPIOInit.Pull 		= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tSPI.tGPIO[0].tGPIOInit.Speed 		= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tSPI.tGPIO[0].tGPIOPort 			= GPIOB,					/* GPIO分组 */	
	.tSPI.tGPIO[1].tGPIOInit.Pin 		= GPIO_PIN_15,				/* GPIO引脚 ,SPI2_MOSI*/
	.tSPI.tGPIO[1].tGPIOInit.Mode 		= GPIO_MODE_AF_PP,			/* GPIO模式 */
	.tSPI.tGPIO[1].tGPIOInit.Pull 		= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tSPI.tGPIO[1].tGPIOInit.Speed 		= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tSPI.tGPIO[1].tGPIOPort 			= GPIOB,					/* GPIO分组 */	
	.tSPI.tGPIO[2].tGPIOInit.Pin 		= GPIO_PIN_14,				/* GPIO引脚 ,SPI2_MISO*/
	.tSPI.tGPIO[2].tGPIOInit.Mode 		= GPIO_MODE_AF_PP,			/* GPIO模式 */
	.tSPI.tGPIO[2].tGPIOInit.Pull 		= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tSPI.tGPIO[2].tGPIOInit.Speed 		= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tSPI.tGPIO[2].tGPIOPort 			= GPIOB,					/* GPIO分组 */		
    .tSPI.tGPIO[3].tGPIOInit.Pin 		= GPIO_PIN_12,				/* GPIO引脚 ,CS*/
	.tSPI.tGPIO[3].tGPIOInit.Mode 		= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
	.tSPI.tGPIO[3].tGPIOInit.Pull 		= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tSPI.tGPIO[3].tGPIOInit.Speed 		= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tSPI.tGPIO[3].tGPIOPort 			= GPIOB,					/* GPIO分组 */	
	.port_num = 8,  //测量的端口数，跟下面配置的端口对应起来
	.port[0].enable = PORT_ENABLE,//端口使能
	.port[0].num = VIN0,  //采集信号的,单端模式，信号的地接Vincom端口;差分模式，信号接对应端口
	.port[0].gain = 1.001648338,  //校正参数增益Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[0].offset= 0.000936992, //校正参数偏移量Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[1].enable = PORT_ENABLE,
	.port[1].num = VIN1,	//采集信号的,单端模式，信号的地接Vincom端口;差分模式，信号接对应端口
	.port[1].gain = 1.00148387,  //校正参数增益Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[1].offset= 0.002501957,//校正参数偏移量Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;	
	.port[2].enable = PORT_ENABLE,
	.port[2].num = VIN2,	//采集信号的,单端模式，信号的地接Vincom端口;差分模式，信号接对应端口
	.port[2].gain = 1.0,  //校正参数增益Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[2].offset= 0.0,//校正参数偏移量Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[3].enable = PORT_ENABLE,//端口使能
	.port[3].num = VIN3,  //采集信号的,单端模式，信号的地接Vincom端口;差分模式，信号接对应端口
	.port[3].gain = 1.0,  //校正参数增益Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[3].offset= 0.0,//校正参数偏移量Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[4].enable = PORT_ENABLE,
	.port[4].num = VIN4,	//采集信号的,单端模式，信号的地接Vincom端口;差分模式，信号接对应端口
	.port[4].gain = 1.001571984,  //校正参数增益Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[4].offset= 0.001703874,//校正参数偏移量Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;	
	.port[5].enable = PORT_ENABLE,
	.port[5].num = VIN5,	//采集信号的,单端模式，信号的地接Vincom端口;差分模式，信号接对应端口
	.port[5].gain = 1.001649837,  //校正参数增益Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[5].offset= 0.001140979,//校正参数偏移量Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
    .port[6].enable = PORT_ENABLE,
	.port[6].num = VIN6,	//采集信号的,单端模式，信号的地接Vincom端口;差分模式，信号接对应端口
	.port[6].gain = 1.001620507,  //校正参数增益Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[6].offset= 0.001374424,//校正参数偏移量Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
    .port[7].enable = PORT_ENABLE,
	.port[7].num = VIN7,	//采集信号的,单端模式，信号的地接Vincom端口;差分模式，信号接对应端口
	.port[7].gain = 1.001448878,  //校正参数增益Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[7].offset= 0.00172139,//校正参数偏移量Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
};

/* AD芯片配置 */
tagAD24BIT_T AD4111_2 =
{
	.tSPI.tSPIHandle.Instance 		    = SPI3,		
    .tSPI.tGPIO[0].tGPIOInit.Pin 		= GPIO_PIN_3,				/* GPIO引脚 ,SPI3_SCK*/
	.tSPI.tGPIO[0].tGPIOInit.Mode 		= GPIO_MODE_AF_PP,			/* GPIO模式 */
	.tSPI.tGPIO[0].tGPIOInit.Pull 		= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tSPI.tGPIO[0].tGPIOInit.Speed 		= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tSPI.tGPIO[0].tGPIOPort 			= GPIOB,					/* GPIO分组 */	
	.tSPI.tGPIO[1].tGPIOInit.Pin 		= GPIO_PIN_5,				/* GPIO引脚 ,SPI3_MOSI*/
	.tSPI.tGPIO[1].tGPIOInit.Mode 		= GPIO_MODE_AF_PP,			/* GPIO模式 */
	.tSPI.tGPIO[1].tGPIOInit.Pull 		= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tSPI.tGPIO[1].tGPIOInit.Speed 		= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tSPI.tGPIO[1].tGPIOPort 			= GPIOB,					/* GPIO分组 */	
	.tSPI.tGPIO[2].tGPIOInit.Pin 		= GPIO_PIN_4,				/* GPIO引脚 ,SPI3_MISO*/
	.tSPI.tGPIO[2].tGPIOInit.Mode 		= GPIO_MODE_AF_PP,			/* GPIO模式 */
	.tSPI.tGPIO[2].tGPIOInit.Pull 		= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tSPI.tGPIO[2].tGPIOInit.Speed 		= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tSPI.tGPIO[2].tGPIOPort 			= GPIOB,					/* GPIO分组 */		
    .tSPI.tGPIO[3].tGPIOInit.Pin 		= GPIO_PIN_7,				/* GPIO引脚 ,SPI3_CS*/
	.tSPI.tGPIO[3].tGPIOInit.Mode 		= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
	.tSPI.tGPIO[3].tGPIOInit.Pull 		= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tSPI.tGPIO[3].tGPIOInit.Speed 		= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tSPI.tGPIO[3].tGPIOPort 			= GPIOD,					/* GPIO分组 */	
	.port_num = 8,  //测量的端口数，跟下面配置的端口对应起来
	.port[0].enable = PORT_ENABLE,//端口使能
	.port[0].num = VIN0,  //采集信号的,单端模式，信号的地接Vincom端口;差分模式，信号接对应端口
	.port[0].gain = 1.001566898,  //校正参数增益Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[0].offset= 0.002822938,//校正参数偏移量Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[1].enable = PORT_ENABLE,
	.port[1].num = VIN1,	//采集信号的,单端模式，信号的地接Vincom端口;差分模式，信号接对应端口
	.port[1].gain = 1.001477404,  //校正参数增益Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[1].offset= 0.002462209,//校正参数偏移量Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;	
	.port[2].enable = PORT_ENABLE,
	.port[2].num = VIN2,	//采集信号的,单端模式，信号的地接Vincom端口;差分模式，信号接对应端口
	.port[2].gain = 1.0,  //校正参数增益Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[2].offset= 0.0,//校正参数偏移量Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[3].enable = PORT_ENABLE,//端口使能
	.port[3].num = VIN3,  //采集信号的,单端模式，信号的地接Vincom端口;差分模式，信号接对应端口
	.port[3].gain = 1.0,  //校正参数增益Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[3].offset= 0.0,//校正参数偏移量Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[4].enable = PORT_ENABLE,
	.port[4].num = VIN4,	//采集信号的,单端模式，信号的地接Vincom端口;差分模式，信号接对应端口
	.port[4].gain = 1.001607121,  //校正参数增益Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[4].offset= 0.002711971,//校正参数偏移量Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;	
	.port[5].enable = PORT_ENABLE,
	.port[5].num = VIN5,	//采集信号的,单端模式，信号的地接Vincom端口;差分模式，信号接对应端口
	.port[5].gain = 1.001525822,  //校正参数增益Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[5].offset= 0.00250732,//校正参数偏移量Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
    .port[6].enable = PORT_ENABLE,
	.port[6].num = VIN6,	//采集信号的,单端模式，信号的地接Vincom端口;差分模式，信号接对应端口
	.port[6].gain = 1.001581494,  //校正参数增益Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[6].offset= 0.003163044,//校正参数偏移量Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
    .port[7].enable = PORT_ENABLE,
	.port[7].num = VIN7,	//采集信号的,单端模式，信号的地接Vincom端口;差分模式，信号接对应端口
	.port[7].gain = 1.00152087,  //校正参数增益Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
	.port[7].offset= 0.002486376,//校正参数偏移量Y =  gain*X+offset,若不需要校正则默认gain = 1.0,offset = 0.0;
};

/* 串口1树莓派 */
tagUART_T Uart1 = 
{
	//串口工作模式配置
	.tUARTHandle.Instance 						= USART1,					/* STM32 串口设备 */
	.tUARTHandle.Init.BaudRate   				= 9600,						/* 串口波特率 */

	.ucPriority									= 1,						/* 中断优先级 */
	.ucSubPriority								= 3,						/* 中断子优先级 */
	
	//串口DMA接收参数配置
	.tUartDMA.bRxEnable							= true,						/* DMA接收使能 */
	.tRxInfo.usDMARxMAXSize             		= 100,              		/* DMA接收缓冲区大小 大小保持在协议最长字节*2以上，确保缓存池一定能够稳定接收一个完整的数据帧*/

	//串口DMA发送参数配置
	.tUartDMA.bTxEnable							= true,						/* DMA发送使能 */
	.tTxInfo.usDMATxMAXSize						= 150,						/* DMA发送缓冲区大小 */
};

/*串口2温深仪(弃用)*/
tagUART_T Uart2 = 
{
	//串口工作模式配置
	.tUARTHandle.Instance 						= USART2,					/* STM32 串口设备 */
	.tUARTHandle.Init.BaudRate   				= 9600,						/* 串口波特率 */

	.ucPriority									= 1,						/* 中断优先级 */
	.ucSubPriority								= 3,						/* 中断子优先级 */
	
	//串口DMA接收参数配置
	.tUartDMA.bRxEnable							= true,						/* DMA接收使能 */
	.tRxInfo.usDMARxMAXSize             		= 100,              		/* DMA接收缓冲区大小 大小保持在协议最长字节*2以上，确保缓存池一定能够稳定接收一个完整的数据帧*/

	//串口DMA发送参数配置
	.tUartDMA.bTxEnable							= true,						/* DMA发送使能 */
	.tTxInfo.usDMATxMAXSize						= 150,						/* DMA发送缓冲区大小 */
};

/* 串口2 JY901S参数设置 */
tagJY901_T JY901S = 
{
	.tConfig.ucBaud 	= JY901_RXBAUD_9600,
	.tConfig.ucRate		= JY901_RX_2HZ,
	.tConfig.usType		= JY901_OUTPUT_ANGLE | JY901_OUTPUT_MAG | JY901_OUTPUT_ACCEL | JY901_OUTPUT_GYRO,

	.tUART.tRxInfo.usDMARxMAXSize             	= 200,                 /* 接收数据长度 长度保持在协议最长字节*2以上，确保缓存池一定能够稳定接收一个完整的数据帧*/

    .tUART.tUartDMA.bRxEnable					= true,					/* DMA接收使能 */
};

/*串口3高度计*/
tagUART_T Uart3 = 
{
	//串口工作模式配置
	.tUARTHandle.Instance 						= USART3,					/* STM32 串口设备 */
	.tUARTHandle.Init.BaudRate   				= 9600,						/* 串口波特率 */

	.ucPriority									= 1,						/* 中断优先级 */
	.ucSubPriority								= 3,						/* 中断子优先级 */
	
	//串口DMA接收参数配置
	.tUartDMA.bRxEnable							= true,						/* DMA接收使能 */
	.tRxInfo.usDMARxMAXSize             		= 150,              		/* DMA接收缓冲区大小 大小保持在协议最长字节*2以上，确保缓存池一定能够稳定接收一个完整的数据帧*/

	//串口DMA发送参数配置
	.tUartDMA.bTxEnable							= true,						/* DMA发送使能 */
	.tTxInfo.usDMATxMAXSize						= 150,						/* DMA发送缓冲区大小 */
};
/*串口4温深仪*/
tagUART_T Uart4 = 
{
	//串口工作模式配置
	.tUARTHandle.Instance 						= UART4,					/* STM32 串口设备 */
	.tUARTHandle.Init.BaudRate   				= 115200,					/* 串口波特率 */

	.ucPriority									= 1,						/* 中断优先级 */
	.ucSubPriority								= 3,						/* 中断子优先级 */
	
	//串口DMA接收参数配置
	.tUartDMA.bRxEnable							= true,						/* DMA接收使能 */
	.tRxInfo.usDMARxMAXSize             		= 150,              		/* DMA接收缓冲区大小 大小保持在协议最长字节*2以上，确保缓存池一定能够稳定接收一个完整的数据帧*/

	//串口DMA发送参数配置
	.tUartDMA.bTxEnable							= true,						/* DMA发送使能 */
	.tTxInfo.usDMATxMAXSize						= 150,						/* DMA发送缓冲区大小 */
};
/*串口5备用*/

/*PWM共12路*/
tagPWM_T PWM[] =
{
	[LVT] =       //PC7 TIM8_CH2 左垂推
	{
		.fDuty					= 5,				/* 初始占空比（%） */
		.ulFreq					= 50,				/* 频率（Hz） */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_7,		/* IO映射 */
		.tGPIO.tGPIOPort		= GPIOC,			/* IO组映射 */
	},
	[RVT] =       //PC6 TIM8_CH1 右垂推
	{
		.fDuty					= 6,				/* 初始占空比（%） */
		.ulFreq					= 50,				/* 频率（Hz） */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_6,		/* IO映射 */
		.tGPIO.tGPIOPort		= GPIOC,			/* IO组映射 */
	},
	[BVT] =       //PD15 TIM4_CH4 后垂推
	{
		.fDuty					= 7.5,				/* 初始占空比（%） */
		.ulFreq					= 50,				/* 频率（Hz） */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_15,		/* IO映射 */
		.tGPIO.tGPIOPort		= GPIOD,			/* IO组映射 */
	},
	[LMT] =       //PD13 TIM4_CH2 左主推
	{
		.fDuty					= 7.5,				/* 初始占空比（%） */
		.ulFreq					= 50,				/* 频率（Hz） */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_13,		/* IO映射 */
		.tGPIO.tGPIOPort		= GPIOD,			/* IO组映射 */
	},
	[RMT] =       //PD14 TIM4_CH3 右主推
	{
		.fDuty					= 7.5,				/* 初始占空比（%） */
		.ulFreq					= 50,				/* 频率（Hz） */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_14,		/* IO映射 */
		.tGPIO.tGPIOPort		= GPIOD,			/* IO组映射 */
	},
	[LS] =       //PD12 TIM4_CH1 左舵机
	{
		.fDuty					= 7.5,				/* 初始占空比（%） */
		.ulFreq					= 50,				/* 频率（Hz） */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_12,		/* IO映射 */
		.tGPIO.tGPIOPort		= GPIOD,			/* IO组映射 */
	},
	[RS] =       //PE13 TIM1_CH3 右舵机
	{
		.fDuty					= 7.5,				/* 初始占空比（%） */
		.ulFreq					= 50,				/* 频率（Hz） */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_13,		/* IO映射 */
		.tGPIO.tGPIOPort		= GPIOE,			/* IO组映射 */
	},
	[LIGHT] =     //PE14 TIM1_CH4 水下灯
	{
		.fDuty					= 7.5,				/* 初始占空比（%） */
		.ulFreq					= 50,				/* 频率（Hz） */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_14,		/* IO映射 */
		.tGPIO.tGPIOPort		= GPIOE,			/* IO组映射 */
	},
	[MANIPULATOR] =       //PB1 TIM3_CH4 机械手
	{
		.fDuty					= 7.5,				/* 初始占空比（%） */
		.ulFreq					= 50,				/* 频率（Hz） */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_1,		/* IO映射 */
		.tGPIO.tGPIOPort		= GPIOB,			/* IO组映射 */
	},
	[PWM_TEST_1] =       //PB0 TIM3_CH3 备用 测试
	{
		.fDuty					= 7.5,				/* 初始占空比（%） */
		.ulFreq					= 50,				/* 频率（Hz） */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_0,		/* IO映射 */
		.tGPIO.tGPIOPort		= GPIOB,			/* IO组映射 */
	},
	[PWM_TEST_2] =       //PC9 TIM8_CH4 备用 测试
	{
		.fDuty					= 9,				/* 初始占空比（%） */
		.ulFreq					= 50,				/* 频率（Hz） */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_9,		/* IO映射 */
		.tGPIO.tGPIOPort		= GPIOC,			/* IO组映射 */
	},
	[PWM_TEST_3] =       //PC8 TIM8_CH3 备用 测试
	{
		.fDuty					= 7.5,				/* 初始占空比（%） */
		.ulFreq					= 50,				/* 频率（Hz） */
        .tGPIO.tGPIOInit.Pin	= GPIO_PIN_8,		/* IO映射 */
		.tGPIO.tGPIOPort		= GPIOC,			/* IO组映射 */
	},
};

CRC_HandleTypeDef hcrc;
void CRC_Init()
{
    __HAL_RCC_CRC_CLK_ENABLE();  // 使能 CRC 时钟
    hcrc.Instance = CRC;
    HAL_CRC_Init(&hcrc);
}


