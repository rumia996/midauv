/****************************************************************************

* Sigma团队

* 文件名: drv_hal_spi.c

* 内容简述：spi驱动文件

* 文件历史：

* 版本号		日期		作者		说明
*  3.4 	 	2024-08-28	 鲍程璐		修复SPI速度设置不正确的问题

*  3.2 	 	2024-04-18	 鲍程璐		Drv_SPI_Transmit函数增加返回值

*  3.0	 	2024-01-26	 鲍程璐		适配STM32F4系列

* 1.1.7 	2022-10-11   鲍程璐		优化执行顺序

* 1.1.4 	2022-09-03   鲍程璐		新增SPI引脚重映射代码

* 1.0.0a 	2020-02-22	 李环宇		创建该文件

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_SPI_ENABLE

/**
 * @brief SPI时钟使能函数
 * @param _tSPI-spi结构体指针
 * @retval Null
*/
static void S_SPI_CLKEnable(tagSPI_T *_tSPI)
{
	if(_tSPI->tSPIHandle.Instance == SPI1)
	{
		__HAL_RCC_SPI1_CLK_ENABLE();
	}
	else if(_tSPI->tSPIHandle.Instance == SPI2)
	{
		__HAL_RCC_SPI2_CLK_ENABLE();
	}
	else if(_tSPI->tSPIHandle.Instance == SPI3)
	{
		__HAL_RCC_SPI3_CLK_ENABLE();
	}
}

/**
 * @brief SPI的GPIO配置函数
 * @param _tSPI-spi结构体指针
 * @retval Null
*/
static void S_SPI_GPIOConfig(tagSPI_T *_tSPI)
{
#ifdef STM32F1_SGA_ENABLE
	/* 开启复用模式时钟 */
	__HAL_RCC_AFIO_CLK_ENABLE();

	/* 根据不同SPI的ucAFMode开启对应的重映射，重映射表在drv_hal_spi.h中 */
	if(_tSPI->tSPIHandle.Instance == SPI1)
	{
		if(_tSPI->tGPIO->ucAFMode == NO_REMAP)				__HAL_AFIO_REMAP_SPI1_DISABLE();
		else if(_tSPI->tGPIO->ucAFMode == FULL_REMAP)		__HAL_AFIO_REMAP_SPI1_ENABLE();		
	}
#endif

#ifdef STM32F4_SGA_ENABLE
	if(_tSPI->tSPIHandle.Instance == SPI1)
	{
		_tSPI->tGPIO[0].tGPIOInit.Alternate = GPIO_AF5_SPI1;
		_tSPI->tGPIO[1].tGPIOInit.Alternate = GPIO_AF5_SPI1;
		_tSPI->tGPIO[2].tGPIOInit.Alternate = GPIO_AF5_SPI1;
	}
	else if(_tSPI->tSPIHandle.Instance == SPI2)
	{
		_tSPI->tGPIO[0].tGPIOInit.Alternate = GPIO_AF5_SPI2;
		_tSPI->tGPIO[1].tGPIOInit.Alternate = GPIO_AF5_SPI2;
		_tSPI->tGPIO[2].tGPIOInit.Alternate = GPIO_AF5_SPI2;
	}
	else if(_tSPI->tSPIHandle.Instance == SPI3)
	{
		_tSPI->tGPIO[0].tGPIOInit.Alternate = GPIO_AF6_SPI3;
		_tSPI->tGPIO[1].tGPIOInit.Alternate = GPIO_AF6_SPI3;
		_tSPI->tGPIO[2].tGPIOInit.Alternate = GPIO_AF6_SPI3;
	}
#endif
	Drv_GPIO_Init(_tSPI->tGPIO, 4);	/* GPIO初始化 */
}

/**
 * @brief spi参数配置函数
 * @param _tSPI-spi结构体指针
 * @retval Null
*/
static void S_SPI_ParamConfig(tagSPI_T *_tSPI)
{
	if (HAL_SPI_Init(&_tSPI->tSPIHandle) != HAL_OK)
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
}

/**
 * @brief 硬件SPI速度设置
 * @param _tSPI-spi结构体指针
 * @param _ucSpeed- 1-高速，0-低速
 * @retval Null 
*/
void Drv_SPI_SpeedConfig(tagSPI_T *_tSPI, uint8_t _ucSpeed)
{
	switch(_ucSpeed)
	{
		case SPI_SPEED_HIGH:
			_tSPI->tSPIHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
		break;
			
		case SPI_SPEED_LOW:
			_tSPI->tSPIHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
		break;
			
		default:
			Drv_HAL_Error(__FILE__, __LINE__);
		break;
	}
    Drv_SPI_Init(_tSPI);
}

/**
 * @brief 硬件SPI发送和接收
 * @param _tSPI-spi结构体指针
 * @param _ucTxData-需要发送的一个字节数据
 * @retval uint8_t-接收到的一个字节数据
*/
uint8_t Drv_SPI_TransmitReceive(tagSPI_T *_tSPI, uint8_t _ucTxData)
{
	uint8_t ucRxData = 0;
	
	HAL_SPI_TransmitReceive(&_tSPI->tSPIHandle,&_ucTxData,&ucRxData,BYTE_SIZE,SPI_TIME_OUT);
	
	return ucRxData;
}

/**
 * @brief 硬件SPI发送
 * @param _tSPI-spi结构体指针
 * @param _ucTxData-需要发送的一个字节数据
* @retval uint8_t 发送状态
*/
uint8_t Drv_SPI_Transmit(tagSPI_T *_tSPI, uint8_t _ucTxData)
{
    uint8_t ucRet;
    
	ucRet = HAL_SPI_Transmit(&_tSPI->tSPIHandle,&_ucTxData,BYTE_SIZE,SPI_TIME_OUT);
    
    return ucRet;
}

/**
 * @brief 硬件SPI初始化
 * @param _tSPI-spi结构体指针
 * @retval null
*/
void Drv_SPI_Init(tagSPI_T *_tSPI)
{
	S_SPI_CLKEnable(_tSPI);
	S_SPI_GPIOConfig(_tSPI);
	S_SPI_ParamConfig(_tSPI);
}

#endif
