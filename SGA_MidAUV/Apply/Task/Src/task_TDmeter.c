#include "task_TDmeter.h"

uint8_t TDmeter_ReceBuf[100] = {0};	//�洢��λ�������ַ���,�޷����ַ���������
uint8_t TDmeter_ReceNum = 0;	//�����ַ�������
//float Temperature = 0;		//�¶�
//float Depth = 0;			//���

/*�����Ǿ��*/
void Task_TDmeter_Handle(void)
{
	if(rt_sem_take(MessageFromTDmeter_Sem,RT_WAITING_FOREVER) == RT_EOK)//��������4
	{		
		TDmeter_ReceNum = Drv_Uart_Receive_DMA(&Uart4,TDmeter_ReceBuf);//���յ���Ϣ,ͨ��DMA�����ַ������洢,����¼�ַ�������
        if(TDmeter_ReceNum != 0)
		{	
			#ifdef DEBUG_MODE
			//Drv_Uart_Transmit(&Uart4,TDmeter_ReceBuf,sizeof(TDmeter_ReceBuf));	
			#endif		
			if (TDmeter_ReceBuf[0] =='T' && TDmeter_ReceBuf[10] =='P' && TDmeter_ReceBuf[TDmeter_ReceNum-1] == '\n')
			{
				uint8_t Buf[30] = {0};
				uint8_t TemperatureBuf[30] = {0};
				uint8_t DepthBuf[30] = {0};
				/*ԭʼ���� T:23.2909,P:10.5725\r\n */
				/*����,Ҫע����ȡ����extract_str_between_2charʹ��strchrֻ���ҵ��ַ����е�һ��ƥ����ַ�*/
				/*������ȡ������ȡ':'��',' , ':'��'\r' ֮�������,����ȡPʱ�����*/
				/*��������ȡ':'��'\n'֮�������,�õ�23.2909,P:10.5725\r */
				/*��ʹ��extract_char_between_indexandchar��ȡT,ʹ��extract_str_between_2char��ȡP*/
//				extract_str_between_2char(TDmeter_ReceBuf,TemperatureBuf,':',',');
//				printf("%s\r\n",TemperatureBuf);
//				extract_str_between_2char(TDmeter_ReceBuf,DepthBuf,':','\r');
//				printf("%s\r\n",DepthBuf);
				extract_str_between_2char(TDmeter_ReceBuf,Buf,':','\n');
				extract_char_between_indexandchar(Buf,TemperatureBuf,0,',');
				extract_str_between_2char(Buf,DepthBuf,':','\r');
				if (TemperatureBuf[0] != '\0' )
				{
					float Temperature = 0.0f;
					float Depth = 0.0f;
					Temperature = atof((const char*)TemperatureBuf);
					Depth = atof((const char*)DepthBuf) - 10.0;	/*ѹ��dbar-10�������ֵ/��*/
					memcpy(&ReportDataBuffer[TEMPERATURE_BASE],&Temperature,FLOAT_SIZE);					
					memcpy(&ReportDataBuffer[DEPTH_BASE],&Depth,FLOAT_SIZE);	
					#ifdef DEBUG_MODE
					printf("Temperature:%.4f,Depth:%.4f\r\n",Temperature,Depth);
					#endif							
				}
			}
		}
		TDmeter_ReceNum = 0;
	}
}

/*���������ǳ���,���ڵ����Ĳ����߳�������*/
void Task_TDmeter_Test(void)
{
	Task_TDmeter_Setcycle(2);
	Task_TDmeter_AutoMode();
	Drv_Delay_Ms(6000);
	Task_TDmeter_Getmessage();
	Task_TDmeter_SleepMode();
	Drv_Delay_Ms(3000);
	Task_TDmeter_Setcycle(3);
	Task_TDmeter_Getmessage();
	Task_TDmeter_AutoMode();
	Drv_Delay_Ms(9000);
	Task_TDmeter_Reset();
	Drv_Delay_Ms(100);
	Task_TDmeter_OnceMode();
	Drv_Delay_Ms(2000);
	Task_TDmeter_OnceMode();
	Drv_Delay_Ms(2000);
	Task_TDmeter_Reset();
	Drv_Delay_Ms(100);
}



/**
 * @brief ��ʼ��������,���ò�����,����Ƶ��;���Զ�ģʽ����
 * @retval 
*/
void TDmeter_Init()
{
	//Task_TDmeter_Reset();		/*�ϵ���Զ���λ,�����ֶ���λ*/
	//Drv_Delay_Ms(2000);
	//Task_TDmeter_Setbaud(115200);
	//Drv_Delay_Ms(10);
	Task_TDmeter_Setcycle(2);
	Drv_Delay_Ms(100);
	Task_TDmeter_AutoMode();	/*�ظ�,��ֹͨ�����⵼�������ǳ�ʼ��ʧ��*/
	Drv_Delay_Ms(100);
	Task_TDmeter_AutoMode();
	Drv_Delay_Ms(100);
}

/**
 * @brief ��λ������		
 * @retval 
*/
void Task_TDmeter_Reset()
{
	uint8_t SendBuffer[30] = {0};//�洢���������ַ���,��<CR><LF>��β
	sprintf((char *)SendBuffer, "reset\r\n");
	Drv_Uart_Transmit(&Uart4,SendBuffer,strlen((const char *)SendBuffer));
}

/**
 * @brief ���ò����ʲ�;��ʵ������,�޸Ĳ����ʺ���޷�ͨ����!	
 * @param ������ ֻ����9600��14400��19200��38400��57600��115200;Ĭ��Ϊ115200;
*/
void Task_TDmeter_Setbaud(uint32_t baudrate)
{
	uint8_t SendBuffer[30] = {0};//�洢���������ַ���,��<CR><LF>��β	
	if (baudrate == 9600 || baudrate == 14400 || baudrate == 19200 || baudrate == 38400 ||baudrate == 57600 || baudrate == 115200)
	{
		sprintf((char *)SendBuffer, "setbaud %d\r\n",baudrate);
		Drv_Uart_Transmit(&Uart4,SendBuffer,strlen((const char *)SendBuffer));
	}else 
	{
		#ifdef DEBUG_MODE
		printf("��֧�ֲ�����9600��14400��19200��38400��57600��115200!\r\n");
		#endif
	}
}

/**
 * @brief ���ò���Ƶ��
 * @param ����Ƶ�� ����ģʽ��ÿ����ý���һ�����.������-4,-2,-1,1,2,3...��������Hz,-4��4Hz,0.25s;����������,3��3��;��������2s����һ��	
*/
void Task_TDmeter_Setcycle(int8_t cycle)
{
	uint8_t SendBuffer[30] = {0};//�洢���������ַ���,��<CR><LF>��β	
	sprintf((char *)SendBuffer, "setcycle %d\r\n",cycle);
	Drv_Uart_Transmit(&Uart4,SendBuffer,strlen((const char *)SendBuffer));
}

/**
 * @brief ���������Զ�ģʽ����,ÿ��һ��ʱ�䷢���¶Ⱥ�ѹ��ֵ;
*/
void Task_TDmeter_AutoMode()
{
	uint8_t SendBuffer[30] = {0};//�洢���������ַ���,��<CR><LF>��β	
	sprintf((char *)SendBuffer, "run\r\n");
	Drv_Uart_Transmit(&Uart4,SendBuffer,strlen((const char *)SendBuffer));
}

/**
 * @brief ���β���ģʽ,ÿִ��һ�β���һ��
*/
void Task_TDmeter_OnceMode()
{
	uint8_t SendBuffer[30] = {0};	
	sprintf((char *)SendBuffer, "once\r\n");
	Drv_Uart_Transmit(&Uart4,SendBuffer,strlen((const char *)SendBuffer));
}

/**
 * @brief �����ǽ���͹��Ĵ���ģʽ;�ϵ�/��λ�������ڴ�ģʽ
*/
void Task_TDmeter_SleepMode()
{
	uint8_t SendBuffer[30] = {0};	
	sprintf((char *)SendBuffer, "stop\r\n");
	Drv_Uart_Transmit(&Uart4,SendBuffer,strlen((const char *)SendBuffer));
}

/**
 * @brief ��ȡ��������Ϣ,������,����Ҫ�ھ�����մ���Ӵ������ڴ���1��ӡ��Ϣ
*/
void Task_TDmeter_Getmessage()
{
	uint8_t SendBuffer[30] = {0};	
	sprintf((char *)SendBuffer, "getmessage\r\n");
	Drv_Uart_Transmit(&Uart4,SendBuffer,strlen((const char *)SendBuffer));
}
