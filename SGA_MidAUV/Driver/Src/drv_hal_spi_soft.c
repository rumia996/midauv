/****************************************************************************

* Sigma�Ŷ�

* �ļ���: drv_hal_spi_soft.c

* ���ݼ��������spiģ�������ļ�

* �ļ���ʷ��

* �汾��		����		����		˵��
*  1.2		2023-2-22	  �����     ���վ����Դ������ͳһ��ʼ��˳��

* 1.1.8 	2022-10-25     ���		 �������ļ�

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_SPI_SOFT_ENABLE

/**
 * @brief SPICLK ���
 * @param _tSPISoft-���spi�ṹ��ָ��
 * @param _ucLevel-��ƽ 0-�� 1-��  
 * @retval NULL
*/
static void S_SPI_CLK(tagSPISoft_T *_tSPISoft, uint8_t _ucLevel)
{
	HAL_GPIO_WritePin(_tSPISoft->tSPISoft[0].tGPIOPort, _tSPISoft->tSPISoft[0].tGPIOInit.Pin, _ucLevel?GPIO_PIN_SET:GPIO_PIN_RESET);
}

/**
 * @brief SPIMISO ���
 * @param _tSPISoft-���spi�ṹ��ָ��
 * @retval GPIO_PinState-�����ĵ�ƽ
*/
static GPIO_PinState S_SPI_MISO(tagSPISoft_T *_tSPISoft)
{
	return HAL_GPIO_ReadPin(_tSPISoft->tSPISoft[1].tGPIOPort, _tSPISoft->tSPISoft[1].tGPIOInit.Pin);
}

/**
 * @brief SPIMOSI ���
 * @param _tSPISoft-���spi�ṹ��ָ��
 * @param _ucLevel-��ƽ 0-�� 1-��  
 * @retval NULL
*/
static void S_SPI_MOSI(tagSPISoft_T *_tSPISoft, uint8_t _ucLevel)
{
	HAL_GPIO_WritePin(_tSPISoft->tSPISoft[2].tGPIOPort, _tSPISoft->tSPISoft[2].tGPIOInit.Pin, _ucLevel?GPIO_PIN_SET:GPIO_PIN_RESET);
}

/**
 * @brief SPIƬѡ
 * @param _tSPISoft-���spi�ṹ��ָ��
 * @param _ucLevel-���� 0-�� 1-��  
 * @retval NULL
*/
void Drv_SPI_NSS(tagSPISoft_T *_tSPISoft,uint8_t _ucLevel)
{
	HAL_GPIO_WritePin(_tSPISoft->tSPISoft[3].tGPIOPort, _tSPISoft->tSPISoft[3].tGPIOInit.Pin, _ucLevel?GPIO_PIN_SET:GPIO_PIN_RESET);
}

/**
 * @brief ��дһ���ֽ�
 * @param _tSPISoft-���spi�ṹ��ָ��
 * @param _ucData-д������  
 * @retval ��ȡ������
*/
uint8_t Drv_SPI_WriteByte_Soft(tagSPISoft_T *_tSPISoft,uint8_t _ucData)
{
	uint8_t i = 0;
	uint8_t temp = 0;
	uint8_t read_data = 0xFF;
	
	for(i=0;i<8;i++) 
	{
		temp = ((_ucData&0x80)==0x80)? 1:0;
		_ucData = _ucData<<1;
		read_data = read_data<<1;
		S_SPI_CLK(_tSPISoft,0);
		S_SPI_MOSI(_tSPISoft,temp);
		SPI_Delay
		S_SPI_CLK(_tSPISoft,1);
		SPI_Delay
		if(S_SPI_MISO(_tSPISoft)==1) {read_data = read_data + 1; } 
	}
	S_SPI_CLK(_tSPISoft,0);
	return read_data;
}

/**
 * @brief ģ��SPI��ʼ��
 * @param _tSPISoft-SPI�ṹ��ָ��
 * @retval Null 
*/
void Drv_SPISoft_Init(tagSPISoft_T *_tSPISoft)
{
	Drv_GPIO_Init(_tSPISoft->tSPISoft, 4);
}


#endif

