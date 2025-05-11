/****************************************************************************

* Sigma�Ŷ�

* �ļ���: ocd_ms5837.c

* ���ݼ�����ms5837ģ���ļ�

* �ļ���ʷ��

* �汾��		����	  ����		˵��
*  2.7  	2023-07-19	�����	 �������ļ�

****************************************************************************/
#include "ocd_ms5837.h"

/*
dT ʵ�ʺͲο��¶�֮��Ĳ���
TEMP һ�ײ�������¶�ֵ
*/
int32_t dT = 0,TEMP = 0;

/*
OFF ʵ���¶Ȳ���
SENS ʵ���¶�������
*/
int64_t OFF = 0,SENS = 0;

/* ����ѹ��ֵ,�����¶�ֵ */
uint32_t D1_Press = 0,D2_Temp = 0;	

/* �м������� */
int32_t OFFi = 0,SENSi = 0,Ti = 0;
int64_t OFF2 = 0,SENS2 = 0;

/**
 * @brief MS5837��λ����
 * @param _tMS5837-MS5837���ָ��
 * @retval Null
*/
static void S_MS5837_Reset(tagMS5837_T *_tMS5837)
{
    Drv_IICSoft_Start(&_tMS5837->tIIC);
    Drv_IICSoft_SendByte(&_tMS5837->tIIC,MS5837_WRITEADDR);
    Drv_IICSoft_WaitAck(&_tMS5837->tIIC);
    Drv_IICSoft_SendByte(&_tMS5837->tIIC,MS5837_RESET);
    Drv_IICSoft_WaitAck(&_tMS5837->tIIC);
    Drv_IICSoft_Stop(&_tMS5837->tIIC);
}

/**
 * @brief MS5837��ȡУ�����ݺ���
 * @param _tMS5837-MS5837���ָ��
 * @retval Null
*/
static void S_MS5837_GetCalibrationData(tagMS5837_T *_tMS5837)
{
    uint8_t ucHbyte,ucLbyte;

    S_MS5837_Reset(_tMS5837);
    Drv_Delay_Ms(20);

    /* ��ȡ����У��ֵ */
    for(int index = 0;index < 7; index++)
    {
        Drv_IICSoft_Start(&_tMS5837->tIIC);
        Drv_IICSoft_SendByte(&_tMS5837->tIIC,MS5837_WRITEADDR);
        Drv_IICSoft_WaitAck(&_tMS5837->tIIC);
        Drv_IICSoft_SendByte(&_tMS5837->tIIC,MS5837_PROM_READ + (index*2));
        Drv_IICSoft_WaitAck(&_tMS5837->tIIC);
        Drv_IICSoft_Stop(&_tMS5837->tIIC);

        Drv_Delay_Us(5);

        Drv_IICSoft_Start(&_tMS5837->tIIC);
        Drv_IICSoft_SendByte(&_tMS5837->tIIC,MS5837_READADDR);
        Drv_Delay_Us(5);
        Drv_IICSoft_WaitAck(&_tMS5837->tIIC);
        ucHbyte = Drv_IICSoft_ReadByte(&_tMS5837->tIIC,1);
        Drv_Delay_Us(5);
        ucLbyte = Drv_IICSoft_ReadByte(&_tMS5837->tIIC,0);
        Drv_IICSoft_Stop(&_tMS5837->tIIC);

        _tMS5837->usaCaliData[index] = (((uint16_t)ucHbyte << 8) | ucLbyte);
    }
}

/**
 * @brief MS5837����ת������
 * @param _tMS5837-MS5837���ָ��
 * @param _ucOSR-�ֱ��ʲ���
 * @retval Null
*/
static uint32_t S_MS5837_GetConversion(tagMS5837_T *_tMS5837, uint8_t _ucOSR)
{
    uint32_t ulConversion = 0;
    uint8_t ucaAdcValue[3];

    Drv_IICSoft_Start(&_tMS5837->tIIC);
    Drv_IICSoft_SendByte(&_tMS5837->tIIC,MS5837_WRITEADDR);
    Drv_IICSoft_WaitAck(&_tMS5837->tIIC);
    Drv_IICSoft_SendByte(&_tMS5837->tIIC,_ucOSR);
    Drv_IICSoft_WaitAck(&_tMS5837->tIIC);
    Drv_IICSoft_Stop(&_tMS5837->tIIC);

    Drv_Delay_Ms(10);

    Drv_IICSoft_Start(&_tMS5837->tIIC);
    Drv_IICSoft_SendByte(&_tMS5837->tIIC,MS5837_WRITEADDR);
    Drv_IICSoft_WaitAck(&_tMS5837->tIIC);
    Drv_IICSoft_SendByte(&_tMS5837->tIIC,MS5837_ADC_READ);
    Drv_IICSoft_WaitAck(&_tMS5837->tIIC);
    Drv_IICSoft_Stop(&_tMS5837->tIIC);

    Drv_IICSoft_Start(&_tMS5837->tIIC);
    Drv_IICSoft_SendByte(&_tMS5837->tIIC,MS5837_READADDR);
    Drv_IICSoft_WaitAck(&_tMS5837->tIIC);
    ucaAdcValue[0] = Drv_IICSoft_ReadByte(&_tMS5837->tIIC,1);    /* ��ACK�Ķ�����  bit 23-16 */
    ucaAdcValue[1] = Drv_IICSoft_ReadByte(&_tMS5837->tIIC,1);    /* ��ACK�Ķ�����  bit 8-15 */
    ucaAdcValue[2] = Drv_IICSoft_ReadByte(&_tMS5837->tIIC,0);    /* ��NACK�Ķ����� bit 0-7 */
    Drv_IICSoft_Stop(&_tMS5837->tIIC);

    ulConversion = (uint32_t)ucaAdcValue[0];
    ulConversion = (ulConversion << 8) + (uint32_t)ucaAdcValue[1];
    ulConversion = (ulConversion << 8) + (uint32_t)ucaAdcValue[2];

    return ulConversion;
}

/**
 * @brief MS5837 CRC4У�麯��
 * @param _tMS5837-MS5837���ָ��
 * @retval uint8_t 1-У��ɹ� 0-У��ʧ��
*/
static uint8_t S_MS5837_CRC4(tagMS5837_T *_tMS5837)
{
	int32_t cnt;
	int32_t t;
	uint32_t n_rem = 0;
	uint8_t n_bit;	
	uint8_t  a = 0;
	uint8_t  b = 0;
	uint32_t n_prom[8];
	
	for(t = 0;t < 7; t++)
        n_prom[t] = _tMS5837->usaCaliData[t];

	n_prom[0] = ((n_prom[0]) & 0x0FFF);
	n_prom[7] = 0;

	for(cnt = 0; cnt < 16; cnt++)
	{
        if(cnt % 2 == 1)
            n_rem ^= (uint16_t)((n_prom[cnt>>1]) & 0x00FF);	
        else 
            n_rem ^= (uint16_t) (n_prom[cnt>>1] >> 8);

        for(n_bit = 8; n_bit > 0; n_bit--)
        {
            if(n_rem & (0x8000))
                n_rem = (n_rem << 1) ^ 0x3000;
            else 
                n_rem = (n_rem << 1);
        }
	}
	n_rem = ((n_rem >> 12) & 0x000F);	
    a = (n_rem ^ 0x00);
	b = _tMS5837->usaCaliData[0] >> 12;
	if(a == b)
		return 1;
	else 
        return 0;
}

/**
 * @brief MS5837��ȡ���ݺ���
 * @param _tMS5837-MS5837���ָ��
 * @note ʵ���¶ȱ�����fTemperature�У�ʵ����ȱ�����fDepth��
 * @retval Null
*/
void OCD_MS5837_GetData(tagMS5837_T *_tMS5837)
{
    /* ���ݲ����ֱ���ѡ�� */
    switch (_tMS5837->setOSR)
    {
        case MS5837_OSR256:
            D1_Press = S_MS5837_GetConversion(_tMS5837,MS5837_D1_OSR_256);
            D2_Temp  = S_MS5837_GetConversion(_tMS5837,MS5837_D2_OSR_256);
            break;
        case MS5837_OSR512:
            D1_Press = S_MS5837_GetConversion(_tMS5837,MS5837_D1_OSR_512);
            D2_Temp  = S_MS5837_GetConversion(_tMS5837,MS5837_D2_OSR_512);
            break;
        case MS5837_OSR1024:
            D1_Press = S_MS5837_GetConversion(_tMS5837,MS5837_D1_OSR_1024);
            D2_Temp  = S_MS5837_GetConversion(_tMS5837,MS5837_D2_OSR_1024);        
            break;
        case MS5837_OSR2048:
            D1_Press = S_MS5837_GetConversion(_tMS5837,MS5837_D1_OSR_2048);
            D2_Temp  = S_MS5837_GetConversion(_tMS5837,MS5837_D2_OSR_2048);        
            break;
        case MS5837_OSR4096:
            D1_Press = S_MS5837_GetConversion(_tMS5837,MS5837_D1_OSR_4096);
            D2_Temp  = S_MS5837_GetConversion(_tMS5837,MS5837_D2_OSR_4096);        
            break;
        case MS5837_OSR8192:
            D1_Press = S_MS5837_GetConversion(_tMS5837,MS5837_D1_OSR_8192);
            D2_Temp  = S_MS5837_GetConversion(_tMS5837,MS5837_D2_OSR_8192);        
            break;
    }

    dT   = D2_Temp - (uint32_t)C5 * 256L;
    TEMP = 2000L + (int64_t)(dT) * C6 / 8388608LL;
    SENS = (int64_t)C1 * 32768L + ((int64_t)C3 * dT) / 256L;
    OFF  = (int64_t)C2 * 65536L + ((int64_t)C4 * dT) / 128L;

    _tMS5837->fTemp1 = TEMP/100;
    _tMS5837->fPress1 = (D1_Press * SENS / 2097152L - OFF) / 8192L / 10;

    /* �����¶Ȳ��� */
    if(TEMP < 2000)
    {
        Ti = (3*(int64_t)(dT)*(int64_t)(dT) / (8589934592LL));
		OFFi = (3*(TEMP - 2000)*(TEMP - 2000)) / 2;
		SENSi = (5*(TEMP - 2000)*(TEMP - 2000)) / 8;
        if(TEMP < -1500)
        {
            OFFi = OFFi + 7 * (TEMP + 1500) * (TEMP + 1500);
            SENSi = SENSi + 4 * (TEMP +1500) * (TEMP + 1500);
        }
    }
    else
    {
        Ti = 2*(dT*dT)/137438953472LL;
		OFFi = (1*(TEMP - 2000)*(TEMP - 2000))/16;
		SENSi = 0;
    }

    OFF2  = OFF - OFFi;
    SENS2 = SENS - SENSi;
    _tMS5837->fPress2 = (float)((D1_Press * SENS2)/2097152L - OFF2) / 8192L / 10;           /* У׼��ѹ������ */
    _tMS5837->fTemp2 = (float)(TEMP - Ti)/100.0f;                                           /* У׼���¶����� */

    _tMS5837->fTemperature = _tMS5837->fTemp2;                                              /* ����ʵ���¶� */
    _tMS5837->fDepth = (_tMS5837->fPress2 - ATM)/1000 * G * 100;                            /* ����ʵ����� */
}

/**
 * @brief MS5837��ʼ������
 * @param _tMS5837-MS5837���ָ��
 * @retval uint8_t 1-�ɹ� 0-ʧ��
*/
uint8_t OCD_MS5837_Init(tagMS5837_T *_tMS5837)
{
    Drv_IICSoft_Init(&_tMS5837->tIIC);

    S_MS5837_Reset(_tMS5837);
    Drv_Delay_Ms(20);

    S_MS5837_GetCalibrationData(_tMS5837);
    Drv_Delay_Ms(20);

    if(!S_MS5837_CRC4(_tMS5837))
        return 0;
    else return 1;
}
