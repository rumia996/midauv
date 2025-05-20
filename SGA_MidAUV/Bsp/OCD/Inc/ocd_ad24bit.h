#ifndef __OCD_AD24bit_H_
#define __OCD_AD24bit_H_

#include "drv_hal_conf.h"

#define	Comm_ERROR -21//SPIͨ��������
#define	REG_ADDR_ERROR  -22//��ȡ�Ĵ�����ַ����
#define	REG_READ_ERROR  -23//������
#define	REG_WRITE_ERROR  -24//д����
#define	REG_VALUE_ERROR  -25//ֵ����
#define REG_TIMEROUT_ERROR  -26//��ʱ����
#define PORT_NUM_ERROR  -27//�˿ںŴ���
#define RESULT_TIMEROUT_ERROR  -28//��ȡ�����ʱ

#define REG_READ_SUCCESS  0//�����ݳɹ�
#define RESULT_SUCCESS  0//�����ݳɹ�
typedef enum {
	AD24BIT_DISABLE,
	AD24BIT_USE_CRC,
	AD24BIT_USE_XOR,
} ad24bit_crc_mode;

typedef enum {
	VIN0,
	VIN1,
	VIN2,
	VIN3,
	VIN4,
	VIN5,
	VIN6,
	VIN7,
	VIN1_0,
	VIN3_2,
	VIN5_4,
	VIN7_6,
	IIN0,
	IIN1,
	IIN2,
	IIN3,
	PORT_NULL,
} input_port;


typedef enum {
 PORT_DISABLE= 0,  
 PORT_ENABLE, 
} port_enable;

/*! Port measurement info */
typedef struct {
	port_enable enable;
	input_port  num;
	uint32_t value; // ad�����������
	double gain;
	double offset;
	double result;// ad����ĵ�ѹ������ֵ
} ad24bit_input_port;

/*! AD24BIT register info */
typedef struct {
	int32_t addr;
	int32_t value;
	int32_t size;
} ad24bit_st_reg;
/*
 * The structure describes the device and is used with the AD24BIT driver.
 * @slave_select_id: The ID of the Slave Select to be passed to the SPI calls.
 * @regs: A reference to the register list of the device that the user must
 *       provide when calling the Setup() function.
 * @num_regs: The length of the register list.
 * @userCRC: Error check type to use on SPI transfers.
 */
typedef struct {
	/* SPI */
	tagSPI_T		tSPI;
	/* Device Settings */
	ad24bit_st_reg		*regs;
	uint8_t			num_regs;
	ad24bit_crc_mode		useCRC;
	ad24bit_input_port  port[12];
	uint8_t			port_num;
}tagAD24BIT_T;






/*****************************************************************************/
/******************* AD24BIT Constants ****************************************/
/*****************************************************************************/
#define AD24BIT_CRC8_POLYNOMIAL_REPRESENTATION 0x07 /* x8 + x2 + x + 1 */

/*****************************************************************************/
/************************ Functions Declarations *****************************/
/*****************************************************************************/


/**
 * @brief ��ʼ��ADоƬ
 * @param device-��ʼ��AD24BITʵ��ָ��
 * @retval  0 :��ʼ���ɹ�
						-1��device������
						Comm_ERROR:SPIͨ�ų����������ߺ��������device->tSPI.tSPIHandle.Instance;
*/
int32_t OCD_AD24BIT_Init(tagAD24BIT_T* device );


/**
 * @brief ��ȡAD24bitת����ĵ�ѹ�����ֵ������洢�ڶ�Ӧ�˿ڵ�device->port[i].result��
 * @param device-��ʼ��AD24BITʵ��ָ��,timeout-�趨��ʱʱ�䵥λms���趨ʱ����û�ж�ȡ���������˳�
 * @retval  RESULT_SUCCESS :��ȡ�ɹ�
						REG_ADDR_ERROR:����device����
					  REG_TIMEROUT_ERROR��ADת����ʱ��һ����SPIӲ���ӿ�����
					  REG_READ_ERROR����ȡʧ�ܣ�һ����SPIӲ���ӿ�����
						RESULT_TIMEROUT_ERROR;//��ʱ����
*/
int32_t OCD_AD24bit_GetResult(tagAD24BIT_T *device,uint32_t timeout);
/**
 * @brief ��ȡAD24bitת�������ֵ������洢�ڶ�Ӧ�˿ڵ�device->port[i].value��
 * @param device-��ʼ��AD24BITʵ��ָ��,timeout-�趨��ʱʱ�䵥λms���趨ʱ����û�ж�ȡ���������˳�
 * @retval  RESULT_SUCCESS :��ȡ�ɹ�
						REG_ADDR_ERROR:����device����
					  REG_TIMEROUT_ERROR��ADת����ʱ��һ����SPIӲ���ӿ�����
					  REG_READ_ERROR����ȡʧ�ܣ�һ����SPIӲ���ӿ�����
						RESULT_TIMEROUT_ERROR;//��ʱ����
*/
int32_t OCD_AD24bit_GetValue(tagAD24BIT_T *device,uint32_t timeout);
#endif
