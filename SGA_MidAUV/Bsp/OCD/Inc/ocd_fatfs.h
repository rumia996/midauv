#ifndef __OCD_SDCARD_FUNC_H_
#define __OCD_SDCARD_FUNC_H_

#include "ocd_sdcard.h"

#include <ff.h>

#define FILE_NAME_LENTH		20		/* �洢�ļ�������֧��20�ֽ��ַ����üܹ��µ�fatfs�ļ�ϵͳ�ݲ�֧�������ļ��� */

/* FATFS�ļ�ϵͳ�����Ϣ�ṹ�� */
typedef struct 
{
	FATFS tFATFS;
	FIL tFIL;

	FILINFO tFILINFO;
	DIR   tDIR;
}tagFATFSInfo_T;

/* FATFS�ļ������Ϣ�ṹ�� */
typedef struct 
{
	uint8_t ucFile_Name[FILE_NAME_LENTH];
}tagFileInfo_T;		/* FATFS�ļ������Ϣ�ṹ�� */

/* FATFS�ļ�ϵͳ�ṹ�� */
typedef struct
{
	tagSPI_T 		tSPI;		/* STM32�ڲ�SPI�豸ָ�� */
	tagFATFSInfo_T	tFATFSInfo;	/* FATFS�ļ�ϵͳ�����Ϣ�ṹ�� */
}tagFATFS_T;

int8_t OCD_FATFS_CreateDir(tagFATFS_T *_tFATFS, const char *_cpPath);
int8_t OCD_FATFS_ReadDir(tagFATFS_T *_tFATFS, tagFileInfo_T *_tpINFO, const char *_cpPath, uint8_t _ucNameLen);
int8_t OCD_FATFS_Read_SpecifyFiles(tagFATFS_T *_tFATFS, char *_cpFileName, uint8_t *_ucpData, uint32_t _ulLength, uint32_t *_ulpByteVal);
int8_t OCD_FATFS_Read_SpecifyIndex(tagFATFS_T *_tFATFS, char *_cpFileName, uint8_t *_ucpData, uint32_t _ulLength, uint32_t _ulOffset, uint32_t *_ulpByteVal);
int8_t OCD_FATFS_Write_SpecifyIndex(tagFATFS_T *_tFATFS, char *_cpFileName, uint8_t *_ucpData, uint32_t _ulLength, uint32_t _ulOffset, uint32_t *_ulpByteVal);
int8_t OCD_FATFS_Write_End(tagFATFS_T *_tFATFS, char *_cpFileName, uint8_t *_ucpData, uint32_t _ulLength, uint32_t *_ulpByteVal);
int8_t OCD_FATFS_Write(tagFATFS_T *_tFATFS, char *_cpFileName,uint8_t *_ucpData,uint32_t _ulLength, uint32_t *_ulpByteVal);
int8_t OCD_FATFS_GetFileInfo(tagFATFS_T *_tFATFS, char *_cpFileName);
uint8_t OCD_FATFS_Init(tagFATFS_T *_tFATFS);

#endif
