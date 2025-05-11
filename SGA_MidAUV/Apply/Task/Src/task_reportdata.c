#include "task_reportdata.h"
#include "config.h"
uint8_t ReportDataBuffer[FRAMEEND_BASE + 1 ]={0};	//存储回报字符串,+1是为了字符串末尾的\0,数据帧长70字节,字符串总长70+1字节

void Task_ReportData_Handle(void)
{
	/*帧头帧尾*/
	ReportDataBuffer[FRAMESTART_BASE]='@';
	ReportDataBuffer[FRAMESTART_BASE + 1]='S';
	ReportDataBuffer[FRAMESTART_BASE + 2]='D';
	ReportDataBuffer[FRAMEEND_BASE]='$';
    /*填充字符串,需放到各个驱动文件中*/
	
//	uint16_t Yaw = 123;
//	uint16_t Pitch = 456;
//	uint16_t Roll = 789;
//	memcpy(&ReportDataBuffer[RPY_ATTITUED_BASE 						],&Yaw,UINT16_T_SIZE);
//	memcpy(&ReportDataBuffer[RPY_ATTITUED_BASE + 1 * UINT16_T_SIZE	],&Pitch,UINT16_T_SIZE);
//	memcpy(&ReportDataBuffer[RPY_ATTITUED_BASE + 2 * UINT16_T_SIZE	],&Roll,UINT16_T_SIZE);
	
	
//	uint16_t Temperature= 123;
//	uint16_t Depth = 456;
//	uint16_t Height = 789;
//	memcpy(&ReportDataBuffer[TEMPERATURE_BASE],&Temperature,UINT16_T_SIZE);
//	memcpy(&ReportDataBuffer[DEPTH_BASE],&Depth,UINT16_T_SIZE);
//	memcpy(&ReportDataBuffer[HEIGHT_BASE],&Height,UINT16_T_SIZE);
	
	float frontmagnet_x = 1234.5678;
	float frontmagnet_y = 1234.5678;
	float frontmagnet_z = 1234.5678;
	memcpy(&ReportDataBuffer[FRONT_MAGNET_XYZ_BASE					],&frontmagnet_x,FLOAT_SIZE);
	memcpy(&ReportDataBuffer[FRONT_MAGNET_XYZ_BASE + 1 * FLOAT_SIZE	],&frontmagnet_y,FLOAT_SIZE);
	memcpy(&ReportDataBuffer[FRONT_MAGNET_XYZ_BASE + 2 * FLOAT_SIZE	],&frontmagnet_z,FLOAT_SIZE);

	float gradientmagnet_left_x = 1234.5678;
	float gradientmagnet_left_y = 1234.5678;
	float gradientmagnet_left_z = 1234.5678;
	memcpy(&ReportDataBuffer[GRADIENTMAGNET_LEFT_BASE 					],&gradientmagnet_left_x,FLOAT_SIZE);
	memcpy(&ReportDataBuffer[GRADIENTMAGNET_LEFT_BASE + 1 * FLOAT_SIZE	],&gradientmagnet_left_y,FLOAT_SIZE);
	memcpy(&ReportDataBuffer[GRADIENTMAGNET_LEFT_BASE + 2 * FLOAT_SIZE	],&gradientmagnet_left_z,FLOAT_SIZE);
	
	float gradientmagnet_right_x = 1234.5678;
	float gradientmagnet_right_y = 1234.5678;
	float gradientmagnet_right_z = 1234.5678;
	memcpy(&ReportDataBuffer[GRADIENTMAGNET_RIGHT_BASE 					],&gradientmagnet_right_x,FLOAT_SIZE);
	memcpy(&ReportDataBuffer[GRADIENTMAGNET_RIGHT_BASE + 1 * FLOAT_SIZE	],&gradientmagnet_right_y,FLOAT_SIZE);
	memcpy(&ReportDataBuffer[GRADIENTMAGNET_RIGHT_BASE + 2 * FLOAT_SIZE	],&gradientmagnet_right_z,FLOAT_SIZE);
	
	uint16_t battery_control = 12;
	uint16_t battery_power = 48;
	memcpy(&ReportDataBuffer[BATTARY_BASE					],&battery_control,UINT16_T_SIZE);
	memcpy(&ReportDataBuffer[BATTARY_BASE + UINT16_T_SIZE	],&battery_power,UINT16_T_SIZE);
	
	
	//更新数据&输出回报
    while(1)
    {
		//printf("test\r\n");
		/*各传感器数据在各自线程中更新,此处应执行没有独立线程的数据更新*/
		PWM_update();
		CRC_update();
		for (int i = 0;i < sizeof(ReportDataBuffer); i++){//逐字节输出,不输出末尾/0
			printf("%c",ReportDataBuffer[i]);
		}
#ifdef DEBUG_MODE 
		//printf("\r\n");
#endif
		//printf("%s\r\n",ReportDataBuffer);//会在碰到00时中断输出,别用
		//printf("%d\r\n",FRAMEEND_BASE + 1);
        //Drv_Delay_Ms(2000);//发送周期2秒
		Drv_Delay_Ms(2000);
    }
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
