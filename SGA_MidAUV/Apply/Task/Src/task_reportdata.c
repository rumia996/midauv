#include "task_reportdata.h"
#include "config.h"
#include "task_conf.h"
uint8_t ReportDataBuffer[FRAMEEND_BASE + 1 ]={0};	//�洢�ر��ַ���,+1��Ϊ���ַ���ĩβ��\0,����֡��86�ֽ�,�ַ����ܳ�86+1�ֽ�
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
	
	//��������&����ر�
    while(1)
    {
		/*�������������ڸ����߳��и���,�˴�Ӧִ��û�ж����̵߳����ݸ���*/
		PWM_update();
		CRC_update();
		//���ֽ����,�����ĩβ/0
		if (MODE != MANUAL_MODE)	//�ֱ�ģʽ�²�������,��ʱ�����ݮ�ɲ���ͬʱ�շ�������
		for (int i = 0;i < sizeof(ReportDataBuffer); i++){
			printf("%c",ReportDataBuffer[i]);
		}
#ifdef DEBUG_MODE 
		printf("\r\n");
		//�������
	
		printf("���:%.3f ����:%.3f ����:%.3f\r\n",*p_roll,*p_pitch,*p_yaw);
		printf("�¶�:%.3f ���:%.3f �߶�:%.3f\r\n",*p_temperature,*p_depth,*p_height);	
		printf("ǰ��X:%.2f ǰ��Y:%.2f ǰ��Z:%.2f\r\n",*p_gradientmagnet_front_x,*p_gradientmagnet_front_y,*p_gradientmagnet_front_z);
		printf("���X:%.2f ���Y:%.2f ���Z:%.2f\r\n",*p_gradientmagnet_left_x,*p_gradientmagnet_left_y,*p_gradientmagnet_left_z);
		printf("�Ҵ�X:%.2f �Ҵ�Y:%.2f �Ҵ�Z:%.2f\r\n",*p_gradientmagnet_right_x,*p_gradientmagnet_right_y,*p_gradientmagnet_right_z);
		//printf("������:%d ������:%d ����:%d ����:%d �Ҵ���:%d ����:%d �Ҷ��:%d\r\n",*p_pwm_lmt,*p_pwm_rmt,*p_pwm_bvt,*p_pwm_lvt,*p_pwm_rvt,*p_pwm_ls,*p_pwm_rs);
		printf("������:%d ������:%d ����:%d ����:%d �Ҵ���:%d ����:%d �Ҷ��:%d\r\n",Task_MotorSys_GetThrusterSpeed(LMT),Task_MotorSys_GetThrusterSpeed(RMT),Task_MotorSys_GetThrusterSpeed(BVT),Task_MotorSys_GetThrusterSpeed(LVT),Task_MotorSys_GetThrusterSpeed(RVT),(int8_t)round(Task_MotorSys_GetRudder_Angle(LS)),(int8_t)round(Task_MotorSys_GetRudder_Angle(RS)));
		printf("���ڵ��:%d ������:%d\r\n",*p_battery_control,*p_battery_power);
#endif
		//printf("%s\r\n",ReportDataBuffer);//ֱ������ַ�����������00ʱ�ж����,����	
		Drv_Delay_Ms(2000);//��������2��
    }
}

/*��ղ���ʼ��buffer*/
void ReportDataBuffer_Reset()
{
	/*�������*/
	memset(ReportDataBuffer,0,sizeof(ReportDataBuffer));
	/*֡ͷ֡β*/
	ReportDataBuffer[FRAMESTART_BASE]='@';
	ReportDataBuffer[FRAMESTART_BASE + 1]='S';
	ReportDataBuffer[FRAMESTART_BASE + 2]='D';
	ReportDataBuffer[FRAMEEND_BASE]='$';
	//��ص���δʵװ
	uint16_t battery_control = 12;
	uint16_t battery_power = 48;
	memcpy(&ReportDataBuffer[BATTARY_BASE					],&battery_control,UINT16_T_SIZE);
	memcpy(&ReportDataBuffer[BATTARY_BASE + UINT16_T_SIZE	],&battery_power,UINT16_T_SIZE);
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
