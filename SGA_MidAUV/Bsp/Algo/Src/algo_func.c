/****************************************************************************

* Sigma�Ŷ�

* �ļ���: algo_func.c

* ���ݼ�����SGA���ж��ⳣ�ú����ļ�

* �ļ���ʷ��

* �汾��		����	  ����				˵��
*  2.1   	2023-03-22	�����		����ļ�λ��

* 1.1.6 	2022-09-24	�����		����Hex��Dec��ת������

* 1.0.1a 	2020-03-15	���		����SGA_StringToHex����
									�޸�SGA_Function_Query_Subִ���߼��������ͷ���ֵ

* 1.0.0a 	2020-02-22	���		�������ļ�

****************************************************************************/
#include "algo_func.h"

/**
 * @brief ��ѯһ���ַ���a���Ƿ������һ���ַ���b
 * @param _ucpStrA-�ַ���a��ַָ��
 * @param _ucpStrB-�ַ���b��ַָ��
 * @retval tag_StringInfo
*/
tag_StringInfo Algo_Function_Query_Sub(uint8_t *_ucpStrA, uint8_t *_ucpStrB)
{
	uint8_t *ucpSrc; 
	uint8_t *ucpSub;
	uint16_t usNum = 1;
	tag_StringInfo pos = {0,0};
	
	if(!_ucpStrA||!_ucpStrB)
	{
	   return pos;
	}
	/* ����src�ַ��� */
	while(*_ucpStrA)
	{ 
		/* ���������Ӵ� */
		ucpSrc = _ucpStrA;
		ucpSub = _ucpStrB;
		pos.ulIndex++;
		while(*ucpSrc++ == *ucpSub++)
		{
			usNum += 1;
			if(*ucpSub == ':')	/* ����sub�Ľ���λ�ã�����srcλ�� */
			{
				pos.ucpAddr = ucpSrc + 2;
				pos.ulIndex += usNum;
				return pos;
			}	
		}
		usNum = 1;
		_ucpStrA++;	
	}
	
	return pos; 
}

#define SGA_INSERT_LENGTH	30
static char ucpInsertBuffer[SGA_INSERT_LENGTH] = {0};
/**
 * @brief ���ַ���1�е�ָ��λ�ò����ַ���2(ע�ⳤ��)
 * @param _ucpStrA-�ַ���1��ַָ��
 * @param _ucpStrB-�ַ���2��ַָ��
 * @param _usOffset-ƫ����
 * @retval Null
*/
void Algo_Function_Insert_Sub(uint8_t *_ucpStrA, uint8_t *_ucpStrB, uint16_t _usOffset)
{
	uint8_t ucLen = 0;
	
	ucLen = strlen((char*)_ucpStrB);
	
	if(_usOffset)
	{
		memcpy(ucpInsertBuffer, _ucpStrA, _usOffset);
		memcpy(ucpInsertBuffer + _usOffset, _ucpStrB, ucLen);
		memcpy(ucpInsertBuffer + _usOffset + ucLen, _ucpStrA + _usOffset, strlen((char*)_ucpStrA) - _usOffset);
	}
	else
	{
		memcpy(ucpInsertBuffer, _ucpStrB, ucLen);
		memcpy(ucpInsertBuffer + ucLen, _ucpStrA, strlen((char*)_ucpStrA));
	}
	
	memcpy(_ucpStrA, ucpInsertBuffer, strlen((char*)_ucpStrA) + ucLen);
	memset(ucpInsertBuffer, 0, SGA_INSERT_LENGTH);
}

/**
 * @brief ת����uint8_t
 * @param _ucpBuffer-�ַ���1��ַָ��
 * @param _ucpSrcBuffer-�ַ���2��ַָ��
 * @retval Null
*/
static void S_SGA_Function_InvertUint8(uint8_t *_ucpBuffer, uint8_t *_ucpSrcBuffer)  
{  
    int i;  
    uint8_t ucpTmp[4];  
    ucpTmp[0] = 0;

    for(i=0;i< 8;i++)  
    {  
      if(_ucpSrcBuffer[0]& (1 << i))  
        ucpTmp[0] |= 1<<(7-i);  
    }  
    _ucpBuffer[0] = ucpTmp[0];       
}  

/**
 * @brief ת����uint16_t
 * @param _ucpBuffer-�ַ���1��ַָ��
 * @param _ucpSrcBuffer-�ַ���2��ַָ��
 * @retval Null
*/
static void S_SGA_Function_InvertUint16(uint16_t *_ucpBuffer, uint16_t *_ucpSrcBuffer)  
{  
    int i;  
    uint16_t ucpTmp[4];  
    ucpTmp[0] = 0;

    for(i=0;i< 16;i++)  
    {  
      if(_ucpSrcBuffer[0] & (1 << i))  
        ucpTmp[0] |= 1<<(15 - i);  
    }  
    _ucpBuffer[0] = ucpTmp[0];  
} 

/**
 * @brief Crc16λУ�飨modbus��
 * @param _ucpMsg-��ҪУ������������ַָ��
 * @param _usDataLen-��ҪУ����������鳤��
 * @retval uint16_t-2�ֽ�CRCУ����
*/
uint16_t Algo_Function_Crc16_Modbus(uint8_t *_ucpMsg, uint16_t _usDataLen)
{
	uint16_t usCRCin = 0xFFFF;  
	uint16_t usCPoly = 0x8005;  
	uint8_t ucChar = 0;  
		
	while (_usDataLen--)     
	{  
		ucChar = *(_ucpMsg++);  
		S_SGA_Function_InvertUint8(&ucChar,&ucChar);  
		usCRCin ^= (ucChar << 8); 
		
		for(int i = 0;i < 8;i++)  
		{  
			if(usCRCin & 0x8000)  
				usCRCin = (usCRCin << 1) ^ usCPoly;  
			else  
				usCRCin = usCRCin << 1;  
		}  
	}  
	
	S_SGA_Function_InvertUint16(&usCRCin,&usCRCin); 
	
	return (usCRCin);
}

/**
 * @brief �ַ���תHex
 * @param _ucpStrA-��Ҫת�����ַ���
 * @param _ucLen-ת������
 * @retval uint32_t-ת����Ľ��
*/
uint32_t Algo_StringToHex(uint8_t *_ucpStrA, uint8_t _ucLen)
{
	uint8_t index;
	uint32_t ulRes = 0;

	for(index = 0; index < _ucLen; index++)
		ulRes += (_ucpStrA[index] - '0') << ((_ucLen - index - 1) * 4);

	return ulRes;
}

/**
 * @brief ʮ����תHex��ʾ����23ת��Ϊ0x23��
 * @param _ucDec-��Ҫת����ʮ������
 * @retval uint8_t-ת�����ʮ��������
*/
uint8_t Algo_DecToHex(uint8_t _ucDec)
{
	uint8_t res;

	res = _ucDec + (_ucDec / 10) * 6;	
	return res;
}

/**
 * @brief ʮ������תDec��ʾ����0x37ת��Ϊ55��
 * @param _ucHex-��Ҫת����ʮ��������
 * @retval uint8_t-ת�����ʮ������
*/
uint8_t Algo_HexToDec(uint8_t _ucHex)
{
	uint8_t dec;
	uint8_t num1 = _ucHex >> 4;
	uint8_t num2 = _ucHex & 0x0F;
	
	dec = num1*16 + num2;
	return dec;
}
