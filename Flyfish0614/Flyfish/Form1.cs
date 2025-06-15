using SharpDX;
using SharpDX.XInput;       //支持Xbox遥控手柄的程序包
using System;
using System.Drawing;       //支持Color类
using System.IO;            //支持File类
using System.IO.Ports;
using System.Net.Sockets;   //支持TcpClient类
using System.Text;          //支持Encoding类
using System.Windows.Forms; //支持窗体编程的类
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace Flyfish
{
    public partial class Form1 : Form
    {
        private TcpClient client = new TcpClient(); //声明一个Tcpclient类的对象client
        private NetworkStream stream;               //声明一个NetworkStream类的对象stream
        string server = "192.168.1.102";                //本地主机地址
//        string server = "127.0.0.1";
        string port = "8886";                      //服务器监听的端口

        private Controller controller;              //xbox控制对象
        private Gamepad currentGamepadState;        //xbox状态

        int[] a = new int[30];//手柄基本信息20个变量
        char[] nodeState = { 'C', 'C', 'C', 'C', 'C', 'C' };//树莓派节点状态 
        char[] sensorState = { 'C', 'C', 'C', 'C', 'C', 'C', 'C' };//传感器节点状态 
        string[] lines = { "" };//使命框的文本内容
        string[] status = new string[30];//树莓派返回的数据
        int curLine = 0;//使命框读取的当前行数        

        public Form1()
        {
            InitializeComponent();
            toolStripMenuItem3.Enabled = false;
            controller = new Controller(UserIndex.One);//初始化xbox控制器
            status[0] = "";//不让status[0]为NULL，否则后续调用Substring等方法会报错
            timer1.Start();
            timer2.Start();
            this.WindowState = FormWindowState.Maximized;
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        #region code for TCP connect

        #region code for setup control //节点设置与传感器电源控制板块代码
        private void syncButton_Click(object sender, EventArgs e)
        {
            char[] char_datatime = DateTime.Now.ToString().ToCharArray();
            for (int i = 0; i < char_datatime.Length; i++)
            {
                if (char_datatime[i] == ':' | char_datatime[i] == ' ' | char_datatime[i] == '/')
                    char_datatime[i] = ',';
            }
            string str_datatime = new string(char_datatime);
            string command_TimeSyn = "@ST," + str_datatime + ",$";
            sendstring(command_TimeSyn);
        }

        private void acquireButton_Click(object sender, EventArgs e)
        {
            sendstring("@DU$");
        }

        private void PiButton_Click(object sender, EventArgs e)
        {
            if (nodeState[0] == 'C')
            {
                PiButton.UseVisualStyleBackColor = true;
                nodeState[0] = 'O';
            }
            else
            {
                PiButton.UseVisualStyleBackColor = false;
                nodeState[0] = 'C';
            }
        }

        private void STM32Button_Click(object sender, EventArgs e)
        {
            if (nodeState[1] == 'C')
            {
                STM32Button.UseVisualStyleBackColor = true;
                nodeState[1] = 'O';
            }
            else
            {
                STM32Button.UseVisualStyleBackColor = false;
                nodeState[1] = 'C';
            }
        }

        private void BDGPSButton_Click(object sender, EventArgs e)
        {
            if (nodeState[2] == 'C')
            {
                BDGPSButton.UseVisualStyleBackColor = true;
                nodeState[2] = 'O';
            }
            else
            {
                BDGPSButton.UseVisualStyleBackColor = false;
                nodeState[2] = 'C';
            }
        }

        private void UACMButton_Click(object sender, EventArgs e)
        {
            if (nodeState[3] == 'C')
            {
                UACMButton.UseVisualStyleBackColor = true;
                nodeState[3] = 'O';
            }
            else
            {
                UACMButton.UseVisualStyleBackColor = false;
                nodeState[3] = 'C';
            }
        }

        private void CAMERAButton_Click(object sender, EventArgs e)
        {
            if (nodeState[4] == 'C')
            {
                CAMERAButton.UseVisualStyleBackColor = true;
                nodeState[4] = 'O';
            }
            else
            {
                CAMERAButton.UseVisualStyleBackColor = false;
                nodeState[4] = 'C';
            }
        }

        private void AUTOButton_Click(object sender, EventArgs e)
        {
            if (nodeState[5] == 'C')
            {
                AUTOButton.UseVisualStyleBackColor = true;
                nodeState[5] = 'O';
            }
            else
            {
                AUTOButton.UseVisualStyleBackColor = false;
                nodeState[5] = 'C';
            }
        }

        private void NodeSetupButton_Click(object sender, EventArgs e)
        {
            string strNodeState = new string(nodeState);
            string command_NodeSetup = $"@NS" + strNodeState + "$";
            sendstring(command_NodeSetup);
        }

        private void NodeStatusRefreshButton_Click(object sender, EventArgs e)
        {
            sendstring("@NI$");
        }

        private void TDButton_Click(object sender, EventArgs e)
        {
            if (sensorState[0] == 'C')
            {
                TDButton.UseVisualStyleBackColor = true;
                sensorState[0] = 'O';
            }
            else
            {
                TDButton.UseVisualStyleBackColor = false;
                sensorState[0] = 'C';
            }
        }

        private void AltimeterButton_Click(object sender, EventArgs e)
        {
            if (sensorState[1] == 'C')
            {
                AltimeterButton.UseVisualStyleBackColor = true;
                sensorState[1] = 'O';
            }
            else
            {
                AltimeterButton.UseVisualStyleBackColor = false;
                sensorState[1] = 'C';
            }
        }

        private void BDGPSPowerButton_Click(object sender, EventArgs e)
        {
            if (sensorState[2] == 'C')
            {
                BDGPSPowerButton.UseVisualStyleBackColor = true;
                sensorState[2] = 'O';
            }
            else
            {
                BDGPSPowerButton.UseVisualStyleBackColor = false;
                sensorState[2] = 'C';
            }
        }

        private void UACMPowerButton_Click(object sender, EventArgs e)
        {
            if (sensorState[3] == 'C')
            {
                UACMPowerButton.UseVisualStyleBackColor = true;
                sensorState[3] = 'O';
            }
            else
            {
                UACMPowerButton.UseVisualStyleBackColor = false;
                sensorState[3] = 'C';
            }
        }

        private void ManipulatorButton_Click(object sender, EventArgs e)
        {
            if (sensorState[4] == 'C')
            {
                ManipulatorButton.UseVisualStyleBackColor = true;
                sensorState[4] = 'O';
            }
            else
            {
                ManipulatorButton.UseVisualStyleBackColor = false;
                sensorState[4] = 'C';
            }
        }

        private void MagnetometerButton_Click(object sender, EventArgs e)
        {
            if (sensorState[5] == 'C')
            {
                MagnetometerButton.UseVisualStyleBackColor = true;
                sensorState[5] = 'O';
            }
            else
            {
                MagnetometerButton.UseVisualStyleBackColor = false;
                sensorState[5] = 'C';
            }
        }

        private void CameraPowerButton_Click(object sender, EventArgs e)
        {
            if (sensorState[6] == 'C')
            {
                CameraPowerButton.UseVisualStyleBackColor = true;
                sensorState[6] = 'O';
            }
            else
            {
                CameraPowerButton.UseVisualStyleBackColor = false;
                sensorState[6] = 'C';
            }
        }

        private void PowerSetupButton_Click(object sender, EventArgs e)
        {
            string strSensorState = new string(sensorState);
            string command_NodeSetup = $"@RS" + strSensorState + "$";
            sendstring(command_NodeSetup);
        }

        private void PowerStatusRefreshButton_Click(object sender, EventArgs e)
        {
            sendstring("@RI$");
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            string command_SecurityParaSetup = "@SP," + textBox1.Text.ToString() + "," + textBox2.Text.ToString() + "," + textBox3.Text.ToString() + "," + textBox4.Text.ToString() + ",$";
            sendstring(command_SecurityParaSetup);
        }
        #endregion

        private void toolStripMenuItem2_Click(object sender, EventArgs e)
        {
            try
            {
                client = new TcpClient();
                client.Connect(server, int.Parse(port));
                stream = client.GetStream();

                Print($"树莓派已连接 {server}:{port}");
                toolStripMenuItem3.Enabled = true;

                // 启动接收线程‌:ml-citation{ref="2,3" data="citationList"}
                var receiveThread = new System.Threading.Thread(ReceiveData);
                receiveThread.IsBackground = true;
                receiveThread.Start();
            }
            catch (Exception ex)
            {
                Print($"树莓派连接失败: {ex.Message}");
            }
        }

        private void toolStripMenuItem3_Click(object sender, EventArgs e)
        {
            Disconnect();
        }

        private void toolStripMenuItem4_Click(object sender, EventArgs e)
        {
            try
            {
                string[] ports = SerialPort.GetPortNames();
                if (ports.Length > 0)
                    UACM.PortName = ports[0];
                UACM.Open();
                Print($"声通机已连接： {UACM.PortName}");
                toolStripMenuItem5.Enabled = true;
            }
            catch (Exception ex)
            {
                MessageBox.Show($"声通机打开失败：{ex.Message}");
            }
        }

        private void toolStripMenuItem5_Click(object sender, EventArgs e)
        {
            UACM.Close();
            Print("声通机连接已断开");
        }

        private void Disconnect()
        {
            stream?.Close();
            client?.Close();
            Print("树莓派连接已断开");
        }

        private void sendstring(string MessageString) 
        {
            try
            {
                byte[] data = Encoding.UTF8.GetBytes(MessageString);
                if (stream != null)
                {
                    stream.Write(data, 0, data.Length);
                }
                else
                {
                    Print(MessageString);
                    Print("树莓派未连接"); 
                }
            }
            catch (Exception ex)
            {
                Print($"发送失败: {ex.Message}");
                Disconnect();
            }
        } 

        private void Print(string msg)
        {
            if (messageBox.InvokeRequired)
            {
                messageBox.Invoke(new Action(() =>
                {
                    messageBox.AppendText($"[{DateTime.Now}] {msg}\r\n");
                }));
            }
            else
            {
                messageBox.AppendText($"[{DateTime.Now}] {msg}\r\n");
            }
        }

        private void ReceiveData()
        {
            byte[] buffer = new byte[1024];
            try
            {
                while (client.Connected)
                {
                    int bytesRead = stream.Read(buffer, 0, buffer.Length);
                    if (bytesRead == 0) break;

                    string received = Encoding.UTF8.GetString(buffer, 0, bytesRead);
                    if (receiveTextBox.InvokeRequired)
                    {
                        receiveTextBox.Invoke(new Action(() =>
                        {
                            receiveTextBox.AppendText($"[{DateTime.Now}] {received}\r\n");
                        }));
                    }
                    else
                    {
                        receiveTextBox.AppendText($"[{DateTime.Now}] {received}\r\n");
                    }
                    if (received[1] == 'V' && received[2] == 'S')
                    {
                        status = received.Split(',');
                        Print("status updated!");
                    }
                }
            }
            catch
            {
                Disconnect();
            }
        }

        #region code for mission download //使命下载板块代码
        private void saveButton_Click(object sender, EventArgs e)
        {
            senddataBox.ReadOnly = true;
            senddataBox.BackColor = Color.Gray;
            File.WriteAllText("mission.txt", senddataBox.Text);
            lines = File.ReadAllLines("mission.txt");
            downloadButton.Enabled = true;
        }

        private void clearButton_Click(object sender, EventArgs e)
        {
            senddataBox.ReadOnly = false;
            senddataBox.BackColor = Color.White;
            senddataBox.Text = "";
            curLine = 0;
            sendstring("@AC$");
        }

        private void downloadButton_Click(object sender, EventArgs e)
        {
            int liner = lines.Length;
            if (liner > curLine)
            {
                string str_liner = liner.ToString();
                if (str_liner.Length == 1)
                    str_liner = "0" + str_liner;  //使命序列的总行数若是单数，前面添0。比如总行数若为8，记为08
                string str_lineindex = (curLine + 1).ToString();
                if (str_lineindex.Length == 1)
                    str_lineindex = "0" + str_lineindex; //使命序列的当前行号若是单数，前面添0。比如当前为第3行，记为03
                string sendtext = "@AW" + str_liner + str_lineindex + lines[curLine] + "$";
                sendstring(sendtext);
            }
            else if (liner == curLine)
                startButton.Enabled = true;
            curLine++;
        }

        private void startButton_Click(object sender, EventArgs e)
        {
            sendstring("@AA$");
        }
        #endregion

        #endregion

        #region code for RS232 connect
        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string data = UACM.ReadExisting();
            if (data[1] == 'V' && data[2] == 'S')
            {
                status = data.Split(',');
                Print("status updated!");
            }
        }
        #endregion

        #region code for remote controller //手柄遥控器板块代码
        private bool isConnected = true;

        private bool IsButtonPressed(GamepadButtonFlags button)//判断某个按钮是否按下，返回是否
        {
            return (currentGamepadState.Buttons & button) == button;//&表示按位and
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (!controller.IsConnected)
            {
                Print("Xbox控制器未连接。");
            }
            Print("Xbox控制器已连接。");
        }

        private void UpdateControllerState()
        {
            // 获取手柄状态并发送到服务器
            SharpDX.XInput.State state = controller.GetState();
            currentGamepadState = state.Gamepad;
            // string controllerInfo = label3.Text;
            // SendMessage(controllerInfo);
        }

        private double GetLeftTriggerValue()
        {
            // 获取左扳机（左触发器）的按下程度
            float L = currentGamepadState.LeftTrigger / 255f;
            return (double)Math.Round(L, 1) * 100;
        }

        private double GetRightTriggerValue()
        {
            // 获取右扳机（右触发器）的按下程度
            float R = currentGamepadState.RightTrigger / 255f;
            return (double)Math.Round(R, 1) * 100;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            try
            {
                UpdateControllerState();
                isConnected = true;
                //                Print("重新连接手柄成功");
            }
            catch (SharpDXException ex)
            {
                isConnected = false;
                //                Print("连接手柄出现错误情况!");
            }

            //            changecolor();


            ////////////////////////////////////////
            ////////获取左摇杆与右摇杆数据值////////
            ////////////////////////////////////////
            //目前的设计方案是仿照艾尔登法环，右滑竿的视角改为姿态调整
            //10.5现在的设计方案又改了，像拳皇


            //左摇杆与右摇杆的角度值计算
            //angle1 = yjnangle(angle1,x1,y1);//用到了姚江南的方法yjnangle，是符合鲍程璐格式的
            //angle1 = sffw(angle1, x1, y1);//马满宏要求的四分方位
            //angle2 = yjnangle(angle2,x2,y2);//用到了姚江南的方法yjnangle，是符合鲍程璐格式的
            //angle2 = sffw(angle2, x2, y2);//马满宏要求的四分方位


            ////////////////////////////////////
            ////////获取手柄全部状态信息////////
            ////////////////////////////////////
            // 使用 switch 语句检查多个按钮的状态
            GamepadButtonFlags pressedButtons = currentGamepadState.Buttons;

            // 判断A按钮是否按下
            /*if ((pressedButtons & GamepadButtonFlags.A) != 0)
            {
                label1.Text = "Press"; // A按钮按下
            }
            else
            {
                label1.Text = "Release"; // A按钮未按下
            }*/
            //右边字母区域
            a[0] = (int)(pressedButtons & GamepadButtonFlags.A);
            a[1] = (int)(pressedButtons & GamepadButtonFlags.B);
            a[2] = (int)(pressedButtons & GamepadButtonFlags.X);
            a[3] = (int)(pressedButtons & GamepadButtonFlags.Y);
            //左边十字区域
            a[4] = (int)(pressedButtons & GamepadButtonFlags.DPadDown);
            a[5] = (int)(pressedButtons & GamepadButtonFlags.DPadRight);
            a[6] = (int)(pressedButtons & GamepadButtonFlags.DPadLeft);
            a[7] = (int)(pressedButtons & GamepadButtonFlags.DPadUp);
            //中间菜单键
            a[8] = (int)(pressedButtons & GamepadButtonFlags.Start);
            a[9] = (int)(pressedButtons & GamepadButtonFlags.Back);
            //上面瞄准键
            a[10] = (int)(pressedButtons & GamepadButtonFlags.RightShoulder);
            a[11] = (int)(pressedButtons & GamepadButtonFlags.LeftShoulder);
            //左右摇杆按下键
            a[12] = (int)(pressedButtons & GamepadButtonFlags.LeftThumb);
            a[13] = (int)(pressedButtons & GamepadButtonFlags.RightThumb);
            //左摇杆X、Y方向偏移量
            a[14] = currentGamepadState.LeftThumbX;
            a[15] = currentGamepadState.LeftThumbY;
            //右摇杆X、Y方向偏移量
            a[16] = currentGamepadState.RightThumbX;
            a[17] = currentGamepadState.RightThumbY;
            //左右扳机力度
            a[18] = (int)(GetLeftTriggerValue());
            a[19] = (int)(GetRightTriggerValue());

            try
            {
                //推进器测试模式：@MT{推进器编号}{推进器转速挡位}$
                if (a[18] != 0)
                {
                    string sendtext = $"@MT{comboBox1.SelectedIndex + 1},{-a[18] / 10}$";
                    sendstring(sendtext);
                }
                if (a[19] != 0)
                {
                    string sendtext = $"@MT{comboBox1.SelectedIndex + 1},{a[19] / 10}$";
                    sendstring(sendtext);
                }

                //舵机测试模式：@MD"角度增减命令"{舵机编号}$
                if (a[8] != 0)
                {
                    string sendtext = $"@MDP{comboBox2.SelectedIndex + 1}$";
                    sendstring(sendtext);
                }
                if (a[9] != 0)
                {
                    string sendtext = $"@MDM{comboBox2.SelectedIndex + 1}$";
                    sendstring(sendtext);
                }

                //平动模式：@MM"平动控制命令"{平动速度参数}$
                if (a[7] != 0)
                {
                    string sendtext = $"@MMF{comboBox3.SelectedIndex + 1}$";
                    sendstring(sendtext);
                }
                if (a[4] != 0)
                {
                    string sendtext = $"@MMB{comboBox3.SelectedIndex + 1}$";
                    sendstring(sendtext);
                }
                if (a[10] != 0)
                {
                    string sendtext = $"@MMU{comboBox3.SelectedIndex + 1}$";
                    sendstring(sendtext);
                }
                if (a[11] != 0)
                {
                    string sendtext = $"@MMD{comboBox3.SelectedIndex + 1}$";
                    sendstring(sendtext);
                }

                //转动模式：@MR"转动控制命令"{转动速度参数}$
                if (a[6] != 0)
                {
                    string sendtext = $"@MRL{comboBox4.SelectedIndex + 1}$";
                    sendstring(sendtext);
                }
                if (a[5] != 0)
                {
                    string sendtext = $"@MRR{comboBox4.SelectedIndex + 1}$";
                    sendstring(sendtext);
                }
                if (a[3] != 0)
                {
                    string sendtext = $"@MRB{comboBox4.SelectedIndex + 1}$";
                    sendstring(sendtext);
                }
                if (a[0] != 0)
                {
                    string sendtext = $"@MRF{comboBox4.SelectedIndex + 1}$";
                    sendstring(sendtext);
                }
                if (a[2] != 0)
                {
                    string sendtext = $"@MRU{comboBox4.SelectedIndex + 1}$";
                    sendstring(sendtext);
                }
                if (a[1] != 0)
                {
                    string sendtext = $"@MRD{comboBox4.SelectedIndex + 1}$";
                    sendstring(sendtext);
                }

                //摇杆航行控制模式：@MS{速度参数}{角度参数}$
                if ((a[14] != 0 && a[14] != -1) || a[15] != 0)
                {
                    int NormofLeftThumb, AngleofLeftThumb;
                    (NormofLeftThumb, AngleofLeftThumb) = getNormAndAngle(a[14], a[15]);
                    string sendtext = $"@MS{NormofLeftThumb},{AngleofLeftThumb}$";
                    sendstring(sendtext);
                }
                if (a[12] != 0)
                {
                    string sendtext = $"@MAS$";
                    sendstring(sendtext);
                }

                //摇杆姿态控制模式：@MP{力度参数}{方位参数}$
                if ((a[16] != 0 && a[16] != -1) || a[17] != 0)
                {
                    int NormofRightThumb, AngleofRightThumb;
                    (NormofRightThumb, AngleofRightThumb) = getNormAndAngle(a[16], a[17]);
                    string sendtext = $"@MP{NormofRightThumb},{AngleofRightThumb}$";
                    sendstring(sendtext);
                }
                if (a[13] != 0)
                {
                    string sendtext = $"@MAP$";
                    sendstring(sendtext);
                }
            }
            catch (Exception ex)
            {
                // 捕获异常，您可以在这里处理异常或者记录日志
                Print("手柄数据发生异常!");
                //MessageBox.Show("请打开串口");
            }

            // 继续检查其他按钮...

            //// 或者可以使用 switch 语句来检查多个按钮
            //switch (currentGamepadState.Buttons)
            //{
            //    case GamepadButtonFlags.A:
            //        // A 按钮按下
            //        // 执行相应操作
            //        break;
            //    case GamepadButtonFlags.B:
            //        // B 按钮按下
            //        // 执行相应操作
            //        break;
            //        // 继续检查其他按钮...
            //}
        }

        /*以下是计算摇杆模值与角度的常规算法*/
        public (int, int) getNormAndAngle(int x, int y)
        {
            if (Math.Abs(x) <= 1600)
                x = 0;
            if (Math.Abs(y) <= 1600)
                y = 0;
            double norm = Math.Sqrt(x * x + y * y) / 410;
            double angle = Math.Round(Math.Atan2(y, x), 4) * 180 / 3.1415;//+ 180
            return ((int)norm, (int)angle);
        }
        /*以下是马满宏四分方位的方法定义*/
        public double sffw(double angle0, double a1, double b1)
        {
            String f = "Z";
            if (a1 == b1 & a1 == 0)
            {
                angle0 = 999;
            }
            else
            {
                if (angle0 <= 135 && angle0 >= 45)
                {
                    angle0 = 0;
                    f = "N";
                }
                else if (angle0 >= -135 && angle0 <= -45)
                {
                    angle0 = 180;
                    f = "S";
                }
                else if (angle0 <= 45 && angle0 >= -45)
                {
                    angle0 = 90;
                    f = "E";
                }
                else
                {
                    angle0 = -90;
                    f = "W";
                }
            }
            return angle0;
        }
        /*以下是姚江南计算方位角的方法定义*/
        public double yjnangle(double angle0, double a1, double b1)
        {
            if ((a1 > 0 && b1 > 0) || (a1 == 0 && b1 > 0))
            {
                angle0 = 90 - angle0;
            }
            else if ((a1 < 0 && b1 > 0) || (a1 < 0 && b1 == 0))
            {
                angle0 = -angle0 + 90;
            }
            else if (a1 < 0 && b1 < 0)
            {
                angle0 = -270 - angle0;
            }
            else if ((a1 > 0 && b1 < 0) || (a1 > 0 && b1 == 0) || (a1 == 0 && b1 < 0))
            {
                angle0 = 90 - angle0;
            }
            return angle0;
        }
        #endregion

        #region code for status monitor //AUV状态监控板块代码
        private void timer2_Tick(object sender, EventArgs e)
        {
            string PiTime = "";
            if (status[0] != "")
            {
                Print(status[0]);
                PiTime = status[0].Substring(3);
            }                
            TimeTextBox.Text = PiTime;
            LongitudeTextBox.Text = status[1];
            LatitudeTextBox.Text = status[2];
            YawTextBox.Text = status[6];
            PitchTextBox.Text = status[7];
            RollTextBox.Text = status[8];
            TempretureTextBox.Text = status[9];
            DepthTextBox.Text = status[10];
            AltimeterTextBox.Text = status[11];
        }
        #endregion

        private void toolStripMenuItem6_Click(object sender, EventArgs e)
        {

        }
    }
}
