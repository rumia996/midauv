/****************************************************************************

* Sigma�Ŷ�

* �ļ���: ocd_ds3231.c

* ���ݼ�����ds3231ģ���ļ�

* �ļ���ʷ��

* �汾��	����		����		˵��

* 1.0.0a 	2020-02-22	���		�������ļ�
* 1.0.1a 	2020-03-10	���		�޸�OCD_DS3231_TimeGetASCII�������߼����������µ�ָ���ܷ�����
									�޸�OCD_DS3231_TimeGetASCII��OCD_DS3231_TimeGetDec�ӿ��Լ�ִ���߼�
									�޸Ĳ��ֺ����ӿ����ƣ���ʮ���������޸�Ϊ��ȷ��ʮ����������
									�޸�ȫ���������ײ������ͣ�ȫ������DS3231���Խṹ��

****************************************************************************/
#include "ocd_ds3231.h"

/**
 * @brief DS3231�ڲ���ʱ����
 * @param _ulUs-��ʱ����λ��us��
 * @retval	NULL
 */
static void S_DS3231_DelayUs(uint8_t _ulUs)
{
	int i,j;

	for(i = 0; i < _ulUs; i++)
		for(j = 0; j < 12; j++);
}

/**
 * @brief ��DS3231������
 * @param _tDS3231-ds3231�ṹ��ָ��
 * @param _ucDevAddr-��ַ
 * @param _ucReg-�Ĵ���
 * @retval uint8_t-0-��ʧ�ܣ�����-�Ĵ�������
 */
static uint8_t S_DS3231_ReadByte(tagDS3231_T *_tDS3231, uint8_t _ucDevAddr, uint8_t _ucReg)
{
	uint8_t res = 0;
	
	Drv_IICSoft_Start(&_tDS3231->tIICSoft);
	
	Drv_IICSoft_SendByte(&_tDS3231->tIICSoft,_ucDevAddr);
	if(Drv_IICSoft_WaitAck(&_tDS3231->tIICSoft)!=0)
		goto cmd_fail;
	
	Drv_IICSoft_SendByte(&_tDS3231->tIICSoft,_ucReg);
	if(Drv_IICSoft_WaitAck(&_tDS3231->tIICSoft)!=0)
		goto cmd_fail;
	
	Drv_IICSoft_Start(&_tDS3231->tIICSoft);
	S_DS3231_DelayUs(1);
	Drv_IICSoft_SendByte(&_tDS3231->tIICSoft,_ucDevAddr|0x01);
	if(Drv_IICSoft_WaitAck(&_tDS3231->tIICSoft)!=0)
		goto cmd_fail;
	
	res = Drv_IICSoft_ReadByte(&_tDS3231->tIICSoft,1);
	
	Drv_IICSoft_Stop(&_tDS3231->tIICSoft);
	
	return res;
	
	cmd_fail:
	
	Drv_IICSoft_Stop(&_tDS3231->tIICSoft);
	return 0;
}

/**
 * @brief ��DS3231д����
 * @param _tDS3231-ds3231�ṹ��ָ��
 * @param _ucDevAddr-��ַ
 * @param _ucReg-�Ĵ���
 * @param _ucData-����
 * @retval uint8_t-0-дʧ�ܣ�1-д�ɹ�
 */
static uint8_t S_DS3231_WriteByte(tagDS3231_T *_tDS3231, uint8_t _ucDevAddr, uint8_t _ucReg, uint8_t _ucData)
{
	Drv_IICSoft_Start(&_tDS3231->tIICSoft);
	_ucDevAddr &= 0xFE;
		
	Drv_IICSoft_SendByte(&_tDS3231->tIICSoft, _ucDevAddr);
	if(Drv_IICSoft_WaitAck(&_tDS3231->tIICSoft)!=0)
		goto cmd_fail;
		
	Drv_IICSoft_SendByte(&_tDS3231->tIICSoft, _ucReg);
	if(Drv_IICSoft_WaitAck(&_tDS3231->tIICSoft)!=0)
		goto cmd_fail;
	
	Drv_IICSoft_SendByte(&_tDS3231->tIICSoft, _ucData);
	if(Drv_IICSoft_WaitAck(&_tDS3231->tIICSoft)!=0)
		goto cmd_fail;
	
	Drv_IICSoft_Stop(&_tDS3231->tIICSoft);
	
	return(1);     
	
	cmd_fail:

	Drv_IICSoft_Stop(&_tDS3231->tIICSoft);
	return(0);
}

/**
 * @brief �ַ���ת��ʮ������
 * @param _ucpSrc-�ַ���ַ
 * @param _ucLen-�ַ�����
 * @retval uint32_t-ʮ������
 */
static uint32_t S_DS3231_StringToHex(uint8_t *_ucpSrc, uint8_t _ucLen)
{
	uint8_t index;
	uint32_t ulRes;

	for(index = 0; index < _ucLen; index++)
		ulRes += (_ucpSrc[index] - '0') << (_ucLen - index);

	return ulRes;
}

/**
 * @brief ��DS3231ʱ�����ã�ʮ�����ƣ�
 * @param _tDS3231-ds3231�ṹ��ָ��
 * @param _tTime-ʱ��ṹ���ַָ��
 * @retval uint8_t-0-����ʧ�ܣ�1-���óɹ�
 */
uint8_t OCD_DS3231_TimeSetHex(tagDS3231_T *_tDS3231, tagDS3231Time_T *_tTime)
{
	do
	{
		/* ������ */
		if(S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_YEAR,(_tTime->ucYear)) == 0)
			return 0;
		
		/* ������ */
		if(S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_MONTH,(_tTime->ucMonth)) == 0)
			return 0;
		
		/* ������ */
		if(S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_DAY,(_tTime->ucDate)) == 0)
			return 0;
		
		/* ����ʱ */
		if(S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_HOUR,(_tTime->ucHour)) == 0)
			return 0;
		
		/* ���÷� */
		if(S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_MINUTE,(_tTime->ucMinute)) == 0)
			return 0;
		
		/* ������ */
		if(S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_SECOND,(_tTime->ucSecond)) == 0)
			return 0;
		
		/* �������ڼ� */
		if(S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_WEEK,(_tTime->ucWeek)) == 0)
			return 0;
	}while(0);
	
	return 1;
}

/**
 * @brief ��DS3231ʱ�����ã�ASCII��
 * @param _tDS3231-ds3231�ṹ��ָ��
 * @param _tTime-ʱ��ṹ���ַָ��
 * @retval uint8_t-0-����ʧ�ܣ�1-���óɹ�
 */
uint8_t OCD_DS3231_TimeSetASCII(tagDS3231_T *_tDS3231, tagDS3231TimeASCII_T *_tTime)
{
	volatile tagDS3231Time_T tTime;
	
	tTime.ucYear = S_DS3231_StringToHex(_tTime->ucYear, 2);
	tTime.ucMonth = S_DS3231_StringToHex(_tTime->ucMonth, 2);
	tTime.ucDate = S_DS3231_StringToHex(_tTime->ucDate, 2);
	tTime.ucHour = S_DS3231_StringToHex(_tTime->ucHour, 2);
	tTime.ucMinute = S_DS3231_StringToHex(_tTime->ucMinute, 2);
	tTime.ucSecond = S_DS3231_StringToHex(_tTime->ucSecond, 2);
	
	return 1;
}

/**
 * @brief ��DS3231ʱ���ȡ��ʮ�����ƣ�
 * @param _tDS3231-ds3231�ṹ��ָ��
 * @param _tTime-ʱ��ṹ���ַָ��
 * @retval uint8_t-0-����ʧ�ܣ�1-���óɹ�
 */
uint8_t OCD_DS3231_TimeGetHex(tagDS3231_T *_tDS3231, tagDS3231Time_T *_tTime)
{	
	_tTime->ucYear = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_YEAR);		/* �� */
	_tTime->ucMonth = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_MONTH);	/* �� */
	_tTime->ucDate = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_DAY);		/* �� */
	_tTime->ucHour = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_HOUR);		/* ʱ */
	_tTime->ucMinute = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_MINUTE);	/* �� */
	_tTime->ucSecond = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_SECOND);	/* �� */
	_tTime->ucWeek = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_WEEK);		/* ���ڼ� */
	
	return 1;
}

/**
 * @brief ��DS3231ʱ���ȡ��ASCII��
 * @param _tDS3231-ds3231�ṹ��ָ��
 * @param _tTime-ʱ��ṹ���ַָ��
 * @retval uint8_t-0-����ʧ�ܣ�1-���óɹ�
 */
uint8_t OCD_DS3231_TimeGetASCII(tagDS3231_T *_tDS3231, tagDS3231TimeASCII_T *_tTime)
{
	tagDS3231Time_T tTime;

	OCD_DS3231_TimeGetHex(_tDS3231, &tTime);
	
	_tTime->ucYear[0] = (tTime.ucYear >> 4) + '0';
	_tTime->ucYear[1] = (tTime.ucYear & 0x0f) + '0';
	_tTime->ucMonth[0] = (tTime.ucMonth >> 4) + '0';
	_tTime->ucMonth[1] = (tTime.ucMonth & 0x0f) + '0';
	_tTime->ucDate[0] = (tTime.ucDate >> 4) + '0';
	_tTime->ucDate[1] = (tTime.ucDate & 0x0f) + '0';
	_tTime->ucHour[0] = (tTime.ucHour >> 4) + '0';
	_tTime->ucHour[1] = (tTime.ucHour & 0x0f) + '0';
	_tTime->ucMinute[0] = (tTime.ucMinute >> 4) + '0';
	_tTime->ucMinute[1] = (tTime.ucMinute & 0x0f) + '0';
	_tTime->ucSecond[0] = (tTime.ucSecond >> 4) + '0';
	_tTime->ucSecond[1] = (tTime.ucSecond & 0x0f) + '0';
	_tTime->ucWeek[0] = (tTime.ucWeek >> 4) + '0';
	_tTime->ucWeek[1] = (tTime.ucWeek & 0x0f) + '0';
	
	return 1;
}

/**
 * @brief DS3231�������ã�ASCII��
 * @param _tDS3231-ds3231�ṹ��ָ��
 * @param _tTime-ʱ��ṹ���ַָ��
 * @retval Null
 */
void OCD_DS3231_Alarm1ConfigASCII(tagDS3231_T *_tDS3231, tagDS3231TimeASCII_T *_tTime)
{	
	uint8_t ucRes;
	uint8_t ucHour, ucMin, ucSec = 0;
	
	ucHour = ((_tTime->ucHour[0] - '0') << 4) | (_tTime->ucHour[1] - '0');
	ucMin = ((_tTime->ucMinute[0] - '0') << 4) | (_tTime->ucMinute[1] - '0');
	ucSec = ((_tTime->ucSecond[0] - '0') << 4) | (_tTime->ucSecond[1] - '0');
	
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_ALARM1WEEK,0X80);
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_ALARM1HOUR,(ucHour));
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_ALARM1MINUTE,(ucMin));
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_ALARM1SECOND,(ucSec));
	
	ucRes = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_CONTROL);
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_CONTROL, ucRes|0x05);
	
	ucRes = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_STATUS);
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_STATUS, ucRes&0xfC);	/* ����1��2��־λ���� */
	
	//DS3231_Interrupt_ENABLE(Interrupt_EXTI_X_IRQn,ENABLE);
}

/**
 * @brief DS3231�������ã�Hex��
 * @param _tDS3231-iic�ṹ��ָ��
 * @param _tTime-ʱ��ṹ���ַָ��
 * @retval Null
 */
void OCD_DS3231_Alarm1ConfigHex(tagDS3231_T *_tDS3231, tagDS3231Time_T *_tTime)
{	
	uint8_t ucRes;
	
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_ALARM1WEEK,0X80);
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_ALARM1HOUR,(_tTime->ucHour));
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_ALARM1MINUTE,(_tTime->ucMinute));
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_ALARM1SECOND,(_tTime->ucSecond));
	
	ucRes = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_CONTROL);
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_CONTROL, ucRes|0x05);
	
	ucRes = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_STATUS);
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_STATUS, ucRes&0xfC);	/* ����1��2��־λ���� */
	
	//DS3231_Interrupt_ENABLE(Interrupt_EXTI_X_IRQn,ENABLE);
}

/**
 * @brief DS3231��ʼ��
 * @param _tDS3231-ds3231�ṹ��ָ��
 * @retval Null
 */
void OCD_DS3231_Init(tagDS3231_T *_tDS3231)
{
	Drv_IICSoft_Init(&_tDS3231->tIICSoft);
	Drv_Delay_Ms(200);
	
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_CONTROL, 0x04);
	Drv_Delay_Ms(200);
	
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_STATUS, 0);
	Drv_Delay_Ms(200);
}
