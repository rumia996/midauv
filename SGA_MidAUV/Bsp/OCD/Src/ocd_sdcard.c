/****************************************************************************

* Sigma�Ŷ�

* �ļ���: ocd_sdcard.c

* ���ݼ�����SD��ģ���ļ�

* �ļ���ʷ��

* �汾��	����		����		˵��

* 1.0.0a 	2020-02-22	���		�������ļ�

****************************************************************************/
#include "ocd_sdcard.h"

#include "diskio.h"			/* ��ʹ��FATFSʱע�͵� */

/**
 * @brief SD�����͵ȴ���ֹ�ź�
 * @param NULL
 * @retval Null
*/
static void S_SDCard_WaitStop(void)
{
	Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
}

/**
 * @brief SD�����͵ȴ������ź�
 * @param NULL
 * @retval uint8_t-1,������0-����
*/
static uint8_t S_SDCard_WaitReady(void)
{
	uint8_t res = 0;
	uint32_t ulTxNum = 0;	/* ���ʹ��� */
	
	do
	{		
		res = Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
		if(res ==0xFF)
			return 0;
		
		ulTxNum++;		  	
	}while(ulTxNum < 0xFFFFFF);
	
	return 1;
}

/**
 * @brief SD���ȴ�
 * @param NULL
 * @retval uint8_t-0,������0-����
*/
static uint8_t S_SDCard_Wait(void)
{
	if(S_SDCard_WaitReady()==0)
	{
		return 0;
	}
	else
	{
		S_SDCard_WaitStop();
		return 1;
	}
}

/**
 * @brief SD����������
 * @param _ucCmd-����
 * @param _ulArg-
 * @param _ucCrc-CRCУ��
 * @retval uint8_t-0xff,��������-���ص�һ���ֽ�����
*/
static uint8_t S_SDCard_SendCMD(uint8_t _ucCmd, uint32_t _ulArg, uint8_t _ucCrc)
{
	uint8_t ucRx;
	uint8_t ucReTryNum = 0;
	
	S_SDCard_WaitStop();
	
	if(S_SDCard_Wait()==1)
		return 0xFF; 

    Drv_SPI_TransmitReceive( &s_tSPI, _ucCmd | 0x40);
    Drv_SPI_TransmitReceive( &s_tSPI, _ulArg >> 24);
    Drv_SPI_TransmitReceive( &s_tSPI, _ulArg >> 16);
    Drv_SPI_TransmitReceive( &s_tSPI, _ulArg >>  8);
    Drv_SPI_TransmitReceive( &s_tSPI, _ulArg);	  
    Drv_SPI_TransmitReceive( &s_tSPI, _ucCrc);
	if(_ucCmd == CMD12)
		Drv_SPI_TransmitReceive( &s_tSPI, 0xFF);
	
	ucReTryNum = 0x1F;	/* �ط����� */
	do
	{
		ucRx = Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
	}while((ucRx & 0x80) && ucReTryNum--);
	
	return ucRx;
}

/**
 * @brief SD���ĳ�ʼ��
 * @param NULL
 * @retval uint8_t
*/
static uint8_t S_SDCard_Init(void)
{
	uint8_t ucRx;      
	uint8_t index;
	uint16_t usReTryNum;  
	uint8_t ucaRxBuffer[4];
	
	Drv_SPI_Init(&s_tSPI);
	Drv_SPI_SpeedConfig(&s_tSPI, 0);
		
	for(index = 0;index < 10;index++)
		Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
		
	usReTryNum = 20;
	do
	{
		ucRx = S_SDCard_SendCMD( CMD0, 0, 0x95);
	}while((ucRx != 0X01) && usReTryNum--);
			
	if(ucRx == 0X01)
	{
		if(S_SDCard_SendCMD( CMD8, 0x1AA, 0x87) == 1)
		{
			for(index = 0; index < 4; index++)
				ucaRxBuffer[index] = Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);	
						
			/* ���Ƿ�֧��2.7~3.6V */
			if(ucaRxBuffer[2]==0x01 && ucaRxBuffer[3]==0xAA)
			{
				usReTryNum = 0XFFFE;
				do
				{
					ucRx = S_SDCard_SendCMD(CMD55, 0, 0x01);
					ucRx = S_SDCard_SendCMD(CMD41, 0x40000000, 0x01);
				}
				while(ucRx && usReTryNum--);
				if(usReTryNum&&S_SDCard_SendCMD(CMD58, 0, 0x01)==0)
				{
					for(index = 0; index < 4; index++)
						ucaRxBuffer[index] = Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
					
					if(ucaRxBuffer[0] & 0x40)
						s_ucSdType=SD_TYPE_V2HC;    
					else 
						s_ucSdType=SD_TYPE_V2;   
				}
			}
		}
		
		else
		{
			S_SDCard_SendCMD(CMD55,0,0x01);		
			ucRx = S_SDCard_SendCMD(CMD41, 0, 0x01);	
			if(ucRx <= 1)
			{		
				s_ucSdType = SD_TYPE_V1;
				usReTryNum = 0xFFFE;
				do 
				{
					S_SDCard_SendCMD(CMD55, 0, 0x01);	
					ucRx = S_SDCard_SendCMD(CMD41, 0, 0x01);
				}while(ucRx && usReTryNum--);
			}
			else
			{
				s_ucSdType = SD_TYPE_MMC;
				usReTryNum = 0xFFFE;
				do 
				{											    
					ucRx = S_SDCard_SendCMD(CMD1,0,0x01);
				}while(ucRx && usReTryNum--);
			}
			
			if(usReTryNum==0 || S_SDCard_SendCMD(CMD16, 512, 0x01) != 0)
				s_ucSdType = SD_TYPE_ERR;
		}
	}
	
	S_SDCard_WaitStop();
	Drv_SPI_SpeedConfig(&s_tSPI, 1);
	if(s_ucSdType)
	{
		return 0;
	}
	else 
		if(ucRx) 
			return ucRx; 	
	
	return 0XAA;		
}

/**
 * @brief SD���Ļ����Ӧ
 * @param _ucAck-Ӧ���ź�
 * @retval uint8_t
*/
static uint8_t S_SDCard_GetResponse(uint8_t _ucAck)
{
	int lCnt = 0XFFFF;
	
	while ((Drv_SPI_TransmitReceive(&s_tSPI, 0XFF) != _ucAck) && lCnt)
		lCnt--;  
		
	if (lCnt==0)
		return 0xff;
	else 
		return  0;
}

/**
 * @brief SD���Ľ�������
 * @param _ucpBuffer-��������ַ
 * @param _usLength-���ݳ���
 * @retval uint8_t
*/
static uint8_t S_SDCard_ReceiveData(uint8_t *_ucpBuffer, uint16_t _usLength)
{
	if(S_SDCard_GetResponse(0xFE) == 1)
		return 1;
	
    while(_usLength--)
    {
        *_ucpBuffer = Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
        _ucpBuffer++;
    }
	
    Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
    Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);	
	
    return 0;
}

/**
 * @brief SD���Ķ�ȡ��������
 * @param _ucpBuffer-��������ַ
 * @param _ulSec-����
 * @param _ucCnt-���ݳ���
 * @retval uint8_t
*/
static uint8_t S_SDCard_ReadDisk(uint8_t *_ucpBuffer, uint32_t _ulSec, uint8_t _ucCnt)
{
	uint8_t ucRx;
	
	if(s_ucSdType!=SD_TYPE_V2HC)
		_ulSec <<= 9;
	
	if(_ucCnt==1)
	{
		ucRx = S_SDCard_SendCMD( CMD17, _ulSec, 0x01);
		if(ucRx == 0)
		{
			ucRx = S_SDCard_ReceiveData( _ucpBuffer, 512);
		}
	}
	else
	{
		ucRx = S_SDCard_SendCMD( CMD18, _ulSec, 0x01);
		do
		{
			ucRx = S_SDCard_ReceiveData( _ucpBuffer, 512);	 
			_ucpBuffer += 512;  
		}
		while(--_ucCnt && ucRx==0);
		
		S_SDCard_SendCMD( CMD12, 0, 0x01);	
	}   
	
	S_SDCard_WaitStop();
	return ucRx;
}

/**
 * @brief SD���ķ������ݿ�
 * @param _ucpBuffer-��������ַ
 * @param _ucCmd-����
 * @retval uint8_t
*/
static uint8_t S_SDCard_SendBlock(uint8_t *_ucpBuffer, uint8_t _ucCmd)
{
	uint16_t t;
	
	if(S_SDCard_Wait()==1)
		return 1;
		
	Drv_SPI_TransmitReceive(&s_tSPI, _ucCmd);
		
	if(_ucCmd != 0xFD)
	{
		for(t = 0; t < 512; t++)
			Drv_SPI_TransmitReceive(&s_tSPI, _ucpBuffer[t]);
			
		Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
		Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
		t = Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
			
		if((t & 0x1F) != 0x05)
			return 2;							  					    
	}		
		
	return 0;
}

/**
 * @brief SD����д����
 * @param _ucpBuffer-��������ַ
 * @param _ulSec-����
 * @param _ucCnt-���ݳ���
 * @retval uint8_t
*/
static uint8_t S_SDCard_WriteDisk(uint8_t *_ucpBuffer, uint32_t _ulSec, uint8_t _ucCnt)
{
	uint8_t ucRx=0;
	
	if(s_ucSdType != SD_TYPE_V2HC)
		_ulSec <<= 9;
	
	if(_ucCnt == 1)
	{
		ucRx=S_SDCard_SendCMD( CMD24, _ulSec, 0x01);
		if(!ucRx)
		{
			ucRx=S_SDCard_SendBlock( _ucpBuffer,0xFE);  
		}
	}
	else
	{
		S_SDCard_SendCMD( CMD55, 0, 0x01);	
		S_SDCard_SendCMD( CMD23, _ucCnt, 0x01);
		ucRx=S_SDCard_SendCMD( CMD25, _ulSec, 0x01);
		
		if(!ucRx)
		{
			do
			{
				ucRx = S_SDCard_SendBlock( _ucpBuffer,0xFC);
				_ucpBuffer += 512;  
			}while(--_ucCnt && ucRx==0);
						
			ucRx = S_SDCard_SendBlock( 0,0xFD);
		}
	}   
	
	S_SDCard_WaitStop();
	return ucRx;
}

/**
 * @brief SD����SPI��ʼ��
 * @param _tSPI-spi�ṹ��
 * @retval uint8_t
*/
uint8_t OCD_SDCard_SPIInit(tagSPI_T _tSPI)
{
	s_tSPI = _tSPI;
	
	return S_SDCard_Init();
}

/* FATFS�ļ�ϵͳ start */
#ifdef FRAMEWORK_FATFS_ENABLE

#define SD_CARD	 0  /* Example: Map MMC/SD card to physical drive 1 */

#define FLASH_SECTOR_SIZE 	512		/* ����������С */
#define FLASH_BLOCK_SIZE	8		/* ÿƬBlock��8������ */
#define FLASH_SECTOR_COUNT	2048*10	/* ָ����������-->FATFS���ô�СΪ10M */

DSTATUS disk_initialize (
	BYTE _drv				/* Physical drive nmuber (0..) */
)
{
	return RES_OK; //��ʼ���ɹ�
}

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return RES_OK;
}

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *ucRxBuffer,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{	
	uint8_t res=0; 
    if (!count)
		return RES_PARERR;	/* count���ܵ���0�����򷵻ز������� */	
	
	switch(drv)
	{
		case SD_CARD:		/* SD�� */
			res = S_SDCard_ReadDisk( ucRxBuffer, sector, count);	 

		 	if(res)			/* STM32 SPI��bug,��sd������ʧ�ܵ�ʱ�������ִ����������,���ܵ���SPI��д�쳣 */
			{
				Drv_SPI_SpeedConfig(&s_tSPI, 0);
				Drv_SPI_TransmitReceive(&s_tSPI, 0XFF);		/* �ṩ�����8��ʱ�� */
				Drv_SPI_SpeedConfig(&s_tSPI, 1);
			}
		break;
		
		default:
			res=1; 
	}
	
	/* ������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ */
    if(res==0x00)
		return RES_OK;	 
    else 
		return RES_ERROR;
}

DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *ucRxBuffer,	        /* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
	unsigned char res=0;
    if (!count)
		return RES_PARERR;	/* count���ܵ���0�����򷵻ز������� */
	
	switch(drv)
	{
		case SD_CARD:		/* SD�� */
			res=S_SDCard_WriteDisk( (unsigned char*)ucRxBuffer,sector,count);
			while(res)
			{
				S_SDCard_Init();
				res=S_SDCard_WriteDisk( (unsigned char*)ucRxBuffer,sector,count);
			}
			break;
			
		default:
			res=1; 
	}
    /* ������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ */
    if(res == 0x00)
		return RES_OK;	 
    else 
		return RES_ERROR;		 
}

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *ucRxBuffer		/* Buffer to send/receive control data */
)
{	
	DRESULT res;						  			     
	if(drv==SD_CARD)		/* SD�� */
	{
	    switch(ctrl)
	    {
		    case CTRL_SYNC:
				return RES_OK; 
		    case GET_SECTOR_COUNT:
		        *(unsigned int*)ucRxBuffer = FLASH_SECTOR_COUNT;		/* ��ȡ������ */
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_SIZE:
		        *(unsigned short*)ucRxBuffer = FLASH_SECTOR_SIZE;		/* ��ȡ������С */
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
				*(unsigned short*)ucRxBuffer = FLASH_BLOCK_SIZE;		/* ��ȡ���С */
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
	}
	else 
		res=RES_ERROR; /* �����Ĳ�֧�� */
	return 
	  res;
}

DWORD get_fattime (void)
{				 
	return 0;
}
#endif
/* FATFS�ļ�ϵͳ end */
