#ifndef __DRV_UART_H_
#define __DRV_UART_H_

#include "drv_hal_conf.h"

/* 串口超时时间 */
#define UART_TIME_OUT	0xff

/* 串口中断接收长度 */
#define UART_IT_RX_CACHE_SIZE 100

/* 串口接收信息结构体 */
typedef struct
{
	/* 中断相关信息 */
	uint16_t			usRxCnt;				/* 接收数据计数器(中断使用) */
	uint16_t			usRxLength;			    /* 总接收数据长度(中断使用) */
	uint8_t				ucpRxBuffer[1];			/* 中断接收Buffer(中断使用) */
	uint8_t				*ucpITRxCache;			/* 中断接收缓冲区(中断使用) */

	/* DMA相关信息 */
	uint16_t			usDMARxLength;			/* DMA总接收数据长度(DMA使用) */
	uint16_t			usDMARxMAXSize;			/* DMA接收缓冲区大小(DMA使用) */
	uint8_t				*ucpDMARxCache;			/* DMA接收缓冲区(DMA使用) */

	/* 标志位信息 */
	uint8_t 			ucRxCplt;				/* 接收完成标志(中断使用) */
	uint8_t				ucDMARxCplt;			/* DMA接收完成标志(DMA使用) */
}tagUartRxInfo_T;

/* 串口发送信息结构体 */
typedef struct 
{
	/* DMA相关信息 */
	uint16_t			usDMATxLength;			/* 总发送数据长度(DMA使用) */
	uint16_t			usDMATxMAXSize;			/* DMA发送缓冲区大小(DMA使用) */
	uint8_t				*ucpDMATxCache;			/* DMA发送缓冲区(DMA使用) */

	/* 标志位信息 */
	uint8_t 			ucTxCplt;				/* 发送完成标志(中断使用) */
	uint8_t 			ucDMATxCplt;			/* DMA发送完成标志(DMA使用) */
}tagUartTxInfo_T;

/* 串口DMA配置 */
typedef struct
{
	DMA_HandleTypeDef	tDMATx;					/* DMA发送句柄 */
	DMA_HandleTypeDef	tDMARx;					/* DMA接收句柄 */

	bool 				bTxEnable;				/* 发送使能符号 */
	bool				bRxEnable;				/* 接收使能符号 */

	uint8_t				ucDMARxPriority;		/* DMA接收中断优先级，0-15 */
	uint8_t 			ucDMARxSubPriority;		/* DMA接收中断子优先级，0-15 */

	uint8_t				ucDMATxPriority;		/* DMA发送中断优先级，0-15 */
	uint8_t 			ucDMATxSubPriority;		/* DMA发送中断子优先级，0-15 */
}tagDMAUart_T;

/* 串口设备结构体 */
typedef struct
{
	UART_HandleTypeDef 	tUARTHandle;	/* STM32内部串口设备指针 */
	tagDMAUart_T		tUartDMA;		/* 串口DMA配置 */
	tagUartRxInfo_T		tRxInfo;		/* 串口接收信息 */
	tagUartTxInfo_T		tTxInfo;		/* 串口发送信息 */
	tagGPIO_T			tGPIO[2];		/* GPIO句柄 */
	uint8_t				ucPriority;		/* 中断优先级，0-15 */
	uint8_t 			ucSubPriority;	/* 中断子优先级，0-15 */
}tagUART_T;

/* F1系列串口重映射表
	USART1	full remap (TX/PB6, RX/PB7)
			no remap (TX/PA9, RX/PA10)
	USART2	full remap (CTS/PD3, RTS/PD4, TX/PD5, RX/PD6, CK/PD7)
			no remap (CTS/PA0, RTS/PA1, TX/PA2, RX/PA3, CK/PA4)
	USART3	full remap (TX/PD8,  RX/PD9,  CK/PD10, CTS/PD11, RTS/PD12)
			partial	remap (TX/PC10, RX/PC11, CK/PC12, CTS/PB13, RTS/PB14)
			no remap (TX/PB10, RX/PB11, CK/PB12, CTS/PB13, RTS/PB14)
*/

/* F1系列串口DMA通道对应 
	USART1	TX	DMA1_Channel4_IRQHandler
			RX	DMA1_Channel5_IRQHandler
	USART2	TX	DMA1_Channel7_IRQHandler
			RX	DMA1_Channel6_IRQHandler
	USART3	TX	DMA1_Channel2_IRQHandler
			RX	DMA1_Channel3_IRQHandler
	UART4	TX	DMA2_Channel5_IRQHandler
			RX	DMA2_Channel3_IRQHandler
*/

/* F4系列串口重映射表
	|        | TXD1 | RXD1 | TXD2     | RXD2     |
	| ------ | ---- | ---- | -------- | -------- |
	| USART1 | PA9  | PA10 | PB6      | PB7      |
	| USART2 | PA2  | PA3  | PD5      | PD6      |
	| USART3 | PB10 | PB11 | PC10/PD8 | PC11/PD9 |
	| UART4  | PA0  | PA1  | PC10     | PC11     |
	| UART5  | PC12 | PD2  |          |          |
	| USART6 | PC6  | PC7  | PG14     | PG9      |
*/

/* F4系列串口DMA通道对应 
	USART1	TX	DMA2_Stream7_IRQHandler
			RX	DMA2_Stream2_IRQHandler | DMA2_Stream5_IRQHandler
	USART2	TX	DMA1_Stream6_IRQHandler
			RX	DMA1_Stream5_IRQHandler
	USART3	TX	DMA1_Stream3_IRQHandler
			RX	DMA1_Stream1_IRQHandler
	UART4	TX	DMA1_Stream4_IRQHandler
			RX	DMA1_Stream2_IRQHandler
	UART5	TX	DMA1_Stream7_IRQHandler
			RX	DMA1_Stream0_IRQHandler
	USART6	TX	DMA2_Stream6_IRQHandler | DMA2_Stream7_IRQHandler
			RX	DMA2_Stream1_IRQHandler	| DMA2_Stream2_IRQHandler
*/

void Drv_Uart_ITInit(tagUART_T *_tUART);
void Drv_Uart_DMAInit(tagUART_T *_tUART);
void Drv_Uart_Transmit(tagUART_T *_tUART, uint8_t *_ucpTxData, uint16_t _usSize);
void Drv_Uart_Transmit_IT(tagUART_T *_tUART, uint8_t *_ucpTxData, uint16_t _usSize);
void Drv_Uart_Transmit_DMA(tagUART_T *_tUART, uint8_t *_ucpTxData, uint16_t _usSize);
uint16_t Drv_Uart_Receive_IT(tagUART_T *_tUART, uint8_t *_ucpRxData);
uint16_t Drv_Uart_Receive_DMA(tagUART_T *_tUART, uint8_t *_ucpRxData);
void Drv_Uart_ReceIT_Enable(tagUART_T *_tUART, uint8_t *_ucpRxData, uint16_t _usSize);
void Drv_Uart_IRQHandler(tagUART_T *_tUART);
void Drv_Uart_IT_RxHandler(tagUART_T *_tUART, uint8_t _ucEndChar);
void Drv_Uart_DMA_RxHandler(tagUART_T *_tUART);
void Drv_Uart_DMA_TxHandler(tagUART_T *_tUART);
void Drv_Uart_DMA_IRQHandler(tagUART_T *_tUART);

#endif
