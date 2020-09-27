
namespace MaxStatsDesktop
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.updatetick = new System.Windows.Forms.Timer(this.components);
            this.titleText = new System.Windows.Forms.Label();
            this.cpuGroup = new System.Windows.Forms.GroupBox();
            this.cpuLoad = new System.Windows.Forms.Label();
            this.cpuTemp = new System.Windows.Forms.Label();
            this.cpuFreq = new System.Windows.Forms.Label();
            this.cpuName = new System.Windows.Forms.Label();
            this.gpuGroup = new System.Windows.Forms.GroupBox();
            this.gpuVramClock = new System.Windows.Forms.Label();
            this.gpuLoad = new System.Windows.Forms.Label();
            this.gpuTemp = new System.Windows.Forms.Label();
            this.gpuCoreClock = new System.Windows.Forms.Label();
            this.gpuName = new System.Windows.Forms.Label();
            this.ramGroup = new System.Windows.Forms.GroupBox();
            this.ramTotal = new System.Windows.Forms.Label();
            this.ramUsed = new System.Windows.Forms.Label();
            this.cpuGroup.SuspendLayout();
            this.gpuGroup.SuspendLayout();
            this.ramGroup.SuspendLayout();
            this.SuspendLayout();
            // 
            // updatetick
            // 
            this.updatetick.Enabled = true;
            this.updatetick.Interval = 500;
            this.updatetick.Tick += new System.EventHandler(this.updatetick_Tick);
            // 
            // titleText
            // 
            this.titleText.AutoSize = true;
            this.titleText.Location = new System.Drawing.Point(12, 9);
            this.titleText.Name = "titleText";
            this.titleText.Size = new System.Drawing.Size(95, 13);
            this.titleText.TabIndex = 0;
            this.titleText.Text = "MaxStats - Testing";
            // 
            // cpuGroup
            // 
            this.cpuGroup.Controls.Add(this.cpuLoad);
            this.cpuGroup.Controls.Add(this.cpuTemp);
            this.cpuGroup.Controls.Add(this.cpuFreq);
            this.cpuGroup.Controls.Add(this.cpuName);
            this.cpuGroup.Location = new System.Drawing.Point(12, 25);
            this.cpuGroup.Name = "cpuGroup";
            this.cpuGroup.Size = new System.Drawing.Size(223, 76);
            this.cpuGroup.TabIndex = 1;
            this.cpuGroup.TabStop = false;
            this.cpuGroup.Text = "CPU";
            // 
            // cpuLoad
            // 
            this.cpuLoad.AutoSize = true;
            this.cpuLoad.Location = new System.Drawing.Point(6, 55);
            this.cpuLoad.Name = "cpuLoad";
            this.cpuLoad.Size = new System.Drawing.Size(34, 13);
            this.cpuLoad.TabIndex = 3;
            this.cpuLoad.Text = "Load:";
            // 
            // cpuTemp
            // 
            this.cpuTemp.AutoSize = true;
            this.cpuTemp.Location = new System.Drawing.Point(6, 42);
            this.cpuTemp.Name = "cpuTemp";
            this.cpuTemp.Size = new System.Drawing.Size(37, 13);
            this.cpuTemp.TabIndex = 2;
            this.cpuTemp.Text = "Temp:";
            // 
            // cpuFreq
            // 
            this.cpuFreq.AutoSize = true;
            this.cpuFreq.Location = new System.Drawing.Point(6, 29);
            this.cpuFreq.Name = "cpuFreq";
            this.cpuFreq.Size = new System.Drawing.Size(31, 13);
            this.cpuFreq.TabIndex = 1;
            this.cpuFreq.Text = "Freq:";
            // 
            // cpuName
            // 
            this.cpuName.AutoSize = true;
            this.cpuName.Location = new System.Drawing.Point(6, 16);
            this.cpuName.Name = "cpuName";
            this.cpuName.Size = new System.Drawing.Size(41, 13);
            this.cpuName.TabIndex = 0;
            this.cpuName.Text = "Name: ";
            // 
            // gpuGroup
            // 
            this.gpuGroup.Controls.Add(this.gpuVramClock);
            this.gpuGroup.Controls.Add(this.gpuLoad);
            this.gpuGroup.Controls.Add(this.gpuTemp);
            this.gpuGroup.Controls.Add(this.gpuCoreClock);
            this.gpuGroup.Controls.Add(this.gpuName);
            this.gpuGroup.Location = new System.Drawing.Point(12, 107);
            this.gpuGroup.Name = "gpuGroup";
            this.gpuGroup.Size = new System.Drawing.Size(223, 86);
            this.gpuGroup.TabIndex = 4;
            this.gpuGroup.TabStop = false;
            this.gpuGroup.Text = "GPU";
            // 
            // gpuVramClock
            // 
            this.gpuVramClock.AutoSize = true;
            this.gpuVramClock.Location = new System.Drawing.Point(6, 42);
            this.gpuVramClock.Name = "gpuVramClock";
            this.gpuVramClock.Size = new System.Drawing.Size(71, 13);
            this.gpuVramClock.TabIndex = 4;
            this.gpuVramClock.Text = "VRAM Clock:";
            // 
            // gpuLoad
            // 
            this.gpuLoad.AutoSize = true;
            this.gpuLoad.Location = new System.Drawing.Point(6, 68);
            this.gpuLoad.Name = "gpuLoad";
            this.gpuLoad.Size = new System.Drawing.Size(34, 13);
            this.gpuLoad.TabIndex = 3;
            this.gpuLoad.Text = "Load:";
            // 
            // gpuTemp
            // 
            this.gpuTemp.AutoSize = true;
            this.gpuTemp.Location = new System.Drawing.Point(6, 55);
            this.gpuTemp.Name = "gpuTemp";
            this.gpuTemp.Size = new System.Drawing.Size(37, 13);
            this.gpuTemp.TabIndex = 2;
            this.gpuTemp.Text = "Temp:";
            // 
            // gpuCoreClock
            // 
            this.gpuCoreClock.AutoSize = true;
            this.gpuCoreClock.Location = new System.Drawing.Point(6, 29);
            this.gpuCoreClock.Name = "gpuCoreClock";
            this.gpuCoreClock.Size = new System.Drawing.Size(62, 13);
            this.gpuCoreClock.TabIndex = 1;
            this.gpuCoreClock.Text = "Core Clock:";
            // 
            // gpuName
            // 
            this.gpuName.AutoSize = true;
            this.gpuName.Location = new System.Drawing.Point(6, 16);
            this.gpuName.Name = "gpuName";
            this.gpuName.Size = new System.Drawing.Size(41, 13);
            this.gpuName.TabIndex = 0;
            this.gpuName.Text = "Name: ";
            // 
            // ramGroup
            // 
            this.ramGroup.Controls.Add(this.ramTotal);
            this.ramGroup.Controls.Add(this.ramUsed);
            this.ramGroup.Location = new System.Drawing.Point(12, 199);
            this.ramGroup.Name = "ramGroup";
            this.ramGroup.Size = new System.Drawing.Size(223, 47);
            this.ramGroup.TabIndex = 5;
            this.ramGroup.TabStop = false;
            this.ramGroup.Text = "RAM";
            // 
            // ramTotal
            // 
            this.ramTotal.AutoSize = true;
            this.ramTotal.Location = new System.Drawing.Point(6, 29);
            this.ramTotal.Name = "ramTotal";
            this.ramTotal.Size = new System.Drawing.Size(34, 13);
            this.ramTotal.TabIndex = 4;
            this.ramTotal.Text = "Total:";
            // 
            // ramUsed
            // 
            this.ramUsed.AutoSize = true;
            this.ramUsed.Location = new System.Drawing.Point(6, 16);
            this.ramUsed.Name = "ramUsed";
            this.ramUsed.Size = new System.Drawing.Size(35, 13);
            this.ramUsed.TabIndex = 1;
            this.ramUsed.Text = "Used:";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(247, 255);
            this.Controls.Add(this.ramGroup);
            this.Controls.Add(this.gpuGroup);
            this.Controls.Add(this.cpuGroup);
            this.Controls.Add(this.titleText);
            this.Name = "MainForm";
            this.Text = "MaxStats";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.cpuGroup.ResumeLayout(false);
            this.cpuGroup.PerformLayout();
            this.gpuGroup.ResumeLayout(false);
            this.gpuGroup.PerformLayout();
            this.ramGroup.ResumeLayout(false);
            this.ramGroup.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Timer updatetick;
        private System.Windows.Forms.Label titleText;
        private System.Windows.Forms.GroupBox cpuGroup;
        private System.Windows.Forms.Label cpuFreq;
        private System.Windows.Forms.Label cpuName;
        private System.Windows.Forms.Label cpuTemp;
        private System.Windows.Forms.Label cpuLoad;
        private System.Windows.Forms.GroupBox gpuGroup;
        private System.Windows.Forms.Label gpuVramClock;
        private System.Windows.Forms.Label gpuLoad;
        private System.Windows.Forms.Label gpuTemp;
        private System.Windows.Forms.Label gpuCoreClock;
        private System.Windows.Forms.Label gpuName;
        private System.Windows.Forms.GroupBox ramGroup;
        private System.Windows.Forms.Label ramTotal;
        private System.Windows.Forms.Label ramUsed;
    }
}

