#include "task_JY901.h"
float Roll = 0;	//���
float Pitch = 0;	//����
float Yaw = 0;	//����
/**
 * @brief JY901������
 * @param Null
 */
void Task_JY901_Handle(void)
{
	if(rt_sem_take(JY901S_Sem,RT_WAITING_FOREVER) == RT_EOK)//��������2
	{
		if(OCD_JY901_DataProcess(&JY901S))
		{
			OCD_JY901_DataConversion(&JY901S);	//����ת��
			//OCD_JY901_Printf(&JY901S); 			//���ݴ�ӡ
			//printf("\r\n");
#ifdef DEBUG_MODE
//				//��ӡŷ����
//	            if(JY901S.tConfig.usType & JY901_OUTPUT_ANGLE)	    
//	                printf("J Angle %.3f %.3f %.3f\r\n",
//	                       JY901S.stcAngle.ConRoll,
//	                       JY901S.stcAngle.ConPitch,
//	                       JY901S.stcAngle.ConYaw);

//            //��ӡ�ų�ֵ
//            if(JY901S.tConfig.usType & JY901_OUTPUT_MAG)	    
//                printf("J Mag %.3f %.3f %.3f\r\n\r\n",
//                        JY901S.stcMag.ConMagX,
//                        JY901S.stcMag.ConMagY,
//                        JY901S.stcMag.ConMagZ);  

//            //��ӡ���ٶ�
//            if(JY901S.tConfig.usType & JY901_OUTPUT_ACCEL)	    
//                printf("J Accel %.3f %.3f %.3f\r\n\r\n",
//                        JY901S.stcAcc.ConAccX,
//                        JY901S.stcAcc.ConAccY,
//                        JY901S.stcAcc.ConAccZ);

//            //��ӡ���ٶ�
//            if(JY901S.tConfig.usType & JY901_OUTPUT_GYRO)	    
//                printf("J Gyro %.3f %.3f %.3f\r\n\r\n",
//                        JY901S.stcGyro.ConGyroX,
//                        JY901S.stcGyro.ConGyroY,
//                        JY901S.stcGyro.ConGyroZ);   
#endif
			//�����ݷ��뱨�����ݻ�����
			/*������,IM��Y����AUV�������İ�װ��ʽ��,ʵ�ʵĹ�ת��Roll��ԭʼ���ݵĸ�����Pitch;ʵ�ʵĸ�����Pitch��ԭʼ���ݵĹ�ת��Roll,���������λ�����˵���*/
			Roll = JY901S.stcAngle.ConPitch;
			Pitch = JY901S.stcAngle.ConRoll;
			Yaw = normalize_angle(JY901S.stcAngle.ConYaw + 17);
			memcpy(&ReportDataBuffer[RPY_ATTITUED_BASE 					],&Roll,FLOAT_SIZE);//��ת��Roll
			memcpy(&ReportDataBuffer[RPY_ATTITUED_BASE + 1 * FLOAT_SIZE	],&Pitch,FLOAT_SIZE);	//������Pitch
			memcpy(&ReportDataBuffer[RPY_ATTITUED_BASE + 2 * FLOAT_SIZE	],&Yaw,FLOAT_SIZE); 	//�����Yaw		
		}
       //������ݻ�ò��Ҵ������
	}
}       

// �Ƕȹ�һ��������ȷ���Ƕ��� [-180, 180] ��Χ��
float normalize_angle(float angle) 
{
    while (angle > 180) angle -= 360;
    while (angle <= -180) angle += 360;
    return angle;
}

// �Ƕ�ת������ 
float transform_angle(float target_yaw) 
{
    return normalize_angle(target_yaw - 90);
}
