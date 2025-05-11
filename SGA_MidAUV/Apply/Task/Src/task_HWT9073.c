//#include "task_HWT9073.h"

//uint8_t HWT9073_ReceBuf[100];	//存储上位机命令字符串,无符号字符类型数组
//uint8_t HWT9073_ReceNum = 0;	//命令字符串长度
////float Roll = 0;	//横滚
////float Pitch = 0;	//俯仰
////float Yaw = 0;	//航向
//const uint8_t SendBuffer[] = {0x50, 0x03, 0x00, 0x3D, 0x00, 0x06, 0x59, 0x85};//查询角度请求

//void Task_HWT9073_send_Handle(void)
//{
//	//每1秒发送查询角度请求
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
//	//接收
//	if(rt_sem_take(MessageFromHWT9073_Sem,RT_WAITING_FOREVER) == RT_EOK)//监听串口4
//	{
//		HWT9073_ReceNum = Drv_Uart_Receive_DMA(&Uart4,HWT9073_ReceBuf);//若收到信息,通过DMA接收字符串并存储,并记录字符串长度
//		if(HWT9073_ReceNum != 0)
//		{			
////			#ifdef DEBUG_MODE
////			for (int i=0;i<HWT9073_ReceNum;i++)
////			{
////				printf("%c",HWT9073_ReceBuf[i]);
////			}			
////			#endif
//			//不管CRC校验位;
//			//实际的顺序是Pitch Roll Yaw;
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
//				//先换算成int32_t
//				int32_t Roll_D = ((HRollH)<<24|(HRollL)<<16|(LRollH)<<8|(LRollL));
//				int32_t Pitch_D = ((HPitchH)<<24|(HPitchL)<<16|(LPitchH)<<8|(LPitchL));
//				int32_t Yaw_D = ((HYawH)<<24|(HYawL)<<16|(LYawH)<<8|(LYawL));
//				//再换算成float,得到原始角度值
//				float Roll_F = Roll_D / 1000.0;
//				float Pitch_F = Pitch_D / 1000.0;
//				float Yaw_F = Yaw_D / 1000.0;
//				//依据2.18协议,直接返回原始数据
//				Roll = Roll_F;
//				Pitch = Pitch_F;
//				Yaw = Yaw_F;
//				
//				//协议需要uint_16的R P Y,但没必要用无符号类型!用int16_t即可表示-32768~32767,满足需求,显然更方便;修改协议;
////				//只保存整数部分
////				int16_t Roll_Old = round(Roll_F);
////				int16_t Pitch_Old = round(Pitch_F);
////				int16_t Yaw_Old = round(Yaw_F);
////				//原始数据中,航向角Yaw 0表示正北,90为正西,-90为正东,正南有-180到180的跳变
////				//按协议要求,0度表示正东,90为正北,-90为正南,正西为-180/180 需要对Yaw进行转换
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

//// 角度归一化函数，确保角度在 [-180, 180] 范围内
//float normalize_angle(float angle) 
//{
//    while (angle > 180) angle -= 360;
//    while (angle <= -180) angle += 360;
//    return angle;
//}

//// 角度转换函数 
//float transform_angle(float old_angle) 
//{
//    return normalize_angle(90 - old_angle);
//}


