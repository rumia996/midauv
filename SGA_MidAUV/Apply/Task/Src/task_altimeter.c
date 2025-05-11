#include "task_altimeter.h"

uint8_t Altimeter_ReceBuf[100]={0};	//�洢��λ�������ַ���,�޷����ַ���������
uint8_t Altimeter_ReceNum = 0;	//�����ַ�������
float Height = 0;
void Task_Altimeter_Handle(void)
{
	if(rt_sem_take(MessageFromAltimeter_Sem,RT_WAITING_FOREVER) == RT_EOK)//��������3
		{
			Altimeter_ReceNum = Drv_Uart_Receive_DMA(&Uart3,Altimeter_ReceBuf);//���յ���Ϣ,ͨ��DMA�����ַ������洢,����¼�ַ�������
			if (Altimeter_ReceNum != 0)
			{
				#ifdef DEBUG_MODE
				//Drv_Uart_Transmit_DMA(&Uart3,Altimeter_ReceBuf,sizeof(Altimeter_ReceBuf));
				#endif 
			}
            if(Altimeter_ReceNum == 10)
			{	
				if(Altimeter_ReceBuf[3] == '.' && Altimeter_ReceBuf[7] == 'm' && Altimeter_ReceBuf[8] == 0x0d && Altimeter_ReceBuf[9] == 0x0a)//������֡β��CR LF, 16�����µ�0D 0A
					{
						uint8_t heightbuf[20] = {0};
						extract_char_between_indexandchar(Altimeter_ReceBuf,heightbuf,0,'m');
						Height = atof((const char*)heightbuf);//ASCIIת����
						memcpy(&ReportDataBuffer[HEIGHT_BASE],&Height,FLOAT_SIZE);
#ifdef DEBUG_MODE
						printf("�߶ȼ�ԭʼ����:%s,ת������:%f\r\n",Altimeter_ReceBuf,Height);
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
