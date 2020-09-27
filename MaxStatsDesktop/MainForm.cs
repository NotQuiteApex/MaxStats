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
            IsMotherboardEnabled = true
        };
        private UpdateVisitor visitor = new UpdateVisitor();

        private int idx = 0;

        public MainForm()
        {
            InitializeComponent();
            comp.Open();
        }

        private void updatetick_Tick(object sender, EventArgs e)
        {
            // Update the sensors
            comp.Accept(visitor);

            // Iterate through all the hardware
            foreach (IHardware hardware in comp.Hardware)
            {
                // Let's check each of the hardwarez
                // First up is CPU, let's grab the average frequency and temp
                if (hardware.HardwareType == HardwareType.Cpu)
                {
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

                        Console.WriteLine("{0}, {1}: {2}", sensor.Name, sensor.SensorType, sensor.Value);
                    }

                    cpuName.Text = $"Name: {hardware.Name}";
                    cpuFreq.Text = $"Freq: {(_freqSum / _coreCount / 1000).ToString("n2")} GHz";
                    cpuTemp.Text = $"Temp: {_cpuTemp.ToString("n1")} ° C";
                    cpuLoad.Text = $"Load: {_cpuLoad.ToString("n1")} %";

                    cpuName.Refresh();
                    cpuFreq.Refresh();
                    cpuTemp.Refresh();
                    cpuLoad.Refresh();
                }
                else if (hardware.HardwareType == HardwareType.GpuNvidia || hardware.HardwareType == HardwareType.GpuAmd)
                {

                    decimal _gpuTemp = 0;

                    decimal _gpuCoreClock = 0;
                    decimal _gpuCoreLoad = 0;

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
                            /*else if (sensor.Name == "GPU Memory")
                                gpuMemoryLoad = (decimal)sensor.Value;*/
                        }
                        /*else if (sensor.SensorType == SensorType.SmallData)
                        {
                            if (sensor.Name == "GPU Memory Used")
                                gpuMemoryUsed = (decimal)sensor.Value;
                            else if (sensor.Name == "GPU Memory Total")
                                gpuMemoryTotal = (decimal)sensor.Value;
                        }*/
                    }

                    gpuName.Text = $"Name: {hardware.Name}";
                    gpuTemp.Text = $"Temp: {_gpuTemp.ToString("n1")} ° C";
                    gpuLoad.Text = $"Load: {_gpuCoreLoad.ToString("n1")} %";
                    gpuCoreClock.Text = $"Core Clock: {_gpuCoreClock.ToString("n1")} MHz";
                    gpuVramClock.Text = $"VRAM Clock: {_gpuVramClock.ToString("n1")} MHz";

                    gpuName.Refresh();
                    gpuTemp.Refresh();
                    gpuLoad.Refresh();
                    gpuCoreClock.Refresh();
                    gpuVramClock.Refresh();
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

                    ramUsed.Text = $"Used: {_ramUsed.ToString("n1")} GB";
                    ramTotal.Text = $"Total: {Decimal.Round(_ramUsed + _ramAvailable).ToString("n1")} GB";

                    ramUsed.Refresh();
                    ramTotal.Refresh();
                }
            }
        }

        private void MainForm_Load(object sender, EventArgs e)
        {

        }
    }

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
