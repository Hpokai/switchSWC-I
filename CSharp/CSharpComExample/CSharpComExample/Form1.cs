using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;

namespace CSharpComExample
{
    public partial class Form1 : Form
    {

        [DllImport("CommDll.dll", EntryPoint = "Connection", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public extern static bool Connection(string strIP, string strPort);
        [DllImport("CommDll.dll", EntryPoint = "MsgSend", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public extern static bool MsgSend(string str);
        [DllImport("CommDll.dll", EntryPoint = "MsgReceive", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public extern static IntPtr MsgReceive();
        [DllImport("CommDll.dll", EntryPoint = "DisConnection", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public extern static bool DisConnection();

        List<TextBox> cmd_list = new List<TextBox>();
        List<TextBox> tempV_list = new List<TextBox>();
        List<string> sensor_type = new List<string>();
        public Form1()
        {
            InitializeComponent();

            TextBox[] arr = new TextBox[] {this.cmd1_textBox, this.cmd2_textBox, this.cmd3_textBox,
                                         this.cmd4_textBox, this.cmd5_textBox, this.cmd6_textBox,
                                         this.cmd7_textBox, this.cmd8_textBox, this.cmd9_textBox,
                                         this.cmd10_textBox, this.cmd11_textBox, this.cmd12_textBox};
            this.cmd_list.AddRange(arr);

            TextBox[] temp_arr = new TextBox[] {this.tempV_01_textBox, this.tempV_02_textBox, this.tempV_03_textBox, 
                                                this.tempV_04_textBox, this.tempV_05_textBox, this.tempV_06_textBox};
            this.tempV_list.AddRange(temp_arr);

            RadioButton[] rb_arr_01 = new RadioButton[] { this.rb_none_01, this.rb_kcm_01, this.rb_sdc15_01, this.rb_husky_01 };
            RadioButton[] rb_arr_02 = new RadioButton[] { this.rb_none_02, this.rb_kcm_02, this.rb_sdc15_02, this.rb_husky_02 };
            RadioButton[] rb_arr_03 = new RadioButton[] { this.rb_none_03, this.rb_kcm_03, this.rb_sdc15_03, this.rb_husky_03 };
            RadioButton[] rb_arr_04 = new RadioButton[] { this.rb_none_04, this.rb_kcm_04, this.rb_sdc15_04, this.rb_husky_04 };
            RadioButton[] rb_arr_05 = new RadioButton[] { this.rb_none_05, this.rb_kcm_05, this.rb_sdc15_05, this.rb_husky_05 };
            RadioButton[] rb_arr_06 = new RadioButton[] { this.rb_none_06, this.rb_kcm_06, this.rb_sdc15_06, this.rb_husky_06 };
            RadioButton[] rb_arr_07 = new RadioButton[] { this.rb_none_07, this.rb_kcm_07, this.rb_sdc15_07, this.rb_husky_07 };
            RadioButton[] rb_arr_08 = new RadioButton[] { this.rb_none_08, this.rb_kcm_08, this.rb_sdc15_08, this.rb_husky_08 };
            RadioButton[] rb_arr_09 = new RadioButton[] { this.rb_none_09, this.rb_kcm_09, this.rb_sdc15_09, this.rb_husky_09 };
            RadioButton[] rb_arr_10 = new RadioButton[] { this.rb_none_10, this.rb_kcm_10, this.rb_sdc15_10, this.rb_husky_10 };
            RadioButton[] rb_arr_11 = new RadioButton[] { this.rb_none_11, this.rb_kcm_11, this.rb_sdc15_11, this.rb_husky_11 };
            RadioButton[] rb_arr_12 = new RadioButton[] { this.rb_none_12, this.rb_kcm_12, this.rb_sdc15_12, this.rb_husky_12 };

            string[] none_str = new string[]{"0", "None", "None", "None", "None", "None", "None", 
                                                  "None", "None", "None", "None", "None", "None"};
            this.sensor_type.AddRange(none_str);

            //
            string[] words = Properties.Settings.Default.type.Split(',');
            for (int i = 0; i < 12; i++)
            {
                Console.WriteLine(words[i]);
            }
        }

        private void ConnButton_Click(object sender, EventArgs e)
        {
            //Connection("192.168.0.120", "5000");
            Connection(this.textBox1.Text, "5000");
            if (this.checkBox1.Checked)
            {
                this.button10.PerformClick();
            }
        }

        bool isMsgGot = false;
        private void button2_Click(object sender, EventArgs e)
        {
            this.RMessageTextBox.Text = string.Empty;
            
            string rmsg;
            if (MessageTextBox.Text != "")
            {
                MsgSend(MessageTextBox.Text);
                rmsg = System.Runtime.InteropServices.Marshal.PtrToStringAnsi(MsgReceive());
                this.parse_return_cmd(rmsg);
                RMessageTextBox.Text = rmsg.ToString();
                this.isMsgGot = true;
            }
        }

        private void button14_Click(object sender, EventArgs e)
        {
            this.get_temp_value("");
        }

        private int get_temp_value(string all_str)
        {
            int ret = 0;

            string[] sub_str_split = all_str.Split('-');
            Console.WriteLine(sub_str_split[5]);

            int temp_v_int = Convert.ToInt32(sub_str_split[5], 16);
            Console.WriteLine(temp_v_int);

            ret = temp_v_int;

            return ret;
        }

        private void parse_return_cmd(string re_cmd)
        {
            if ( re_cmd.Contains("EQType"))
            {
                this.re_cmd_label.Text = "EQType";
                for (int i = 0; i < 12; i++)
                {
                    this.cmd_list[i].Text = string.Empty;
                }
            }
            else if (re_cmd.Contains("EQCmd"))
            {
                this.re_cmd_label.Text = "EQCmd";
                for (int i = 0; i < 12; i++)
                {
                    this.cmd_list[i].Text = string.Empty;
                }
            }
            else
            {
                try
                {
                    this.re_cmd_label.Text = re_cmd.Substring(0, re_cmd.IndexOf("*") - 0);

                    int start = 0, end = re_cmd.IndexOf("&") - 1;
                    for (int i = 12; i > 0; i--)
                    {
                        start = re_cmd.IndexOf("#" + i.ToString());
                        this.cmd_list[i - 1].Text = re_cmd.Substring(start, end - start);
                        end = start;
                    }

                    if (this.re_cmd_label.Text == "EQData")
                    {
                        for (int i = 0; i < 6; i++)
                        {
                            if (this.cmd_list[i].Text.Length < 6)
                            {
                                this.tempV_list[i].Text = "0";
                            }
                            else
                            {
                                string sub = this.cmd_list[i].Text.Substring(3);
                                string[] cmd_split = sub.Split('@');
                                Console.WriteLine(cmd_split[i]);

                                for (int k = 0; k < 6; k++)
                                {
                                    int temp = this.get_temp_value(cmd_split[k]);

                                    if (temp > 0)
                                    {
                                        this.tempV_list[k].Text = temp.ToString();
                                    }
                                }
                            }
                        }
                    }
                }
                catch
                {
                }
            }
        }

        private void DisConnButton_Click(object sender, EventArgs e)
        {
            this.button8.PerformClick();
            DisConnection();
        }

        private void button8_Click(object sender, EventArgs e)
        {
            this.timer1.Stop();
            this.timer2.Stop();
            this.MessageTextBox.Text = string.Empty;            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.MessageTextBox.Text = "EQSId*@" + this.textBox1.Text + 
                                       "#1,0#2,0#3,0#4,0#5,0#6,0#7,0#8,0#9,0#10,0#11,0#12,0#&";
        }

        private void button3_Click(object sender, EventArgs e)
        {
            //this.MessageTextBox.Text = "EQBaudrate*#1,9600,8,N,1#2,19200,8,N,1#3,19200,8,N,1#4,9600,8,N,1#5,NULL#6,NULL#7,NULL#8,NULL#9,NULL#10,NULL#11,NULL#12,NULL#&";
            //this.MessageTextBox.Text = "EQBaudrate*#1,19200,8,N,1#2,9600,8,N,1#3,NULL#4,NULL#5,NULL#6,NULL#7,NULL#8,NULL#9,NULL#10,NULL#11,NULL#12,NULL#&";

            string br = string.Empty;
            string cmd = "EQBaudrate*";
            for (int i = 1; i < 13; i++)
            {
                switch (this.sensor_type[i])
                {
                    case "KCM":
                        br = "9600,8,N,1";
                        break;
                    case "SDC15":
                        br = "19200,8,N,1";
                        break;
                    case "HUSKY":
                        br = "19200,8,N,1";
                        break;
                    default:
                        br = "NULL";
                        break;
                }
                cmd += ("#" + i.ToString() + "," + br);
            }
            cmd += "#&";

            this.MessageTextBox.Text = cmd;
        }

        private void button4_Click(object sender, EventArgs e)
        {
            //this.MessageTextBox.Text = "EQType*@KCM$N6:2,9,10,4,5,6@Husky$N2:7,8@SDC15$N4:1,3,11,12@&";
            //this.MessageTextBox.Text = "EQType*@KCM$N2:2,3@Husky$N0:0@SDC15$N1:1@&";

            int kcm_cnt = 0, sdc15_cnt = 0, husky_cnt = 0;
            string kcm_str = ":", sdc15_str = ":", husky_str = ":";
            for (int i = 1; i < 13; i++)
            {
                switch (this.sensor_type[i])
                {
                    case "KCM":
                        kcm_cnt++;
                        kcm_str += (i.ToString() + ",");
                        break;
                    case "SDC15":
                        sdc15_cnt++;
                        sdc15_str += (i.ToString() + ",");
                        break;
                    case "HUSKY":
                        husky_cnt++;
                        husky_str += (i.ToString() + ",");
                        break;
                    default:
                        break;
                }
            }

            if (kcm_cnt == 0)   kcm_str += "0";
            else kcm_str = kcm_str.Substring(0, kcm_str.Length-1);
            if (sdc15_cnt == 0)   sdc15_str += "0";
            else sdc15_str = sdc15_str.Substring(0, sdc15_str.Length-1);
            if (husky_cnt == 0)   husky_str += "0";
            else husky_str = husky_str.Substring(0, husky_str.Length-1);

            this.MessageTextBox.Text = "EQType*@KCM$N" + kcm_cnt.ToString() + kcm_str +
                                        "@Husky$N" + husky_cnt.ToString() + husky_str +
                                        "@SDC15$N" + sdc15_cnt.ToString() + sdc15_str +
                                        "@&";
        }

        private void button5_Click(object sender, EventArgs e)
        {
            this.MessageTextBox.Text = "EQCmd*@KCM:010310010001D10A@Husky:04262031222005@Husky:04262032222005@Husky:04262033222005@Husky:04262034222005@Husky:04262035222005@Husky:04262036222005@Husky:04262037222005@Husky:04262038222005@Husky:04262039222005@Husky:0426203A222005@Husky:0426203B222005@Husky:0426203C222005@SDC15:0103238D00025FA4@SDC15:0203238D00025F97@SDC15:0303238D00025E46@SDC15:0403238D00025FF1@SDC15:0503238D00025E20@SDC15:0603238D00025E13@SDC15:0703238D00025FC2@SDC15:0803238D00025F3D@SDC15:0903238D00025EEC@SDC15:0A03238D00025EDF@SDC15:0B03238D00025F0E@SDC15:0C03238D00025EB9@&";
        }

        private void button6_Click(object sender, EventArgs e)
        {
            this.MessageTextBox.Text = "EQId*@" + this.textBox1.Text +
                                       "#1,0#2,0#3,0#4,0#5,0#6,0#7,0#8,0#9,0#10,0#11,0#12,0#&";
        }

        private void button7_Click(object sender, EventArgs e)
        {
            this.MessageTextBox.Text = "EQData*#12#&";
        }

        private void button13_Click(object sender, EventArgs e)
        {
            this.MessageTextBox.Text = "EQReset*#&";
        }

        bool isData = false;
        private void timer1_Tick(object sender, EventArgs e)
        {
            if (isData)
            {
                this.button7.PerformClick();    // data
                this.isData = false;
            }
            else
            {
                this.button6.PerformClick();    // id
                this.isData = true;
            }
            this.button2.PerformClick();
        }

        private void button9_Click(object sender, EventArgs e)
        {
            this.timer1.Start();
        }

        int state = 0;
        private void timer2_Tick(object sender, EventArgs e)
        {
            switch (state)
            {
                case 0:
                    this.button1.PerformClick();
                    this.button2.PerformClick();
                    state++;
                    break;
                case 1:
                    if (this.isMsgGot)
                    {
                        this.button3.PerformClick();
                        this.isMsgGot = false;
                        this.button2.PerformClick();
                        state++;
                    }
                    break;
                case 2:
                    if (this.isMsgGot)
                    {
                        this.button4.PerformClick();
                        this.isMsgGot = false;
                        this.button2.PerformClick();
                        state++;
                    }
                    break;
                case 3:
                    if (this.isMsgGot)
                    {
                        this.button5.PerformClick();
                        this.isMsgGot = false;
                        this.button2.PerformClick();
                        state++;
                    }
                    break;
                case 4:
                    if (this.isMsgGot)
                    {
                        this.button9.PerformClick();
                        this.isMsgGot = false;
                        state++;
                    }
                    break;
                default:
                    this.timer2.Stop();
                    break;
            }
        }

        private void button10_Click(object sender, EventArgs e)
        {
            System.Threading.Thread.Sleep(500);
            this.state = 0;
            this.timer2.Start();
        }

        private void button12_Click(object sender, EventArgs e)
        {
            this.textBox1.Text = "192.168.0.22";
        }

        private void button11_Click(object sender, EventArgs e)
        {
            this.textBox1.Text = "192.168.0.120";
        }

        private void rb_set_type(object sender, int num)
        {
            RadioButton rb = (RadioButton)sender;
            if (rb.Checked)
            {
                string type = rb.Name.Substring(3, rb.Name.Length - 6);
                this.sensor_type[num] = type.ToUpper();
                //MessageBox.Show(rb.Name + " ; " + this.sensor_type[num].ToUpper());

                string save_type = string.Empty;
                for (int i = 1; i < 13; i++)
			    {
                    save_type += this.sensor_type[i];
                    if (i != 12)
                        save_type += ",";
			    }
                Properties.Settings.Default.type = save_type;
                Properties.Settings.Default.Save();
            }
        }

        private void rb_01_CheckedChanged(object sender, EventArgs e)
        {
            int num = 1;
            this.rb_set_type(sender, num);
        }

        private void rb_02_CheckedChanged(object sender, EventArgs e)
        {
            int num = 2;
            this.rb_set_type(sender, num);
        }

        private void rb_03_CheckedChanged(object sender, EventArgs e)
        {
            int num = 3;
            this.rb_set_type(sender, num);
        }

        private void rb_04_CheckedChanged(object sender, EventArgs e)
        {
            int num = 4;
            this.rb_set_type(sender, num);
        }

        private void rb_05_CheckedChanged(object sender, EventArgs e)
        {
            int num = 5;
            this.rb_set_type(sender, num);
        }

        private void rb_06_CheckedChanged(object sender, EventArgs e)
        {
            int num = 6;
            this.rb_set_type(sender, num);
        }

        private void rb_07_CheckedChanged(object sender, EventArgs e)
        {
            int num = 7;
            this.rb_set_type(sender, num);
        }

        private void rb_08_CheckedChanged(object sender, EventArgs e)
        {
            int num = 8;
            this.rb_set_type(sender, num);
        }

        private void rb_09_CheckedChanged(object sender, EventArgs e)
        {
            int num = 9;
            this.rb_set_type(sender, num);
        }

        private void rb_10_CheckedChanged(object sender, EventArgs e)
        {
            int num = 10;
            this.rb_set_type(sender, num);
        }

        private void rb_11_CheckedChanged(object sender, EventArgs e)
        {
            int num = 11;
            this.rb_set_type(sender, num);
        }

        private void rb_12_CheckedChanged(object sender, EventArgs e)
        {
            int num = 12;
            this.rb_set_type(sender, num);
        }

        private void timer3_Tick(object sender, EventArgs e)
        {
            if (this.checkBox2.Checked) this.checkBox2.Checked = false;
            else this.checkBox2.Checked = true;
        }
    }
}
