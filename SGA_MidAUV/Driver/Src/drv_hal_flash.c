/****************************************************************************

* Sigma�Ŷ�

* �ļ���: drv_hal_flash.c

* ���ݼ�����Flash�����ļ�

* �ļ���ʷ��

* �汾��	   ����		  ����		  ˵��
*  2.8	 	2023-09-22	�����		�������ļ�

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_FLASH_ENABLE

#ifdef STM32F1_SGA_ENABLE

/**
 * @brief Flash����ҳ��F1ϵ�У�
 * 
 * @param _ulPageAddr ��ʼ��ַ
 * @param _ulNum ������ҳ��
 * @return int 0 �ɹ� -1 ʧ��
 */
int Drv_Flash_Erase_Page(uint32_t _ulPageAddr,uint32_t _ulNum)
{
    /* ����Flash */
	HAL_FLASH_Unlock();

    /* ����Flash*/
	FLASH_EraseInitTypeDef FlashSet;
	FlashSet.TypeErase = FLASH_TYPEERASE_PAGES;
	FlashSet.PageAddress = _ulPageAddr;
	FlashSet.NbPages = _ulNum;

    /*����PageError�����ò�������*/
	uint32_t PageError = 0;
	if(HAL_FLASHEx_Erase(&FlashSet, &PageError) == HAL_ERROR)
    {
        /* ����flash */
	    HAL_FLASH_Lock();
        return -1;
    }

    /* ����flash */
	HAL_FLASH_Lock();

    return 0;
}
#endif

#ifdef STM32F4_SGA_ENABLE

/**
 * @brief ��ȡ��ַ���ڵ�sector
 * @param _ulAddress  ��ʼ��ַ	
 * @return uint32_t ulSector ������
 */
static uint32_t S_Flash_Get_Sector(uint32_t _ulAddress)
{
    uint32_t ulSector = 0;

	(_ulAddress <= 0x080FFFFF && _ulAddress >= 0x080E0000)? ulSector = 11:
	(_ulAddress >= 0x080C0000)? ulSector = 10:
	(_ulAddress >= 0x080A0000)? ulSector = 9:
	(_ulAddress >= 0x08080000)? ulSector = 8:
	(_ulAddress >= 0x08060000)? ulSector = 7:
	(_ulAddress >= 0x08040000)? ulSector = 6:
	(_ulAddress >= 0x08020000)? ulSector = 5:
	(_ulAddress >= 0x08010000)? ulSector = 4:
	(_ulAddress >= 0x0800C000)? ulSector = 3:
	(_ulAddress >= 0x08008000)? ulSector = 2:
	(_ulAddress >= 0x08006000)? ulSector = 1:
	(_ulAddress >= 0x08004000)? ulSector = 1:0;
	
    return ulSector;
}

/**
 * @brief Flash����Sector��F4ϵ�У�
 * @param _ulStart_Addr  ��ʼ��ַ	
 * @param _ulEnd_Addr    ������ַ
 * @return 0 �ɹ� -1 ʧ��
 */
int Drv_Flash_Erase_Sector(uint32_t _ulStart_Addr, uint32_t _ulEnd_Addr)
{
	uint32_t UserStartSector;
	uint32_t SectorError = 0;
	FLASH_EraseInitTypeDef FlashSet;

	/* ����flash */
	HAL_FLASH_Unlock();
	
	/* ��ȡ��ʼ��ַ������������FLASH*/
	UserStartSector = S_Flash_Get_Sector(_ulStart_Addr);

	FlashSet.TypeErase = TYPEERASE_SECTORS;
	FlashSet.Sector = UserStartSector;
	FlashSet.NbSectors = S_Flash_Get_Sector(_ulEnd_Addr) - UserStartSector;
	FlashSet.VoltageRange = VOLTAGE_RANGE_3;
    
	/*���ò�������*/
	if(HAL_FLASHEx_Erase(&FlashSet, &SectorError) == HAL_ERROR)
    {
        /* ����Flash */
	    HAL_FLASH_Lock();
        return -1;
    }
	
	/* ����Flash */
	HAL_FLASH_Lock();
	return 0;
}

#endif

/**
 * @brief Flashд���ɸ�����(word)
 * @param _ulAddr      д��ĵ�ַ
 * @param _ulpBuf      д�����ݵ���ʼ��ַ
 * @param _ulWordSize  ����
 * @return NULL
 */
void Drv_Flash_Write(uint32_t _ulAddr,uint32_t *_ulpBuf,uint32_t _ulWordSize)
{
	/* ����Flash */
	HAL_FLASH_Unlock();

	/* ��д��ָ����ַ */
	for(uint32_t i = 0;i < _ulWordSize;i++)
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,_ulAddr + 4*i,_ulpBuf[i]);

	/* ����Flash */
	HAL_FLASH_Lock();
    
    return;
}

/**
 * @brief Flash�����ɸ�����(word)
 * @param _ulAddr      �����ݵĵ�ַ
 * @param _ulpBuf      �������ݵ�����ָ��
 * @param _ulWordSize  ����
 * @return NULL
 */
void Drv_Flash_Read(uint32_t _ulAddr, uint32_t *_ulpBuf,uint32_t _ulWordSize)
{
	memcpy(_ulpBuf, (uint32_t*) _ulAddr, _ulWordSize * sizeof(uint32_t));
}

#endif
