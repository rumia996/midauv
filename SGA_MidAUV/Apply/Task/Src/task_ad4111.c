#include "task_ad4111.h"

/**
 * @brief AD4111数据采集函数
 * @param Null
 */
void AD_Acquisition(tagAD24BIT_T* device)
{
    uint8_t ret;

	ret = OCD_AD24bit_GetResult(device, 200);
	if(ret != 0)
	{
	    DBG_ERROR("OCD_AD24bit_GetResult error:%d\r\n",ret);
	}

//#define DEBUG_PRINTF	
#ifdef DEBUG_PRINTF
    double V0,V1,V2,V3,V4,V5,V6,V7; 
	V0 = device->port[0].result * 10.0f;
	V1 = device->port[1].result * 10.0f;
	V2 = device->port[2].result * 10.0f;
	V3 = device->port[3].result * 10.0f;
	V4 = device->port[4].result * 10.0f;
	V5 = device->port[5].result * 10.0f;
    V6 = device->port[6].result * 10.0f;
    V7 = device->port[7].result * 10.0f;
	
	//原始数据单位为V
	//磁力仪输出+-10V,对应量程+-100nT,将原始数据乘10即为磁感应强度
	printf("V0 %f V1 %f V4 %f\r\n",V0,V1,V4);
	printf("V5 %f V6 %f V7 %f\r\n",V5,V6,V7);	
    //printf("V0 %f V1 %f V2 %f\r\n",V0,V1,V2);
    //printf("V3 %f V4 %f V5 %f\r\n",V3,V4,V5);
    //printf("V6 %f V7 %f\r\n\r\n",V6,V7); 
#endif
}

/**
 * @brief AD4111同步函数
 * @param Null
 */
void Task_AD4111_Sync(void)
{
    Drv_GPIO_Reset(&SYNC);
    Drv_Delay_Ms(500);
	Drv_GPIO_Set(&SYNC);
}

/**
 * @brief AD4111处理函数
 * @param Null
 */
void Task_AD4111_Handle(void)
{
	float gradientmagnet_front_x = 0.0f;
	float gradientmagnet_front_y = 0.0f;
	float gradientmagnet_front_z = 0.0f;
	float gradientmagnet_left_x = 0.0f;
	float gradientmagnet_left_y = 0.0f;
	float gradientmagnet_left_z = 0.0f;
	float gradientmagnet_right_x = 0.0f;
	float gradientmagnet_right_y = 0.0f;
	float gradientmagnet_right_z = 0.0f;
	
    AD_Acquisition(&AD4111_1);
     AD_Acquisition(&AD4111_2);

	
    //将数据放入报告数据缓存区
	//原始数据单位为V
	//磁力仪输出+-10V,对应量程+-100nT,将原始数据乘10即为磁感应强度
	//前置磁力仪	XYZ
	gradientmagnet_front_x = (float)AD4111_2.port[0].result * 10;
	gradientmagnet_front_y = (float)AD4111_2.port[1].result * 10;
	gradientmagnet_front_z = (float)AD4111_2.port[4].result * 10;
	memcpy(&ReportDataBuffer[GRADIENTMAGNET_FRONT_BASE					],&gradientmagnet_front_x,FLOAT_SIZE);
	memcpy(&ReportDataBuffer[GRADIENTMAGNET_FRONT_BASE + 1 * FLOAT_SIZE	],&gradientmagnet_front_y,FLOAT_SIZE);
	memcpy(&ReportDataBuffer[GRADIENTMAGNET_FRONT_BASE + 2 * FLOAT_SIZE	],&gradientmagnet_front_z,FLOAT_SIZE);
	
	//左磁力仪	XYZ
	gradientmagnet_left_x = (float)AD4111_2.port[5].result * 10;
	gradientmagnet_left_y = (float)AD4111_2.port[6].result * 10;
	gradientmagnet_left_z = (float)AD4111_2.port[7].result * 10;
	memcpy(&ReportDataBuffer[GRADIENTMAGNET_LEFT_BASE 					],&gradientmagnet_left_x,FLOAT_SIZE);
	memcpy(&ReportDataBuffer[GRADIENTMAGNET_LEFT_BASE + 1 * FLOAT_SIZE	],&gradientmagnet_left_y,FLOAT_SIZE);
	memcpy(&ReportDataBuffer[GRADIENTMAGNET_LEFT_BASE + 2 * FLOAT_SIZE	],&gradientmagnet_left_z,FLOAT_SIZE);
	
	//右磁力仪	XYZ
	gradientmagnet_right_x = (float)AD4111_1.port[0].result * 10;
	gradientmagnet_right_y = (float)AD4111_1.port[1].result * 10;
	gradientmagnet_right_z = (float)AD4111_1.port[4].result * 10;
	memcpy(&ReportDataBuffer[GRADIENTMAGNET_RIGHT_BASE 					],&gradientmagnet_right_x,FLOAT_SIZE);
	memcpy(&ReportDataBuffer[GRADIENTMAGNET_RIGHT_BASE + 1 * FLOAT_SIZE	],&gradientmagnet_right_y,FLOAT_SIZE);
	memcpy(&ReportDataBuffer[GRADIENTMAGNET_RIGHT_BASE + 2 * FLOAT_SIZE	],&gradientmagnet_right_z,FLOAT_SIZE);
	
	#define DEBUG_PRINTF
	#ifdef DEBUG_PRINTF
	printf("X1 %f Y1 %f Z1 %f\r\n",gradientmagnet_front_x,gradientmagnet_front_y,gradientmagnet_front_z);
	printf("X2 %f Y2 %f Z2 %f\r\n",gradientmagnet_left_x,gradientmagnet_left_y,gradientmagnet_left_z);
	printf("X3 %f Y3 %f Z3 %f\r\n",gradientmagnet_right_x,gradientmagnet_right_y,gradientmagnet_right_z);
	#endif
}


