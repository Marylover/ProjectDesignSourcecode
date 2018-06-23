using System;
using System.ComponentModel;
using System.Windows.Forms;
using System.Threading;
using System.Globalization;
using System.Media;
using System.IO;
using System.IO.Ports;
using System.Text;
namespace SellerLast
{
    public partial class MianForm : Form
    {
        public static class UART
        {
            public static byte[] Send = new byte[2];
            public static byte[] Allergy = new byte[8];
            public static SerialPort mySerialPort;
        }
        public MianForm()
        {
            InitializeComponent();
            this.ControlBox = false;
        }
       /* public class Allregy
        {
            public static  Char [] allergy = new char [8];
        }  */
        
       public class GoodsNumber
        {
            public static Char[] Goodsnumber = new char[8];
        }
        public class SelectNum
        {
            public static int[] Selectnum = new int[8];
        }
        public class Enon
        {
            public static bool enon;
        }
        private void LanSwitch_Click(object sender, EventArgs e)
        {
            Enon.enon = true;
            Play("//switchtoen.wav");
            string langFlag = "en";
            Thread.CurrentThread.CurrentUICulture = CultureInfo.GetCultureInfo(langFlag);   //应用资源文件，en为英语，zh-CN为中文  
            ApplayResource();          //ApplayResource()中执行资源应用  
            
        }
        private void button1_Click_1(object sender, EventArgs e)
        {
            Play("//SWITCHTOCN.wav");
            Enon.enon = false;
            string langFlag = "zh";
            Thread.CurrentThread.CurrentUICulture = CultureInfo.GetCultureInfo(langFlag);   //应用资源文件，en为英语，zh-CN为中文  
            ApplayResource();          //ApplayResource()中执行资源应用  

        }

        
        private void ApplayResource()             //应用资源文件  
        {
            System.ComponentModel.ComponentResourceManager res = new ComponentResourceManager(typeof(MianForm));    //获取Form1 的资源管理器  
            foreach (Control ctl in Controls)           //循环遍历控件Controlsd对  
            {
                res.ApplyResources(ctl, ctl.Name);    //应用ctl的ctl.Name资源配置文件；  
            }
         
            //由于res中不包含userControl1中资源需要重新引用资源文件  
            //对每个新的窗口都需重新配置res  
            //System.ComponentModel.ComponentResourceManager res1 = new ComponentResourceManager(typeof(userControl1));    //获取userControl1的资源管理器  

           /* foreach (Control item in userControl12.Controls)     //对自己创建的空间进行遍历  
            {
                res1.ApplyResources(item, item.Name);
            }*/

        }
        private void Play (string url)
        {
            SoundPlayer player = new SoundPlayer();
            player.SoundLocation = Application.StartupPath + url;
            player.Load();
            player.Play();
        }
       
        private void ToGoods_Click(object sender, EventArgs e)
        {
            
            byte[] byData = new byte[8];

            UART.Send[0] = 69; //向MCU发送指令‘E’，读取过敏信息
            UART.Send[1] = 27;

            if (UART.mySerialPort.IsOpen)
            {
                UART.mySerialPort.Close();
            }
            if (UART.mySerialPort.IsOpen == false)
            {
                UART.mySerialPort.Open();

                UART.mySerialPort.Write(UART.Send, 0, 2);//发送指令


            }


            
            //char[] charData = new char[1000];
            /*public void Read()
            /*
            FileStream file = new FileStream(@"D:\AllergyInfor.txt", FileMode.Open);
                file.Seek(0, SeekOrigin.Begin);
                file.Read(byData, 0, 8); //byData传进来的字节数组,用以接受FileStream对象中的数据,第2个参数是字节数组中开始写入数据的位置,它通常是0,表示从数组的开端文件中向数组写数据,最后一个参数规定从文件读多少字符.
                Decoder d = Encoding.Default.GetDecoder();
                d.GetChars(byData, 0, byData.Length, Allregy.allergy, 0);
               // Console.WriteLine(charData);
                file.Close();  
            
    */


            if (Enon.enon)
            {
                Play("//goodslist.wav");
            }
            else Play("//GOODSLIST (2).wav");

            Thread.Sleep(100);
            UART.mySerialPort.Close();
            GoodsList goodsList = new GoodsList();
            goodsList.Show();
            this.Hide();
        }
        private void mySerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            MianForm.UART.mySerialPort.Read(MianForm.UART.Allergy, 0, 8);//data数组用于存储读取的数据
            File.WriteAllText(@"D:\AllergyInfor.txt", Encoding.ASCII.GetString(MianForm.UART.Allergy));
           // MessageBox.Show(Encoding.ASCII.GetString(MianForm.UART.Data));
            //23Array.Clear(MianForm.UART.Allergy, 0, 8);
            //mySerialPort.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (Enon.enon)
            {
                Play("//maintainmodel.wav");
            }
            else Play("//MAINTAINMODEL (2).wav");
            if (UART.mySerialPort.IsOpen)
            {
                UART.mySerialPort.Close();
            }
            MaintainModle maintainModle = new MaintainModle();
            maintainModle.Show();
            this.Hide();
        }

        private void UARTInt(object sender, EventArgs e)
        {
            UART.mySerialPort = new SerialPort("COM3");
            UART.mySerialPort.ReceivedBytesThreshold = 2;
            UART.mySerialPort.BaudRate = 115200;
            UART.mySerialPort.Parity = Parity.None;
            UART.mySerialPort.StopBits = StopBits.One;
            UART.mySerialPort.DataBits = 8;
            UART.mySerialPort.Handshake = Handshake.None;
            UART.mySerialPort.DataReceived += new SerialDataReceivedEventHandler(mySerialPort_DataReceived);


            byte[] byData = new byte[8];
            FileStream file = new FileStream(@"D:\Goodnumber.txt", FileMode.Open);
            file.Seek(0, SeekOrigin.Begin);
            file.Read(byData, 0, 8); //byData传进来的字节数组,用以接受FileStream对象中的数据,第2个参数是字节数组中开始写入数据的位置,它通常是0,表示从数组的开端文件中向数组写数据,最后一个参数规定从文件读多少字符.
            Decoder d = Encoding.Default.GetDecoder();
            d.GetChars(byData, 0, byData.Length, GoodsNumber.Goodsnumber, 0);
            // Console.WriteLine(charData);
            file.Close();  

        }
    }
}
