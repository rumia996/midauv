/****************************************************************************

* Sigma�Ŷ�

* �ļ���: ocd_oled.c

* ���ݼ�����4��OLED������SSD1306��
			��I/O����ѡ������ΪSCL��SDA��ʵ��OLED�������������ֿ�6*8�Լ�8*16.
			6*8�ַ�ռһҳ����y = 1
			8*16�ַ�ռ��ҳ����y = 2 
* �ļ���ʷ��

* �汾��	����		����		˵��
*  2.7 	 2023-07-21   �����	��ʽ�Ż�

* 1.1.5  2022-09-08   �����	�������ļ�

****************************************************************************/
#include "ocd_oled.h"
#include "ocd_oled_font.h"

/**
 * @brief 	��SSD1306��д��һ���ֽ����ݻ�������
 * @param   _tOLED-OLED�ṹ��
 * @param 	_ucDat-����
 * @param 	_ucCmd-����/����   OLED_CMD(0)����;OLED_DATA(1)����;
 * @retval	NULL
 */
static void S_OLED_WR_Byte(tagOLED_T *_tOLED,uint8_t _ucDat,uint8_t _ucCmd)
{
    Drv_IICSoft_Start(&_tOLED->tIIC);
    Drv_IICSoft_SendByte(&_tOLED->tIIC,OLED_ADDR);
    Drv_IICSoft_WaitAck(&_tOLED->tIIC);
    if(_ucCmd)
    {
        /* д������ */
        Drv_IICSoft_SendByte(&_tOLED->tIIC,0x40);
    }
    else
    {
        /* д������ */
        Drv_IICSoft_SendByte(&_tOLED->tIIC,0x00);
    }
    Drv_IICSoft_WaitAck(&_tOLED->tIIC);
    Drv_IICSoft_SendByte(&_tOLED->tIIC,_ucDat);
    Drv_IICSoft_WaitAck(&_tOLED->tIIC);
    Drv_IICSoft_Stop(&_tOLED->tIIC);
}

/**
 * @brief 	OLEDָ������
 * @param 	m-����
 * @param 	n-ָ��
 * @retval	uint32_t-m^n�Ľ��
 */
static uint32_t S_OLED_Pow(uint8_t m,uint8_t n)
{
	uint32_t result = 1;

	while(n--)	result *= m;
	return result;
}

/******************************************* ������OLED������غ��� ************************************************/
/**
 * @brief 	OLED��ʾ��������
 * @param   _tOLED-OLED�ṹ��
 * @retval	NULL
 */
void OCD_OLED_DisplayON(tagOLED_T *_tOLED)
{
    S_OLED_WR_Byte(_tOLED,0x8D,OLED_CMD);  /* SET DCDC���� */
    S_OLED_WR_Byte(_tOLED,0X14,OLED_CMD);  /* DCDC ON */
	S_OLED_WR_Byte(_tOLED,0XAF,OLED_CMD);  /* DISPLAY ON */
}

/**
 * @brief 	OLED��ʾ�رպ���
 * @param   _tOLED-OLED�ṹ��
 * @retval	NULL
 */
void OCD_OLED_DisplayOFF(tagOLED_T *_tOLED)
{
    S_OLED_WR_Byte(_tOLED,0x8D,OLED_CMD);  /* SET DCDC���� */
    S_OLED_WR_Byte(_tOLED,0X10,OLED_CMD);  /* DCDC OFF */
	S_OLED_WR_Byte(_tOLED,0XAE,OLED_CMD);  /* DISPLAY OFF */
}

/**
 * @brief 	OLED��������
 * @param   _tOLED-OLED�ṹ��
 * @retval	NULL
 */
void OCD_OLED_Clear(tagOLED_T *_tOLED)
{
    uint8_t i,n;
    
    for(i = 0;i < 8;i++)
    {	
		S_OLED_WR_Byte(_tOLED,0xb0+i,OLED_CMD);
		S_OLED_WR_Byte(_tOLED,0x00,OLED_CMD);
		S_OLED_WR_Byte(_tOLED,0x10,OLED_CMD);
        for(n = 0;n < 128;n++)  S_OLED_WR_Byte(_tOLED,0,OLED_DATA);
    }
}

/**
 * @brief 	OLED���ù��λ��
 * @param   _tOLED-OLED�ṹ��
 * @param 	x-������	x(0-127)
 * @param 	_ucPage-ҳ�� (0-7)
 * @retval	NULL
 */
void OCD_OLED_SetCursorAddress(tagOLED_T *_tOLED,uint8_t x,uint8_t _ucPage)
{
	S_OLED_WR_Byte(_tOLED,0xB0+_ucPage,OLED_CMD); 			/* ����ҳ��ַ */
	S_OLED_WR_Byte(_tOLED,(x&0xF0)>>4|0x10,OLED_CMD);		/* �����и���ʼ��ַ(���ֽ�) */
	S_OLED_WR_Byte(_tOLED,(x&0x0F)|0x00,OLED_CMD);   		/* �����е���ʼ��ַ(���ֽ�) */			
}

/**
 * @brief 	OLED��ҳ��������
 * @param   _tOLED-OLED�ṹ��
 * @param 	_ucPage-ҳ��(0-7)
 * @retval	NULL
 */
void OCD_OLED_PageClear(tagOLED_T *_tOLED,uint8_t _ucPage)
{
	uint8_t j;

	S_OLED_WR_Byte(_tOLED,0xB0+_ucPage,OLED_CMD); /* ����ҳ��ַ */
	S_OLED_WR_Byte(_tOLED,0x10,OLED_CMD);      	 /* �����и���ʼ��ַ(���ֽ�) */
	S_OLED_WR_Byte(_tOLED,0x00,OLED_CMD);      	 /* �����е���ʼ��ַ(���ֽ�) */
	for(j = 0;j < 128;j++)
	{
		S_OLED_WR_Byte(_tOLED,0,OLED_DATA);  /* д���� */
	}
}

/**
 * @brief 	OLEDдһ���ַ�
 * @param   _tOLED-OLED�ṹ��
 * @param 	x-������	x(0-127)
 * @param 	_ucPage-ҳ�� (0-7)
 * @param 	_ucStr-��д�ַ�
 * @param	_ucChar_Size-��ʾ��С ��ѡ8����16
 * @retval	NULL
 */
void OCD_OLED_ShowChar(tagOLED_T *_tOLED,uint8_t x,uint8_t _ucPage,uint8_t _ucStr,uint8_t _ucChar_Size)
{      	
	uint8_t addr = 0,i = 0;
	uint8_t *strflag = &_ucStr;

	if(*strflag >= ' ' && *strflag <= '~') /* ��ʾӢ�� */
	{
		addr = *strflag - ' '; /* ȡģ�ӿո�ʼ�ģ������±� */
		if(_ucChar_Size == 16)
		{
			/* д8*16ASCII�ַ����ϰ벿�� */
			OCD_OLED_SetCursorAddress(_tOLED,x,_ucPage); /* ���ù���λ�� */

			for(i = 0;i < _ucChar_Size/2;i++)      	/* ����дwidth�� */
			{
				S_OLED_WR_Byte(_tOLED,ASCII_8_16[addr][i],OLED_DATA); 
			}

			/* д8*16ASCII�ַ����°벿�� */
			OCD_OLED_SetCursorAddress(_tOLED,x,_ucPage+1); /* ���ù���λ�� */

			for(i = 0;i < _ucChar_Size/2;i++)        /* ����дwidth�� */
			{
				S_OLED_WR_Byte(_tOLED,ASCII_8_16[addr][i+_ucChar_Size/2],OLED_DATA); 
			}
			strflag++; /* ������ʾ��һ���ַ� */
		}
		else if(_ucChar_Size == 8)
		{
			/* д6*8ASCII�ַ� */
			OCD_OLED_SetCursorAddress(_tOLED,x,_ucPage); /* ���ù���λ�� */

			for(i = 0;i < 6;i++)      /* ����дwidth�� */
			{
				S_OLED_WR_Byte(_tOLED,ASCII_6_8[addr][i],OLED_DATA); 
			}
			strflag++; /* ������ʾ��һ���ַ� */
		}
	}
}

/**
 * @brief 	OLEDд�ַ���
 * @param   _tOLED-OLED�ṹ��
 * @param 	x-������	x(0-127)
 * @param 	_ucPage-ҳ�� (0-7)
 * @param 	_ucpStr-�ַ����׵�ַ��������ʾ�ַ�������
 * @param	_ucChar_Size-��ʾ��С ��ѡ8����16
 * @retval	NULL
 */
void OCD_OLED_ShowString(tagOLED_T *_tOLED,uint8_t x,uint8_t _ucPage,char *_cpStr,uint8_t _ucChar_Size)
{
	uint8_t i = 0;
    
	while(_cpStr[i] != '\0') /* ������ʾ */
	{
		OCD_OLED_ShowChar(_tOLED,x,_ucPage,_cpStr[i],_ucChar_Size);
		/* �����8*16���� */
        if(_ucChar_Size == 16)
		{
			x += _ucChar_Size/2;
			/* ������� */
			if((x+8) > MAX_COLUMN)
			{
				x = 0;
				_ucPage += 2;
			}
		}

		/* �����6*8���� */
		else if(_ucChar_Size == 8)
		{
			x += 6;
			/* ������� */
			if((x+6) > MAX_COLUMN)
			{
				x = 0;
				_ucPage += 1;
			}
		}
		i++;
	}
}

/**
 * @brief 	OLED��ʾ����
 * @param   _tOLED-OLED�ṹ��
 * @param 	x-������	x(0-127)
 * @param 	_ucPage-ҳ�� (0-7)
 * @param 	_ulNum-����ʾ����
 * @param	_ucLen-����ʾ���ֳ���
 * @param	_ucChar_Size-��ʾ��С ��ѡ8����16
 * @retval	NULL
 */
void OCD_OLED_ShowNum(tagOLED_T *_tOLED,uint8_t x,uint8_t _ucPage,uint32_t _ulNum,uint8_t _ucLen,uint8_t _ucChar_Size)
{         	
	uint8_t t,temp;
	uint8_t enshow = 0;

	for(t = 0;t < _ucLen;t++)
	{
		temp = (_ulNum/S_OLED_Pow(10,_ucLen-t-1))%10;
		if(enshow == 0 && t < (_ucLen-1))
		{
			if(temp == 0)
			{
				if(_ucChar_Size == 16)	OCD_OLED_ShowChar(_tOLED,x+(_ucChar_Size/2)*t,_ucPage,' ',_ucChar_Size);
				else if(_ucChar_Size == 8)	OCD_OLED_ShowChar(_tOLED,x+_ucChar_Size*t,_ucPage,' ',_ucChar_Size);
				continue;
			}else enshow = 1;
		 	 
		}
	 	if(_ucChar_Size == 16)	OCD_OLED_ShowChar(_tOLED,x+(_ucChar_Size/2)*t,_ucPage,temp+'0',_ucChar_Size);
		else if(_ucChar_Size == 8)	OCD_OLED_ShowChar(_tOLED,x+6*t,_ucPage,temp+'0',_ucChar_Size);
	}
} 

/**
 * @brief 	OLED��ʾ������
 * @param   _tOLED-OLED�ṹ��
 * @param 	x-������	x(0-127)
 * @param 	_ucPage-ҳ�� (0-7)
 * @param 	_fNum-����ʾ������
 * @param	_ucChar_Size-��ʾ��С ��ѡ8����16
 * @retval	NULL
 */
void OCD_OLED_ShowFloatNum(tagOLED_T *_tOLED,uint8_t x,uint8_t _ucPage,float _fNum,uint8_t _ucChar_Size)
{
	char Data[] = "";
	sprintf(Data,"%.3f",_fNum);
	uint8_t i = 0;

	while(Data[i] != '\0') /* ������ʾ */
	{
		OCD_OLED_ShowChar(_tOLED,x,_ucPage,Data[i],_ucChar_Size);
        if(_ucChar_Size == 16)	/* �����8*16���� */
		{
			x += _ucChar_Size/2;
			if((x+_ucChar_Size/2) > MAX_COLUMN)  {x = 0;_ucPage += 2;}	/* ������� */
		}
		else if(_ucChar_Size == 8) /* �����6*8���� */
		{
			x += 6;
			if((x+6) > MAX_COLUMN)  {x = 0;_ucPage += 1;}	/* ������� */
		}
		i++;
	}
}

/**
 * @brief 	OLED��ʾBMPλͼ
 * @param   _tOLED-OLED�ṹ��
 * @param 	x0-��ʼ������	������	x(0-127)
 * @param 	y0-��ʼҳ��		ҳ��	y(0-7)
 * @param	x1-��ֹ������	������	x(0-127)
 * @param	y1-��ֹҳ��		ҳ��	y(0-7)
 * @param 	_ucaBMP-bmp��������
 * @retval	NULL
 */
void OCD_OLED_ShowBMP(tagOLED_T *_tOLED,uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t _ucaBMP[])
{ 	
	unsigned int j = 0;
	uint8_t x,y;
  
  	if(y1%8 == 0) y = y1/8;      
  	else y = y1/8 + 1;

	for(y = y0;y < y1;y++)
	{
		OCD_OLED_SetCursorAddress(_tOLED,x0,y);
    	for(x = x0;x < x1;x++)
	    {     
	    	S_OLED_WR_Byte(_tOLED,_ucaBMP[j++],OLED_DATA);	    	
	    }
	}
} 

/**
 * @brief 	OLED��ʼ������
 * @param   _tOLED-OLED�ṹ��
 * @retval	NULL
 */
void OCD_OLED_Init(tagOLED_T *_tOLED)
{
    Drv_IICSoft_Init(&_tOLED->tIIC);
    Drv_Delay_Ms(500);
    S_OLED_WR_Byte(_tOLED,0xAE,OLED_CMD);	/* �ر���ʾ */
	S_OLED_WR_Byte(_tOLED,0x00,OLED_CMD);	/* ���õ��е�ַ */
	S_OLED_WR_Byte(_tOLED,0x10,OLED_CMD);	/* ���ø��е�ַ */
	S_OLED_WR_Byte(_tOLED,0x40,OLED_CMD);	/* ������ʼ�е�ַ,��ӳ��RAM��ʾ��ʼ��(0x00~0x3F) */
	S_OLED_WR_Byte(_tOLED,0x81,OLED_CMD);	/* ���öԱȶȿ��ƼĴ��� */
	S_OLED_WR_Byte(_tOLED,0xCF,OLED_CMD);	/* ����SEG����������� */
	S_OLED_WR_Byte(_tOLED,0xA1,OLED_CMD);	/* ���ض�������,bit0:0,0->0;1,0->127; 0xa0���ҷ��� 0xa1���� */
	S_OLED_WR_Byte(_tOLED,0xC8,OLED_CMD);	/* ����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��   0xc0���·��� 0xc8���� */
	S_OLED_WR_Byte(_tOLED,0xA6,OLED_CMD);	/* ����������ʾ(������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	) */
	S_OLED_WR_Byte(_tOLED,0xA8,OLED_CMD);	/* ��������·�� ���ö�·���ñ�(1��64) */
	S_OLED_WR_Byte(_tOLED,0x3F,OLED_CMD);	/* 1/64 duty(Ĭ��0X3F(1/64)) */
	S_OLED_WR_Byte(_tOLED,0xD3,OLED_CMD);	/* ������ʾƫ��λ��ӳ��RAM������(0x00~0x3F) */
	S_OLED_WR_Byte(_tOLED,0x00,OLED_CMD);	/* -not offset */
	S_OLED_WR_Byte(_tOLED,0xD5,OLED_CMD);	/* ������ʾʱ�ӷ�Ƶ��/����Ƶ�� */
	S_OLED_WR_Byte(_tOLED,0x80,OLED_CMD);	/* ���÷�Ƶ�ȣ�����ʱ��Ϊ100֡/�� */
	S_OLED_WR_Byte(_tOLED,0xD9,OLED_CMD);	/* ����Ԥ������� */
	S_OLED_WR_Byte(_tOLED,0xF1,OLED_CMD);	/* ����Ԥ��15��ʱ�ӣ��ŵ�1��ʱ��([3:0],PHASE 1;[7:4],PHASE 2;) */
	S_OLED_WR_Byte(_tOLED,0xDA,OLED_CMD);	/* ����COMӲ���������� */
	S_OLED_WR_Byte(_tOLED,0x12,OLED_CMD);	/* [5:4]���� */
	S_OLED_WR_Byte(_tOLED,0xDB,OLED_CMD);	/* ����VCOMH ��ѹ���� */
	S_OLED_WR_Byte(_tOLED,0x40,OLED_CMD);	/* Set VCOM �ͷŵ�ѹ([6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;) */
	S_OLED_WR_Byte(_tOLED,0x20,OLED_CMD);	/* ����ҳ��Ѱַģʽ(0x00/0x01/0x02) */
	S_OLED_WR_Byte(_tOLED,0x02,OLED_CMD);	/* [1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10; */
	S_OLED_WR_Byte(_tOLED,0x8D,OLED_CMD);	/* ���ó�������/���� */
	S_OLED_WR_Byte(_tOLED,0x14,OLED_CMD);	/* ����(0x10����,0x14����) */
	S_OLED_WR_Byte(_tOLED,0xA4,OLED_CMD);	/* ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����) (0xa4/0xa5) */
	S_OLED_WR_Byte(_tOLED,0xA6,OLED_CMD);	/* ������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ (0xa6/a7)  */
	S_OLED_WR_Byte(_tOLED,0xAF,OLED_CMD);	/* ������ʾ	 */
	OCD_OLED_Clear(_tOLED);
	OCD_OLED_SetCursorAddress(_tOLED,0,0);
}
