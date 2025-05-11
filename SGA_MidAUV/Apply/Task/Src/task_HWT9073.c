//#include "task_HWT9073.h"

//uint8_t HWT9073_ReceBuf[100];	//�洢��λ�������ַ���,�޷����ַ���������
//uint8_t HWT9073_ReceNum = 0;	//�����ַ�������
////float Roll = 0;	//���
////float Pitch = 0;	//����
////float Yaw = 0;	//����
//const uint8_t SendBuffer[] = {0x50, 0x03, 0x00, 0x3D, 0x00, 0x06, 0x59, 0x85};//��ѯ�Ƕ�����

//void Task_HWT9073_send_Handle(void)
//{
//	//ÿ1�뷢�Ͳ�ѯ�Ƕ�����
//	//uint8_t SendBuffer[30] = {0};
//	//uint8_t data[] = {0x50, 0x03, 0x00, 0x3D, 0x00, 0x06, 0x59, 0x85};
//	//memcpy(SendBuffer, data, sizeof(data));
//	//Drv_Uart_Transmit_DMA(&Uart4,data,sizeof(data));
//	//Drv_Uart_Transmit(&Uart4,data,sizeof(data));
//	
//	RS485_Send_On();
//	Drv_Uart_Transmit(&Uart4,(uint8_t *)SendBuffer,sizeof(SendBuffer));
//	//Drv_Delay_Ms(10);
//	RS485_Receive_On();
//	Drv_Delay_Ms(2000);
//	
//}

//void Task_HWT9073_Handle()
//{	
//	//����
//	if(rt_sem_take(MessageFromHWT9073_Sem,RT_WAITING_FOREVER) == RT_EOK)//��������4
//	{
//		HWT9073_ReceNum = Drv_Uart_Receive_DMA(&Uart4,HWT9073_ReceBuf);//���յ���Ϣ,ͨ��DMA�����ַ������洢,����¼�ַ�������
//		if(HWT9073_ReceNum != 0)
//		{			
////			#ifdef DEBUG_MODE
////			for (int i=0;i<HWT9073_ReceNum;i++)
////			{
////				printf("%c",HWT9073_ReceBuf[i]);
////			}			
////			#endif
//			//����CRCУ��λ;
//			//ʵ�ʵ�˳����Pitch Roll Yaw;
//			if (HWT9073_ReceNum == 17 && HWT9073_ReceBuf[0] == 0x50 && HWT9073_ReceBuf[1] == 0x03 && HWT9073_ReceBuf[2] == 0x0C)
//			{
//				uint8_t LPitchH = HWT9073_ReceBuf[3];
//				uint8_t LPitchL = HWT9073_ReceBuf[4];
//				uint8_t HPitchH = HWT9073_ReceBuf[5];
//				uint8_t HPitchL = HWT9073_ReceBuf[6];
//				uint8_t LRollH = HWT9073_ReceBuf[7];
//				uint8_t LRollL = HWT9073_ReceBuf[8];
//				uint8_t HRollH = HWT9073_ReceBuf[9];
//				uint8_t HRollL = HWT9073_ReceBuf[10];
//				uint8_t LYawH = HWT9073_ReceBuf[11];
//				uint8_t LYawL = HWT9073_ReceBuf[12];
//				uint8_t HYawH = HWT9073_ReceBuf[13];
//				uint8_t HYawL = HWT9073_ReceBuf[14];
//				//�Ȼ����int32_t
//				int32_t Roll_D = ((HRollH)<<24|(HRollL)<<16|(LRollH)<<8|(LRollL));
//				int32_t Pitch_D = ((HPitchH)<<24|(HPitchL)<<16|(LPitchH)<<8|(LPitchL));
//				int32_t Yaw_D = ((HYawH)<<24|(HYawL)<<16|(LYawH)<<8|(LYawL));
//				//�ٻ����float,�õ�ԭʼ�Ƕ�ֵ
//				float Roll_F = Roll_D / 1000.0;
//				float Pitch_F = Pitch_D / 1000.0;
//				float Yaw_F = Yaw_D / 1000.0;
//				//����2.18Э��,ֱ�ӷ���ԭʼ����
//				Roll = Roll_F;
//				Pitch = Pitch_F;
//				Yaw = Yaw_F;
//				
//				//Э����Ҫuint_16��R P Y,��û��Ҫ���޷�������!��int16_t���ɱ�ʾ-32768~32767,��������,��Ȼ������;�޸�Э��;
////				//ֻ������������
////				int16_t Roll_Old = round(Roll_F);
////				int16_t Pitch_Old = round(Pitch_F);
////				int16_t Yaw_Old = round(Yaw_F);
////				//ԭʼ������,�����Yaw 0��ʾ����,90Ϊ����,-90Ϊ����,������-180��180������
////				//��Э��Ҫ��,0�ȱ�ʾ����,90Ϊ����,-90Ϊ����,����Ϊ-180/180 ��Ҫ��Yaw����ת��
////				Roll = Roll_Old;
////				Pitch = Pitch_Old;
////				Yaw = transform_angle(Yaw_Old);
//			}

//			
//			#ifdef DEBUG_MODE
//			printf("\r\nRoll:%f,Pitch:%f,Yaw:%f\r\n",Roll,Pitch,Yaw);
//			#endif
//			
//			memcpy(&ReportDataBuffer[RPY_ATTITUED_BASE 					],&Roll,FLOAT_SIZE);
//			memcpy(&ReportDataBuffer[RPY_ATTITUED_BASE + 1 * FLOAT_SIZE	],&Pitch,FLOAT_SIZE);
//			memcpy(&ReportDataBuffer[RPY_ATTITUED_BASE + 2 * FLOAT_SIZE	],&Yaw,FLOAT_SIZE);						
//		}
//		HWT9073_ReceNum = 0;
//	}

//}

//// �Ƕȹ�һ��������ȷ���Ƕ��� [-180, 180] ��Χ��
//float normalize_angle(float angle) 
//{
//    while (angle > 180) angle -= 360;
//    while (angle <= -180) angle += 360;
//    return angle;
//}

//// �Ƕ�ת������ 
//float transform_angle(float old_angle) 
//{
//    return normalize_angle(90 - old_angle);
//}


