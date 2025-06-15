#include "task_ipccmd.h"
#include "task_automode.h"

uint8_t IPC_ReceBuf[100] = {0};	//存储上位机命令,无符号字符类型数组,
uint8_t IPC_ReceNum = 0;	//命令字符串长度
uint8_t reportcmdbuffer[9] = {0};//存储应答命令,长度固定9字节

/**
 * @brief 应答命令,在命令解析中调用
 * @param cmdtype_1 功能字符1 ASCII 'R' 'M' 'Z' 
 * @param cmdtype_2 功能字符2 ASCII
 * @param cmdtype_3 功能字符3 不定
*/
void ReportCmd(uint8_t cmdtype_1,uint8_t cmdtype_2,uint8_t cmdtype_3)
{
	reportcmdbuffer[0]='@';
	reportcmdbuffer[1]='A';
	reportcmdbuffer[2]='C';
	reportcmdbuffer[3]='K';
	reportcmdbuffer[4]='_';
	//枚举所有情况确保回报符合协议
	switch (cmdtype_1){
		case 'R':reportcmdbuffer[5]='R';//R电源控制 
			switch (cmdtype_2)
			{ 
				case 'S':reportcmdbuffer[6]='S';reportcmdbuffer[7]='S';break;//为R时,S成功设置电源 为RS时,S;
				case 'I':reportcmdbuffer[6]='I';reportcmdbuffer[7]=cmdtype_3;break;//I查询当前设置状态 为RI时,8位字符表示开关 7位备用 6位无线电台(协议上写了水下灯),5位磁力仪,4位机械手,3位声通机,2位北斗GPS,1位高度计,0位温深仪;
				default:
				#ifdef DEBUG_MODE	
				Drv_HAL_Error(__FILE__, __LINE__);
				#endif
				break;
			}
		break;
			
		case 'Z':reportcmdbuffer[5]='Z';//Z自动运动控制
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
		Drv_HAL_Error(__FILE__, __LINE__);//打印错误信息,仅用于调试
		#endif
		break;
	}
	
	reportcmdbuffer[8]='$';
	/*输出*/
	for (int i=0;i<9;i++){
		printf("%c",reportcmdbuffer[i]);
	}
#ifdef DEBUG_MODE 
	printf("\r\n");
#endif
}


/*接收命令并解析,回报,执行*/
void Task_IPCcmd_Handle(void)
{
	if(rt_sem_take(CmdFromIPC_Sem,RT_WAITING_FOREVER) == RT_EOK)//监听串口1
        {
            IPC_ReceNum = Drv_Uart_Receive_DMA(&Uart1,IPC_ReceBuf);//若收到信息,通过DMA接收字符串并存储,并记录字符串长度
            if(IPC_ReceNum != 0)
            {
                //通信协议解析
                if(IPC_ReceBuf[0] == '@' && IPC_ReceBuf[IPC_ReceNum-1] == '$')//帧头为@帧尾为$,是上位机命令
                {
					IPC_ReceBuf[IPC_ReceNum]= 0;//使得IPC_ReceBuf满足字符串格式,方便printf输出
					//收到命令后回报
					
					switch (IPC_ReceBuf[1])
					{
						/*电源控制R命令*/
						//R命令需要应答
						case 'R':
						{
							switch (IPC_ReceBuf[2])
							{
								case 'S'://设置电源 @RSCOOCCCO$ 此命令下:c1磁力仪高电平关 c2温深仪高电平关 c3高度计低电平开 c4水下灯高电平关 c5北斗GPS低电平开 c6无线电台低电平开 c7声通机高电平关
								{
									uint8_t powercmd[20] = {0};
									extract_char_between_indexandchar(IPC_ReceBuf,powercmd,3,'$');
									bool powercmdflag = true;
									if (strlen((const char *)powercmd) != 7){//检查参数长度 7位
										#ifdef DEBUG_MODE
										printf("Invalid power setting parameter length,cmd=%s/r/n",IPC_ReceBuf);
										#endif
										powercmdflag = false;
										return;
									}
									switch (powercmd[0])
									{
										case 'O':Power_On(POW_TDM);break;//开温深仪
										case 'C':Power_Off(POW_TDM);break;//关温深仪
										default :powercmdflag = false;
									}
									switch (powercmd[1])
									{
										case 'O':Power_On(POW_HEIGHT);break;//开高度计
										case 'C':Power_Off(POW_HEIGHT);break;//关高度计
										default :powercmdflag = false;
									}
									switch (powercmd[2])
									{
										case 'O':Power_On(POW_BDGPS);break;//开北斗GPS
										case 'C':Power_Off(POW_BDGPS);break;//关北斗GPS
										default :powercmdflag = false;
									}
									switch (powercmd[3])
									{
										case 'O':Power_On(POW_UACM);break;//开声通机
										case 'C':Power_Off(POW_UACM);break;//关声通机
										default :powercmdflag = false;
									}
									switch (powercmd[4])
									{
										case 'O':Power_On(POW_RADIO);break;//开无线电台
										case 'C':Power_Off(POW_RADIO);break;//关无线电台
										default :powercmdflag = false;
									}
									switch (powercmd[5])
									{
										case 'O':Power_On(POW_MAG);break;//开磁力仪
										case 'C':Power_Off(POW_MAG);break;//关磁力仪
										default :powercmdflag = false;
									}	
									switch (powercmd[6])
									{
										case 'O':Power_On(POW_LIGHT);break;//开水下灯
										case 'C':Power_Off(POW_LIGHT);break;//关水下灯										
										default :powercmdflag = false;
									}
									if (powercmdflag != true){
										#ifdef DEBUG_MODE
										printf("Invalid power setting,cmd=%s/r/n",IPC_ReceBuf);
										#endif									
										return;
									}else {
										//成功则回报
										ReportCmd('R','S','S');									
										break;									
									}

								}
								case 'I'://查询当前电源状态 @RI$
								{							
									powerstate = Power_GetState();
									//回报
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
						
						/*遥控控制M命令*/
						//M命令无应答
						case 'M':
						{
							switch (IPC_ReceBuf[2])//2位决定命令类型
							{
								case 'T'://推进器测试 @MT1,-12$
								{
									uint8_t index = 0;
									int8_t speed = 0;
									switch (IPC_ReceBuf[3])//推进器编号1-5
									{
										case '1':index=LMT;break;//左后主推
										case '2':index=RMT;break;//右后主推
										case '3':index=LVT;break;//左前垂推
										case '4':index=RVT;break;//右前垂推
										case '5':index=BVT;break;//后垂推								
										default :
											#ifdef DEBUG_MODE
										printf("Invalid thruster number:%c\r\n",IPC_ReceBuf[3]);
											#endif
											return;
									}
									uint8_t speedcmd[10]={0};
									extract_str_between_2char(IPC_ReceBuf,speedcmd,',','$');//提取转速档位 -12~12
									speed = atoi((const char *)speedcmd);//转换为整数
									#ifdef DEBUG_MODE
									printf("cmd=%s,thruster number=%c,thrusterspeed=%d\r\n",IPC_ReceBuf,IPC_ReceBuf[3],speed);
									#endif
									//执行
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
								case 'D'://舵机控制 增量/指定角度  @MDA2,-2$ @MDS3,20$
								{
									uint8_t index = 0;
									int16_t ang = 0;//存储舵机角度增量/指定角度
									bool bothrudder = 0;
									switch (IPC_ReceBuf[4])//要操作的舵板
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
									//提取角度值
									uint8_t angcmd[10]={0};
									extract_str_between_2char(IPC_ReceBuf,angcmd,',','$');
									ang = atoi((const char *)angcmd);
									
									switch (IPC_ReceBuf[3])//判断工作模式并执行
									{
										case 'A'://增减角度
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
										case 'S'://指定角度
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
								case 'M'://平动控制 @MMF12$
								{		
									uint8_t speed = 0;
									uint8_t speedcmd[10]={0};
									extract_char_between_indexandchar(IPC_ReceBuf,speedcmd,4,'$');//提取速度 0~12
									//printf("speedcmd=%s",speedcmd);
									speed = atoi((const char *)speedcmd);//转换为整数
									if (!(speed <=12)){//检查参数 speed>=0&&speed<=12 speed是无符号整型必定>=0,省略判断
										#ifdef DEBUG_MODE	
										printf("Invalid translational speed value:speed=%d\r\n",speed);
										#endif								
										return;
									}
									switch (IPC_ReceBuf[3])//平动控制命令
									{
										case 'F':Task_MotorSys_GoForward(speed);break;//前进
										case 'B':Task_MotorSys_GoBack(speed);break;//后退
										case 'U':Task_MotorSys_Float(speed);break;//上浮
										case 'D':Task_MotorSys_Dive(speed);break;//下潜
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
								case 'R'://转动控制 @MRL5$
								{
									uint8_t speed = 0;
									switch (IPC_ReceBuf[4])//转动速度 0-5档 对应推进器转速0,2,4,6,8,10档
									{
										case '0':speed = 0;break;//停止
										case '1':speed = 2;break;//很慢
										case '2':speed = 4;break;//较慢
										case '3':speed = 6;break;//一般
										case '4':speed = 8;break;//较敏捷
										case '5':speed = 10;break;//非常敏捷
										default:
											#ifdef DEBUG_MODE	
											printf("Invalid rotation speed value:%c\r\n",IPC_ReceBuf[4]);
											#endif
											return;
									}
									//执行
									switch (IPC_ReceBuf[3])//转动控制命令
									{
										case 'L':Task_MotorSys_TurnLeft(speed);break;//左转
										case 'R':Task_MotorSys_TurnRight(speed);break;//右转
										case 'B':Task_MotorSys_LeanForward(speed);break;//前倾
										case 'F':Task_MotorSys_LeanBack(speed);break;//后倾
										case 'U':Task_MotorSys_LeanLeft(speed);break;//左倾
										case 'D':Task_MotorSys_LeanRight(speed);break;//右倾
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
								case 'S'://摇杆航行控制 @MS120,-135$
								{
									uint8_t speed = 0;//1-120
									int16_t angle = 0;//-180~180
									uint8_t speedcmd[10] = {0};
									uint8_t anglecmd[10] = {0};
									extract_char_between_indexandchar(IPC_ReceBuf,speedcmd,3,'$');//提取速度
									extract_str_between_2char(IPC_ReceBuf,anglecmd,',','$');//提取方向
									speed = atoi((const char *)speedcmd);//转换为整数
									angle = atoi((const char *)anglecmd);//转换为整数
									#ifdef DEBUG_MODE
									printf("cmd=%s,speed=%d,angle=%d\r\n",IPC_ReceBuf,speed,angle);
									#endif	
									if (speed <= 120 && angle >=-180 && angle <=180){//检查参数
										/*先原地转向，然后前进*/
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
								case 'P'://摇杆姿态控制 @MP120,-135$
								{
									uint8_t force = 0;//1-120
									int16_t angle = 0;//-180~180
									uint8_t forcecmd[10] = {0};
									uint8_t anglecmd[10] = {0};
									extract_char_between_indexandchar(IPC_ReceBuf,forcecmd,3,'$');//提取力度
									extract_str_between_2char(IPC_ReceBuf,anglecmd,',','$');//提取方向
									force = atoi((const char *)forcecmd);//转换为整数
									angle = atoi((const char *)anglecmd);//转换为整数								
									#ifdef DEBUG_MODE
									printf("cmd=%s,force=%d,angle=%d\r\n",IPC_ReceBuf,force,angle);
									#endif
									if (force <= 120 && angle >=-180 && angle <=180){//检查参数
										//执行待填写
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
	//							case 'S'://时间同步,提取数组4-9位作为日期,10-17位作为时间 例:@MS220216123625.6$表示2022年02月16日12时36分25.6秒
	//							{
	//								char year_buf[3];//用于存储的缓冲区字符串,注意为\0预留空间
	//								char month_buf[3];
	//								char day_buf[3];
	//								char hour_buf[3];
	//								char minute_buf[3];
	//								char second_i_buf[3];//秒数整数位
	//								char second_f;//秒数小数位,就1位所以直接用char存储
	//								memset(year_buf, '\0', sizeof(year_buf));//初始化字符串数组,主要是为了将最后一位设置为\0
	//								memset(month_buf, '\0', sizeof(month_buf));
	//								memset(day_buf, '\0', sizeof(day_buf));
	//								memset(hour_buf, '\0', sizeof(hour_buf));
	//								memset(minute_buf, '\0', sizeof(minute_buf));
	//								memset(second_i_buf, '\0', sizeof(second_i_buf));
	//								rt_strncpy(year_buf,(const char*)IPC_ReceBuf+3,2);//从第3位开始提取2个字符 22
	//								rt_strncpy(month_buf,(const char*)IPC_ReceBuf+5,2);
	//								rt_strncpy(day_buf,(const char*)IPC_ReceBuf+7,2);							
	//								rt_strncpy(hour_buf,(const char*)IPC_ReceBuf+9,2);
	//								rt_strncpy(minute_buf,(const char*)IPC_ReceBuf+11,2);
	//								rt_strncpy(second_i_buf,(const char*)IPC_ReceBuf+13,2);
	//								second_f=IPC_ReceBuf[16];
	//								//将字符串转换为数值
	//								uint16_t year 	= (year_buf[0]-'0') 	* 10 + (year_buf[1]-'0') + 2000;
	//								uint8_t month 	= (month_buf[0]-'0') 	* 10 + (month_buf[1]-'0');
	//								uint8_t day 	= (day_buf[0]-'0') 		* 10 + (day_buf[1]-'0');
	//								uint8_t hour	= (hour_buf[0]-'0') 	* 10 + (hour_buf[1]-'0');
	//								uint8_t minute 	= (minute_buf[0]-'0') 	* 10 + (minute_buf[1]-'0');
	//								float_t second 	= (second_i_buf[0]-'0') * 10 + (second_i_buf[1]-'0') + (second_f-'0') * 0.1;
	//								//32修改RTC时钟
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
							//若命令有效,切换当前模式为MANUAL_MODE,并释放信号量
							MODE_Switch(MANUAL_MODE);
//							#ifdef DEBUG_MODE
//							printf("进入手柄模式\r\n");
//							#endif
//							MODE = MANUAL_MODE;
//							rt_sem_release(ManualCmd_Sem);
							
						}	
						break;
						
						/*运动控制Z命令&安全机制*/
						//需要回报
						case 'Z':
						{
							switch (IPC_ReceBuf[2])
							{
								case 'A':
								{
									uint8_t cmdbuf[20] = {0};//存储控制字符串	
									extract_char_between_indexandchar(IPC_ReceBuf,cmdbuf,4,'$');//提取控制字符串
									int16_t cmd = atoi((const char*)cmdbuf);//将ASCII数字转换为整数;除@ZAA,@ZAX,@ZAW命令外,参数均为ASCII整数								
									bool reportflag = true;									
									switch (IPC_ReceBuf[3])		
									{
										case 'F':{	//前进	@ZAF12$
											param_check_0to12(cmd,&reportflag);
											if (reportflag != false){
												AMInfo.target_speed = cmd;
												AMInfo.Auto_Horizontal_Mode = AUTO_GOFORWARD;
											}										
											Report_Z(reportflag);
										}break;
										case 'B':{	//后退								
											param_check_0to12(cmd,&reportflag);
											if (reportflag != false){
												AMInfo.target_speed = cmd;
												AMInfo.Auto_Horizontal_Mode = AUTO_GOBACK;
											}										
											Report_Z(reportflag);
										}break;
										case 'L':{ //左转
											param_check_0to12(cmd,&reportflag);
											if (reportflag != false){
												AMInfo.target_speed = cmd;//
												AMInfo.Auto_Horizontal_Mode = AUTO_TURNLEFT;
											}										
											Report_Z(reportflag);										
										}break;
										case 'R':{	//右转
											param_check_0to12(cmd,&reportflag);
											if (reportflag != false){
												AMInfo.target_speed = cmd;//
												AMInfo.Auto_Horizontal_Mode = AUTO_TURNRIGHT;
											}										
											Report_Z(reportflag);										
										}break;
										case 'U':{	//上浮
											param_check_0to12(cmd,&reportflag);
											if (reportflag != false){
												AMInfo.target_speed = cmd;
												AMInfo.Auto_Vertical_Mode = AUTO_FLOAT;
											}										
											Report_Z(reportflag);										
										}break;
										case 'D':{	//下潜
											param_check_0to12(cmd,&reportflag);
											if (reportflag != false){
												AMInfo.target_speed = cmd;
												AMInfo.Auto_Vertical_Mode = AUTO_DIVE;
											}										
											Report_Z(reportflag);										
										}break;
										case 'S':{	//定深	@ZAS30$
											AMInfo.target_depth = cmd;
											AMInfo.Auto_Vertical_Mode = AUTO_SETDEPTH;
											Report_Z(reportflag);
										}break;
										case 'E':{	//定高	@ZAE30$
											AMInfo.target_height = cmd;
											AMInfo.Auto_Vertical_Mode = AUTO_SETHEIGHT;
											Report_Z(reportflag);
										}break;
										case 'H':{	//定艏	@ZAH-20$
											AMInfo.target_yaw = cmd;
											AMInfo.Auto_Horizontal_Mode= AUTO_SETYAW;
											Report_Z(reportflag);
										}break;
										case 'A':{	//定姿	@ZAA-10,120$
											if (strchr((const char*)IPC_ReceBuf,',') != NULL )
											{
												uint8_t pitchbuf[8];
												uint8_t rollbuf[8];
												extract_char_between_indexandchar(IPC_ReceBuf,pitchbuf,4,',');//提取pitch
												extract_str_between_2char(IPC_ReceBuf,rollbuf,',','$');//提取roll
												uint8_t pitch = atoi((const char*)pitchbuf);
												uint8_t roll = atoi((const char*)rollbuf);												
											}else {reportflag = false;}
											//实现
											//还没写
											Report_Z(reportflag);
										}break;
										case 'V':{	//定速	@ZAV5$										
											//无法实现
											reportflag = false;
											Report_Z(reportflag);
										}break;
										case 'X':{	//紧急停止	@ZAX0$
											if (IPC_ReceBuf[4] == '0'){
												//实现
												Task_MotorSys_AllThruster_Stop();
												AMInfo.Auto_Vertical_Mode = AUTO_STOP;
												AMInfo.Auto_Horizontal_Mode = AUTO_STOP;
											}else {reportflag = false;}
											Report_Z(reportflag);
										}break;
										case 'W':{	//紧急上浮	@ZAWF$
											if (IPC_ReceBuf[4]== 'F'){
												//实现
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
							//若命令有效,切换当前模式为AUTO_MODE
							MODE_Switch(AUTO_MODE);
//							MODE = AUTO_MODE;
//							#ifdef DEBUG_MODE
//							printf("进入自动模式\r\n");
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
