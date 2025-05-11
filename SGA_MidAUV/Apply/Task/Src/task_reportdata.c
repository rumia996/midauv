#include "task_reportdata.h"
#include "config.h"
uint8_t ReportDataBuffer[FRAMEEND_BASE + 1 ]={0};	//�洢�ر��ַ���,+1��Ϊ���ַ���ĩβ��\0,����֡��70�ֽ�,�ַ����ܳ�70+1�ֽ�

void Task_ReportData_Handle(void)
{
	/*֡ͷ֡β*/
	ReportDataBuffer[FRAMESTART_BASE]='@';
	ReportDataBuffer[FRAMESTART_BASE + 1]='S';
	ReportDataBuffer[FRAMESTART_BASE + 2]='D';
	ReportDataBuffer[FRAMEEND_BASE]='$';
    /*����ַ���,��ŵ����������ļ���*/
	
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
	
	
	//��������&����ر�
    while(1)
    {
		//printf("test\r\n");
		/*�������������ڸ����߳��и���,�˴�Ӧִ��û�ж����̵߳����ݸ���*/
		PWM_update();
		CRC_update();
		for (int i = 0;i < sizeof(ReportDataBuffer); i++){//���ֽ����,�����ĩβ/0
			printf("%c",ReportDataBuffer[i]);
		}
#ifdef DEBUG_MODE 
		//printf("\r\n");
#endif
		//printf("%s\r\n",ReportDataBuffer);//��������00ʱ�ж����,����
		//printf("%d\r\n",FRAMEEND_BASE + 1);
        //Drv_Delay_Ms(2000);//��������2��
		Drv_Delay_Ms(2000);
    }
}


/*����CRC32(��̫��)���洢,�˺���Ӧ�����������ݸ��º�ִ��*/
void CRC_update()
{
	//ʹ��STM32Ӳ��CRCģ�����CRC32;
	//crc��ֵΪ0,ʹ�ö���ʽ0x04C11DB7,
	//ȡ֡ͷ@SD֮��,�Ӻ���ǵ�������ص�78�ֽ�����,ĩβ��2��0x00�ճ�80�ֽ�����
	const uint8_t crcdatalength = 80;//����crcУ��������ֽ���,32�н���У������ݱ�����uint32���� 1��uint32=4���ֽ� �������������4�ı���
	const uint8_t factcrcdatalength = 78;//ʵ��������������ֽ���
	uint8_t buffer[crcdatalength] = {0};  
	memcpy(buffer,&ReportDataBuffer[3],factcrcdatalength);// ���� ReportDataBuffer[3] ��ʼ�� 78 �ֽڵ� buffer ;��ʱĩβ2���ֽ��Ѿ���0
	
    // ���� CRC
    uint32_t crc_result = HAL_CRC_Calculate(&hcrc, (uint32_t *)buffer, crcdatalength / 4); // HAL�����ü��㺯����Ҫuint32_t ��bufferת��Ϊuin32 80�ֽ� = 20�� uint32_t
    memcpy(&ReportDataBuffer[MYCRC_BASE], &crc_result, UINT32_T_SIZE);	
	//ע��,CRC�������������Ҳ��С�������,����0xA9F20A86 �ô��ڵ������ִ�ӡ��86 0A F2 A9
}

/*��ȡ��ǰ���õ�PWM�ߵ�ƽʱ�䲢�洢*/
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
