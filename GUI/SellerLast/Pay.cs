using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Media;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
namespace SellerLast
{
    public partial class Pay : Form
    {
        byte[] Send = new byte[8];
        byte[] Data = new byte[8];
        private SerialPort mySerialPort;
        public Pay()
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
            else
            { Play("//RETURN (2).wav"); }
            GoodsList goodsList = new GoodsList();
            mySerialPort.Close();
            goodsList.Show();
            this.Close();
        }

      
       private void ShowPicture(object sender, EventArgs e)
        {
            mySerialPort = new SerialPort("COM3");
            mySerialPort.ReceivedBytesThreshold = 8;
            mySerialPort.BaudRate = 115200;
            mySerialPort.Parity = Parity.None;
            mySerialPort.StopBits = StopBits.One;
            mySerialPort.DataBits = 8;
            mySerialPort.Handshake = Handshake.None;
            mySerialPort.DataReceived += new SerialDataReceivedEventHandler(mySerialPort_DataReceived);

            mySerialPort.Open();



            if (MianForm.Enon.enon)
            {
               label1.Text = "Cola:" + Convert.ToString(MianForm.SelectNum.Selectnum[0]);
                label2.Text = "Chips:" + Convert.ToString(MianForm.SelectNum.Selectnum[1]);
                label3.Text = "Hamburger:" + Convert.ToString(MianForm.SelectNum.Selectnum[2]);
                label4.Text = "Icecream:" + Convert.ToString(MianForm.SelectNum.Selectnum[3]);
                label5.Text = "Milk:" + Convert.ToString(MianForm.SelectNum.Selectnum[4]);
                label6.Text = "Pizza:" + Convert.ToString(MianForm.SelectNum.Selectnum[5]);

            }
            else
            {
                label1.Text = "可乐:" + Convert.ToString(MianForm.SelectNum.Selectnum[0]);
                label2.Text = "薯条:" + Convert.ToString(MianForm.SelectNum.Selectnum[1]);
                label3.Text = "汉堡:" + Convert.ToString(MianForm.SelectNum.Selectnum[2]);
                label4.Text = "雪糕:" + Convert.ToString(MianForm.SelectNum.Selectnum[3]);
                label5.Text = "牛奶:" + Convert.ToString(MianForm.SelectNum.Selectnum[4]);
                label6.Text = "披萨:" + Convert.ToString(MianForm.SelectNum.Selectnum[5]);
            }


            if (MianForm.Enon.enon)
            {
                Play("//YourOrder.wav");
            }
            else
            { Play("//YourOrder1.wav"); }

        }
   private void GoodsOut_Click(object sender, EventArgs e)
        {
            if (mySerialPort.IsOpen)
            {
                mySerialPort.Close();
            }
           for(int i = 0; i<=5;i++)
            {
                Send[i] = Convert.ToByte(MianForm.SelectNum.Selectnum[i] + 48);
            }
            Send[6] = 48;
            Send[7] = 27;
            if (mySerialPort.IsOpen == false)
            {
                mySerialPort.Open();

                mySerialPort.Write(Send, 0, 8);//发送指令

            }
            if (MianForm.Enon.enon)
                { 
                    Play("//buy.wav");
                }
                else Play("//OUT.wav");
            }

        private void mySerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            mySerialPort.Read(Data, 0, 8);//data数组用于存储读取的数据
            File.WriteAllText(@"D:\Goodnumber.txt", Encoding.ASCII.GetString(Data));
            //MessageBox.Show(Encoding.ASCII.GetString(Data));
            Array.Clear(Data, 0, 8);
            //mySerialPort.Close(); 
        }
    }
}
