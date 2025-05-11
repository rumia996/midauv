/****************************************************************************

* Sigma�Ŷ�

* �ļ���: algo_pid.c

* ���ݼ�����PID��غ���

* �ļ���ʷ��

* �汾��		����	    ����		    ˵��
*  2.4 	    2023-05-12	  �����		 �������ļ�

****************************************************************************/
#include "algo_pid.h"

/**
 * @brief ����PIDϵ������
 * @param _tPid-PID�ṹ��ָ��
 * @param _faPID-PIDϵ����Ҫ����˳��ΪP,I,D
 * @retval Null
*/
void Algo_PID_Update(tagPID_T *_tPid,float _faPID[3])
{
    /* ����PIDϵ�� */
    _tPid->fKp = _faPID[0];
    _tPid->fKi = _faPID[1];
    _tPid->fKd = _faPID[2];
}

/**
 * @brief PID���㺯��
 * @param _tPid-PID�ṹ��ָ��
 * @param _fCurrValue-��ǰֵ
 * @param _fExpValue-����ֵ
 * @retval float ����PID���������ֵ
*/
float Algo_PID_Calculate(tagPID_T *_tPid,float _fCurrValue,float _fExpValue)
{
    int Delt_Out; /* PID�������ֵ */

    /* �趨����ֵ�͵�ǰֵ */
    _tPid->fExp_Value  = _fExpValue;
    _tPid->fCurr_Value = _fCurrValue;
    
    _tPid->fError[0] = _fExpValue - _fCurrValue;

    /* ��������ʽPID */

    /* �Ա���������ϴ����Ĳ�ֵ��Ϊ����������������� */
    _tPid->fPout = _tPid->fKp * (_tPid->fError[0] - _tPid->fError[1]);

    /* �Ա��������Ϊ������������ */
    _tPid->fIout = _tPid->fKi * _tPid->fError[0];

    /* ����΢��������� */
    _tPid->fDbuf[2] = _tPid->fDbuf[1];
    _tPid->fDbuf[1] = _tPid->fDbuf[0];

    /* �Ա���������ϴ����Ĳ�ֵ��ȥ�ϴ���������ϴ����Ĳ�ֵ��Ϊ΢��������������� */
    _tPid->fDbuf[0] = (_tPid->fError[0] - 2.0f * _tPid->fError[1] + _tPid->fError[2]);
    _tPid->fDout = _tPid->fKd * _tPid->fDbuf[0];
    
    /* ������� */
    Delt_Out = (int)(_tPid->fPout + _tPid->fIout + _tPid->fDout);    /* ����������� */
    _tPid->fCtrl_Out = _tPid->fPre_Value + Delt_Out;                 /* ��ǰֵ�ۼ� */
    _tPid->fPre_Value = _tPid->fCtrl_Out;                            /* ��¼��ֵ��Ϊ�´�PID��׼�� */

    /* ��Ź�ȥ�������ֵ */
    _tPid->fError[2] = _tPid->fError[1];
    _tPid->fError[1] = _tPid->fError[0];

    return _tPid->fCtrl_Out;
}

/**
 * @brief PID������պ���
 * @param _tPid-PID�ṹ��ָ��
 * @retval Null
*/
void Algo_PID_Clear(tagPID_T *_tPid)
{
	/* ��ǰ������� */
    _tPid->fError[0] = _tPid->fError[1] = _tPid->fError[2] = 0.0f;

    /* ΢�������� */
    _tPid->fDbuf[0] = _tPid->fDbuf[1] = _tPid->fDbuf[2] = 0.0f;

    /* ������� */
    _tPid->fCtrl_Out = _tPid->fPout = _tPid->fIout = _tPid->fDout = 0.0f;

    /* Ŀ��ֵ�͵�ǰֵ���� */
    _tPid->fCurr_Value = _tPid->fExp_Value = 0.0f;
}

/**
 * @brief PID��ʼ����
 * @param _tPid-PID�ṹ��ָ��
 * @param _faPID-PIDϵ����Ҫ����˳��ΪP,I,D
 * @retval Null
*/
void Algo_PID_Init(tagPID_T *_tPid)
{
    /* �������� */
    if(_tPid == NULL)
    {
        Drv_HAL_Error(__FILE__,__LINE__);
        while(1);
    }

    /* ��ʼ���������� */
    _tPid->fDbuf[0] = _tPid->fDbuf[1] = _tPid->fDbuf[2] = 0.0f;
    _tPid->fError[0] = _tPid->fError[1] = _tPid->fError[2] = _tPid->fPout = _tPid->fIout = _tPid->fDout = _tPid->fCtrl_Out = 0.0f;
}

