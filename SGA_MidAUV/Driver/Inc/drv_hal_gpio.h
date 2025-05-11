#ifndef __DRV_GPIO_H_
#define __DRV_GPIO_H_

#include "drv_hal.h"

/** @defgroup ucAFMode_define remap
  * @brief GPIO remap
  * @{
  */
#define NO_REMAP 		0	/*������ӳ�����*/
#define PARTIAL_REMAP 	1	/*������ӳ��*/
#define PARTIAL_REMAP2 	2	/*������ӳ��2 ��TIM2����*/
#define FULL_REMAP 		3	/*��ȫ��ӳ��*/

typedef struct
{
	GPIO_InitTypeDef 	tGPIOInit;
	GPIO_TypeDef        *tGPIOPort;
	uint8_t				ucPriority;		/* �ж����ȼ���0-15 */
	uint8_t 			ucSubPriority;	/* �ж������ȼ���0-15 */
#ifdef STM32F1_SGA_ENABLE
	uint8_t				ucAFMode;		/* ��ӳ�����ò��� @ref ucAFMode_define*/
#endif
}tagGPIO_T;

void Drv_GPIO_Set(tagGPIO_T *_tGPIO);
void Drv_GPIO_Reset(tagGPIO_T *_tGPIO);
void Drv_GPIO_Write(tagGPIO_T *_tGPIO, GPIO_PinState _PinStatus);
GPIO_PinState Drv_GPIO_Read(tagGPIO_T *_tGPIO);
void Drv_GPIO_Toggle(tagGPIO_T *_tGPIO);

void Drv_GPIO_Init(tagGPIO_T *_tGPIO, uint8_t _ucNum);
void Drv_GPIO_DeInit(tagGPIO_T *_tGPIO);

void Drv_GPIO_EXTI_IRQHandler(tagGPIO_T *_tGPIO);
void Drv_GPIO_NVICDisable(tagGPIO_T *_tGPIO);
void Drv_GPIO_NVICConfig(tagGPIO_T *_tGPIO);

#endif
