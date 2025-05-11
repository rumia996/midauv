/****************************************************************************

* Sigma�Ŷ�

* �ļ���: ocd_fatfs.c

* ���ݼ�����FATFSģ���ļ�
            �����ocd_sdcard.cʹ��
			ʹ��ǰ��Ҫ�ڹ��������Bsp/Fatfs�ļ��У�����ff.c��diskio.c

* �ļ���ʷ��

* �汾��		����		����		    ˵��
*  2.5	 	2023-05-14	  �����		�޸���д�����޷�����Ч�ֽ��������⣬��ʽ�Ż�

* 1.0.1a 	2020-03-29	  ���		�޸�OCD_FATFS_ReadDir�Ľӿڲ����Լ��ڲ��߼����޸�OCD_FATFS_ReadData_SpecifyIndexִ���߼�
									    ����OCD_FATFS_GetFileInfo����

* 1.0.0a 	2020-02-22	  ���		�������ļ�

****************************************************************************/
#include "ocd_fatfs.h"

/**
 * @brief FATFS�ļ�ϵͳ�����ļ���
 * @param _tFATFS-FATFS�ṹ��ָ��
 * @param _cpPath-�ļ���·��ָ��
 * @retval uint8_t FR_OK���ɹ� FR_EXIST:�ļ����Ѿ����� -1��ʧ��
 */
int8_t OCD_FATFS_CreateDir(tagFATFS_T *_tFATFS, const char *_cpPath)
{
	FRESULT res;

	f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1); /* ΪSD������һ�������� */

	res = f_mkdir(_cpPath);
    
    /* �ļ����Ѿ����� */
    if(res == FR_EXIST)
    {
        f_mount(NULL, 0, 1);	/* ע�������� */
        return FR_EXIST;
    }
        
	/* �����ļ���ʧ�� */
	if(res != FR_OK)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	f_mount(NULL, 0, 1);	/* ע�������� */
	return FR_OK;
}

/**
 * @brief FATFS�ļ�ϵͳ��ȡ��Ӧ�ļ���Ŀ¼�µ��ļ���
 * @param _tFATFS-FATFS�ṹ��ָ��
 * @param _tpINFO-�ļ������Ϣ�ṹ��ָ��
 * @param _cpPath-�ļ���·��ָ��
 * @param _ucNameLen-�ļ�������
 * @retval uint8_t-�ļ�����
 */
int8_t OCD_FATFS_ReadDir(tagFATFS_T *_tFATFS, tagFileInfo_T *_tpINFO, const char *_cpPath, uint8_t _ucNameLen)
{  
	uint8_t index = 0;
	uint8_t j;
	
	f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	
	if(f_opendir(&_tFATFS->tFATFSInfo.tDIR, _cpPath) == FR_OK)    
	{   
		while (f_readdir(&_tFATFS->tFATFSInfo.tDIR, &_tFATFS->tFATFSInfo.tFILINFO) == FR_OK)    
		{  
			if(!_tFATFS->tFATFSInfo.tFILINFO.fname[0])
				break;
				
			for(j = 0; j < _ucNameLen; j++)
			{
				_tpINFO[index].ucFile_Name[j] = _tFATFS->tFATFSInfo.tFILINFO.fname[j];
			}
			index++;
		}
	}
	else 
	{
		return -1;
	}
	   
	f_mount(NULL, 0, 1);
	return index;
}

/**
 * @brief FATFS�ļ�ϵͳ��ȡ��Ӧ·�����ļ�ָ�����ȵ�����
 * @param _tFATFS-FATFS�ļ�ϵͳ�ṹ��
 * @param _cpFileName-�ļ�·��ָ��
 * @param _ucpData-������ݵĻ�������ָ��
 * @param _usLen-���ݳ���
 * @param _ulpByteVal-���ض�ȡ������Ч�ֽ����ĵ�ַ
 * @retval uint8_t FR_OK���ɹ� -1��ʧ��
 */
int8_t OCD_FATFS_Read_SpecifyFiles(tagFATFS_T *_tFATFS, char *_cpFileName, uint8_t *_ucpData, uint32_t _ulLength, uint32_t *_ulpByteVal)
{
	FRESULT res;
	
	f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	res = f_open(&_tFATFS->tFATFSInfo.tFIL, _cpFileName, FA_OPEN_EXISTING|FA_READ);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
		
	res = f_read(&_tFATFS->tFATFSInfo.tFIL, _ucpData, _ulLength, _ulpByteVal);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	res = f_close(&_tFATFS->tFATFSInfo.tFIL);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	f_mount(NULL, 0, 1);
	return FR_OK;
}

/**
 * @brief FATFS�ļ�ϵͳ��ȡ��Ӧ·�����ļ���ָ��λ�õ�����
 * @param _tFATFS-�ļ�ϵͳ�ṹ��
 * @param _cpFileName-�ļ�·��ָ��
 * @param _ucpData-������ݵĻ�������ָ��
 * @param _ulLength-��ȡ���ݵĳ���
 * @param _ulOffset-��ȡλ�õ�ƫ����
 * @param _ulpByteVal-���ض�ȡ������Ч�ֽ����ĵ�ַ
 * @retval uint8_t FR_OK���ɹ� -1��ʧ��
 */
int8_t OCD_FATFS_Read_SpecifyIndex(tagFATFS_T *_tFATFS, char *_cpFileName, uint8_t *_ucpData, uint32_t _ulLength, uint32_t _ulOffset, uint32_t *_ulpByteVal)
{
	FRESULT res;
	
	f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	res = f_open(&_tFATFS->tFATFSInfo.tFIL, _cpFileName, FA_OPEN_EXISTING|FA_READ);	
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	res = f_lseek(&_tFATFS->tFATFSInfo.tFIL,_ulOffset);	
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	res = f_read(&_tFATFS->tFATFSInfo.tFIL, _ucpData, _ulLength, _ulpByteVal);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	res = f_close(&_tFATFS->tFATFSInfo.tFIL);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	f_mount(NULL, 0, 1);
	return FR_OK;
}

/**
 * @brief FATFS�ļ�ϵͳ��Ӧ·�����ļ���ָ��λ��д������
 * @param _tFATFS-�ļ�ϵͳ�ṹ��
 * @param _cpFileName-�ļ�·��ָ��
 * @param _ucpData-��Ҫд�����ݵĻ�������ָ��
 * @param _ulLength-д�����ݵĳ���
 * @param _ulOffset-д��λ�õ�ƫ����
 * @param _ulpByteVal-����д�뵽����Ч�ֽ����ĵ�ַ
 * @retval uint8_t FR_OK���ɹ� -1��ʧ��
 */
int8_t OCD_FATFS_Write_SpecifyIndex(tagFATFS_T *_tFATFS, char *_cpFileName, uint8_t *_ucpData, uint32_t _ulLength, uint32_t _ulOffset, uint32_t *_ulpByteVal)
{
	FRESULT res;
	
	res = f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	res = f_open(&_tFATFS->tFATFSInfo.tFIL, _cpFileName, FA_WRITE | FA_CREATE_ALWAYS);		
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	res = f_lseek(&_tFATFS->tFATFSInfo.tFIL, _ulOffset);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	res = f_write(&_tFATFS->tFATFSInfo.tFIL,_ucpData, _ulLength, _ulpByteVal);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	res = f_close(&_tFATFS->tFATFSInfo.tFIL);	
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	f_mount(NULL, 0, 1);
	return FR_OK;
}

/**
 * @brief FATFS�ļ�ϵͳ��Ӧ·�����ļ���β��д������
 * @param _tFATFS-�ļ�ϵͳ�ṹ��
 * @param _cpFileName-�ļ�·��ָ��
 * @param _ucpData-��Ҫд�����ݵĻ�������ָ��
 * @param _ulLength-д�����ݵĳ���
 * @param _ulpByteVal-����д�뵽����Ч�ֽ����ĵ�ַ
 * @retval uint8_t FR_OK���ɹ� -1��ʧ��
 */
int8_t OCD_FATFS_Write_End(tagFATFS_T *_tFATFS, char *_cpFileName, uint8_t *_ucpData, uint32_t _ulLength , uint32_t *_ulpByteVal)
{
	FRESULT res = FR_OK;
	
	res = f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	res = f_open(&_tFATFS->tFATFSInfo.tFIL, _cpFileName, FA_WRITE | FA_OPEN_ALWAYS);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	res = f_lseek(&_tFATFS->tFATFSInfo.tFIL, f_size(&_tFATFS->tFATFSInfo.tFIL));
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	res = f_write(&_tFATFS->tFATFSInfo.tFIL,_ucpData, _ulLength, _ulpByteVal);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	res = f_close(&_tFATFS->tFATFSInfo.tFIL);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	f_mount(NULL, 0, 1);
	return FR_OK;
}

/**
 * @brief FATFS�ļ�ϵͳ��Ӧ·�����ļ�д������
 * @param _tFATFS-�ļ�ϵͳ�ṹ��
 * @param _cpFileName-�ļ�·��ָ��
 * @param _ucpData-��Ҫд�����ݵĻ�������ָ��
 * @param _ulLength-д�����ݵĳ���
 * @param _ulpByteVal-����д�뵽����Ч�ֽ����ĵ�ַ
 * @retval uint8_t FR_OK���ɹ� -1��ʧ��
 */
int8_t OCD_FATFS_Write(tagFATFS_T *_tFATFS, char *_cpFileName,uint8_t *_ucpData, uint32_t _ulLength, uint32_t *_ulpByteVal)
{
	FRESULT res;
	
	res = f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	res = f_open(&_tFATFS->tFATFSInfo.tFIL, _cpFileName, FA_WRITE | FA_OPEN_ALWAYS);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	res = f_write(&_tFATFS->tFATFSInfo.tFIL,_ucpData, _ulLength, _ulpByteVal);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	res = f_close(&_tFATFS->tFATFSInfo.tFIL);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	f_mount(NULL, 0, 1);
	return FR_OK;
}

/**
 * @brief FATFS�ļ�ϵͳ��ȡ�ļ���Ϣ
 * @param _tFATFS-�ļ�ϵͳ�ṹ��
 * @param _cpFileName-�ļ�·��ָ��
 * @retval uint8_t FR_OK���ɹ� -1��ʧ��
 */
int8_t OCD_FATFS_GetFileInfo(tagFATFS_T *_tFATFS, char *_cpFileName)
{
	FRESULT res;
	
	f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	res = f_open(&_tFATFS->tFATFSInfo.tFIL, _cpFileName, FA_OPEN_EXISTING|FA_READ);	
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	res = f_stat(_cpFileName, &_tFATFS->tFATFSInfo.tFILINFO);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	f_mount(NULL, 0, 1);
	return FR_OK;
}

/**
 * @brief FATFS�ļ�ϵͳ��ʼ��
 * @param _tFATFS-FATFS�ṹ��ָ��
 * @retval uint8_t
 */
uint8_t OCD_FATFS_Init(tagFATFS_T *_tFATFS)
{
	return OCD_SDCard_SPIInit(_tFATFS->tSPI);
}

