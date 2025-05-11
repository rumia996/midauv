#include "task_conf.h"
#include "usercode.h"
#include "config.h"


/**
 * @brief �ⲿ�ж�5-9������
 * @retval Null
*/
void EXTI9_5_IRQHandler(void)
{
    //Drv_GPIO_EXTI_IRQHandler(&DS3231_ALARM_IRQ);	/* ����ӣ��������޸� */
}

/**
 * @brief �ⲿ�ж�10-15������
 * @retval Null
*/
void EXTI15_10_IRQHandler(void)
{
    //Drv_GPIO_EXTI_IRQHandler(&tPCUart.tGPIO[1]);	/* ����ӣ��������޸� */
}

/**
 * @brief ����1�жϷ����� ����ݮ��ͨ����&��
 * @retval Null
*/
void USART1_IRQHandler(void)
{
    rt_interrupt_enter();
    Drv_Uart_IRQHandler(&Uart1);
    rt_sem_release(CmdFromIPC_Sem);	//�ͷ��ź���,������λ�������߳̿�ʼ����
    Drv_Uart_DMA_RxHandler(&Uart1);
    rt_interrupt_leave();
}

//����1DMA���ͱ�Ҫ�жϷ�����	������DMA1��ͨ��4�л��շ�״̬ DMAͨ���ʹ����ǰ󶨵Ĳ��ܸ�,���ֲ�//ʵ��û��Ҳ��
void DMA1_Channel4_IRQHandler(void)
{
    Drv_Uart_DMA_IRQHandler(&Uart1);
}

/**
 * @brief ����2�жϷ����� JY901
 * @retval Null
*/
void USART2_IRQHandler(void)
{
//    rt_interrupt_enter();
//    Drv_Uart_IRQHandler(&Uart2);   
//    Drv_Uart_DMA_RxHandler(&Uart2);
//	rt_sem_release(MessageFromTDmeter_Sem);	
//    rt_interrupt_leave();
	rt_interrupt_enter();    //�����ٽ���
    Drv_Uart_DMA_RxHandler(&JY901S.tUART);
	rt_sem_release(JY901S_Sem);	//�ͷ��ź���,�̻߳�ȡ�ź�����ʼ����
	rt_interrupt_leave();    //�˳��ٽ���
}

//����2DMA���ͱ�Ҫ�жϷ�����
//void DMA1_Channel7_IRQHandler(void)
//{
//    Drv_Uart_DMA_IRQHandler(&Uart2);
//}
/**
 * @brief ����3�жϷ����� �߶ȼƽ�����
 * @retval Null
*/
void USART3_IRQHandler(void)
{
    rt_interrupt_enter();
    Drv_Uart_IRQHandler(&Uart3);
    rt_sem_release(MessageFromAltimeter_Sem);	//�ͷ��ź���,�����߳̿�ʼ����
    Drv_Uart_DMA_RxHandler(&Uart3);
    rt_interrupt_leave();
}
void DMA1_Channel2_IRQHandler(void)
{
    Drv_Uart_DMA_IRQHandler(&Uart3);
}
/**
 * @brief ����4�жϷ����� ��������/��
 * @retval Null
*/
void UART4_IRQHandler(void)
{
    rt_interrupt_enter();
    Drv_Uart_IRQHandler(&Uart4);
	rt_sem_release(MessageFromTDmeter_Sem);	
    Drv_Uart_DMA_RxHandler(&Uart4);
    rt_interrupt_leave();	
}
//����4DMA���ͱ�Ҫ�жϷ�����
//void DMA2_Channel5_IRQHandler(void)
//{
//    Drv_Uart_DMA_IRQHandler(&Uart4);
//}

/**
 * @brief ����5�жϷ�����
 * @retval Null
*/
void UART5_IRQHandler(void)
{

}

/**
 * @brief ��ʱ��2�жϷ�����
 * @retval Null
*/
void TIM2_IRQHandler(void)
{
	/* ʾ�� */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief ��ʱ��3�жϷ�����
 * @retval Null
*/
void TIM3_IRQHandler(void)
{
	/* ʾ�� */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief ��ʱ��4�жϷ�����
 * @retval Null
*/
void TIM4_IRQHandler(void)
{
    /* ʾ�� */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief ��ʱ��5�жϷ�����
 * @retval Null
*/
void TIM5_IRQHandler(void)
{
    /* ʾ�� */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief ��ʱ��6�жϷ�����
 * @note F4ϵ��ʹ�ú�����TIM6_DAC_IRQHandler
 * @retval Null
*/
void TIM6_IRQHandler(void)
{
    /* ʾ�� */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief ��ʱ��7�жϷ�����
 * @retval Null
*/
void TIM7_IRQHandler(void)
{
	/* ʾ�� */
    //Drv_Timer_IRQHandler(&demoTIM);
}



