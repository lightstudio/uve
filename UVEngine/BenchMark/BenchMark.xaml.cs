using System;
using System.Threading;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using System.Windows.Media.Animation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using UVEngineNative;
using System.IO;
using System.IO.IsolatedStorage;


namespace UVEngine.BenchMark
{
    public class BenchResult
    {
        public UInt64 totalcount = 0, charcount = 0, intcount = 0, floatcount = 0, diskcount = 0;

    }
    public partial class BenchMark : PhoneApplicationPage
    {
        IsolatedStorageFile isf = IsolatedStorageFile.GetUserStoreForApplication();
        IsolatedStorageSettings iss = IsolatedStorageSettings.ApplicationSettings;
        IsolatedStorageFileStream isfs;
        const string tocut = "testcommand 1,2,32,4,5,6,234,1,234,3,523,5,23,513,1,24,12,6,124,12,4,265,1,23,2,3,1254,1,2,52,3,2,1,24,5,6,2,41,23,6,232,34,23,457";
        BenchResult res = new BenchResult();
        bool BenchMarking = false;
        Thread thd;
        Storyboard Timer = new Storyboard();
        UInt16 currentitem = 0;
        Image img = new Image();
        public BenchMark()
        {
            if (iss.Contains("benchresult")) res = (BenchResult)iss["benchresult"];
            else iss.Add("benchresult", res);
            InitializeComponent();
            TotalScore.Text = res.totalcount.ToString();
            TestItem_Char.Text = res.charcount.ToString();
            TestItem_Int.Text = res.intcount.ToString();
            TestItem_Float.Text = res.floatcount.ToString();
            TestItem_Disk.Text = res.diskcount.ToString();
            GamePage.useNative = true;
            Timer.Duration = TimeSpan.FromMilliseconds(5000);
            Timer.Completed += new EventHandler(TimerCompleted);
            thd = new Thread(new ThreadStart(ThdProc));

        }
        private void TimerCompleted(object sender, EventArgs e)
        {
            thd.Abort();
            thd = new Thread(new ThreadStart(ThdProc));
            switch (currentitem)
            {
                case 0:
                    currentitem++;
                    thd.Start();
                    Timer.Begin();
                    TotalProgress.Value = 25;
                    TotalScore.Text = res.totalcount.ToString();
                    break;
                case 1:
                    currentitem++;
                    thd.Start();
                    Timer.Begin();
                    TotalProgress.Value = 50;
                    TotalScore.Text = res.totalcount.ToString();
                    break;
                case 2:
                    currentitem++;
                    if (isf.AvailableFreeSpace / 1048576 < 300)
                    {
                        MessageBox.Show(UVEngine.Resources.UVEngine.notenoughspacefortest);
                        currentitem++;
                        TotalScore.Text = res.totalcount.ToString();
                        TotalProgress.Value = 100;
                        TotalScore.Text = res.totalcount.ToString();
                        St.Content = UVEngine.Resources.UVEngine.starttest;
                        Now.Text = UVEngine.Resources.UVEngine.testfinished;
                        BenchMarking = false;
                        iss["benchresult"] = res;
                        iss.Save();
                    }
                    else
                    {
                        thd.Start();
                        Timer.Begin();
                        TotalProgress.Value = 75;
                        TotalScore.Text = res.totalcount.ToString();
                    }
                    break;
                case 3:
                    currentitem++;
                    TotalScore.Text = res.totalcount.ToString();
                    isfs.Position = 0;

                    TotalProgress.Value = 100;
                    TotalScore.Text = res.totalcount.ToString();
                    St.Content = UVEngine.Resources.UVEngine.starttest;
                    Now.Text = UVEngine.Resources.UVEngine.testfinished;
                    BenchMarking = false;
                    iss["benchresult"] = res;
                    iss.Save();
                    break;
            }

        }
        private void ThdProc()
        {
            switch (currentitem)
            {
                case 0:
                    this.Dispatcher.BeginInvoke(() => Now.Text = UVEngine.Resources.UVEngine.str);
                    while (true)
                    {
                        string[] str;
                        for (int i = 0; i < 100; i++)
                        {
                            StringToolkit.CutParam(tocut, ',', out str);
                            //str=stn.CutParam(tocut, ',');
                        }
                        this.Dispatcher.BeginInvoke(() =>
                        {
                            res.charcount++;
                            res.totalcount++;
                            TestItem_Char.Text = res.charcount.ToString();
                            CurrentProgress.Value = Timer.GetCurrentTime().TotalSeconds / 5.0f * 100;
                        });
                    }
                case 1:
                    this.Dispatcher.BeginInvoke(() => Now.Text = UVEngine.Resources.UVEngine.integer);
                    while (true)
                    {
                        for (int i = 0; i < 10000; i++)
                        {
                            int temp1 = int.Parse("10235");
                            int temp2 = int.Parse("-2903");
                            Int64 temp3 = Int64.Parse("-130140");
                            UInt64 temp4 = UInt64.Parse("239951");
                        }
                        this.Dispatcher.BeginInvoke(() =>
                        {
                            res.intcount++;
                            res.totalcount++;
                            TestItem_Int.Text = res.intcount.ToString();
                            CurrentProgress.Value = Timer.GetCurrentTime().TotalSeconds / 5.0f * 100;
                        });
                    }
                case 2:
                    this.Dispatcher.BeginInvoke(() => Now.Text = UVEngine.Resources.UVEngine.flo);
                    while (true)
                    {
                        for (int i = 0; i < 10000; i++)
                        {
                            float temp1 = float.Parse("102.35");
                            float temp2 = float.Parse("-29.03");
                            Double temp3 = Double.Parse("-1301.40");
                            Double temp4 = Double.Parse("239.951");
                        }
                        this.Dispatcher.BeginInvoke(() =>
                        {
                            res.floatcount++;
                            res.totalcount++;
                            TestItem_Float.Text = res.floatcount.ToString();
                            CurrentProgress.Value = Timer.GetCurrentTime().TotalSeconds / 5.0f * 100;
                        });
                    }
                case 3:
                    this.Dispatcher.BeginInvoke(() => Now.Text = UVEngine.Resources.UVEngine.disk);
                    isfs = new IsolatedStorageFileStream("bench", FileMode.Create, isf);
                    while (true)
                    {
                        for (int i = 0; i < 1048576; i++)
                        {
                            isfs.WriteByte(0);
                        }
                        this.Dispatcher.BeginInvoke(() =>
                        {
                            res.diskcount++;
                            res.totalcount++;
                            TestItem_Disk.Text = res.diskcount.ToString();
                            CurrentProgress.Value = Timer.GetCurrentTime().TotalSeconds / 5.0f * 100;
                        });
                    }
            }
        }
        private void Start(object sender, RoutedEventArgs e)
        {
            if (!BenchMarking)
            {
                TotalScore.Text = "0";
                TestItem_Char.Text = "0";
                TestItem_Disk.Text = "0";
                TestItem_Float.Text = "0";
                TestItem_Int.Text = "0";
                res.totalcount = 0;
                res.intcount = 0;
                res.charcount = 0;
                res.diskcount = 0;
                res.floatcount = 0;
                TotalProgress.Value = 0;
                CurrentProgress.Value = 0;
                currentitem = 0;
                thd = new Thread(new ThreadStart(ThdProc));
                thd.Start();
                Timer.Begin();
                BenchMarking = true;
                St.Content = UVEngine.Resources.UVEngine.stoptest;
            }
            else
            {
                thd.Abort();
                Thread.Sleep(100);
                TotalProgress.Value = 0;
                CurrentProgress.Value = 0;
                if (isf.FileExists("bench"))
                {
                    isfs.Close();
                    isf.DeleteFile("bench");
                }
                BenchMarking = false;
                Timer.Stop();
                St.Content = UVEngine.Resources.UVEngine.starttest;
            }
        }
        protected override void OnBackKeyPress(System.ComponentModel.CancelEventArgs e)
        {
            base.OnBackKeyPress(e);
            if (BenchMarking && MessageBox.Show(UVEngine.Resources.UVEngine.perftestexittip, UVEngine.Resources.UVEngine.warning, MessageBoxButton.OKCancel) != MessageBoxResult.OK)
            {
                e.Cancel = true;
            }
            else
            {
                thd.Abort();
                try
                {
                    isfs.Close();
                    isf.DeleteFile("bench");
                }
                catch
                {

                }
            }
            
        }
    }
}