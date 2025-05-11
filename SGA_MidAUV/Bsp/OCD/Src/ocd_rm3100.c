/****************************************************************************

* Sigma�Ŷ�

* �ļ���: ocd_rm3100.c

* ���ݼ�����rm3100ģ���ļ�

* �ļ���ʷ��

* �汾��		����	  ����		˵��
* 2.2.2		2023-04-09	�����	�����ع���ͳһAPI

* 1.1.8		2022-10-25	���	�������ļ�

****************************************************************************/
#include "ocd_rm3100.h"

/* RM3100��ʼ������ */
struct config tRM3100 = 
{
    .ucCycle_Count  = 200,
    .fSample_Rate   = 37,
    .fMax_Data_Rate = 440,
    .fGain          = 75
};

/**
 * @brief RM3100Ƭѡ(Ӳ��SPIר��)
 * @param _tRM3100-RM3100���ָ��
 * @param _ucLevel-��ƽ
 * @retval Null
*/
static void S_RM3100_CS(tagRM3100_T *_tRM3100,uint8_t _ucLevel)
{
	HAL_GPIO_WritePin(_tRM3100->tSPI.tGPIO[3].tGPIOPort,_tRM3100->tSPI.tGPIO[3].tGPIOInit.Pin,_ucLevel?GPIO_PIN_SET:GPIO_PIN_RESET);
}

/**
 * @brief RM3100�Ķ��ֽ�д��
 * @param _tRM3100-RM3100���ָ��
 * @param _ucReg_addr-�Ĵ�����ַ
 * @param _ucpDataIn-д�����ݵĵ�ַ
 * @param _ucLenth-д�����ݵĳ���
 * @retval Null
*/
static void S_RM3100_Write(tagRM3100_T *_tRM3100,uint8_t _ucReg_addr,uint8_t *_ucpDataIn,uint8_t _ucLenth)
{
	uint8_t ucByteCnt = 0;
	uint8_t ucTemp = 0;

	if(_tRM3100->bSPIEnable == true)
	{
		S_RM3100_CS(_tRM3100,0);

		ucTemp = 0x00 + (_ucReg_addr&0x3f);
		Drv_SPI_TransmitReceive(&_tRM3100->tSPI,ucTemp);

		for(ucByteCnt = 0;ucByteCnt < _ucLenth;ucByteCnt++)
		{
			ucTemp = *(_ucpDataIn+ucByteCnt);
			Drv_SPI_TransmitReceive(&_tRM3100->tSPI,ucTemp);
		}

		S_RM3100_CS(_tRM3100,1);
	}
	else if(_tRM3100->bSPISoftEnable == true)
	{
		Drv_SPI_NSS(&_tRM3100->tSoftSPI,0);

		ucTemp = 0x00 + (_ucReg_addr&0x3f);
		Drv_SPI_WriteByte_Soft(&_tRM3100->tSoftSPI,ucTemp);

		for(ucByteCnt = 0;ucByteCnt < _ucLenth;ucByteCnt++)
		{
			ucTemp = *(_ucpDataIn+ucByteCnt);
			Drv_SPI_WriteByte_Soft(&_tRM3100->tSoftSPI,ucTemp);
		}

		Drv_SPI_NSS(&_tRM3100->tSoftSPI,1);
	}

}

/**
 * @brief RM3100�Ķ��ֽڶ�ȡ
 * @param _tRM3100-RM3100���ָ��
 * @param _ucReg_addr-�Ĵ�����ַ
 * @param _ucpDataOut-�������ݵĵ�ַ
 * @param _ucLenth-�������ݵĳ���
 * @retval Null
*/
static void S_RM3100_Read(tagRM3100_T *_tRM3100,uint8_t _ucReg_addr,uint8_t *_ucpDataOut,uint8_t _ucLenth)
{
	uint8_t ucByteCnt = 0;
	uint8_t ucTemp = 0;
	
	if(_tRM3100->bSPIEnable == true)
	{
		S_RM3100_CS(_tRM3100,0);

		ucTemp = 0x80 + (_ucReg_addr);
		Drv_SPI_TransmitReceive(&_tRM3100->tSPI,ucTemp);
		
		for(ucByteCnt = 0;ucByteCnt < _ucLenth;ucByteCnt++)
		{
			ucTemp = Drv_SPI_TransmitReceive(&_tRM3100->tSPI,0);
			*(_ucpDataOut+ucByteCnt) = ucTemp;
		}

		S_RM3100_CS(_tRM3100,1);
	}
	else if(_tRM3100->bSPISoftEnable == true)
	{
		Drv_SPI_NSS(&_tRM3100->tSoftSPI,0);

		ucTemp = 0x80 + (_ucReg_addr);
		Drv_SPI_WriteByte_Soft(&_tRM3100->tSoftSPI,ucTemp);

		for(ucByteCnt = 0;ucByteCnt < _ucLenth;ucByteCnt++)
		{
			ucTemp = Drv_SPI_WriteByte_Soft(&_tRM3100->tSoftSPI,0);
			*(_ucpDataOut+ucByteCnt) = ucTemp;
		}

		Drv_SPI_NSS(&_tRM3100->tSoftSPI,1);
	}

}

/**
 * @brief RM3100�����Ƿ�ת�����
 * @param _tRM3100-RM3100���ָ��
 * @retval uint8_t-ת�����-1  δת����-0
*/
static uint8_t S_RM3100_GetDataReadyStatus(tagRM3100_T *_tRM3100) 
{
    char cData[1];

    S_RM3100_Read(_tRM3100, M3D_3100_STATUS, (uint8_t*) cData,1);
    return (cData[0] & STATUS_MASK);
}

/**
 * @brief RM3100Ϊ��������ģʽʱ����������ѭ������
 * @param _tRM3100-RM3100���ָ��
 * @param _usValue-����
 * @retval uint8_t-�ɹ�-1  ʧ��-0
*/
uint8_t OCD_RM3100_SetCycleCount(tagRM3100_T *_tRM3100,uint16_t _usValue) 
{
    uint8_t ucReg[6];

    if (_usValue > 65535)
        return 0;
    else
	{
		if (_usValue > 400)
        	_usValue = 400;
		else if (_usValue < 30)
        	_usValue = 30;

		ucReg[0] = _usValue>>8;
		ucReg[1] = _usValue;
		ucReg[2] = ucReg[0];
		ucReg[3] = ucReg[1];
		ucReg[4] = ucReg[0];
		ucReg[5] = ucReg[1];

		S_RM3100_Write(_tRM3100, M3D_3100_CCX, ucReg,6);
    }
    return 1;
}

/**
 * @brief RM3100Ϊ��������ģʽʱ����������ת������
 * @param _tRM3100-RM3100���ָ��
 * @param _ucConf-����
 * @retval uint8_t-�ɹ�-1  ʧ��-0
*/
uint8_t OCD_RM3100_SetCMM_DataRate(tagRM3100_T *_tRM3100,uint8_t _ucConf) 
{
    if (_ucConf < CMM_UPDATERATE_600 || _ucConf > CMM_UPDATERATE_0_075)
        return 0;
    else{
        uint8_t *ptr;
        float fTemp = 1000;

        switch (_ucConf)
		{
            case CMM_UPDATERATE_600:   fTemp=600;
                                    break;
            case CMM_UPDATERATE_300:   fTemp=300;
                                    break;
            case CMM_UPDATERATE_150:   fTemp=150;
                                    break;
            case CMM_UPDATERATE_75 :   fTemp=75;
                                    break;
            case CMM_UPDATERATE_37 :   fTemp=37;
                                    break;
            case CMM_UPDATERATE_18 :   fTemp=18;
                                    break;
            case CMM_UPDATERATE_9  :   fTemp=9;
                                    break;
            case CMM_UPDATERATE_4_5:   fTemp=4.5;
                                    break;
            case CMM_UPDATERATE_2_3:   fTemp=2.3;
                                    break;
            case CMM_UPDATERATE_1_2:   fTemp=1.2;
                                    break;
            case CMM_UPDATERATE_0_6:   fTemp=0.6;
                                    break;
            case CMM_UPDATERATE_0_3:   fTemp=0.3;
                                    break;
            case CMM_UPDATERATE_0_15:  fTemp=0.15;
                                    break;
            case CMM_UPDATERATE_0_075: fTemp=0.075;
                                    break;
        }
        if (fTemp > tRM3100.fMax_Data_Rate)				
            return 0;

        ptr = &_ucConf;
        S_RM3100_Write(_tRM3100, M3D_3100_TMRC, ptr,1);
    }
    return 1;
}

/**
 * @brief RM3100������������ģʽ
 * @param _tRM3100-RM3100���ָ��
 * @param _ucConf-����
 * @retval Null
*/
void OCD_RM3100_ContinuousModeConfig(tagRM3100_T *_tRM3100,uint8_t _ucConf) 
{
    uint8_t *ptr;

    ptr = &_ucConf;
    S_RM3100_Write(_tRM3100, M3D_3100_CMM, ptr,1);
}

/**
 * @brief RM3100���õ��β���ģʽ
 * @param _tRM3100-RM3100���ָ��
 * @param _ucConf-����
 * @retval Null
*/
void OCD_RM3100_SingleModeConfig(tagRM3100_T *_tRM3100,uint8_t _ucConf) 
{
    uint8_t *ptr;

    ptr = &_ucConf;
    S_RM3100_Write(_tRM3100, M3D_3100_POLL, ptr,1);
}

/**
 * @brief �趨RM3100����ģʽ��ÿ�ζ������ݺ����ٵ��ô˺���
 * @param _tRM3100-RM3100���ָ��
 * @retval Null
*/
void OCD_RM3100_ModeConfig(tagRM3100_T *_tRM3100)
{
#ifdef RM3100_SINGLE
	OCD_RM3100_SingleModeConfig(_tRM3100,CMM_ALL_AXIS_ON);     /* ����ģʽ */
#else
  	OCD_RM3100_ContinuousModeConfig(_tRM3100,CMM_ALL_AXIS_ON|DRDY_WHEN_ALL_AXIS_MEASURED|CM_START); 			/* ���ò�����,drdy ��������ģʽ */
  	OCD_RM3100_SetCycleCount(_tRM3100,200);												
  	OCD_RM3100_SetCMM_DataRate(_tRM3100,12);
#endif
}

/**
 * @brief RM3100��ȡ���ݣ�����tMagData��Ա��
 * @param _tRM3100-RM3100���ָ��
 * @retval Null
*/
void OCD_RM3100_GetData(tagRM3100_T *_tRM3100)
{
	uint8_t i;
	uint8_t ucTemp[9] = {0};
	int32_t ulMag_Data[3] = {0};

	while(S_RM3100_GetDataReadyStatus(_tRM3100) == 0);

	if(_tRM3100->bSPISoftEnable == true)
		Drv_SPI_NSS(&_tRM3100->tSoftSPI,1); 
	
	S_RM3100_Read(_tRM3100,0x24,ucTemp,9);
	ulMag_Data[0] = ucTemp[0]<<16 | ucTemp[1]<<8 | ucTemp[2];			
	ulMag_Data[1] = ucTemp[3]<<16 | ucTemp[4]<<8 | ucTemp[5];
	ulMag_Data[2] = ucTemp[6]<<16 | ucTemp[7]<<8 | ucTemp[8];

	for(i = 0;i < 3;i++)
	{
		if(ulMag_Data[i]&0x00800000)
			ulMag_Data[i] |= 0xff000000;
	}
	
	_tRM3100->tMagData.MAG_X =  ulMag_Data[1]*13;
	_tRM3100->tMagData.MAG_Y = -ulMag_Data[0]*13;
	_tRM3100->tMagData.MAG_Z = -ulMag_Data[2]*13; 
}

/**
 * @brief RM3100��ʼ������
 * @param _tRM3100-RM3100���ָ��
 * @param _ucNum-��ʼ������
 * @retval Null
*/
void OCD_RM3100_Init(tagRM3100_T *_tRM3100,uint8_t _ucNum)
{
	uint8_t index;

	/* ����ʹ�ܱ�־���ν��г�ʼ�� */
	for(index = 0;index < _ucNum;index++)
	{
		/* �����Ӳ��SPI���� */
		if(_tRM3100[index].bSPIEnable == true)
		{
			Drv_SPI_Init(&_tRM3100[index].tSPI);
		}

		/* ��������SPI���� */
		else if(_tRM3100[index].bSPISoftEnable == true)
		{
			Drv_SPISoft_Init(&_tRM3100[index].tSoftSPI);
		}

		/* ���߽Բ��ǣ����� */
		else
		{
			Drv_HAL_Error(__FILE__, __LINE__);
			while(1);
		}

		/* RM3100ģʽ���� */
		OCD_RM3100_ModeConfig(&_tRM3100[index]);
	}
}

