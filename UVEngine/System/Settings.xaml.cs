using Microsoft.Phone.Controls;
using Microsoft.Phone.Notification;
using Microsoft.Phone.Tasks;
using Microsoft.Phone.Storage;
using OpenXLive;
using OpenXLive.Silverlight.Controls;
using System;
using System.IO.IsolatedStorage;
using System.Text;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Navigation;
using System.IO;
using ICSharpCode.SharpZipLib.Zip;
using Windows.Phone.System.Memory;

namespace UVEngine
{
    public partial class SettingsPage : PhoneApplicationPage
    {
        IsolatedStorageSettings appSettings = IsolatedStorageSettings.ApplicationSettings;
        string ToastChannelName = "OpenXLivePushNotificationHostingChannel";
        public SettingsPage()
        {
            InitializeComponent();
            if ((int)appSettings["pushenabled"] == 1) PushSwitch.IsChecked = true;
            else PushSwitch.IsChecked = false;
            externalVideo.IsChecked = UVEngineNative.ONScripterSettings.playExternVideo;
            logOutput.IsChecked = UVEngineNative.ONScripterSettings.logOutput;
            passwdbox.Password = UVEngineNative.UVEngineSettings.Passwd;
            quickshare.IsChecked = UVEngineNative.UVEngineSettings.QuickShare;
            passwd.IsChecked = UVEngineNative.UVEngineSettings.ReqPasswd;
            setOpacity.IsChecked = UVEngineNative.UVEngineSettings.translucent;
            showDonate.IsChecked = !UVEngineNative.UVEngineSettings.NoMoreDisplay;
            if (!UVEngineNative.UVEngineSettings.Donated) showDonate.IsEnabled = false;
            if (UVEngineNative.UVEngineSettings.ReqPasswd)
            {
                passwdbox.Visibility = Visibility.Visible; save.Visibility = Visibility.Visible; 
            }
            else
            {
                passwdbox.Visibility = Visibility.Collapsed; save.Visibility = Visibility.Collapsed;
            }
            switch (UVEngineNative.UVEngineSettings.language)
            {
                case "":
                    Auto.IsChecked = true;
                    break;
                case "zh-CN":
                    CHS.IsChecked = true;
                    break;
                case "en-US":
                    ENG.IsChecked = true;
                    break;
            }
            switch (UVEngineNative.UVEngineSettings.ConfirmExit)
            {
                case 0:
                    directExit.IsChecked = true;
                    break;
                case 1:
                    presstwotimes.IsChecked = true;
                    break;
                case 2:
                    messageboxconfirm.IsChecked = true;
                    break;
            }
            //if ((int)appSettings["onsTrans"] == 1) onsTrans.IsChecked = true;
            //else onsTrans.IsChecked = false;
            //if ((int)appSettings["bgmOut"] == 1) bgmOut.IsChecked = true;
            //else bgmOut.IsChecked = false;
            //if ((int)appSettings["mp3Out"] == 1) mp3Out.IsChecked = true;
            //else mp3Out.IsChecked = false;
            //if ((int)appSettings["dwaveOut"] == 1) dwaveOut.IsChecked = true;
            //else dwaveOut.IsChecked = false;
            //if ((int)appSettings["audioBackground"] == 1) audioBackground.IsChecked = true;
            //else audioBackground.IsChecked = false;
            //if ((int)appSettings["videoPlay"] == 1) videoPlay.IsChecked = true;
            //else videoPlay.IsChecked = false;
            //if ((int)appSettings["textEffect"] == 1) textEffect.IsChecked = true;
            //else textEffect.IsChecked = false;
            //if ((int)appSettings["useNative"] == 1) useNative.IsChecked = true;
            //else useNative.IsChecked = false;
            //if ((int)appSettings["allowError"] == 1) allowError.IsChecked = true;
            //else allowError.IsChecked = false;
            if ((int)appSettings["showDebug"] == 0) showDebug.IsChecked = true;
            else showDebug.IsChecked = false;
            this.Dispatcher.BeginInvoke(() =>
                {
                    rsp.Text += (storage.AvailableFreeSpace / 1048576).ToString() + " MB";
                    sp.Text += (GetDirectoryLength("") / 1048576).ToString() + " MB";
                });
            //if (!storage.FileExists("temp.zip")) extract.IsEnabled = false;
            switch ((int)appSettings["extstorage"])
            {
                case -1:
                    extstorage.IsEnabled = false;
                    extstorage.IsChecked = false;
                    SDIntroduction.Text = UVEngine.Resources.UVEngine.nosd;
                    Exists.Text = "";
                    break;
                case 0:
                    extstorage.IsEnabled = true;
                    extstorage.IsChecked = false;
                    break;
                case 1:
                    extstorage.IsEnabled = true;
                    extstorage.IsChecked = true;
                    break;
            }
            AvailableRAM.Text = (MemoryManager.ProcessCommittedLimit / 1048576).ToString() + " MB";
            NowUsingRAM.Text = (MemoryManager.ProcessCommittedBytes / 1048576).ToString() + " MB";
            TotalRAM.Text = (Microsoft.Phone.Info.DeviceStatus.DeviceTotalMemory / 1048576).ToString() + " MB";
            MaximumUsage.Text = (Microsoft.Phone.Info.DeviceStatus.ApplicationPeakMemoryUsage / 1048676).ToString() + " MB";
            logOutput.Checked += ONScripterSettings_Checked;
            logOutput.Unchecked += ONScripterSettings_Unchecked;
            externalVideo.Checked += ONScripterSettings_Checked;
            externalVideo.Unchecked += ONScripterSettings_Unchecked;
            passwd.Checked += delegate { UVEngineNative.UVEngineSettings.ReqPasswd = true; passwdbox.Visibility = Visibility.Visible; save.Visibility = Visibility.Visible; };
            passwd.Unchecked += delegate { UVEngineNative.UVEngineSettings.ReqPasswd = false; passwdbox.Visibility = Visibility.Collapsed; save.Visibility = Visibility.Collapsed; };
            quickshare.Checked += delegate { UVEngineNative.UVEngineSettings.QuickShare = true; };
            quickshare.Unchecked += delegate { UVEngineNative.UVEngineSettings.QuickShare = false; };
            setOpacity.Checked += delegate { UVEngineNative.UVEngineSettings.translucent = true; };
            setOpacity.Unchecked += delegate { UVEngineNative.UVEngineSettings.translucent = false; };
            showDonate.Checked += delegate { UVEngineNative.UVEngineSettings.NoMoreDisplay = false; };
            showDonate.Unchecked += delegate { UVEngineNative.UVEngineSettings.NoMoreDisplay = true; };
            Auto.Checked += RadioButton_Checked_1;
            CHS.Checked += RadioButton_Checked_2;
            ENG.Checked += RadioButton_Checked_3;
            directExit.Checked += directExit_Checked;
            presstwotimes.Checked += presstwotimes_Checked;
            messageboxconfirm.Checked += messageboxconfirm_Checked;
        }

        void messageboxconfirm_Checked(object sender, RoutedEventArgs e)
        {
            UVEngineNative.UVEngineSettings.ConfirmExit = 2;
        }

        void presstwotimes_Checked(object sender, RoutedEventArgs e)
        {
            UVEngineNative.UVEngineSettings.ConfirmExit = 1;
        }

        void directExit_Checked(object sender, RoutedEventArgs e)
        {
            UVEngineNative.UVEngineSettings.ConfirmExit = 0;
        }
        private async System.Threading.Tasks.Task ShowList()
        {
            ExternalStorageDevice device = (await ExternalStorage.GetExternalStorageDevicesAsync()).FirstOrDefault();
            ExternalStorageFolder folder = await device.RootFolder.GetFolderAsync("ADV");
            ExternalStorageFolder[] gamefolders = (await folder.GetFoldersAsync()).ToArray<ExternalStorageFolder>();
            foreach (ExternalStorageFolder f in gamefolders)
            {
                ExternalStorageFile[] files = (await f.GetFilesAsync()).ToArray<ExternalStorageFile>();
                int count = 0;
                foreach (ExternalStorageFile ef in files)
                {
                    if (ef.Name == "nscript.dat" || ef.Name == "nscript.___" || ef.Name == "nscr.dat") count++;
                }
                if (count >= 1)
                    List.Text += f.Name + '\n';
            }
        }
        private void stChecked(object sender, RoutedEventArgs e)
        {
            appSettings["extstorage"] = 1;
            appSettings.Save();
            SDIntroduction.Text = UVEngine.Resources.UVEngine.extenabled;
            Exists.Text = UVEngine.Resources.UVEngine.detectedfolders;
            ShowList();
        }
        private void stUnChecked(object sender, RoutedEventArgs e)
        {
            appSettings["extstorage"] = 0;
            appSettings.Save();
            SDIntroduction.Text = UVEngine.Resources.UVEngine.sdnotenabled;
            Exists.Text = "";
            List.Text = "";
        }
        private void Checked(object sender, RoutedEventArgs e)
        {
            string sendername = (sender as CheckBox).Name;
            appSettings[sendername] = 1;
            appSettings.Save();
        }
        private void UnChecked(object sender, RoutedEventArgs e)
        {
            string sendername = (sender as CheckBox).Name;
            appSettings[sendername] = 0;
            appSettings.Save();

        }
        private void Debug_Checked(object sender, RoutedEventArgs e)
        {
            appSettings["showDebug"] = 0;
            appSettings.Save();
            MemoryDiagnosticsHelper.Stop();
            Application.Current.Host.Settings.EnableFrameRateCounter = false;
        }
        private void Debug_UnChecked(object sender, RoutedEventArgs e)
        {
            appSettings["showDebug"] = 1;
            appSettings.Save();
            MemoryDiagnosticsHelper.Start(TimeSpan.FromMilliseconds(1000), true);
            Application.Current.Host.Settings.EnableFrameRateCounter = true;
        }
        private void Push_Checked(object sender, RoutedEventArgs e)
        {
            HttpNotificationChannel pushChannel = HttpNotificationChannel.Find(ToastChannelName);

            if ((int)appSettings["pushenabled"] == 0)
            {
                ToastPrompt toast = new ToastPrompt(); //实例化 
                toast.Height = 100;
                toast.Title = UVEngine.Resources.UVEngine.tip; //设置标题 
                toast.Message = UVEngine.Resources.UVEngine.pushhasstarted; //设置正文消息 
                toast.FontSize = 20; //设置文本大小(可选) 
                toast.TextOrientation = System.Windows.Controls.Orientation.Vertical; //设置呈现为纵向 

                toast.Show();
                appSettings["pushenabled"] = 1;
                appSettings.Save();
            }
            // If the channel was not found, then create a new connection to the push service. 
            if (pushChannel == null)
            {
                pushChannel = new HttpNotificationChannel(ToastChannelName);

                // Register for all the events before attempting to open the channel.
                pushChannel.ChannelUriUpdated += new EventHandler<NotificationChannelUriEventArgs>(PushChannel_ChannelUriUpdated);
                pushChannel.ErrorOccurred += new EventHandler<NotificationChannelErrorEventArgs>(PushChannel_ErrorOccurred);

                // Register for this notification only if you need to receive the notifications while your application is running.
                pushChannel.ShellToastNotificationReceived += new EventHandler<NotificationEventArgs>(PushChannel_ShellToastNotificationReceived);

                pushChannel.Open();

                // Bind this new channel for Tile events.
                pushChannel.BindToShellToast();
            }
            else
            {
                // The channel was already open, so just register for all the events.
                pushChannel.ChannelUriUpdated += new EventHandler<NotificationChannelUriEventArgs>(PushChannel_ChannelUriUpdated);
                pushChannel.ErrorOccurred += new EventHandler<NotificationChannelErrorEventArgs>(PushChannel_ErrorOccurred);

                // Register for this notification only if you need to receive the notifications while your application is running.
                pushChannel.ShellToastNotificationReceived += new EventHandler<NotificationEventArgs>(PushChannel_ShellToastNotificationReceived);
            }

            // Send Channel Uri to OpenXLive Hosting Server
            RegisterNotificationUri(pushChannel.ChannelUri);


        }
        private void Push_Unchecked(object sender, RoutedEventArgs e)
        {
            HttpNotificationChannel pushChannel = HttpNotificationChannel.Find(ToastChannelName);
            appSettings["pushenabled"] = 0;
            appSettings.Save();
            if (pushChannel != null)
            {
                pushChannel.UnbindToShellToast();
                pushChannel.Close();
            }
        }
        void PushChannel_ErrorOccurred(object sender, NotificationChannelErrorEventArgs e)
        {


        }
        void PushChannel_ChannelUriUpdated(object sender, NotificationChannelUriEventArgs e)
        {
            if (e.ChannelUri != null)
            {
                RegisterNotificationUri(e.ChannelUri);
            }

            //            Dispatcher.BeginInvoke(() =>
            //            {
            //                // Display the new URI for testing purposes. Normally, the URI would be passed back to your web service at this point.
            ////                System.Diagnostics.Debug.WriteLine(e.ChannelUri.ToString());
            //            });
        }
        private void RegisterNotificationUri(Uri uri)
        {
            // Send Channel Uri to OpenXLive Hosting Server 
            if (OpenXLive.XLiveGameManager.CurrentSession != null
                && OpenXLive.XLiveGameManager.CurrentSession.IsValid
                && uri != null)
            {
                OpenXLive.XLiveGameManager.CurrentSession.RegistNotificationUriCompleted += new OpenXLive.Features.AsyncEventHandler(CurrentSession_RegistNotificationUriCompleted);
                OpenXLive.XLiveGameManager.CurrentSession.RegisterNotificationUri(uri);
            }
        }
        void CurrentSession_RegistNotificationUriCompleted(object sender, OpenXLive.AsyncEventArgs e)
        {
            OpenXLive.Features.AsyncProcessResult result = e.Result;
            //            if (result.ReturnValue)
            //            {
            ////                System.Diagnostics.Debug.WriteLine("Channel Uri has been send to OpenXLive Hosting Server");
            //            }
            //            else
            //            {
            ////                System.Diagnostics.Debug.WriteLine(result.ErrorMessage);
            //            }
        }
        void PushChannel_ShellToastNotificationReceived(object sender, NotificationEventArgs e)
        {
            StringBuilder message = new StringBuilder();
            string relativeUri = string.Empty;


            // Parse out the information that was part of the message. 
            foreach (string key in e.Collection.Keys)
            {
                message.AppendFormat("{1} ", key, e.Collection[key]);

                if (string.Compare(
                    key,
                    "wp:Param",
                    System.Globalization.CultureInfo.InvariantCulture,
                    System.Globalization.CompareOptions.IgnoreCase) == 0)
                {
                    relativeUri = e.Collection[key];
                }
            }
            this.Dispatcher.BeginInvoke(() =>
            {
                ToastPrompt toast = new ToastPrompt(); //实例化 
                toast.Height = 100;
                toast.Title = UVEngine.Resources.UVEngine.message; //设置标题 
                toast.Message = message.ToString(); //设置正文消息 
                toast.FontSize = 20; //设置文本大小(可选) 
                toast.TextOrientation = System.Windows.Controls.Orientation.Vertical; //设置呈现为纵向 
                toast.Show();
            });

            // Display a dialog of all the fields in the toast.
            //            Dispatcher.BeginInvoke(() => MessageBox.Show(message.ToString()));
        }
        void RegisterPush()
        {
            // Try to find the push channel.
            HttpNotificationChannel pushChannel = HttpNotificationChannel.Find(ToastChannelName);

            // If the channel was not found, then create a new connection to the push service. 
            if (pushChannel == null)
            {
                pushChannel = new HttpNotificationChannel(ToastChannelName);

                // Register for all the events before attempting to open the channel.
                pushChannel.ChannelUriUpdated += new EventHandler<NotificationChannelUriEventArgs>(PushChannel_ChannelUriUpdated);
                pushChannel.ErrorOccurred += new EventHandler<NotificationChannelErrorEventArgs>(PushChannel_ErrorOccurred);

                // Register for this notification only if you need to receive the notifications while your application is running.
                pushChannel.ShellToastNotificationReceived += new EventHandler<NotificationEventArgs>(PushChannel_ShellToastNotificationReceived);

                pushChannel.Open();

                // Bind this new channel for Tile events.
                pushChannel.BindToShellToast();
            }
            else
            {
                // The channel was already open, so just register for all the events.
                pushChannel.ChannelUriUpdated += new EventHandler<NotificationChannelUriEventArgs>(PushChannel_ChannelUriUpdated);
                pushChannel.ErrorOccurred += new EventHandler<NotificationChannelErrorEventArgs>(PushChannel_ErrorOccurred);

                // Register for this notification only if you need to receive the notifications while your application is running.
                pushChannel.ShellToastNotificationReceived += new EventHandler<NotificationEventArgs>(PushChannel_ShellToastNotificationReceived);
            }

            // Send Channel Uri to OpenXLive Hosting Server
            RegisterNotificationUri(pushChannel.ChannelUri);



        }
        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            if (MessageBox.Show(UVEngine.Resources.UVEngine.clearwarning, UVEngine.Resources.UVEngine.warning, MessageBoxButton.OKCancel) == MessageBoxResult.OK
                && MessageBox.Show(UVEngine.Resources.UVEngine.clearconfirm, UVEngine.Resources.UVEngine.confirmagain, MessageBoxButton.OKCancel) == MessageBoxResult.OK)
            {
                MessageBox.Show(UVEngine.Resources.UVEngine.willremove);
                IsolatedStorageFile.GetUserStoreForApplication().Remove();
                Application.Current.Terminate();
            }
        }
        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            MessageBox.Show(UVEngine.Resources.UVEngine.willrestart);
            IsolatedStorageSettings.ApplicationSettings.Clear();
            IsolatedStorageSettings.ApplicationSettings.Save();
            storage.DeleteFile("nsconfig.ini");
            storage.DeleteFile("uveconfig.ini");
            Application.Current.Terminate();
        }
        IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication();
        //public bool DeCompress(string sourceFilePath, string destinationPath)
        //{
        //    try
        //    {
        //        Stream stream = storage.OpenFile(sourceFilePath, System.IO.FileMode.Open);
        //        while (stream.Position != stream.Length)
        //        {
        //            if (stream.ReadByte() == 0x50)
        //                if (stream.ReadByte() == 0x4b)
        //                    if (stream.ReadByte() == 0x03)
        //                        if (stream.ReadByte() == 0x04)
        //                            break;
        //        }
        //        if (stream.Position == stream.Length) return false;
        //        stream.Position = stream.Position - 4;
        //        using (ZipInputStream zs = new ZipInputStream(stream))
        //        {
        //            ZipEntry entry = null;
        //            //解压缩*.rar文件运行至此处出错：Wrong Local header signature: 0x21726152，解压*.zip文件不出错 
        //            while ((entry = zs.GetNextEntry()) != null)
        //            {
        //                string directoryName = Path.GetDirectoryName(entry.Name);
        //                string fileName = Path.GetFileName(entry.Name);

        //                if (!string.IsNullOrEmpty(fileName))
        //                {
        //                    storage.CreateDirectory(Path.Combine(destinationPath, directoryName));
        //                    using (IsolatedStorageFileStream streamWriter = storage.CreateFile(Path.Combine(destinationPath, directoryName, fileName)))
        //                    {
        //                        int size = 2048;
        //                        byte[] data = new byte[size];
        //                        while (true)
        //                        {
        //                            size = zs.Read(data, 0, data.Length);
        //                            if (size > 0)
        //                            {
        //                                streamWriter.Write(data, 0, size);
        //                            }
        //                            else
        //                            {
        //                                break;
        //                            }
        //                        }
        //                    }
        //                }
        //            }
        //        }
        //        stream.Close();
        //    }
        //    catch (System.Exception Ex)
        //    {
        //        this.Dispatcher.BeginInvoke(() => { MessageBox.Show(Ex.Message, UVEngine.Resources.UVEngine.error, MessageBoxButton.OK); });
        //        }
        //    return true;
        //}
        long GetDirectoryLength(string dirPath)
        {
            if (!storage.DirectoryExists(dirPath))
                return 0;
            long len = 0;
            var fi = storage.GetFileNames(dirPath + "\\*");
            var dis = storage.GetDirectoryNames(dirPath + "\\*");
            foreach (string file in fi)
            {
                try
                {
                    var f = storage.OpenFile(dirPath + "\\" + file, FileMode.Open);
                    len += f.Length;
                    f.Close();
                }
                catch
                {

                }
            }
            //DirectoryInfo di = new DirectoryInfo(dirPath);
            //foreach (FileInfo fi in di.GetFiles())
            //{
            //    len += fi.Length;
            //}
            //DirectoryInfo[] dis = di.GetDirectories();
            if (dis.Length > 0)
            {
                for (int i = 0; i < dis.Length; i++)
                {
                    len += GetDirectoryLength(dirPath + "\\" + dis[i]);
                }
            }
            return len;
        }

        private void ONScripterSettings_Checked(object sender, RoutedEventArgs e)
        {
            CheckBox c = sender as CheckBox;
            if (c.Name == "logOutput")
                UVEngineNative.ONScripterSettings.logOutput = true;
            else if (c.Name == "externalVideo")
                UVEngineNative.ONScripterSettings.playExternVideo = true;

        }
        private void ONScripterSettings_Unchecked(object sender, RoutedEventArgs e)
        {
            CheckBox c = sender as CheckBox;
            if (c.Name == "logOutput")
                UVEngineNative.ONScripterSettings.logOutput = false;
            else if (c.Name == "externalVideo")
                UVEngineNative.ONScripterSettings.playExternVideo = false;
        }


        private void Button_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show(UVEngine.Resources.UVEngine.passwordsaved);
            UVEngineNative.UVEngineSettings.Passwd = passwdbox.Password;
        }

        private void RadioButton_Checked_1(object sender, RoutedEventArgs e)
        {
            UVEngineNative.UVEngineSettings.language = "";
            MessageBox.Show("需要重新启动应用使设置生效\nNeed restarting the app to apply the changes");
        }

        private void RadioButton_Checked_2(object sender, RoutedEventArgs e)
        {
            UVEngineNative.UVEngineSettings.language = "zh-CN";
            MessageBox.Show("需要重新启动应用使设置生效");
        }

        private void RadioButton_Checked_3(object sender, RoutedEventArgs e)
        {
            UVEngineNative.UVEngineSettings.language = "en-US";
            MessageBox.Show("Need restarting the app to apply the changes");
        }
    }
}