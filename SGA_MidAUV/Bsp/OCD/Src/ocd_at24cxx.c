/****************************************************************************

* Sigma�Ŷ�

* �ļ���: ocd_at24c02.c

* ���ݼ�����at24c02ģ���ļ�

* �ļ���ʷ��

* �汾��	����		����		˵��

* 1.0.0a 	2020-03-14	���		�������ļ�
* 1.1.0a 	2020-03-29	���		�޸Ĳ����ı����޸�OCD_AT24CXX_WriteLenByte��OCD_AT24CXX_ReadLenByte����

****************************************************************************/
#include "ocd_at24cxx.h"

/**
 * @brief AT24CXXָ����ַ��ʼ��һ���ֽ�����
 * @param _tAT24CXX-AT24CXX�ṹ��
 * @param _usAddr-��ַ
 * @retval uint8_t-����������
 */
uint8_t S_AT24CXX_ReadOneByte(tagAT24CXX_T *_tAT24CXX, uint16_t _usAddr)
{				  
	uint8_t ucTemp = 0;

    Drv_IICSoft_Start(&_tAT24CXX->tIIC);  
	if(EE_TYPE>AT24C16)
	{
		Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, 0XA0);		/* ����д���� */
		Drv_IICSoft_WaitAck(&_tAT24CXX->tIIC);
		Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, _usAddr>>8);	/* ���͸ߵ�ַ */    
	}
	else 
		Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, 0XA0+((_usAddr/256)<<1));   /* ����������ַ0XA0,д���� */
	
	Drv_IICSoft_WaitAck(&_tAT24CXX->tIIC);
    Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, _usAddr % 256);  /* ���͵͵�ַ */
	Drv_IICSoft_WaitAck(&_tAT24CXX->tIIC);	    
	Drv_IICSoft_Start(&_tAT24CXX->tIIC);  	 	   
	Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, 0XA1);           /* �������ģʽ */			   
	Drv_IICSoft_WaitAck(&_tAT24CXX->tIIC);	 
    ucTemp = Drv_IICSoft_ReadByte(&_tAT24CXX->tIIC, 0);		   
    Drv_IICSoft_Stop(&_tAT24CXX->tIIC);						/* ����һ��ֹͣ���� */	
    
	return ucTemp;
}

/**
 * @brief AT24CXX��ʱ����
 * @param _ucMs-��ʱʱ��
 * @retval Null
 */
static void S_AT24CXX_Delay(uint8_t _ucMs)
{
	Drv_Delay_Ms(_ucMs);
}

/**
 * @brief AT24CXXָ����ַ��ʼд��һ���ֽ�����
 * @param _tAT24CXX-AT24CXX�ṹ��
 * @param _usAddr-��ַ
 * @param _ucData-д�������
 * @retval Null
 */
static void S_AT24CXX_WriteOneByte(tagAT24CXX_T *_tAT24CXX, uint16_t _usAddr, uint8_t _ucData)
{				   	  	    																 
    Drv_IICSoft_Start(&_tAT24CXX->tIIC); 
	if(EE_TYPE>AT24C16)
	{
		Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, 0XA0);	    //����д����
		Drv_IICSoft_WaitAck(&_tAT24CXX->tIIC);
		Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, _usAddr>>8);//���͸ߵ�ַ	  
	}
	else 
		Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, 0XA0 + ((_usAddr/256)<<1));   //����������ַ0XA0,д���� 	
	
	Drv_IICSoft_WaitAck(&_tAT24CXX->tIIC);	   
    Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, _usAddr % 256);   //���͵͵�ַ
	Drv_IICSoft_WaitAck(&_tAT24CXX->tIIC); 	 										  		   
	Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, _ucData);     //�����ֽ�							   
	Drv_IICSoft_WaitAck(&_tAT24CXX->tIIC);  		    	   
    Drv_IICSoft_Stop(&_tAT24CXX->tIIC);//����һ��ֹͣ���� 
	S_AT24CXX_Delay(10);	 
}

/**
 * @brief AT24CXXָ����ַ��ʼд�볤��ΪLen������
 * @param _tAT24CXX-AT24CXX�ṹ��
 * @param _usAddr-��ַ
 * @param _ulpData-���ݵ��׵�ַ
 * @param _ucLen-����
 * @retval Null
 */
void OCD_AT24CXX_WriteLenByte(tagAT24CXX_T *_tAT24CXX, uint16_t _usAddr, uint8_t *_ulpData, uint8_t _ucLen)
{  	
	uint8_t index;

	for(index = 0; index < _ucLen; index++)
	{
		S_AT24CXX_WriteOneByte(_tAT24CXX, _usAddr + index, _ulpData[index]);
	}												    
}

/**
 * @brief AT24CXXָ����ַ��ʼ��������ΪLen������
 * @param _tAT24CXX-AT24CXX�ṹ��
 * @param _usAddr-��ַ
 * @param _ulpData-���ݵ��׵�ַ
 * @param _ucLen-����
 * @retval Null
 */
void OCD_AT24CXX_ReadLenByte(tagAT24CXX_T *_tAT24CXX, uint16_t _usAddr, uint8_t *_ulpData, uint8_t _ucLen)
{  	
	uint8_t index;
	
	for(index = 0; index < _ucLen; index++)
	{
		_ulpData[index] = S_AT24CXX_ReadOneByte(_tAT24CXX, _usAddr + index); 	 				   
	}											    
}

/**
 * @brief AT24CXX���
 * @param _tAT24CXX-AT24CXX�ṹ��
 * @retval uint8_t-0-���ʧ�ܣ�1-���ɹ�
 */
uint8_t OCD_AT24CXX_Check(tagAT24CXX_T *_tAT24CXX)
{
	uint8_t ucTemp;
	
	ucTemp = S_AT24CXX_ReadOneByte(_tAT24CXX, 255);		/* ����ÿ�ο�����дAT24CXX */		   
	if(ucTemp == 0X55)
		return 1;
	else	/* �ų���һ�γ�ʼ������� */
	{
		S_AT24CXX_WriteOneByte(_tAT24CXX, 255,0X55);
	    ucTemp = S_AT24CXX_ReadOneByte(_tAT24CXX, 255);
		
		if(ucTemp == 0X55)
			return 1;
	}
	
	return 0;											  
}

/**
 * @brief AT24CXXָ����ַ��ʼ����ָ������������
 * @param _tAT24CXX-AT24CXX�ṹ��
 * @param _usAddr-��ַ
 * @param _ucpBuffer-�����ַ
 * @param _usNum-����
 * @retval Null
 */
void OCD_AT24CXX_Read(tagAT24CXX_T *_tAT24CXX, uint16_t _usAddr, uint8_t *_ucpBuffer, uint16_t _usNum)
{
	/* ���δ��ȡ�� */
	while(_usNum)
	{
		*_ucpBuffer ++= S_AT24CXX_ReadOneByte(_tAT24CXX, _usAddr++);	
		_usNum--;
	}
}  

/**
 * @brief AT24CXXָ����ַ��ʼд��ָ������������
 * @param _tAT24CXX-AT24CXX�ṹ��
 * @param _usAddr-��ַ
 * @param _ucpBuffer-�����ַ
 * @param _usNum-����
 * @retval Null
 */
void OCD_AT24CXX_Write(tagAT24CXX_T *_tAT24CXX, uint16_t _usAddr, uint8_t *_ucpBuffer, uint16_t _usNum)
{
	/* ���δд���� */
	while(_usNum--)
	{
		S_AT24CXX_WriteOneByte(_tAT24CXX, _usAddr,*_ucpBuffer);
		_usAddr++;
		_ucpBuffer++;
	}
}

/**
 * @brief AT24CXX��ʼ������
 * @param _tAT24CXX-AT24CXX�ṹ��
 * @retval Null
 */
void OCD_AT24CXX_Init(tagAT24CXX_T *_tAT24CXX)
{
	Drv_IICSoft_Init(&_tAT24CXX->tIIC);
}
