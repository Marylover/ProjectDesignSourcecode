using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Media;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;


namespace SellerLast
{
    public partial class MaintainModle : Form
    {
          byte[] Send = new byte[2];
          byte[] Data = new byte[8];
          public static SerialPort mySerialPort;
        public MaintainModle()
        {

            InitializeComponent();
            this.ControlBox = false;
        }
        private void Play(string url)
        {
            SoundPlayer player = new SoundPlayer();
            player.SoundLocation = Application.StartupPath + url;
            player.Load();
            player.Play();
        }

        private void Return_Click(object sender, EventArgs e)
        {
            if (MianForm.Enon.enon)
            {
                Play("//return.wav");
            }
            else Play("//RETURN (2).wav");

            if (mySerialPort.IsOpen)
            {
                mySerialPort.Close();
            }
            MianForm mianForm = new MianForm();
            mianForm.Show();
            Close();
        }

        private void RunMotor_Click(object sender, EventArgs e)
        {
            if (mySerialPort.IsOpen)
            {
                mySerialPort.Close();
            }
            Send[0] = 65;  //向MCU发送指令‘A’,转动伺服电机,角度1
            Send[1] = 27;
            if (mySerialPort.IsOpen == false)
            {
                mySerialPort.Open(); 
                
                mySerialPort.Write(Send, 0, 2);//发送指令
                
            }
          /*  if (MianForm.UART.mySerialPort.IsOpen)
            {
                MianForm.UART.mySerialPort.Close();
            }   */
            if (MianForm.Enon.enon)
            {
                Play("//runsevormotor.wav");
            }
            else Play("//SEVORMOTOR.wav");
        }

        private void ReadSensor_Click(object sender, EventArgs e)
        {
        
             Send [0] = 68; //向MCU发送指令‘D’，读取颜色传感器
             Send[1] = 27;
            if (mySerialPort.IsOpen)
            {
                mySerialPort.Close();
            }
            if (mySerialPort.IsOpen == false)
            {
                mySerialPort.Open();
                   
                mySerialPort.Write(Send, 0, 2);//发送指令
                
            }
            /*if (mySerialPort.IsOpen)
            {
                mySerialPort.Close();
            }*/
            if (MianForm.Enon.enon)
            {
                Play("//readsensor.wav");
            }
            else Play("//READSENSOR (2).wav");
        }
        private string StringToHexString(string s, Encoding encode)
        {
            byte[] b = encode.GetBytes(s);//按照指定编码将string编程字节数组
            string result = string.Empty;
            for (int i = 0; i < b.Length; i++)//逐字节变为16进制字符，以%隔开
            {
                result += Convert.ToString(b[i], 16);
            }
            return result;
        }
        private void MaintainModle_Load(object sender, EventArgs e)
        {
             mySerialPort = new SerialPort("COM3");
             mySerialPort.ReceivedBytesThreshold = 2;
             mySerialPort.BaudRate = 115200;
             mySerialPort.Parity = Parity.None;
             mySerialPort.StopBits = StopBits.One;
             mySerialPort.DataBits = 8;
             mySerialPort.Handshake = Handshake.None;
             mySerialPort.DataReceived += new SerialDataReceivedEventHandler(mySerialPort_DataReceived);
            
             mySerialPort.Open(); 
            if (MianForm.Enon.enon)
            {
                Cola.Text = "Cola:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[0]);
                Chips.Text = "Chips:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[1]);
                Hamburger.Text = "Hamburger:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[2]);
                Icecream.Text = "Icecream:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[3]);
                Milk.Text = "Milk:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[4]);
                Pizza.Text = "Pizza:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[5]);
                
            }
            else
            {
                Cola.Text = "可乐:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[0]);
                Chips.Text = "薯条:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[1]);
                Hamburger.Text = "汉堡:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[2]);
                Icecream.Text = "雪糕:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[3]);
                Milk.Text = "牛奶:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[4]);
                Pizza.Text = "披萨:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[5]);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Send[0] = 67; //向MCU发送指令‘C’,读取距离传感器
            Send[1] = 10;
            if (mySerialPort.IsOpen)
            {
                mySerialPort.Close();
            }
            if (mySerialPort.IsOpen == false)
            {
                mySerialPort.Open();
                
                    mySerialPort.Write(Send, 0, 2);//发送指令

            }
            /*if (mySerialPort.IsOpen)
            {
                mySerialPort.Close();
            }*/
            if (MianForm.Enon.enon)
            {
                Play("//readsensor.wav");
            }
            else Play("//READSENSOR (2).wav");
        }

        private void Runmotor2_Click(object sender, EventArgs e)
        {
            if (mySerialPort.IsOpen)
            {
                mySerialPort.Close();
            }
            Send[0] = 66;  //向MCU发送指令‘B’,转动伺服电机，角度2
            Send[1] = 27;
            if (mySerialPort.IsOpen == false)
            {
                mySerialPort.Open();
                
                    mySerialPort.Write(Send, 0, 2);//发送指令

            }
            /*  if (MianForm.UART.mySerialPort.IsOpen)
              {
                  MianForm.UART.mySerialPort.Close();
              }   */
            if (MianForm.Enon.enon)
            {
                Play("//runsevormotor.wav");
            }
            else Play("//SEVORMOTOR.wav");
        }
private void mySerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
{
mySerialPort.Read(Data, 0, 8);//data数组用于存储读取的数据
File.WriteAllText(@"D:\OutPut.txt", Encoding.ASCII.GetString(Data));
MessageBox.Show(Encoding.ASCII.GetString(Data));
Array.Clear(Data, 0, 8);
//mySerialPort.Close(); 
}
    }
}
