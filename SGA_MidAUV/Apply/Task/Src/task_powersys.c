#include "task_powersys.h"
#include "task_altimeter.h"
#include "task_TDmeter.h"
uint8_t PowerSys_ReceBuf[100];	//�洢��λ�������ַ���,�޷����ַ���������
uint8_t PowerSys_ReceNum = 0;	//�����ַ�������
uint8_t powerstate = 0;
uint8_t counter = 0;
/**
 * @brief �������Դ
 * @param �̵������ ������config.h��
 * @retval null
*/
void Power_On(uint8_t index)
{

	Drv_GPIO_Reset(&Control_GPIO[index]);
//	if (index == POW_TDM)		/*�������ϵ���Ҫ��ʼ��*/ /*ʹ�����ǲ����̵���ֱ��12V��Դ,�����˸�λ����*/
//	{
//		Drv_Delay_Ms(2000);		/*��ʱ2��ȴ��������ϵ縴λ*/
//		TDmeter_Init();
//	}
}

/**
 * @brief �ر������Դ
 * @param �̵������ ������config.h��
 * @retval null
*/
void Power_Off(uint8_t index)
{
	Drv_GPIO_Set(&Control_GPIO[index]);
}
/**
 * @brief ��Դȫ���ر�
 * @retval null
*/
void Power_AllOff()
{
	Power_Off(POW_MAG);
	Power_Off(POW_TDM);
	Power_Off(POW_HEIGHT);
	Power_Off(POW_LIGHT);
	Power_Off(POW_BDGPS);
	Power_Off(POW_RADIO);
	Power_Off(POW_UACM);
	Power_Off(POW_TEST_1);
	Power_Off(POW_TEST_2);
}

/**
 * @brief ��Դȫ������ ���ó���
 * @retval null
*/
void Power_AllOn()
{
	Power_On(POW_MAG);
	Power_On(POW_TDM);
	Power_On(POW_HEIGHT);
	Power_On(POW_LIGHT);
	Power_On(POW_BDGPS);
	Power_On(POW_RADIO);
	Power_On(POW_UACM);
	//Power_On(POW_TEST_1);
	//Power_On(POW_TEST_2);
}

/**
 * @brief ����������Դ
 * @retval null
*/
void Power_SensorOn()
{
	Power_On(POW_MAG);
	Power_On(POW_TDM);
	Power_On(POW_HEIGHT);
}

/**
 * @brief �ش�������Դ
 * @retval null
*/
void Power_SensorOff()
{
	Power_Off(POW_MAG);
	Power_Off(POW_TDM);
	Power_Off(POW_HEIGHT);
}

/**
 * @brief ��ͨ�ŵ�Դ
 * @retval null
*/
void Power_CommunicationOn()
{
	Power_On(POW_BDGPS);
	Power_On(POW_RADIO);
	Power_On(POW_UACM);
}

/**
 * @brief ��ͨ�ŵ�Դ
 * @retval null
*/
void Power_CommunicationOff()
{
	Power_Off(POW_BDGPS);
	Power_Off(POW_RADIO);
	Power_Off(POW_UACM);
}

/**
 * @brief ���Ե�Դ���� 
 * @retval null
*/
void Power_Test()
{
	Power_On(POW_MAG);
	Power_On(POW_TDM);
	Power_On(POW_HEIGHT);
	Power_On(POW_BDGPS);
	Power_Off(POW_LIGHT);
	Power_On(POW_RADIO);
	Power_On(POW_UACM);	
	Power_Off(POW_TEST_1);
	Power_Off(POW_TEST_2);	
}
/**
 * @brief ��ȡ��ǰ�̵�������״̬ 
 * @retval �̵���״̬ ��0-7λ�ֱ�Ϊ ������ �߶ȼ� ����GPS ��ͨ�� ˮ�µ� ������ ���ߵ�̨
 * �̵����ǵ͵�ƽ0���ߵ�ƽ1��,����Э��Ҫ��,������״̬ȡ��,1��ʾ��,0��ʾ��;
*/
uint8_t Power_GetState()
{
	uint8_t powerstate= 0;
	powerstate = 1;	//��7λ ��ʱ����
	powerstate |= Drv_GPIO_Read(&Control_GPIO[POW_RADIO]);	//��6λ �˴���Э�鲻ͬ,û��ˮ�µ�
	powerstate <<=1;
	powerstate |= Drv_GPIO_Read(&Control_GPIO[POW_MAG]);	//��5λ
	powerstate <<=1;
	powerstate |= Drv_GPIO_Read(&Control_GPIO[POW_LIGHT]);	//��4λ
	powerstate <<=1;
	powerstate |= Drv_GPIO_Read(&Control_GPIO[POW_UACM]);	//��3λ
	powerstate <<=1;
	powerstate |= Drv_GPIO_Read(&Control_GPIO[POW_BDGPS]);	//��2λ
	powerstate <<=1;
	powerstate |= Drv_GPIO_Read(&Control_GPIO[POW_HEIGHT]);	//��1λ
	powerstate <<=1;
	powerstate |= Drv_GPIO_Read(&Control_GPIO[POW_TDM]);	//��0λ
	
	return ~powerstate;

//	Drv_GPIO_Read(&Control_GPIO[POW_TEST_1]);
//	Drv_GPIO_Read(&Control_GPIO[POW_TEST_2]);
}

void Task_PowerSys_Handle()
{

	/*���С��1m����ͨ��Ϊ����״̬���Զ��ر���ͨ����Դ*/
	if(*p_height < 1 && !Drv_GPIO_Read(&Control_GPIO[POW_UACM]))
	{
		/*ÿ100ms counter+1 ��ʱ3���ر�*/
		counter+=1;
		//printf("counter=%d\r\n",counter);
		if(counter>= 30)
		{			
			counter = 0;
			//Power_Off(POW_UACM);
			#ifdef DEBUG_MODE
			printf("��ǰ���:%.3f ��ͨ���ѹر�\r\n",*p_height);
			#endif
		}
	}

	
	/*ÿ100msι��*/
	Drv_IWDG_Feed(&demoIWDG);
	#ifdef DEBUG_MODE
	//printf("ι��!\r\n");
	#endif
	Drv_Delay_Ms(100);
}
