#include "task_altimeter.h"

uint8_t Altimeter_ReceBuf[100]={0};	//存储上位机命令字符串,无符号字符类型数组
uint8_t Altimeter_ReceNum = 0;	//命令字符串长度
float Height = 0;
void Task_Altimeter_Handle(void)
{
	if(rt_sem_take(MessageFromAltimeter_Sem,RT_WAITING_FOREVER) == RT_EOK)//监听串口3
		{
			Altimeter_ReceNum = Drv_Uart_Receive_DMA(&Uart3,Altimeter_ReceBuf);//若收到信息,通过DMA接收字符串并存储,并记录字符串长度
			if (Altimeter_ReceNum != 0)
			{
				#ifdef DEBUG_MODE
				//Drv_Uart_Transmit_DMA(&Uart3,Altimeter_ReceBuf,sizeof(Altimeter_ReceBuf));
				#endif 
			}
            if(Altimeter_ReceNum == 10)
			{	
				if(Altimeter_ReceBuf[3] == '.' && Altimeter_ReceBuf[7] == 'm' && Altimeter_ReceBuf[8] == 0x0d && Altimeter_ReceBuf[9] == 0x0a)//正常的帧尾是CR LF, 16进制下的0D 0A
					{
						uint8_t heightbuf[20] = {0};
						extract_char_between_indexandchar(Altimeter_ReceBuf,heightbuf,0,'m');
						Height = atof((const char*)heightbuf);//ASCII转数字
						memcpy(&ReportDataBuffer[HEIGHT_BASE],&Height,FLOAT_SIZE);
#ifdef DEBUG_MODE
						printf("高度计原始数据:%s,转换数字:%f\r\n",Altimeter_ReceBuf,Height);
						for (int i=HEIGHT_BASE;i<HEIGHT_BASE+4;i++)
						{
							printf("%c",ReportDataBuffer[i]);
						}
						printf("\r\n");
#endif
			
					}
			}
			Altimeter_ReceNum = 0;
		}	
}
