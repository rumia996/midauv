/****************************************************************************

* Sigma团队

* 文件名: drv_hal_pwr.c

* 内容简述：PWR电源管理驱动文件

* 文件历史：

* 版本号	日期		作者		说明
*  3.3   2024-06-22	  鲍程璐	增加PVD电源监测驱动

*  2.5   2023-06-03	  鲍程璐	创建该文件

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_PWR_ENABLE

/**
 * @brief 恢复HSE时钟函数
 * @param Null
 * @note 用于停止模式被唤醒时，恢复到HSE高速时钟
 * @retval Null 
*/
static void S_PWR_RestoreHSEClk(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    uint32_t pFLatency = 0;

    /* 启用电源控制时钟 */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* 根据内部RCC寄存器获取振荡器配置 */
    HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

    /* 从停止模式唤醒后重新配置系统时钟:
    启用HSE和PLL */
    RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState        = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_ON;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Drv_HAL_Error(__FILE__, __LINE__);
    }

    /* 根据内部RCC寄存器获取时钟配置 */
    HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);

    /* 选择 PLL 作为系统时钟源, 并配置 HCLK、PCLK1 和 PCLK2时钟分频系数 */
    RCC_ClkInitStruct.ClockType     = RCC_CLOCKTYPE_SYSCLK;
    RCC_ClkInitStruct.SYSCLKSource  = RCC_SYSCLKSOURCE_PLLCLK;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK)
    {
        Drv_HAL_Error(__FILE__, __LINE__);
    }

    /* 根据时钟寄存器的值更新SystemCoreClock变量 */
    SystemCoreClockUpdate();
}

/**
 * @brief 进入睡眠模式函数
 * @param Null
 * @note 睡眠时:
 *       关闭内核时钟，内核停止，而外设正常运行，在软件上表现为不再执行新的代码。
 *       唤醒后:
 *       若由任意中断唤醒，先进入中断，退出中断服务程序后，接着执行WFI指令后的程序；
 *       若由事件唤醒，直接接着执行WFE后的程序。
 * @retval Null 
*/
void Drv_PWR_EnterSleepMode(void)
{
    /* 暂停滴答时钟，防止通过滴答时钟中断唤醒 */
    HAL_SuspendTick();

    /* 进入睡眠模式，代码将在此处暂停，等待唤醒 */
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON,PWR_SLEEPENTRY_WFI);

    /* 从低功耗模式唤醒后清除唤醒标志位 */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

    /* 被唤醒后，恢复滴答时钟 */
    HAL_ResumeTick();
}

/**
 * @brief 进入停止模式函数
 * @param Null
 * @note 停止时:
 *       关闭内核时钟，内核停止，片上外设也停止。
 *       唤醒后:
 *       STM32会使用HSI作为系统时钟。
 *       若由EXTI中断唤醒，先进入中断，退出中断服务程序后，接着执行WFI指令后的程序；
 *       若由事件唤醒，直接接着执行WFE后的程序。
 * @retval Null 
*/
void Drv_PWR_EnterStopMode(void)
{
    /* 暂停滴答时钟，防止通过滴答时钟中断唤醒 */
    HAL_SuspendTick();

    /* 进入停止模式，代码将在此处暂停，等待唤醒  */
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI);

    /* 从低功耗模式唤醒后清除唤醒标志位 */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

    /* 重启HSE高速时钟 */
    S_PWR_RestoreHSEClk();

    /* 被唤醒后，恢复滴答时钟 */
    HAL_ResumeTick();
}

/**
 * @brief 进入待机模式函数
 * @param Null
 * @note 待机时:
 *       全部关闭。除复位引脚、RTC_AF1引脚及WKUP引脚，其它I/O口均工作在高阻态。
 *       唤醒后:
 *       相当于芯片复位，在程序表现为从头开始执行代码。
 * @retval Null 
*/
void Drv_PWR_EnterStandByMode(void)
{
    /* 使能WKUP引脚的唤醒功能 ，使能PA0，上升沿触发唤醒 */
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

    /* 暂停滴答时钟，防止通过滴答时钟中断唤醒 */
    HAL_SuspendTick();

    /* 进入待机模式，唤醒后芯片将会复位 */
    HAL_PWR_EnterSTANDBYMode();
}

/**
 * @brief 初始化PVD电源监测功能
 * @param _ulPVDLevel 监测的电压等级
 *         该参数可以为下列可选值之一
 *           @arg PWR_PVDLEVEL_0    1.9V
 *           @arg PWR_PVDLEVEL_1    2.1V
 *           @arg PWR_PVDLEVEL_2    2.3V
 *           @arg PWR_PVDLEVEL_3    2.5V
 *           @arg PWR_PVDLEVEL_4    2.7V
 *           @arg PWR_PVDLEVEL_5    2.9V
 *           @arg PWR_PVDLEVEL_6    3.1V
 *           @arg PWR_PVDLEVEL_7    外部模拟电压输入(PB7)    
 * @note 1.需要在task_irq.c中void PVD_IRQHandler(void)中调用void Drv_PVD_IRQHandler(void)
         2.需要在task_irq.c中的void HAL_PWR_PVDCallback(void)函数中写入电源不足时的内容
 * @retval Null 
*/
void Drv_PWR_PVD_Init(uint32_t _ulPVDLevel)
{
    /* 开启PWR时钟 */
    __HAL_RCC_PWR_CLK_ENABLE();
 
    /* 配置PVD中断 */
    HAL_NVIC_SetPriority(PVD_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(PVD_IRQn);
 
    /* 配置PVD监测电压等级以及触发类型 */
    PWR_PVDTypeDef tConfigPVD;
    tConfigPVD.PVDLevel = _ulPVDLevel;
    tConfigPVD.Mode = PWR_PVD_MODE_IT_RISING;
    HAL_PWR_ConfigPVD(&tConfigPVD);

    /* 启动PVD监测 */
    HAL_PWR_EnablePVD();
}

/**
 * @brief PVD中断帽子函数(用于PVD中断处理函数中)
 * @note 在void PVD_IRQHandler(void)函数中调用
 * @param Null
 * @retval Null
*/
void Drv_PVD_IRQHandler(void)
{
    HAL_PWR_PVD_IRQHandler();
}

#endif
