using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Media;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SellerLast
{
    public partial class GoodsList : Form
    {
        public GoodsList()
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
            MianForm mianForm = new MianForm();
            mianForm.Show();
            this.Hide();
        }
        private void ShowPay()
        {
            Pay pay = new Pay();
            pay.Show();
            this.Hide();

        }   
        private void ShowLeft(object sender, EventArgs e)
        {
            Num1.Text = "0";
            Num2.Text = "0";
            Num3.Text = "0";
            Num4.Text = "0";
            Num5.Text = "0";
            Num6.Text = "0";
            if (MianForm.Enon.enon)
            {
                /*byte[] byData = new byte[8];
                FileStream file = new FileStream(@"D:\AllergyInfor.txt", FileMode.Open);
                file.Seek(0, SeekOrigin.Begin);
                file.Read(byData, 0, 8); //byData传进来的字节数组,用以接受FileStream对象中的数据,第2个参数是字节数组中开始写入数据的位置,它通常是0,表示从数组的开端文件中向数组写数据,最后一个参数规定从文件读多少字符.
                Decoder d = Encoding.Default.GetDecoder();
                d.GetChars(byData, 0, byData.Length, MianForm.Allregy.allergy, 0);
                // Console.WriteLine(charData);
                file.Close();

                byte[] byData1 = new byte[8];
                FileStream file1 = new FileStream(@"D:\Goodnumber.txt", FileMode.Open);
                file1.Seek(0, SeekOrigin.Begin);
                file1.Read(byData1, 0, 8); //byData传进来的字节数组,用以接受FileStream对象中的数据,第2个参数是字节数组中开始写入数据的位置,它通常是0,表示从数组的开端文件中向数组写数据,最后一个参数规定从文件读多少字符.
                Decoder d1 = Encoding.Default.GetDecoder();
                d1.GetChars(byData1, 0, byData1.Length, MianForm.GoodsNumber.Goodsnumber, 0);
                // Console.WriteLine(charData);
                file.Close();  */


                if (MianForm.UART.Allergy[0] == 48)
                    Aller1.Text = "Allergy";
                if (MianForm.UART.Allergy[1] == 48)
                    Aller2.Text = "Allergy";
                if (MianForm.UART.Allergy[2] == 48)
                    Aller3.Text = "Allergy";
                if (MianForm.UART.Allergy[3] == 48)
                    Aller4.Text = "Allergy";
                if (MianForm.UART.Allergy[4] == 48)
                    Aller5.Text = "Allergy";
                if (MianForm.UART.Allergy[5] == 48)
                    Aller6.Text = "Allergy";
     
                GoodsInfor1.Text = "Remain:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[0]);
                GoodsInfor2.Text = "Remain:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[1]);
                GoodsInfor3.Text = "Remain:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[2]);
                GoodsInfor4.Text = "Remain:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[3]);
                GoodsInfor5.Text = "Remain:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[4]);
                GoodsInfor6.Text = "Remain:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[5]);
            }
            else
            {
                if (MianForm.UART.Allergy[0] == 48)
                    Aller1.Text = "过敏";
                if (MianForm.UART.Allergy[1] == 48)
                    Aller2.Text = "过敏";
                if (MianForm.UART.Allergy[2] == 48)
                    Aller3.Text = "过敏";
                if (MianForm.UART.Allergy[3] == 48)
                    Aller4.Text = "过敏";
                if (MianForm.UART.Allergy[4] == 48)
                    Aller5.Text = "过敏";
                if (MianForm.UART.Allergy[5] == 48)
                    Aller6.Text = "过敏";

                GoodsInfor1.Text = "剩余:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[0]);
                GoodsInfor2.Text = "剩余:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[1]);
                GoodsInfor3.Text = "剩余:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[2]);
                GoodsInfor4.Text = "剩余:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[3]);
                GoodsInfor5.Text = "剩余:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[4]);
                GoodsInfor6.Text = "剩余:" + Convert.ToString(MianForm.GoodsNumber.Goodsnumber[5]);
            }
        }

        private void Ensure_Click(object sender, EventArgs e)
        {
            if (Convert.ToChar(Num1.Text) <= MianForm.GoodsNumber.Goodsnumber[0] &&
                Convert.ToChar(Num2.Text) <= MianForm.GoodsNumber.Goodsnumber[1] &&
                Convert.ToChar(Num3.Text) <= MianForm.GoodsNumber.Goodsnumber[2] &&
                Convert.ToChar(Num4.Text) <= MianForm.GoodsNumber.Goodsnumber[3] &&
                Convert.ToChar(Num5.Text) <= MianForm.GoodsNumber.Goodsnumber[4] &&
                Convert.ToChar(Num6.Text) <= MianForm.GoodsNumber.Goodsnumber[5]
                )
            {
                MianForm.SelectNum.Selectnum[0] = Convert.ToInt32(Num1.Text);
                MianForm.SelectNum.Selectnum[1] = Convert.ToInt32(Num2.Text);
                MianForm.SelectNum.Selectnum[2] = Convert.ToInt32(Num3.Text);
                MianForm.SelectNum.Selectnum[3] = Convert.ToInt32(Num4.Text);
                MianForm.SelectNum.Selectnum[4] = Convert.ToInt32(Num5.Text);
                MianForm.SelectNum.Selectnum[5] = Convert.ToInt32(Num6.Text);
                MianForm.SelectNum.Selectnum[6] = 0;
                MianForm.SelectNum.Selectnum[7] = 0;
                ShowPay();
            }
            else
            {
                if (MianForm.Enon.enon)
                {
                    Play("//warning.wav");
                    MessageBox.Show("The quantity of food you choose exceeds the remaining quantity. Please re enter the quantity of food.");
                }
                else
                    MessageBox.Show("您选择的食物数量超过了剩余数量，请重新输入食物数量");
            }
           
        }
    }
}
