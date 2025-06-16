#include "task_reportdata.h"
#include "config.h"
#include "task_conf.h"
uint8_t ReportDataBuffer[FRAMEEND_BASE + 1 ]={0};	//存储回报字符串,+1是为了字符串末尾的\0,数据帧长86字节,字符串总长86+1字节
float* p_roll = &ReportDataBuffer[RPY_ATTITUED_BASE					 ];
float* p_pitch = &ReportDataBuffer[RPY_ATTITUED_BASE + 1 * FLOAT_SIZE];
float* p_yaw = &ReportDataBuffer[RPY_ATTITUED_BASE + 2 * FLOAT_SIZE];
float* p_temperature = &ReportDataBuffer[TEMPERATURE_BASE];
float* p_depth = &ReportDataBuffer[DEPTH_BASE];
float* p_height = &ReportDataBuffer[HEIGHT_BASE];
float* p_gradientmagnet_front_x = &ReportDataBuffer[GRADIENTMAGNET_FRONT_BASE				  ];
float* p_gradientmagnet_front_y = &ReportDataBuffer[GRADIENTMAGNET_FRONT_BASE + 1 * FLOAT_SIZE];
float* p_gradientmagnet_front_z = &ReportDataBuffer[GRADIENTMAGNET_FRONT_BASE + 2 * FLOAT_SIZE];
float* p_gradientmagnet_left_x = &ReportDataBuffer[GRADIENTMAGNET_LEFT_BASE					];
float* p_gradientmagnet_left_y = &ReportDataBuffer[GRADIENTMAGNET_LEFT_BASE + 1 * FLOAT_SIZE];
float* p_gradientmagnet_left_z = &ReportDataBuffer[GRADIENTMAGNET_LEFT_BASE + 2 * FLOAT_SIZE];
float* p_gradientmagnet_right_x = &ReportDataBuffer[GRADIENTMAGNET_RIGHT_BASE				  ];
float* p_gradientmagnet_right_y = &ReportDataBuffer[GRADIENTMAGNET_RIGHT_BASE + 1 * FLOAT_SIZE];
float* p_gradientmagnet_right_z = &ReportDataBuffer[GRADIENTMAGNET_RIGHT_BASE + 2 * FLOAT_SIZE];
uint16_t* p_pwm_lmt = &ReportDataBuffer[PWM_BASE 						];
uint16_t* p_pwm_rmt = &ReportDataBuffer[PWM_BASE + 1 * UINT16_T_SIZE	];
uint16_t* p_pwm_bvt = &ReportDataBuffer[PWM_BASE + 2 * UINT16_T_SIZE	];
uint16_t* p_pwm_lvt = &ReportDataBuffer[PWM_BASE + 3 * UINT16_T_SIZE	];
uint16_t* p_pwm_rvt = &ReportDataBuffer[PWM_BASE + 4 * UINT16_T_SIZE	];
uint16_t* p_pwm_ls = &ReportDataBuffer[PWM_BASE + 5 * UINT16_T_SIZE	];
uint16_t* p_pwm_rs = &ReportDataBuffer[PWM_BASE + 6 * UINT16_T_SIZE	];
uint16_t* p_battery_control = &ReportDataBuffer[BATTARY_BASE				];
uint16_t* p_battery_power = &ReportDataBuffer[BATTARY_BASE + UINT16_T_SIZE	];

void Task_ReportData_Handle(void)
{
	ReportDataBuffer_Reset();
	
	//更新数据&输出回报
    while(1)
    {
		/*各传感器数据在各自线程中更新,此处应执行没有独立线程的数据更新*/
		PWM_update();
		CRC_update();
		//逐字节输出,不输出末尾/0
		if (MODE != MANUAL_MODE)	//手柄模式下不发报文,临时解决树莓派不能同时收发的问题
		for (int i = 0;i < sizeof(ReportDataBuffer); i++){
			printf("%c",ReportDataBuffer[i]);
		}
#ifdef DEBUG_MODE 
		printf("\r\n");
		//明文输出
	
		printf("横滚:%.3f 俯仰:%.3f 航向:%.3f\r\n",*p_roll,*p_pitch,*p_yaw);
		printf("温度:%.3f 深度:%.3f 高度:%.3f\r\n",*p_temperature,*p_depth,*p_height);	
		printf("前磁X:%.2f 前磁Y:%.2f 前磁Z:%.2f\r\n",*p_gradientmagnet_front_x,*p_gradientmagnet_front_y,*p_gradientmagnet_front_z);
		printf("左磁X:%.2f 左磁Y:%.2f 左磁Z:%.2f\r\n",*p_gradientmagnet_left_x,*p_gradientmagnet_left_y,*p_gradientmagnet_left_z);
		printf("右磁X:%.2f 右磁Y:%.2f 右磁Z:%.2f\r\n",*p_gradientmagnet_right_x,*p_gradientmagnet_right_y,*p_gradientmagnet_right_z);
		//printf("左主推:%d 右主推:%d 后垂推:%d 左垂推:%d 右垂推:%d 左舵机:%d 右舵机:%d\r\n",*p_pwm_lmt,*p_pwm_rmt,*p_pwm_bvt,*p_pwm_lvt,*p_pwm_rvt,*p_pwm_ls,*p_pwm_rs);
		printf("左主推:%d 右主推:%d 后垂推:%d 左垂推:%d 右垂推:%d 左舵角:%d 右舵角:%d\r\n",Task_MotorSys_GetThrusterSpeed(LMT),Task_MotorSys_GetThrusterSpeed(RMT),Task_MotorSys_GetThrusterSpeed(BVT),Task_MotorSys_GetThrusterSpeed(LVT),Task_MotorSys_GetThrusterSpeed(RVT),(int8_t)round(Task_MotorSys_GetRudder_Angle(LS)),(int8_t)round(Task_MotorSys_GetRudder_Angle(RS)));
		printf("舱内电池:%d 舱外电池:%d\r\n",*p_battery_control,*p_battery_power);
#endif
		//printf("%s\r\n",ReportDataBuffer);//直接输出字符串会在碰到00时中断输出,别用	
		Drv_Delay_Ms(2000);//发送周期2秒
    }
}

/*清空并初始化buffer*/
void ReportDataBuffer_Reset()
{
	/*清空数据*/
	memset(ReportDataBuffer,0,sizeof(ReportDataBuffer));
	/*帧头帧尾*/
	ReportDataBuffer[FRAMESTART_BASE]='@';
	ReportDataBuffer[FRAMESTART_BASE + 1]='S';
	ReportDataBuffer[FRAMESTART_BASE + 2]='D';
	ReportDataBuffer[FRAMEEND_BASE]='$';
	//电池电量未实装
	uint16_t battery_control = 12;
	uint16_t battery_power = 48;
	memcpy(&ReportDataBuffer[BATTARY_BASE					],&battery_control,UINT16_T_SIZE);
	memcpy(&ReportDataBuffer[BATTARY_BASE + UINT16_T_SIZE	],&battery_power,UINT16_T_SIZE);
}

/*计算CRC32(以太网)并存储,此函数应当在所有数据更新后执行*/
void CRC_update()
{
	//使用STM32硬件CRC模块计算CRC32;
	//crc初值为0,使用多项式0x04C11DB7,
	//取帧头@SD之后,从横滚角到动力电池的78字节数据,末尾补2个0x00凑成80字节数据
	const uint8_t crcdatalength = 80;//参与crc校验的数据字节数,32中进行校验的数据必须与uint32对齐 1个uint32=4个字节 所以这里必须是4的倍数
	const uint8_t factcrcdatalength = 78;//实际有意义的数据字节数
	uint8_t buffer[crcdatalength] = {0};  
	memcpy(buffer,&ReportDataBuffer[3],factcrcdatalength);// 复制 ReportDataBuffer[3] 开始的 78 字节到 buffer ;此时末尾2个字节已经是0
	
    // 计算 CRC
    uint32_t crc_result = HAL_CRC_Calculate(&hcrc, (uint32_t *)buffer, crcdatalength / 4); // HAL库内置计算函数需要uint32_t 将buffer转换为uin32 80字节 = 20个 uint32_t
    memcpy(&ReportDataBuffer[MYCRC_BASE], &crc_result, UINT32_T_SIZE);	
	//注意,CRC在输出数组中是也是小端排序的,例如0xA9F20A86 用串口调试助手打印是86 0A F2 A9
}

/*获取当前设置的PWM高电平时间并存储*/
void PWM_update()
{
	uint16_t pwm_lmt = Drv_PWM_ReadHighLvTime(&PWM[LMT]);
	uint16_t pwm_rmt = Drv_PWM_ReadHighLvTime(&PWM[RMT]);
	uint16_t pwm_bvt = Drv_PWM_ReadHighLvTime(&PWM[BVT]);
	uint16_t pwm_lvt = Drv_PWM_ReadHighLvTime(&PWM[LVT]);
	uint16_t pwm_rvt = Drv_PWM_ReadHighLvTime(&PWM[RVT]);
	uint16_t pwm_ls = Drv_PWM_ReadHighLvTime(&PWM[LS]);
	uint16_t pwm_rs = Drv_PWM_ReadHighLvTime(&PWM[RS]);
	memcpy(&ReportDataBuffer[PWM_BASE						],&pwm_lmt,UINT16_T_SIZE);
	memcpy(&ReportDataBuffer[PWM_BASE + 1 * UINT16_T_SIZE	],&pwm_rmt,UINT16_T_SIZE);
	memcpy(&ReportDataBuffer[PWM_BASE + 2 * UINT16_T_SIZE	],&pwm_bvt,UINT16_T_SIZE);
	memcpy(&ReportDataBuffer[PWM_BASE + 3 * UINT16_T_SIZE	],&pwm_lvt,UINT16_T_SIZE);
	memcpy(&ReportDataBuffer[PWM_BASE + 4 * UINT16_T_SIZE	],&pwm_rvt,UINT16_T_SIZE);
	memcpy(&ReportDataBuffer[PWM_BASE + 5 * UINT16_T_SIZE	],&pwm_ls,UINT16_T_SIZE);
	memcpy(&ReportDataBuffer[PWM_BASE + 6 * UINT16_T_SIZE	],&pwm_rs,UINT16_T_SIZE);
}
