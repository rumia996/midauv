#include <stdint.h>
#include <stdio.h>
/*用于与32进行通信的设备,实现与32硬件校验相同的结果*/
uint32_t soft_crc32_st(uint32_t* pbuf, uint32_t size)
{
	const uint32_t st_const_value = 0x04c11db7;
	uint32_t	crc_value = 0xffffffff;
	uint32_t	xbit;
	uint32_t 	bits;
	uint32_t	i;

	for (i = 0; i < size; i++)
	{
		xbit = 0x80000000;
		for (bits = 0; bits < 32; bits++)
		{
			if (crc_value & 0x80000000)
			{
				crc_value <<= 1;
				crc_value ^= st_const_value;
			}
			else
			{
				crc_value <<= 1;
			}
			if (pbuf[i] & xbit)
			{
				crc_value ^= st_const_value;
			}
			xbit >>= 1;
		}
	}
	return crc_value;
}
int main()
{
	//数据帧中所有数据小端排序,例如0xA9F20A86 在内存中为0x86 0x0A 0xF2 0xA9用串口调试助手打印是86 0A F2 A9
	uint8_t data_from32[86] = { 0 };//存储32发送的回报数据,共86字节
	uint8_t crc_databuf[80] = { 0 };//存储参与CRC校验的数据,80是参与crc校验的数据字节数,32中进行校验的数据必须与uint32对齐 1个uint32=4个字节 所以这里必须是4的倍数
	const uint8_t factcrcdatalength = 78;//实际有意义的数据字节数,若不为4的倍数需要在末尾补0
	memcpy(crc_databuf, &data_from32[3], factcrcdatalength);//从32数据帧中取帧头@SD之后,从横滚角到动力电池的78字节数据,末尾补2个0x00凑成80字节数据,存入 crc_databuf
	// 计算 CRC
	uint32_t crc_result = soft_crc32_st(crc_databuf, sizeof(crc_databuf) / 4);//一次计算4个字节,总的计算次数为crc_databuf的字节数/4
	printf("CRC32 result: 0x%08X\n", crc_result);	
	return 0;
}

//示例
void example() 
{
	uint32_t data[] = {
		0x12345678,
		//0x44434241
	};
	uint32_t crc_result = soft_crc32_st(data, sizeof(data) / 4);

	uint8_t data2[] = {
		0x78,0x56,0x34,0x12,
		//0x41,0x42,0x43,0x44

		//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,
		//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,
		//0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,
		//0x78 ,0x56 ,0x34 ,0x12 ,0x78 ,0x56 ,0x34 ,0x12 ,
		//0x78 ,0x56 ,0x34 ,0x12 ,0x78 ,0x56 ,0x34 ,0x12 ,
		//0x78 ,0x56 ,0x34 ,0x12 ,0x78 ,0x56 ,0x34 ,0x12 ,
		//0x78 ,0x56 ,0x34 ,0x12 ,0x78 ,0x56 ,0x34 ,0x12 ,
		//0x78 ,0x56 ,0x34 ,0x12 ,0xDC ,0x05 ,0xDC ,0x05 ,
		//0xDC ,0x05 ,0xDC ,0x05 ,0xDC ,0x05 ,0xDC ,0x05 ,
		//0xDC ,0x05 ,0x34 ,0x12 ,0x34 ,0x12 ,0x00 ,0x00 ,
	};
	uint32_t crc_result2 = soft_crc32_st(data2, sizeof(data2) / 4);

	printf("CRC32 result: 0x%08X\n", crc_result);
	printf("CRC32 result: 0x%08X\n", crc_result2);
	//输入0x44434241,正确的结果为0xCF534AE1
	//输入0x12345678,正确的结果为0xDF8A8A2B
}
