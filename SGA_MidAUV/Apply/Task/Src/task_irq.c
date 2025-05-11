#include "task_conf.h"
#include "usercode.h"
#include "config.h"


/**
 * @brief 外部中断5-9服务函数
 * @retval Null
*/
void EXTI9_5_IRQHandler(void)
{
    //Drv_GPIO_EXTI_IRQHandler(&DS3231_ALARM_IRQ);	/* 必须加，参数需修改 */
}

/**
 * @brief 外部中断10-15服务函数
 * @retval Null
*/
void EXTI15_10_IRQHandler(void)
{
    //Drv_GPIO_EXTI_IRQHandler(&tPCUart.tGPIO[1]);	/* 必须加，参数需修改 */
}

/**
 * @brief 串口1中断服务函数 与树莓派通信收&发
 * @retval Null
*/
void USART1_IRQHandler(void)
{
    rt_interrupt_enter();
    Drv_Uart_IRQHandler(&Uart1);
    rt_sem_release(CmdFromIPC_Sem);	//释放信号量,接收上位机数据线程开始工作
    Drv_Uart_DMA_RxHandler(&Uart1);
    rt_interrupt_leave();
}

//串口1DMA发送必要中断服务函数	功能是DMA1的通道4切换收发状态 DMA通道和串口是绑定的不能改,查手册//实测没有也行
void DMA1_Channel4_IRQHandler(void)
{
    Drv_Uart_DMA_IRQHandler(&Uart1);
}

/**
 * @brief 串口2中断服务函数 JY901
 * @retval Null
*/
void USART2_IRQHandler(void)
{
//    rt_interrupt_enter();
//    Drv_Uart_IRQHandler(&Uart2);   
//    Drv_Uart_DMA_RxHandler(&Uart2);
//	rt_sem_release(MessageFromTDmeter_Sem);	
//    rt_interrupt_leave();
	rt_interrupt_enter();    //进入临界区
    Drv_Uart_DMA_RxHandler(&JY901S.tUART);
	rt_sem_release(JY901S_Sem);	//释放信号量,线程获取信号量开始工作
	rt_interrupt_leave();    //退出临界区
}

//串口2DMA发送必要中断服务函数
//void DMA1_Channel7_IRQHandler(void)
//{
//    Drv_Uart_DMA_IRQHandler(&Uart2);
//}
/**
 * @brief 串口3中断服务函数 高度计仅接收
 * @retval Null
*/
void USART3_IRQHandler(void)
{
    rt_interrupt_enter();
    Drv_Uart_IRQHandler(&Uart3);
    rt_sem_release(MessageFromAltimeter_Sem);	//释放信号量,接收线程开始工作
    Drv_Uart_DMA_RxHandler(&Uart3);
    rt_interrupt_leave();
}
void DMA1_Channel2_IRQHandler(void)
{
    Drv_Uart_DMA_IRQHandler(&Uart3);
}
/**
 * @brief 串口4中断服务函数 温深仪收/发
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
//串口4DMA发送必要中断服务函数
//void DMA2_Channel5_IRQHandler(void)
//{
//    Drv_Uart_DMA_IRQHandler(&Uart4);
//}

/**
 * @brief 串口5中断服务函数
 * @retval Null
*/
void UART5_IRQHandler(void)
{

}

/**
 * @brief 定时器2中断服务函数
 * @retval Null
*/
void TIM2_IRQHandler(void)
{
	/* 示例 */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief 定时器3中断服务函数
 * @retval Null
*/
void TIM3_IRQHandler(void)
{
	/* 示例 */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief 定时器4中断服务函数
 * @retval Null
*/
void TIM4_IRQHandler(void)
{
    /* 示例 */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief 定时器5中断服务函数
 * @retval Null
*/
void TIM5_IRQHandler(void)
{
    /* 示例 */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief 定时器6中断服务函数
 * @note F4系列使用函数名TIM6_DAC_IRQHandler
 * @retval Null
*/
void TIM6_IRQHandler(void)
{
    /* 示例 */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief 定时器7中断服务函数
 * @retval Null
*/
void TIM7_IRQHandler(void)
{
	/* 示例 */
    //Drv_Timer_IRQHandler(&demoTIM);
}



