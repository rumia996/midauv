#ifndef __ALGO_PID_H_
#define __ALGO_PID_H_

#include "drv_hal_conf.h"

typedef struct pid
{
    float fKp;          /* ����ϵ�� */
    float fKi;          /* ΢��ϵ�� */
    float fKd;          /* ����ϵ�� */

    float fMax_Out;     /* PID������ */
    float fMax_Iout;    /* PID��������� */

    float fExp_Value;   /* �趨������ֵ */
    float fCurr_Value;  /* ��ǰPIDֵ */

    float fPre_Value;   /* ǰһ��PIDֵ */
    float fCtrl_Out;    /* ����������ֵ */
    
    float fPout;        /* ��������� */
    float fIout;        /* ��������� */
    float fDout;        /* ΢������� */

    float fDbuf[3];     /* ΢�����������ֵ 0Ϊ���� 1��һ�� 2���ϴ� */
    float fError[3];    /* ������������ֵ 0Ϊ���� 1��һ�� 2���ϴ� */

}tagPID_T;

void Algo_PID_Update(tagPID_T *_tPid,float _faPID[3]);
float Algo_PID_Calculate(tagPID_T *_tPid,float _fCurrValue,float _fExpValue);
void Algo_PID_Clear(tagPID_T *_tPid);
void Algo_PID_Init(tagPID_T *_tPid);

#endif
