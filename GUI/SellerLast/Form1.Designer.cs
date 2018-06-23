namespace SellerLast
{
    partial class MianForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MianForm));
            this.LanSwitch = new System.Windows.Forms.Button();
            this.ToGoods = new System.Windows.Forms.Button();
            this.MaintainModule = new System.Windows.Forms.Button();
            this.Welcome = new System.Windows.Forms.Label();
            this.welcom = new System.Windows.Forms.Label();
            this.Copyright = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // LanSwitch
            // 
            this.LanSwitch.BackColor = System.Drawing.Color.Transparent;
            resources.ApplyResources(this.LanSwitch, "LanSwitch");
            this.LanSwitch.FlatAppearance.BorderSize = 0;
            this.LanSwitch.ForeColor = System.Drawing.SystemColors.ControlLightLight;
            this.LanSwitch.Name = "LanSwitch";
            this.LanSwitch.UseVisualStyleBackColor = false;
            this.LanSwitch.Click += new System.EventHandler(this.LanSwitch_Click);
            // 
            // ToGoods
            // 
            this.ToGoods.BackColor = System.Drawing.Color.Transparent;
            this.ToGoods.FlatAppearance.BorderSize = 0;
            resources.ApplyResources(this.ToGoods, "ToGoods");
            this.ToGoods.ForeColor = System.Drawing.SystemColors.ButtonHighlight;
            this.ToGoods.Name = "ToGoods";
            this.ToGoods.UseVisualStyleBackColor = false;
            this.ToGoods.Click += new System.EventHandler(this.ToGoods_Click);
            // 
            // MaintainModule
            // 
            this.MaintainModule.BackColor = System.Drawing.Color.Transparent;
            this.MaintainModule.FlatAppearance.BorderSize = 0;
            resources.ApplyResources(this.MaintainModule, "MaintainModule");
            this.MaintainModule.ForeColor = System.Drawing.SystemColors.ControlLightLight;
            this.MaintainModule.Name = "MaintainModule";
            this.MaintainModule.UseVisualStyleBackColor = false;
            this.MaintainModule.Click += new System.EventHandler(this.button1_Click);
            // 
            // Welcome
            // 
            resources.ApplyResources(this.Welcome, "Welcome");
            this.Welcome.Name = "Welcome";
            // 
            // welcom
            // 
            resources.ApplyResources(this.welcom, "welcom");
            this.welcom.BackColor = System.Drawing.Color.Transparent;
            this.welcom.ForeColor = System.Drawing.SystemColors.ButtonHighlight;
            this.welcom.Name = "welcom";
            // 
            // Copyright
            // 
            resources.ApplyResources(this.Copyright, "Copyright");
            this.Copyright.BackColor = System.Drawing.Color.Transparent;
            this.Copyright.Name = "Copyright";
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.Color.Transparent;
            this.button1.FlatAppearance.BorderSize = 0;
            resources.ApplyResources(this.button1, "button1");
            this.button1.ForeColor = System.Drawing.SystemColors.ControlLightLight;
            this.button1.Name = "button1";
            this.button1.UseVisualStyleBackColor = false;
            this.button1.Click += new System.EventHandler(this.button1_Click_1);
            // 
            // MianForm
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.button1);
            this.Controls.Add(this.Copyright);
            this.Controls.Add(this.welcom);
            this.Controls.Add(this.Welcome);
            this.Controls.Add(this.MaintainModule);
            this.Controls.Add(this.ToGoods);
            this.Controls.Add(this.LanSwitch);
            this.Name = "MianForm";
            this.Load += new System.EventHandler(this.UARTInt);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button LanSwitch;
        private System.Windows.Forms.Button ToGoods;
        private System.Windows.Forms.Button MaintainModule;
        private System.Windows.Forms.Label Welcome;
        private System.Windows.Forms.Label welcom;
        private System.Windows.Forms.Label Copyright;
        private System.Windows.Forms.Button button1;
    }
}

