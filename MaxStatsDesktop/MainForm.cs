using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Management;
using System.Text.RegularExpressions;
using LibreHardwareMonitor.Hardware;
using System.Threading;

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

        // For closing via systray
        private bool closing = false;

        // All the stats we send via serial.
        private string cpuName = "Unknown CPU";
        private string cpuFreq = "0";
        private string cpuTemp = "0";
        private string cpuLoad = "0";

        private string gpuName      = "Unknown GPU";
        private string gpuTemp      = "0";
        private string gpuCoreLoad  = "0";
        private string gpuCoreClock = "0";
        private string gpuVramLoad  = "0";
        private string gpuVramClock = "0";

        private string ramUsed  = "0";
        private string ramTotal = "0";

        // Serial comms variables.
        private bool _continueComms = true;
        private Mutex compMutex = new Mutex();
        private SerialPort serial = new SerialPort();
        private Thread comms;

        public MainForm()
        {
            InitializeComponent();
            comp.Open();

            // Run one update to populate stat values.
            UpdateComp();

            // Set up serial comms.
            comms = new Thread(SerialComms);
            serial.BaudRate = 9600; // Serial.begin(9600, SERIAL_8E2);
            serial.DataBits = 8;
            serial.Parity = Parity.Even;
            serial.StopBits = StopBits.Two;
            serial.Handshake = Handshake.None;

            comms.Start();
        }

        private void UpdateComp()
        {
            // Wait turn for mutex
            compMutex.WaitOne();

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

            // Unlock mutex, let the other thread use it now.
            compMutex.ReleaseMutex();
        }

        private void SerialComms()
        {
            while (_continueComms)
            {
                try
                {
                    // If serial is started, don't try to use it.
                    if (!serial.IsOpen) continue;

                    // TODO: send ping to the device, then check for the response
                    // TODO: send the cpuName, gpuName, and ramTotal, wait for ack
                    // TODO: send the other data on loop every so often (1 second)
                    // Wait for mutex to unlock.
                    //compMutex.WaitOne();

                    // We're done here, stop blocking the other thread.
                    //compMutex.ReleaseMutex();
                }
                catch (TimeoutException) { }
            }
        }

        private void updatetick_Tick(object sender, EventArgs e)
        {
            // Update Computer object, every so often.
            UpdateComp();
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
            // Break down serial comms
            _continueComms = false;
            comms.Join();
            if (serial.IsOpen) serial.Close();

            // Close the program
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
            // Program won't close unless closed from the systray
            if (e.CloseReason == CloseReason.UserClosing && !closing)
            {
                this.Hide();
                trayIcon.Visible = true;
                e.Cancel = true;
            }
        }

        private void trayMenu_Opening(object sender, CancelEventArgs e)
        {
            // Manage items by changes in the list.
            List<string> coms = new List<string>();

            ManagementObjectSearcher searcher = new ManagementObjectSearcher("root\\CIMV2", "SELECT * FROM Win32_PnPEntity WHERE ClassGuid='{4d36e978-e325-11ce-bfc1-08002be10318}'");
            foreach (ManagementObject m in searcher.Get())
                coms.Add(m["Name"].ToString());

            // If any of the items no longer exist (COM's disconnected) or the options are satic, remove them from the list.
            for (int i = trayMenu.Items.Count-1; i >= 0; i--)
            {
                ToolStripItem s = trayMenu.Items[i];
                if ((string)s.Tag != "important" && coms.IndexOf(s.Text) == -1)
                    trayMenu.Items.Remove(s);
            }

            // Go through each COM, check if it exists in the list, and if not add it.
            foreach (string s in coms)
            {
                var item = new ToolStripMenuItem();
                item.Text = s;
                bool shouldadd = true;
                for (int i = trayMenu.Items.Count - 1; i >= 0; i--)
                {
                    ToolStripItem t = trayMenu.Items[i];
                    if (t.Text == s)
                        shouldadd = false;
                }
                if (shouldadd)
                    trayMenu.Items.Insert(1, item);
            }
        }

        private void trayMenu_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            string comchosen = e.ClickedItem.Text;
            Match match = Regex.Match(comchosen, @"[a-zA-Z\ ]+\((COM[0-9]+)\)", RegexOptions.IgnoreCase);

            if (!match.Success)
            {
                Console.WriteLine("Not Found");
                return;
            }

            // Disable the rest of the checkmarks on the items.
            foreach(ToolStripItem t in trayMenu.Items) (t as ToolStripMenuItem).Checked = (t as ToolStripMenuItem).Checked && true;
            // Change the selected item's checkmark
            (e.ClickedItem as ToolStripMenuItem).Checked = !(e.ClickedItem as ToolStripMenuItem).Checked;

            // Time to connect with serial!
            serial.PortName = match.Groups[1].Value;
            // TODO
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
