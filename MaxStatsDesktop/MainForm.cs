using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using LibreHardwareMonitor.Hardware;

namespace MaxStatsDesktop
{
    public partial class MainForm : Form
    {
        private Computer comp = new Computer
        {
            IsCpuEnabled = true,
            IsGpuEnabled = true,
            IsMemoryEnabled = true,
        };
        private UpdateVisitor visitor = new UpdateVisitor();

        private Boolean closing = false;

        private String cpuName = "Unknown CPU";
        private String cpuFreq = "0";
        private String cpuTemp = "0";
        private String cpuLoad = "0";

        private String gpuName      = "Unknown GPU";
        private String gpuTemp      = "0";
        private String gpuCoreLoad  = "0";
        private String gpuCoreClock = "0";
        private String gpuVramLoad  = "0";
        private String gpuVramClock = "0";

        private String ramUsed  = "0";
        private String ramTotal = "0";

        public MainForm()
        {
            InitializeComponent();
            comp.Open();

            // Run one update to populate stat values.
            UpdateComp();
        }

        private void updatetick_Tick(object sender, EventArgs e)
        {
            // Update Computer object, every so often.
            UpdateComp();
        }

        private void UpdateComp()
        {
            // Update the sensors
            comp.Accept(visitor);

            // Iterate through all the hardware
            foreach (IHardware hardware in comp.Hardware)
            {
                // Let's check each of the hardwarez
                if (hardware.HardwareType == HardwareType.Cpu)
                {
                    // First up is CPU, let's grab the average frequency, temperature, and load.

                    decimal _freqSum = 0;
                    decimal _cpuTemp = 0;
                    decimal _cpuLoad = 0;
                    int _coreCount = 0;

                    foreach (ISensor sensor in hardware.Sensors)
                    {
                        if (sensor.SensorType == SensorType.Clock && sensor.Name != "Bus Speed")
                        {
                            _coreCount++;
                            _freqSum += (decimal)sensor.Value;
                        }
                        else if (sensor.SensorType == SensorType.Temperature && sensor.Name == "Core (Tctl)")
                        {
                            _cpuTemp = (decimal)sensor.Value;
                        }
                        else if (sensor.SensorType == SensorType.Load && sensor.Name == "CPU Total")
                        {
                            _cpuLoad = (decimal)sensor.Value;
                        }

                        //Console.WriteLine("{0}, {1}: {2}", sensor.Name, sensor.SensorType, sensor.Value);
                    }

                    cpuName = hardware.Name;
                    cpuFreq = (_freqSum / _coreCount / 1000).ToString("n2");
                    cpuTemp = _cpuTemp.ToString("n1");
                    cpuLoad = _cpuLoad.ToString("n1");

                    if (this.Visible)
                    {
                        // Dump the values to the labels as needed. Only done if the form is open.
                        labelCpuName.Text = $"Name: {cpuName}";
                        labelCpuFreq.Text = $"Freq: {cpuFreq} GHz";
                        labelCpuTemp.Text = $"Temp: {cpuTemp} ° C";
                        labelCpuLoad.Text = $"Load: {cpuLoad} %";
                        // Show on screen, again as needed.
                        labelCpuName.Refresh();
                        labelCpuFreq.Refresh();
                        labelCpuTemp.Refresh();
                        labelCpuLoad.Refresh();
                    }
                }
                else if (hardware.HardwareType == HardwareType.GpuNvidia || hardware.HardwareType == HardwareType.GpuAmd)
                {
                    // Now for gpu. We grab the clock speed of the core and ram, as well as their load

                    decimal _gpuTemp = 0;

                    decimal _gpuCoreLoad = 0;
                    decimal _gpuCoreClock = 0;

                    decimal _gpuVramLoad = 0;
                    decimal _gpuVramClock = 0;

                    foreach (ISensor sensor in hardware.Sensors)
                    {
                        if (sensor.SensorType == SensorType.Clock)
                        {
                            if (sensor.Name == "GPU Core")
                                _gpuCoreClock = (decimal)sensor.Value;
                            else if (sensor.Name == "GPU Memory")
                                _gpuVramClock = (decimal)sensor.Value;
                            /*else if (sensor.Name == "GPU Shader")
                                gpuShaderClock = (decimal)sensor.Value;*/
                        }
                        else if (sensor.SensorType == SensorType.Temperature && sensor.Name == "GPU Core")
                        {
                            _gpuTemp = (decimal)sensor.Value;
                        }
                        else if (sensor.SensorType == SensorType.Load)
                        {
                            if (sensor.Name == "GPU Core")
                                _gpuCoreLoad = (decimal)sensor.Value;
                            else if (sensor.Name == "GPU Memory")
                                _gpuVramLoad = (decimal)sensor.Value;
                        }
                        /*else if (sensor.SensorType == SensorType.SmallData)
                        {
                            if (sensor.Name == "GPU Memory Used")
                                gpuMemoryUsed = (decimal)sensor.Value;
                            else if (sensor.Name == "GPU Memory Total")
                                gpuMemoryTotal = (decimal)sensor.Value;
                        }*/
                    }

                    gpuName = hardware.Name;
                    gpuTemp = _gpuTemp.ToString("n1");
                    gpuCoreLoad = _gpuCoreLoad.ToString("n1");
                    gpuCoreClock = _gpuCoreClock.ToString("n1");
                    gpuVramLoad = _gpuVramLoad.ToString("n1");
                    gpuVramClock = _gpuVramClock.ToString("n1");

                    if (this.Visible)
                    {
                        // Same here with the GPU, dump values and show them
                        labelGpuName.Text = $"Name: {gpuName}";
                        labelGpuTemp.Text = $"Temp: {gpuTemp} ° C";
                        labelGpuCoreLoad.Text = $"Core Load: {gpuCoreLoad} %";
                        labelGpuCoreClock.Text = $"Core Clock: {gpuCoreClock} MHz";
                        labelGpuVramLoad.Text = $"VRAM Load: {gpuVramLoad} % used";
                        labelGpuVramClock.Text = $"VRAM Clock: {gpuVramClock} MHz";

                        labelGpuName.Refresh();
                        labelGpuTemp.Refresh();
                        labelGpuCoreLoad.Refresh();
                        labelGpuCoreClock.Refresh();
                        labelGpuVramClock.Refresh();
                    }
                }
                else if (hardware.HardwareType == HardwareType.Memory)
                {
                    decimal _ramUsed = 0;
                    decimal _ramAvailable = 0;

                    foreach (ISensor sensor in hardware.Sensors)
                    {
                        if (sensor.SensorType == SensorType.Data)
                        {
                            if (sensor.Name == "Memory Used")
                                _ramUsed = (decimal)sensor.Value;
                            else if (sensor.Name == "Memory Available")
                                _ramAvailable = (decimal)sensor.Value;
                        }
                    }

                    ramUsed = _ramUsed.ToString("n1");
                    ramTotal = Decimal.Round(_ramUsed + _ramAvailable).ToString("n0");

                    if (this.Visible)
                    {
                        // Lastly do the same with RAM
                        labelRamUsed.Text = $"Used: {ramUsed} GB";
                        labelRamTotal.Text = $"Total: {ramTotal} GB";

                        labelRamUsed.Refresh();
                        labelRamTotal.Refresh();
                    }
                }
            }
        }

        private void MainForm_Resize(object sender, EventArgs e)
        {
            if (this.WindowState == FormWindowState.Minimized)
            {
                this.Hide();
                trayIcon.Visible = true;
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            closing = true;
            this.Close();
        }

        private void showToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Show();
            trayIcon.Visible = false;
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing && !closing)
            {
                this.Hide();
                trayIcon.Visible = true;
                e.Cancel = true;
            }
        }

        private void trayMenu_Opening(object sender, CancelEventArgs e)
        {

        }
    }

    // Visitor, for grabbing/updating the values of all the hardware we check.
    public class UpdateVisitor : IVisitor
    {
        public void VisitComputer(IComputer computer)
        {
            computer.Traverse(this);
        }
        public void VisitHardware(IHardware hardware)
        {
            hardware.Update();
            foreach (IHardware subHardware in hardware.SubHardware) subHardware.Accept(this);
        }
        public void VisitSensor(ISensor sensor) { }
        public void VisitParameter(IParameter parameter) { }
    }
}
