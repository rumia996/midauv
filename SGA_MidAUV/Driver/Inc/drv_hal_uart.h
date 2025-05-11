#ifndef __DRV_UART_H_
#define __DRV_UART_H_

#include "drv_hal_conf.h"

/* ���ڳ�ʱʱ�� */
#define UART_TIME_OUT	0xff

/* �����жϽ��ճ��� */
#define UART_IT_RX_CACHE_SIZE 100

/* ���ڽ�����Ϣ�ṹ�� */
typedef struct
{
	/* �ж������Ϣ */
	uint16_t			usRxCnt;				/* �������ݼ�����(�ж�ʹ��) */
	uint16_t			usRxLength;			    /* �ܽ������ݳ���(�ж�ʹ��) */
	uint8_t				ucpRxBuffer[1];			/* �жϽ���Buffer(�ж�ʹ��) */
	uint8_t				*ucpITRxCache;			/* �жϽ��ջ�����(�ж�ʹ��) */

	/* DMA�����Ϣ */
	uint16_t			usDMARxLength;			/* DMA�ܽ������ݳ���(DMAʹ��) */
	uint16_t			usDMARxMAXSize;			/* DMA���ջ�������С(DMAʹ��) */
	uint8_t				*ucpDMARxCache;			/* DMA���ջ�����(DMAʹ��) */

	/* ��־λ��Ϣ */
	uint8_t 			ucRxCplt;				/* ������ɱ�־(�ж�ʹ��) */
	uint8_t				ucDMARxCplt;			/* DMA������ɱ�־(DMAʹ��) */
}tagUartRxInfo_T;

/* ���ڷ�����Ϣ�ṹ�� */
typedef struct 
{
	/* DMA�����Ϣ */
	uint16_t			usDMATxLength;			/* �ܷ������ݳ���(DMAʹ��) */
	uint16_t			usDMATxMAXSize;			/* DMA���ͻ�������С(DMAʹ��) */
	uint8_t				*ucpDMATxCache;			/* DMA���ͻ�����(DMAʹ��) */

	/* ��־λ��Ϣ */
	uint8_t 			ucTxCplt;				/* ������ɱ�־(�ж�ʹ��) */
	uint8_t 			ucDMATxCplt;			/* DMA������ɱ�־(DMAʹ��) */
}tagUartTxInfo_T;

/* ����DMA���� */
typedef struct
{
	DMA_HandleTypeDef	tDMATx;					/* DMA���;�� */
	DMA_HandleTypeDef	tDMARx;					/* DMA���վ�� */

	bool 				bTxEnable;				/* ����ʹ�ܷ��� */
	bool				bRxEnable;				/* ����ʹ�ܷ��� */

	uint8_t				ucDMARxPriority;		/* DMA�����ж����ȼ���0-15 */
	uint8_t 			ucDMARxSubPriority;		/* DMA�����ж������ȼ���0-15 */

	uint8_t				ucDMATxPriority;		/* DMA�����ж����ȼ���0-15 */
	uint8_t 			ucDMATxSubPriority;		/* DMA�����ж������ȼ���0-15 */
}tagDMAUart_T;

/* �����豸�ṹ�� */
typedef struct
{
	UART_HandleTypeDef 	tUARTHandle;	/* STM32�ڲ������豸ָ�� */
	tagDMAUart_T		tUartDMA;		/* ����DMA���� */
	tagUartRxInfo_T		tRxInfo;		/* ���ڽ�����Ϣ */
	tagUartTxInfo_T		tTxInfo;		/* ���ڷ�����Ϣ */
	tagGPIO_T			tGPIO[2];		/* GPIO��� */
	uint8_t				ucPriority;		/* �ж����ȼ���0-15 */
	uint8_t 			ucSubPriority;	/* �ж������ȼ���0-15 */
}tagUART_T;

/* F1ϵ�д�����ӳ���
	USART1	full remap (TX/PB6, RX/PB7)
			no remap (TX/PA9, RX/PA10)
	USART2	full remap (CTS/PD3, RTS/PD4, TX/PD5, RX/PD6, CK/PD7)
			no remap (CTS/PA0, RTS/PA1, TX/PA2, RX/PA3, CK/PA4)
	USART3	full remap (TX/PD8,  RX/PD9,  CK/PD10, CTS/PD11, RTS/PD12)
			partial	remap (TX/PC10, RX/PC11, CK/PC12, CTS/PB13, RTS/PB14)
			no remap (TX/PB10, RX/PB11, CK/PB12, CTS/PB13, RTS/PB14)
*/

/* F1ϵ�д���DMAͨ����Ӧ 
	USART1	TX	DMA1_Channel4_IRQHandler
			RX	DMA1_Channel5_IRQHandler
	USART2	TX	DMA1_Channel7_IRQHandler
			RX	DMA1_Channel6_IRQHandler
	USART3	TX	DMA1_Channel2_IRQHandler
			RX	DMA1_Channel3_IRQHandler
	UART4	TX	DMA2_Channel5_IRQHandler
			RX	DMA2_Channel3_IRQHandler
*/

/* F4ϵ�д�����ӳ���
	|        | TXD1 | RXD1 | TXD2     | RXD2     |
	| ------ | ---- | ---- | -------- | -------- |
	| USART1 | PA9  | PA10 | PB6      | PB7      |
	| USART2 | PA2  | PA3  | PD5      | PD6      |
	| USART3 | PB10 | PB11 | PC10/PD8 | PC11/PD9 |
	| UART4  | PA0  | PA1  | PC10     | PC11     |
	| UART5  | PC12 | PD2  |          |          |
	| USART6 | PC6  | PC7  | PG14     | PG9      |
*/

/* F4ϵ�д���DMAͨ����Ӧ 
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
