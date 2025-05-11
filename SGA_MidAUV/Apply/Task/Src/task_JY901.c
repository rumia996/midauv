#include "task_JY901.h"
float Roll = 0;	//横滚
float Pitch = 0;	//俯仰
float Yaw = 0;	//航向
/**
 * @brief JY901处理函数
 * @param Null
 */
void Task_JY901_Handle(void)
{
	if(rt_sem_take(JY901S_Sem,RT_WAITING_FOREVER) == RT_EOK)//监听串口2
	{
		if(OCD_JY901_DataProcess(&JY901S))
		{
			OCD_JY901_DataConversion(&JY901S);	//数据转换
			//OCD_JY901_Printf(&JY901S); 			//数据打印
			//printf("\r\n");
#ifdef DEBUG_MODE
//				//打印欧拉角
//	            if(JY901S.tConfig.usType & JY901_OUTPUT_ANGLE)	    
//	                printf("J Angle %.3f %.3f %.3f\r\n",
//	                       JY901S.stcAngle.ConRoll,
//	                       JY901S.stcAngle.ConPitch,
//	                       JY901S.stcAngle.ConYaw);

//            //打印磁场值
//            if(JY901S.tConfig.usType & JY901_OUTPUT_MAG)	    
//                printf("J Mag %.3f %.3f %.3f\r\n\r\n",
//                        JY901S.stcMag.ConMagX,
//                        JY901S.stcMag.ConMagY,
//                        JY901S.stcMag.ConMagZ);  

//            //打印加速度
//            if(JY901S.tConfig.usType & JY901_OUTPUT_ACCEL)	    
//                printf("J Accel %.3f %.3f %.3f\r\n\r\n",
//                        JY901S.stcAcc.ConAccX,
//                        JY901S.stcAcc.ConAccY,
//                        JY901S.stcAcc.ConAccZ);

//            //打印角速度
//            if(JY901S.tConfig.usType & JY901_OUTPUT_GYRO)	    
//                printf("J Gyro %.3f %.3f %.3f\r\n\r\n",
//                        JY901S.stcGyro.ConGyroX,
//                        JY901S.stcGyro.ConGyroY,
//                        JY901S.stcGyro.ConGyroZ);   
#endif
			//将数据放入报告数据缓存区
			/*经测试,IM板Y轴与AUV中轴对齐的安装方式下,实际的滚转角Roll是原始数据的俯仰角Pitch;实际的俯仰角Pitch是原始数据的滚转角Roll,这里对数据位置作了调整*/
			Roll = JY901S.stcAngle.ConPitch;
			Pitch = JY901S.stcAngle.ConRoll;
			Yaw = normalize_angle(JY901S.stcAngle.ConYaw + 17);
			memcpy(&ReportDataBuffer[RPY_ATTITUED_BASE 					],&Roll,FLOAT_SIZE);//滚转角Roll
			memcpy(&ReportDataBuffer[RPY_ATTITUED_BASE + 1 * FLOAT_SIZE	],&Pitch,FLOAT_SIZE);	//俯仰角Pitch
			memcpy(&ReportDataBuffer[RPY_ATTITUED_BASE + 2 * FLOAT_SIZE	],&Yaw,FLOAT_SIZE); 	//航向角Yaw		
		}
       //如果数据获得并且处理完成
	}
}       

// 角度归一化函数，确保角度在 [-180, 180] 范围内
float normalize_angle(float angle) 
{
    while (angle > 180) angle -= 360;
    while (angle <= -180) angle += 360;
    return angle;
}

// 角度转换函数 
float transform_angle(float target_yaw) 
{
    return normalize_angle(target_yaw - 90);
}
