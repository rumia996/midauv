#include "task_ipccmd.h"
#include "task_automode.h"

uint8_t IPC_ReceBuf[100] = {0};	//�洢��λ������,�޷����ַ���������,
uint8_t IPC_ReceNum = 0;	//�����ַ�������
uint8_t reportcmdbuffer[9] = {0};//�洢Ӧ������,���ȹ̶�9�ֽ�

/**
 * @brief Ӧ������,����������е���
 * @param cmdtype_1 �����ַ�1 ASCII 'R' 'M' 'Z' 
 * @param cmdtype_2 �����ַ�2 ASCII
 * @param cmdtype_3 �����ַ�3 ����
*/
void ReportCmd(uint8_t cmdtype_1,uint8_t cmdtype_2,uint8_t cmdtype_3)
{
	reportcmdbuffer[0]='@';
	reportcmdbuffer[1]='A';
	reportcmdbuffer[2]='C';
	reportcmdbuffer[3]='K';
	reportcmdbuffer[4]='_';
	//ö���������ȷ���ر�����Э��
	switch (cmdtype_1){
		case 'R':reportcmdbuffer[5]='R';//R��Դ���� 
			switch (cmdtype_2)
			{ 
				case 'S':reportcmdbuffer[6]='S';reportcmdbuffer[7]='S';break;//ΪRʱ,S�ɹ����õ�Դ ΪRSʱ,S;
				case 'I':reportcmdbuffer[6]='I';reportcmdbuffer[7]=cmdtype_3;break;//I��ѯ��ǰ����״̬ ΪRIʱ,8λ�ַ���ʾ���� 7λ���� 6λ���ߵ�̨(Э����д��ˮ�µ�),5λ������,4λ��е��,3λ��ͨ��,2λ����GPS,1λ�߶ȼ�,0λ������;
				default:
				#ifdef DEBUG_MODE	
				Drv_HAL_Error(__FILE__, __LINE__);
				#endif
				break;
			}
		break;
			
		case 'Z':reportcmdbuffer[5]='Z';//Z�Զ��˶�����
			if (cmdtype_2=='F'||cmdtype_2=='B'||cmdtype_2=='L'||cmdtype_2=='R'||cmdtype_2=='U'||cmdtype_2=='D'||cmdtype_2=='S'||cmdtype_2=='E'||cmdtype_2=='H'||cmdtype_2=='A'||cmdtype_2=='V'||cmdtype_2=='X'||cmdtype_2=='W')
			{
				reportcmdbuffer[6]=cmdtype_2;
				if (cmdtype_3=='Y'||cmdtype_3=='N')
				{
					reportcmdbuffer[7]=cmdtype_3;
				}
				else {
					#ifdef DEBUG_MODE	
					Drv_HAL_Error(__FILE__, __LINE__);
					#endif	
				}
			}
			else {
				#ifdef DEBUG_MODE	
				Drv_HAL_Error(__FILE__, __LINE__);
				#endif			
			}
		break;
			
		default:
		#ifdef DEBUG_MODE	
		Drv_HAL_Error(__FILE__, __LINE__);//��ӡ������Ϣ,�����ڵ���
		#endif
		break;
	}
	
	reportcmdbuffer[8]='$';
	/*���*/
	for (int i=0;i<9;i++){
		printf("%c",reportcmdbuffer[i]);
	}
#ifdef DEBUG_MODE 
	printf("\r\n");
#endif
}


/*�����������,�ر�,ִ��*/
void Task_IPCcmd_Handle(void)
{
	if(rt_sem_take(CmdFromIPC_Sem,RT_WAITING_FOREVER) == RT_EOK)//��������1
        {
            IPC_ReceNum = Drv_Uart_Receive_DMA(&Uart1,IPC_ReceBuf);//���յ���Ϣ,ͨ��DMA�����ַ������洢,����¼�ַ�������
            if(IPC_ReceNum != 0)
            {
                //ͨ��Э�����
                if(IPC_ReceBuf[0] == '@' && IPC_ReceBuf[IPC_ReceNum-1] == '$')//֡ͷΪ@֡βΪ$,����λ������
                {
					IPC_ReceBuf[IPC_ReceNum]= 0;//ʹ��IPC_ReceBuf�����ַ�����ʽ,����printf���
					//�յ������ر�
					
					switch (IPC_ReceBuf[1])
					{
						/*��Դ����R����*/
						//R������ҪӦ��
						case 'R':
						{
							switch (IPC_ReceBuf[2])
							{
								case 'S'://���õ�Դ @RSCOOCCCO$ ��������:c1�����Ǹߵ�ƽ�� c2�����Ǹߵ�ƽ�� c3�߶ȼƵ͵�ƽ�� c4ˮ�µƸߵ�ƽ�� c5����GPS�͵�ƽ�� c6���ߵ�̨�͵�ƽ�� c7��ͨ���ߵ�ƽ��
								{
									uint8_t powercmd[20] = {0};
									extract_char_between_indexandchar(IPC_ReceBuf,powercmd,3,'$');
									bool powercmdflag = true;
									if (strlen((const char *)powercmd) != 7){//���������� 7λ
										#ifdef DEBUG_MODE
										printf("Invalid power setting parameter length,cmd=%s/r/n",IPC_ReceBuf);
										#endif
										powercmdflag = false;
										return;
									}
									switch (powercmd[0])
									{
										case 'O':Power_On(POW_TDM);break;//��������
										case 'C':Power_Off(POW_TDM);break;//��������
										default :powercmdflag = false;
									}
									switch (powercmd[1])
									{
										case 'O':Power_On(POW_HEIGHT);break;//���߶ȼ�
										case 'C':Power_Off(POW_HEIGHT);break;//�ظ߶ȼ�
										default :powercmdflag = false;
									}
									switch (powercmd[2])
									{
										case 'O':Power_On(POW_BDGPS);break;//������GPS
										case 'C':Power_Off(POW_BDGPS);break;//�ر���GPS
										default :powercmdflag = false;
									}
									switch (powercmd[3])
									{
										case 'O':Power_On(POW_UACM);break;//����ͨ��
										case 'C':Power_Off(POW_UACM);break;//����ͨ��
										default :powercmdflag = false;
									}
									switch (powercmd[4])
									{
										case 'O':Power_On(POW_RADIO);break;//�����ߵ�̨
										case 'C':Power_Off(POW_RADIO);break;//�����ߵ�̨
										default :powercmdflag = false;
									}
									switch (powercmd[5])
									{
										case 'O':Power_On(POW_MAG);break;//��������
										case 'C':Power_Off(POW_MAG);break;//�ش�����
										default :powercmdflag = false;
									}	
									switch (powercmd[6])
									{
										case 'O':Power_On(POW_LIGHT);break;//��ˮ�µ�
										case 'C':Power_Off(POW_LIGHT);break;//��ˮ�µ�										
										default :powercmdflag = false;
									}
									if (powercmdflag != true){
										#ifdef DEBUG_MODE
										printf("Invalid power setting,cmd=%s/r/n",IPC_ReceBuf);
										#endif									
										return;
									}else {
										//�ɹ���ر�
										ReportCmd('R','S','S');									
										break;									
									}

								}
								case 'I'://��ѯ��ǰ��Դ״̬ @RI$
								{							
									powerstate = Power_GetState();
									//�ر�
									ReportCmd('R','I',powerstate);
									break;
								}
								default:
									#ifdef DEBUG_MODE
								printf("Unable to resolve command bit 2:%c\r\n",IPC_ReceBuf[2]);
									#endif
									return;
							}		
						}
						break;
						
						/*ң�ؿ���M����*/
						//M������Ӧ��
						case 'M':
						{
							switch (IPC_ReceBuf[2])//2λ������������
							{
								case 'T'://�ƽ������� @MT1,-12$
								{
									uint8_t index = 0;
									int8_t speed = 0;
									switch (IPC_ReceBuf[3])//�ƽ������1-5
									{
										case '1':index=LMT;break;//�������
										case '2':index=RMT;break;//�Һ�����
										case '3':index=LVT;break;//��ǰ����
										case '4':index=RVT;break;//��ǰ����
										case '5':index=BVT;break;//����								
										default :
											#ifdef DEBUG_MODE
										printf("Invalid thruster number:%c\r\n",IPC_ReceBuf[3]);
											#endif
											return;
									}
									uint8_t speedcmd[10]={0};
									extract_str_between_2char(IPC_ReceBuf,speedcmd,',','$');//��ȡת�ٵ�λ -12~12
									speed = atoi((const char *)speedcmd);//ת��Ϊ����
									#ifdef DEBUG_MODE
									printf("cmd=%s,thruster number=%c,thrusterspeed=%d\r\n",IPC_ReceBuf,IPC_ReceBuf[3],speed);
									#endif
									//ִ��
									if (speed >= -12 && speed <= 12){
										Task_MotorSys_Thruster_Start(index,speed);
									}else{
										#ifdef DEBUG_MODE	
										printf("Invalid thruster speed value:speed=%d\r\n",speed);
										#endif
										return;
									}			
									break;
								}
								case 'D'://������� ����/ָ���Ƕ�  @MDA2,-2$ @MDS3,20$
								{
									uint8_t index = 0;
									int16_t ang = 0;//�洢����Ƕ�����/ָ���Ƕ�
									bool bothrudder = 0;
									switch (IPC_ReceBuf[4])//Ҫ�����Ķ��
									{
										case '1':index = LS;break;
										case '2':index = RS;break;	
										case '3':bothrudder = 1;break;
										default:
											#ifdef DEBUG_MODE	
											printf("Invalid rudder number:%c\r\n",IPC_ReceBuf[4]);
											#endif
											return;
									}
									//��ȡ�Ƕ�ֵ
									uint8_t angcmd[10]={0};
									extract_str_between_2char(IPC_ReceBuf,angcmd,',','$');
									ang = atoi((const char *)angcmd);
									
									switch (IPC_ReceBuf[3])//�жϹ���ģʽ��ִ��
									{
										case 'A'://�����Ƕ�
										{
											if(bothrudder)
											{
												Task_MotorSys_Rudder_Angle_Add(LS,ang);
												Task_MotorSys_Rudder_Angle_Add(RS,ang);
											}
											else
											{
												Task_MotorSys_Rudder_Angle_Add(index,ang);
											}
										}break;
										case 'S'://ָ���Ƕ�
										{
											if (ang<-15 || ang>22)
											{
												#ifdef DEBUG_MODE
												printf("Invalid rudder angle value:%d\r\n",ang);
												#endif	
												return;
											}
											if(bothrudder)
											{
												Task_MotorSys_AllRudder_Angle_Set(ang);
											}
											else
											{
												Task_MotorSys_Rudder_Angle_Set(index,ang);
											}											
										}break;
										default:
											#ifdef DEBUG_MODE
											printf("Invalid rudder command:%c\r\n",IPC_ReceBuf[3]);
											#endif
											return;
									}
									#ifdef DEBUG_MODE
									printf("cmd=%s,ruddernumber=%c,angle=%d\r\n",IPC_ReceBuf,IPC_ReceBuf[4],ang);
									#endif
									break;
								}
								case 'M'://ƽ������ @MMF12$
								{		
									uint8_t speed = 0;
									uint8_t speedcmd[10]={0};
									extract_char_between_indexandchar(IPC_ReceBuf,speedcmd,4,'$');//��ȡ�ٶ� 0~12
									//printf("speedcmd=%s",speedcmd);
									speed = atoi((const char *)speedcmd);//ת��Ϊ����
									if (!(speed <=12)){//������ speed>=0&&speed<=12 speed���޷������ͱض�>=0,ʡ���ж�
										#ifdef DEBUG_MODE	
										printf("Invalid translational speed value:speed=%d\r\n",speed);
										#endif								
										return;
									}
									switch (IPC_ReceBuf[3])//ƽ����������
									{
										case 'F':Task_MotorSys_GoForward(speed);break;//ǰ��
										case 'B':Task_MotorSys_GoBack(speed);break;//����
										case 'U':Task_MotorSys_Float(speed);break;//�ϸ�
										case 'D':Task_MotorSys_Dive(speed);break;//��Ǳ
										default:
											#ifdef DEBUG_MODE	
											printf("Invalid translational command:%c\r\n",IPC_ReceBuf[3]);
											#endif
											return;
									}
									#ifdef DEBUG_MODE
									printf("cmd=%s,movespeed=%d\r\n",IPC_ReceBuf,speed);
									#endif								
									break;
								}
								case 'R'://ת������ @MRL5$
								{
									uint8_t speed = 0;
									switch (IPC_ReceBuf[4])//ת���ٶ� 0-5�� ��Ӧ�ƽ���ת��0,2,4,6,8,10��
									{
										case '0':speed = 0;break;//ֹͣ
										case '1':speed = 2;break;//����
										case '2':speed = 4;break;//����
										case '3':speed = 6;break;//һ��
										case '4':speed = 8;break;//������
										case '5':speed = 10;break;//�ǳ�����
										default:
											#ifdef DEBUG_MODE	
											printf("Invalid rotation speed value:%c\r\n",IPC_ReceBuf[4]);
											#endif
											return;
									}
									//ִ��
									switch (IPC_ReceBuf[3])//ת����������
									{
										case 'L':Task_MotorSys_TurnLeft(speed);break;//��ת
										case 'R':Task_MotorSys_TurnRight(speed);break;//��ת
										case 'B':Task_MotorSys_LeanForward(speed);break;//ǰ��
										case 'F':Task_MotorSys_LeanBack(speed);break;//����
										case 'U':Task_MotorSys_LeanLeft(speed);break;//����
										case 'D':Task_MotorSys_LeanRight(speed);break;//����
										default:
											#ifdef DEBUG_MODE	
											printf("Invalid rotation command:%c\r\n",IPC_ReceBuf[3]);
											#endif
											return;
									}
									#ifdef DEBUG_MODE
									printf("cmd=%s,turnspeed=%d\r\n",IPC_ReceBuf,speed);
									#endif									
									break;
								}
								case 'S'://ҡ�˺��п��� @MS120,-135$
								{
									uint8_t speed = 0;//1-120
									int16_t angle = 0;//-180~180
									uint8_t speedcmd[10] = {0};
									uint8_t anglecmd[10] = {0};
									extract_char_between_indexandchar(IPC_ReceBuf,speedcmd,3,'$');//��ȡ�ٶ�
									extract_str_between_2char(IPC_ReceBuf,anglecmd,',','$');//��ȡ����
									speed = atoi((const char *)speedcmd);//ת��Ϊ����
									angle = atoi((const char *)anglecmd);//ת��Ϊ����
									#ifdef DEBUG_MODE
									printf("cmd=%s,speed=%d,angle=%d\r\n",IPC_ReceBuf,speed,angle);
									#endif	
									if (speed <= 120 && angle >=-180 && angle <=180){//������
										/*��ԭ��ת��Ȼ��ǰ��*/
										if (Task_MotorSys_SetYaw((float)angle))
										{
											//Task_MotorSys_GoForward((int)speed / 10);
										}
							
									}else{
										#ifdef DEBUG_MODE	
										printf("Invalid Joystick navigation parameters:speed=%d,Yaw=%d\r\n",speed,angle);
										#endif
										return;
									}
									break;
								}
								case 'P'://ҡ����̬���� @MP120,-135$
								{
									uint8_t force = 0;//1-120
									int16_t angle = 0;//-180~180
									uint8_t forcecmd[10] = {0};
									uint8_t anglecmd[10] = {0};
									extract_char_between_indexandchar(IPC_ReceBuf,forcecmd,3,'$');//��ȡ����
									extract_str_between_2char(IPC_ReceBuf,anglecmd,',','$');//��ȡ����
									force = atoi((const char *)forcecmd);//ת��Ϊ����
									angle = atoi((const char *)anglecmd);//ת��Ϊ����								
									#ifdef DEBUG_MODE
									printf("cmd=%s,force=%d,angle=%d\r\n",IPC_ReceBuf,force,angle);
									#endif
									if (force <= 120 && angle >=-180 && angle <=180){//������
										//ִ�д���д
										//do something	
									}else{
										#ifdef DEBUG_MODE	
										printf("Invalid Joystick attitude parameters:force=%d,angle=%d\r\n",force,angle);
										#endif
										return;
									}
									break;
								}

	/*
	//							case 'S'://ʱ��ͬ��,��ȡ����4-9λ��Ϊ����,10-17λ��Ϊʱ�� ��:@MS220216123625.6$��ʾ2022��02��16��12ʱ36��25.6��
	//							{
	//								char year_buf[3];//���ڴ洢�Ļ������ַ���,ע��Ϊ\0Ԥ���ռ�
	//								char month_buf[3];
	//								char day_buf[3];
	//								char hour_buf[3];
	//								char minute_buf[3];
	//								char second_i_buf[3];//��������λ
	//								char second_f;//����С��λ,��1λ����ֱ����char�洢
	//								memset(year_buf, '\0', sizeof(year_buf));//��ʼ���ַ�������,��Ҫ��Ϊ�˽����һλ����Ϊ\0
	//								memset(month_buf, '\0', sizeof(month_buf));
	//								memset(day_buf, '\0', sizeof(day_buf));
	//								memset(hour_buf, '\0', sizeof(hour_buf));
	//								memset(minute_buf, '\0', sizeof(minute_buf));
	//								memset(second_i_buf, '\0', sizeof(second_i_buf));
	//								rt_strncpy(year_buf,(const char*)IPC_ReceBuf+3,2);//�ӵ�3λ��ʼ��ȡ2���ַ� 22
	//								rt_strncpy(month_buf,(const char*)IPC_ReceBuf+5,2);
	//								rt_strncpy(day_buf,(const char*)IPC_ReceBuf+7,2);							
	//								rt_strncpy(hour_buf,(const char*)IPC_ReceBuf+9,2);
	//								rt_strncpy(minute_buf,(const char*)IPC_ReceBuf+11,2);
	//								rt_strncpy(second_i_buf,(const char*)IPC_ReceBuf+13,2);
	//								second_f=IPC_ReceBuf[16];
	//								//���ַ���ת��Ϊ��ֵ
	//								uint16_t year 	= (year_buf[0]-'0') 	* 10 + (year_buf[1]-'0') + 2000;
	//								uint8_t month 	= (month_buf[0]-'0') 	* 10 + (month_buf[1]-'0');
	//								uint8_t day 	= (day_buf[0]-'0') 		* 10 + (day_buf[1]-'0');
	//								uint8_t hour	= (hour_buf[0]-'0') 	* 10 + (hour_buf[1]-'0');
	//								uint8_t minute 	= (minute_buf[0]-'0') 	* 10 + (minute_buf[1]-'0');
	//								float_t second 	= (second_i_buf[0]-'0') * 10 + (second_i_buf[1]-'0') + (second_f-'0') * 0.1;
	//								//32�޸�RTCʱ��
	//#ifdef DEBUG_MODE
	//								//printf("year:%s\r\n",year_buf);
	//								printf("year:%d month:%d day:%d hour:%d minute:%d second:%f\r\n",year,month,day,hour,minute,second);
	//#endif
	//								break;
	//							}
	*/
								default:
									#ifdef DEBUG_MODE
									printf("Unable to resolve command bit 2:%c\r\n",IPC_ReceBuf[2]);
									#endif
									return;
							}
							//��������Ч,�л���ǰģʽΪMANUAL_MODE,���ͷ��ź���
							MODE_Switch(MANUAL_MODE);
//							#ifdef DEBUG_MODE
//							printf("�����ֱ�ģʽ\r\n");
//							#endif
//							MODE = MANUAL_MODE;
//							rt_sem_release(ManualCmd_Sem);
							
						}	
						break;
						
						/*�˶�����Z����&��ȫ����*/
						//��Ҫ�ر�
						case 'Z':
						{
							switch (IPC_ReceBuf[2])
							{
								case 'A':
								{
									uint8_t cmdbuf[20] = {0};//�洢�����ַ���	
									extract_char_between_indexandchar(IPC_ReceBuf,cmdbuf,4,'$');//��ȡ�����ַ���
									int16_t cmd = atoi((const char*)cmdbuf);//��ASCII����ת��Ϊ����;��@ZAA,@ZAX,@ZAW������,������ΪASCII����								
									bool reportflag = true;									
									switch (IPC_ReceBuf[3])		
									{
										case 'F':{	//ǰ��	@ZAF12$
											param_check_0to12(cmd,&reportflag);
											if (reportflag != false){
												AMInfo.target_speed = cmd;
												AMInfo.Auto_Horizontal_Mode = AUTO_GOFORWARD;
											}										
											Report_Z(reportflag);
										}break;
										case 'B':{	//����								
											param_check_0to12(cmd,&reportflag);
											if (reportflag != false){
												AMInfo.target_speed = cmd;
												AMInfo.Auto_Horizontal_Mode = AUTO_GOBACK;
											}										
											Report_Z(reportflag);
										}break;
										case 'L':{ //��ת
											param_check_0to12(cmd,&reportflag);
											if (reportflag != false){
												AMInfo.target_speed = cmd;//
												AMInfo.Auto_Horizontal_Mode = AUTO_TURNLEFT;
											}										
											Report_Z(reportflag);										
										}break;
										case 'R':{	//��ת
											param_check_0to12(cmd,&reportflag);
											if (reportflag != false){
												AMInfo.target_speed = cmd;//
												AMInfo.Auto_Horizontal_Mode = AUTO_TURNRIGHT;
											}										
											Report_Z(reportflag);										
										}break;
										case 'U':{	//�ϸ�
											param_check_0to12(cmd,&reportflag);
											if (reportflag != false){
												AMInfo.target_speed = cmd;
												AMInfo.Auto_Vertical_Mode = AUTO_FLOAT;
											}										
											Report_Z(reportflag);										
										}break;
										case 'D':{	//��Ǳ
											param_check_0to12(cmd,&reportflag);
											if (reportflag != false){
												AMInfo.target_speed = cmd;
												AMInfo.Auto_Vertical_Mode = AUTO_DIVE;
											}										
											Report_Z(reportflag);										
										}break;
										case 'S':{	//����	@ZAS30$
											AMInfo.target_depth = cmd;
											AMInfo.Auto_Vertical_Mode = AUTO_SETDEPTH;
											Report_Z(reportflag);
										}break;
										case 'E':{	//����	@ZAE30$
											AMInfo.target_height = cmd;
											AMInfo.Auto_Vertical_Mode = AUTO_SETHEIGHT;
											Report_Z(reportflag);
										}break;
										case 'H':{	//����	@ZAH-20$
											AMInfo.target_yaw = cmd;
											AMInfo.Auto_Horizontal_Mode= AUTO_SETYAW;
											Report_Z(reportflag);
										}break;
										case 'A':{	//����	@ZAA-10,120$
											if (strchr((const char*)IPC_ReceBuf,',') != NULL )
											{
												uint8_t pitchbuf[8];
												uint8_t rollbuf[8];
												extract_char_between_indexandchar(IPC_ReceBuf,pitchbuf,4,',');//��ȡpitch
												extract_str_between_2char(IPC_ReceBuf,rollbuf,',','$');//��ȡroll
												uint8_t pitch = atoi((const char*)pitchbuf);
												uint8_t roll = atoi((const char*)rollbuf);												
											}else {reportflag = false;}
											//ʵ��
											//��ûд
											Report_Z(reportflag);
										}break;
										case 'V':{	//����	@ZAV5$										
											//�޷�ʵ��
											reportflag = false;
											Report_Z(reportflag);
										}break;
										case 'X':{	//����ֹͣ	@ZAX0$
											if (IPC_ReceBuf[4] == '0'){
												//ʵ��
												Task_MotorSys_AllThruster_Stop();
												AMInfo.Auto_Vertical_Mode = AUTO_STOP;
												AMInfo.Auto_Horizontal_Mode = AUTO_STOP;
											}else {reportflag = false;}
											Report_Z(reportflag);
										}break;
										case 'W':{	//�����ϸ�	@ZAWF$
											if (IPC_ReceBuf[4]== 'F'){
												//ʵ��
												Task_MotorSys_MainThruster_Stop();
												Task_MotorSys_VerticalThruster_Start(12);
												AMInfo.Auto_Horizontal_Mode = AUTO_STOP;
											}else {reportflag = false;}
											Report_Z(reportflag);
										}break;
										default:
											#ifdef DEBUG_MODE
											printf("Invalid auto mode commands:%c\r\n",IPC_ReceBuf[3]);
											#endif	
										break;
									}
								}
								break;
								default:
									#ifdef DEBUG_MODE
									printf("Unable to resolve command bit 2:%c\r\n",IPC_ReceBuf[2]);
									#endif
									return;							
							}
							//��������Ч,�л���ǰģʽΪAUTO_MODE
							MODE_Switch(AUTO_MODE);
//							MODE = AUTO_MODE;
//							#ifdef DEBUG_MODE
//							printf("�����Զ�ģʽ\r\n");
//							#endif							
						}	
						break;
						
						default:
							#ifdef DEBUG_MODE
							printf("Unable to resolve command bit 1:%c\r\n",IPC_ReceBuf[1]);	
							#endif
							return;
					}		
                }
				else
				{
					#ifdef DEBUG_MODE
					printf("No header/end of frame was detected:%s\r\n",IPC_ReceBuf);	
					#endif				
				}
            }
        }
}


void Report_Z(bool reportflag)
{
	uint8_t result = 0;
	if (reportflag != false){
		result = 'Y';
	}else{
		result = 'N';
	}
	ReportCmd(IPC_ReceBuf[1],IPC_ReceBuf[2],result);
}

void param_check_0to12(int16_t cmd,bool * reportflag){
	if(cmd <= 12)
	{
		*reportflag = true;
	}else
	{
		#ifdef DEBUG_MODE
		printf("Invalid auto mode commands,cmd=%s\r\n",IPC_ReceBuf);
		#endif
		*reportflag = false;
	}
}
