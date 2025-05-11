/****************************************************************************

* Sigma�Ŷ�

* �ļ���: drv_hal_iic_soft.c

* ���ݼ��������iicģ�������ļ�

* �ļ���ʷ��

* �汾��	����		����		˵��

* 1.0.0a 	2020-02-22	���		�������ļ�
* 1.0.1a 	2020-02-22	���		�޸�Drv_IICSoft_ReadByte������ACK��NACK���������

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_IIC_SOFT_ENABLE

/**
 * @brief IIC��΢����ʱ����
 * @param _ucUs ��Ҫ��ʱ����ֵ
 * @retval Null
*/
static void S_IICSoft_DelayUs(uint8_t _ucUs)
{
	Drv_Delay_Us(_ucUs);
}

/**
 * @brief IIC��SCL������
 * @param _tIIC-IIC�ṹ��ָ��
 * @retval Null
*/
static void S_IICSoft_SCL_H(tagIICSoft_T *_tIIC)
{
	HAL_GPIO_WritePin(_tIIC->tIICSoft[0].tGPIOPort,_tIIC->tIICSoft[0].tGPIOInit.Pin,GPIO_PIN_SET);
}

/**
 * @brief IIC��SCL������
 * @param _tIIC-IIC�ṹ��ָ��
 * @retval Null
*/
static void S_IICSoft_SCL_L(tagIICSoft_T *_tIIC)
{
	HAL_GPIO_WritePin(_tIIC->tIICSoft[0].tGPIOPort,_tIIC->tIICSoft[0].tGPIOInit.Pin,GPIO_PIN_RESET);
}

/**
 * @brief IIC��SDA������
 * @param _tIIC-IIC�ṹ��ָ��
 * @retval Null
*/
static void S_IICSoft_SDA_H(tagIICSoft_T *_tIIC)
{
	HAL_GPIO_WritePin(_tIIC->tIICSoft[1].tGPIOPort,_tIIC->tIICSoft[1].tGPIOInit.Pin,GPIO_PIN_SET);
}

/**
 * @brief IIC��SDA������
 * @param _tIIC-IIC�ṹ��ָ��
 * @retval Null
*/
static void S_IICSoft_SDA_L(tagIICSoft_T *_tIIC)
{
	HAL_GPIO_WritePin(_tIIC->tIICSoft[1].tGPIOPort,_tIIC->tIICSoft[1].tGPIOInit.Pin,GPIO_PIN_RESET);
}

/**
 * @brief IIC��SDA�߶�ȡ
 * @param _tIIC-IIC�ṹ��ָ��
 * @retval GPIO_PinState
*/
static GPIO_PinState S_IICSoft_READ_SDA(tagIICSoft_T *_tIIC)
{
	return HAL_GPIO_ReadPin(_tIIC->tIICSoft[1].tGPIOPort,_tIIC->tIICSoft[1].tGPIOInit.Pin);
}

/**
 * @brief IIC��SDA�����ģʽ
 * @param _tIIC-IIC�ṹ��ָ��
 * @retval Null
*/
static void S_IICSoft_SDA_OutputMode(tagIICSoft_T *_tIIC)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Pin = _tIIC->tIICSoft[1].tGPIOInit.Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(_tIIC->tIICSoft[1].tGPIOPort, &GPIO_InitStruct);
}

/**
 * @brief IIC��SDA������ģʽ
 * @param _tIIC-IIC�ṹ��ָ��
 * @retval Null
*/
static void S_IICSoft_SDA_InputMode(tagIICSoft_T *_tIIC)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Pin = _tIIC->tIICSoft[1].tGPIOInit.Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(_tIIC->tIICSoft[1].tGPIOPort,&GPIO_InitStruct);
}

/**
 * @brief ģ��IIC����Ӧ��
 * @param _tIIC-IIC�ṹ��ָ��
 * @retval Null 
*/
static void S_IICSoft_SendAck(tagIICSoft_T *_tIIC)
{
	S_IICSoft_SCL_L(_tIIC);
	
	S_IICSoft_SDA_OutputMode(_tIIC);
	
	S_IICSoft_SDA_L(_tIIC);
	S_IICSoft_DelayUs(2);
	
	S_IICSoft_SCL_H(_tIIC);
	S_IICSoft_DelayUs(2);

	S_IICSoft_SCL_L(_tIIC);
}

/**
 * @brief ģ��IIC������Ӧ��
 * @param _tIIC-IIC�ṹ��ָ��
 * @retval Null 
*/
static void S_IICSoft_SendNAck(tagIICSoft_T *_tIIC)
{
	S_IICSoft_SCL_L(_tIIC);
	
	S_IICSoft_SDA_OutputMode(_tIIC);

	S_IICSoft_SDA_H(_tIIC);
	S_IICSoft_DelayUs(2);

	S_IICSoft_SCL_H(_tIIC);
	S_IICSoft_DelayUs(2);

	S_IICSoft_SCL_L(_tIIC);
}	

/**
 * @brief ģ��IIC������ʼ�ź�
 * @param _tIIC-IIC�ṹ��ָ��
 * @retval Null 
*/
void Drv_IICSoft_Start(tagIICSoft_T *_tIIC)
{
	S_IICSoft_SDA_OutputMode(_tIIC);	/* sda����� */
	
	S_IICSoft_SDA_H(_tIIC);	  	  
	S_IICSoft_SCL_H(_tIIC);
	S_IICSoft_DelayUs(4);
	
 	S_IICSoft_SDA_L(_tIIC);
	S_IICSoft_DelayUs(4);
	S_IICSoft_SCL_L(_tIIC);
}	

/**
 * @brief ģ��IIC����ֹͣ�ź�
 * @param _tIIC-IIC�ṹ��ָ��
 * @retval Null 
*/
void Drv_IICSoft_Stop(tagIICSoft_T *_tIIC)
{
	S_IICSoft_SDA_OutputMode(_tIIC);
	
	S_IICSoft_SCL_L(_tIIC);
	S_IICSoft_SDA_L(_tIIC);
 	S_IICSoft_DelayUs(4);
	
	S_IICSoft_SCL_H(_tIIC); 
	S_IICSoft_SDA_H(_tIIC);
	S_IICSoft_DelayUs(4);							   	
}

/**
 * @brief ģ��IIC�ȴ�Ӧ��
 * @param _tIIC-IIC�ṹ��ָ��
 * @retval Null 
*/
uint8_t Drv_IICSoft_WaitAck(tagIICSoft_T *_tIIC)
{
	uint8_t ucErrTime=0;

	S_IICSoft_SDA_InputMode(_tIIC);   /* SDA����Ϊ���� */  
	
	S_IICSoft_SDA_H(_tIIC);
	S_IICSoft_DelayUs(1);	
	S_IICSoft_SCL_H(_tIIC);
	S_IICSoft_DelayUs(1);	 			/*SCLΪ�� ���ж�ȡ���� */  

	while(S_IICSoft_READ_SDA(_tIIC))
	{
		ucErrTime++;
		if(ucErrTime > 250)
		{
			Drv_IICSoft_Stop(_tIIC);
			return 1;
		}
	}

	S_IICSoft_SCL_L(_tIIC);			/* ʱ�����0 */ 	  	
	return 0;  
} 				 				     

/**
 * @brief ģ��IIC����һ���ֽ�
 * @param _tIIC-IIC�ṹ��ָ��
 * @param _ucTxData-���͵õ��ֽ�����
 * @retval Null 
*/
void Drv_IICSoft_SendByte(tagIICSoft_T *_tIIC, uint8_t _ucTxData)
{                        
    uint8_t index;   

	S_IICSoft_SDA_OutputMode(_tIIC); 	

    S_IICSoft_SCL_L(_tIIC);/* ����ʱ�ӿ�ʼ���ݴ��� */
	
    for(index = 0;index < TYPE_NUM;index++)
    {     
		if(_ucTxData & 0x80)
			S_IICSoft_SDA_H(_tIIC);
		else
			S_IICSoft_SDA_L(_tIIC);
			
		_ucTxData <<= 1;

		/* ��TEA5767��������ʱ���Ǳ���� */
		S_IICSoft_DelayUs(2);   
		S_IICSoft_SCL_H(_tIIC);
		S_IICSoft_DelayUs(2); 
		S_IICSoft_SCL_L(_tIIC);	
		S_IICSoft_DelayUs(2);
    }	 
} 	    

/**
 * @brief ģ��IIC��ȡһ���ֽ�
 * @param _tIIC-IIC�ṹ��ָ��
 * @param _ucAck-ack=1ʱ������NACK��ack=0������ACK
 * @retval Null 
*/
uint8_t Drv_IICSoft_ReadByte(tagIICSoft_T *_tIIC, uint8_t _ucAck)
{
	uint8_t index,ucRxData=0;

	S_IICSoft_SDA_InputMode(_tIIC);		/* SDA����Ϊ���� */
   
	for(index = 0;index < TYPE_NUM;index++ )
	{
		S_IICSoft_SCL_L(_tIIC); 
		S_IICSoft_DelayUs(2);
		S_IICSoft_SCL_H(_tIIC);
		
		ucRxData <<= 1;
		
		if(S_IICSoft_READ_SDA(_tIIC))
		   ucRxData++;
//		else
//			receive=receive&0xFE;
		
		S_IICSoft_DelayUs(1); 
    }	

    if (_ucAck)
        S_IICSoft_SendAck(_tIIC); 		/* ����ACK */
    else
		S_IICSoft_SendNAck(_tIIC);		/* ����nACK */

    return ucRxData;
}

/**
 * @brief ģ��IIC��ʼ��
 * @param _tIIC-IIC�ṹ��ָ��
 * @retval Null 
*/
void Drv_IICSoft_Init(tagIICSoft_T *_tIIC)
{
	Drv_GPIO_Init(_tIIC->tIICSoft, IIC_GPIO_NUM);
}

#endif
