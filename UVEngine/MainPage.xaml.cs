using Microsoft.Phone.Controls;
using Microsoft.Phone.Notification;
using Microsoft.Phone.Tasks;
using Microsoft.Phone.Shell;
using OpenXLive;
using System;
using System.IO.IsolatedStorage;
using System.Text;
using System.Linq;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Input;
using System.Windows.Resources;
using System.Windows.Navigation;
using Microsoft.Phone.Storage;
using System.Threading.Tasks;
using System.Diagnostics;
using MarketPlaceAuthInternal;
using Microsoft.Xna.Framework.Media;
using System.Collections.Generic;
using UVEngineNative;
using Coding4Fun.Toolkit.Controls;

namespace UVEngine
{
    public partial class MainPage : PhoneApplicationPage
    {
        IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication();
        IsolatedStorageSettings appSettings = IsolatedStorageSettings.ApplicationSettings;
        bool delta = false;
        string ToastChannelName = "OpenXLivePushNotificationHostingChannel";
        public MainPage()
        {
            if (!appSettings.Contains("pushenabled"))
            {
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
                appSettings.Add("pushenabled", 1);
                appSettings.Add("onsTrans", 1);
                appSettings.Add("bgmOut", 1);
                appSettings.Add("mp3Out", 1);
                appSettings.Add("dwaveOut", 1);
                appSettings.Add("audioBackground", 1);
                appSettings.Add("videoPlay", 1);
                appSettings.Add("textEffect", 1);
                appSettings.Add("allowError", 1);
                appSettings.Add("useNative", 1);
                appSettings.Save();

            }
            InitializeComponent();
            //if (!Auth.GetInstallState() && !Debugger.IsAttached)
            //{
            //    if (MessageBox.Show(UVEngine.Resources.UVEngine.notfrommarket, UVEngine.Resources.UVEngine.warning, MessageBoxButton.OKCancel) == MessageBoxResult.OK)
            //    {
            //        if (MessageBox.Show(UVEngine.Resources.UVEngine.trailversion, UVEngine.Resources.UVEngine.tip, MessageBoxButton.OKCancel) == MessageBoxResult.OK)
            //        {
            //            MarketplaceDetailTask mdt = new MarketplaceDetailTask();
            //            mdt.ContentType = MarketplaceContentType.Applications;
            //            mdt.ContentIdentifier = "7666b5f5-c1d5-484a-85ad-0742212a0fca";
            //            mdt.Show();
            //        }
            //        else
            //        {
            //            MarketplaceDetailTask mdt = new MarketplaceDetailTask();
            //            mdt.ContentType = MarketplaceContentType.Applications;
            //            mdt.ContentIdentifier = "14205072-b8d2-476c-a1c7-37bde5c0e985";
            //            mdt.Show();
            //        }
            //        Application.Current.Terminate();
            //    }
            //    else
            //    {
            //        Application.Current.Terminate();
            //    }
            //}
            ((Microsoft.Phone.Shell.ApplicationBarMenuItem)ApplicationBar.MenuItems[0]).Text = UVEngine.Resources.UVEngine.traditionalimport;
            ((Microsoft.Phone.Shell.ApplicationBarMenuItem)ApplicationBar.MenuItems[1]).Text = UVEngine.Resources.UVEngine.changeBackground;
            ((Microsoft.Phone.Shell.ApplicationBarMenuItem)ApplicationBar.MenuItems[2]).Text = UVEngine.Resources.UVEngine.fileclear;
            ((Microsoft.Phone.Shell.ApplicationBarMenuItem)ApplicationBar.MenuItems[3]).Text = UVEngine.Resources.UVEngine.performancetest;
            ((Microsoft.Phone.Shell.ApplicationBarMenuItem)ApplicationBar.MenuItems[4]).Text = UVEngine.Resources.UVEngine.openxlive;
            //((Microsoft.Phone.Shell.ApplicationBarMenuItem)ApplicationBar.MenuItems[4]).Text = UVEngine.Resources.UVEngine.about;
            ((Microsoft.Phone.Shell.ApplicationBarMenuItem)ApplicationBar.MenuItems[5]).Text = UVEngine.Resources.UVEngine.exit;
            ((Microsoft.Phone.Shell.ApplicationBarIconButton)ApplicationBar.Buttons[0]).Text = UVEngine.Resources.UVEngine.import;
            ((Microsoft.Phone.Shell.ApplicationBarIconButton)ApplicationBar.Buttons[1]).Text = UVEngine.Resources.UVEngine.settings;
            Microsoft.Phone.Controls.TiltEffect.TiltableItems.Add(typeof(Microsoft.Phone.Controls.MenuItem));
            GetExtStorage();
            if (!appSettings.Contains("firstopen"))
            {
                MessageBox.Show(UVEngine.Resources.UVEngine.firstUse, UVEngine.Resources.UVEngine.aboutus, MessageBoxButton.OK);
                appSettings.Add("firstopen", 1);
                appSettings.Save();
                this.Dispatcher.BeginInvoke(() =>
                {
                    if (MessageBox.Show(UVEngine.Resources.UVEngine.askbenchmark, UVEngine.Resources.UVEngine.test, MessageBoxButton.OKCancel) == MessageBoxResult.OK)
                    {
                        NavigationService.Navigate(new Uri("/BenchMark/BenchMark.xaml", UriKind.Relative));
                    }
                });
            }

            if ((int)appSettings["pushenabled"] == 1)
            {
                HttpNotificationChannel pushChannel = HttpNotificationChannel.Find(ToastChannelName);
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
            IsInitialized = true;
            ApplicationBar.Mode = Microsoft.Phone.Shell.ApplicationBarMode.Default;

        }
        private bool IsInitialized = false;
        void PushChannel_ErrorOccurred(object sender, NotificationChannelErrorEventArgs e)
        {


        }
        async Task GetExtStorage()
        {
            ExternalStorageDevice sdCard = (await ExternalStorage.GetExternalStorageDevicesAsync()).FirstOrDefault();
            if (!appSettings.Contains("extstorage") || (int)appSettings["extstorage"] == -1)
            {
                if (sdCard != null)
                {
                    if (MessageBox.Show(UVEngine.Resources.UVEngine.sdinserted, UVEngine.Resources.UVEngine.tip, MessageBoxButton.OKCancel) == MessageBoxResult.OK)
                    {
                        appSettings.Add("extstorage", 1);
                    }
                    else
                    {
                        appSettings.Add("extstorage", 0);
                    }

                }
                else
                {
                    //No SDCard Inserted
                    appSettings.Add("extstorage", -1);
                }
                appSettings.Save();
            }
            else if (sdCard == null)
            {
                appSettings["extstorage"] = -1;
            }
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
            //if (OpenXLive.XLiveGameManager.CurrentSession != null
            //    && OpenXLive.XLiveGameManager.CurrentSession.IsValid
            //    && uri != null)
            //{
            try
            {
                OpenXLive.XLiveGameManager.CurrentSession.RegistNotificationUriCompleted += new OpenXLive.Features.AsyncEventHandler(CurrentSession_RegistNotificationUriCompleted);
                OpenXLive.XLiveGameManager.CurrentSession.RegisterNotificationUri(uri);
            }
            catch { }
            //}
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
                    toast.Title = UVEngine.Resources.UVEngine.notify; //设置标题 
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
        void SelectGame(object sender, ManipulationCompletedEventArgs args)
        {
            if (!delta)
            {
                String str = (sender as TextBlock).Text;
                if (storage.FileExists(str+"\\uve-manifest.uvm"))
                {
                    string temp = "/GameDetailPage.xaml?game=" + str;
                    this.NavigationService.Navigate(new Uri(temp, UriKind.Relative));
                }
                else if (storage.FileExists(str + "\\uve-manifestEX.uvm"))
                {
                    string temp = "/ONSCL/ONSCL_GameInfoPage.xaml?game=" + str;
                    this.NavigationService.Navigate(new Uri(temp, UriKind.Relative));
                }
            }
            delta = false;
        }
        void Delta(object sender, ManipulationDeltaEventArgs args)
        {
            delta = true;
            
        }
        //private void Push_Checked(object sender, RoutedEventArgs e)
        //{
        //    HttpNotificationChannel pushChannel = HttpNotificationChannel.Find(ToastChannelName);

        //    if ((int)appSettings["pushenabled"] == 0)
        //    {
        //        ToastPrompt toast = new ToastPrompt(); //实例化 
        //        toast.Height = 100;
        //        toast.Title = UVEngine.Resources.UVEngine.message; //设置标题 
        //        toast.Message = UVEngine.Resources.UVEngine.pushenabled; //设置正文消息 
        //        toast.FontSize = 20; //设置文本大小(可选) 
        //        toast.TextOrientation = System.Windows.Controls.Orientation.Vertical; //设置呈现为纵向 

        //        toast.Show();
        //        appSettings["pushenabled"] = 1;
        //        appSettings.Save();
        //    }
        //    // If the channel was not found, then create a new connection to the push service. 
        //    if (pushChannel == null)
        //    {
        //        pushChannel = new HttpNotificationChannel(ToastChannelName);

        //        // Register for all the events before attempting to open the channel.
        //        pushChannel.ChannelUriUpdated += new EventHandler<NotificationChannelUriEventArgs>(PushChannel_ChannelUriUpdated);
        //        pushChannel.ErrorOccurred += new EventHandler<NotificationChannelErrorEventArgs>(PushChannel_ErrorOccurred);

        //        // Register for this notification only if you need to receive the notifications while your application is running.
        //        pushChannel.ShellToastNotificationReceived += new EventHandler<NotificationEventArgs>(PushChannel_ShellToastNotificationReceived);

        //        pushChannel.Open();

        //        // Bind this new channel for Tile events.
        //        pushChannel.BindToShellToast();
        //    }
        //    else
        //    {
        //        // The channel was already open, so just register for all the events.
        //        pushChannel.ChannelUriUpdated += new EventHandler<NotificationChannelUriEventArgs>(PushChannel_ChannelUriUpdated);
        //        pushChannel.ErrorOccurred += new EventHandler<NotificationChannelErrorEventArgs>(PushChannel_ErrorOccurred);

        //        // Register for this notification only if you need to receive the notifications while your application is running.
        //        pushChannel.ShellToastNotificationReceived += new EventHandler<NotificationEventArgs>(PushChannel_ShellToastNotificationReceived);
        //    }

        //    // Send Channel Uri to OpenXLive Hosting Server
        //    RegisterNotificationUri(pushChannel.ChannelUri);


        //}
        //private void Push_Unchecked(object sender, RoutedEventArgs e)
        //{
        //    HttpNotificationChannel pushChannel = HttpNotificationChannel.Find(ToastChannelName);
        //    appSettings["pushenabled"] = 0;
        //    appSettings.Save();
        //    if (pushChannel != null)
        //    {
        //        pushChannel.UnbindToShellToast();
        //        pushChannel.Close();
        //    }
        //}
        // 为 ViewModel 项加载数据
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            if (this.NavigationService.CanGoBack)
                this.NavigationService.RemoveBackEntry();
            //System.Windows.Application.LoadComponent(this, new System.Uri("/UVEngine;component/MainPage.xaml", System.UriKind.Relative));
            //this.listBox = ((System.Windows.Controls.ListBox)(this.FindName("listBox")));
            //listBox.Items.Clear();
            listBox.ItemsSource = new List.GameLists();
            //listBox.
            listBox.InvalidateArrange();
            if (UVEngineNative.UVEngineSettings.translucent)
            {
                ApplicationBar.Opacity = 0.6;
                listBox.Margin = new Thickness(0, 0, 0, 80);
            }
            else
            {
                ApplicationBar.Opacity = 1.0;
                listBox.Margin = new Thickness(0, 0, 0, 0);
            }
            try
            {
                BitmapImage bitmapImage = new BitmapImage();
                IsolatedStorageFileStream fs = new IsolatedStorageFileStream("Images\\Main_bg.png", System.IO.FileMode.Open, storage);
                if (UVEngineNative.UVEngineSettings.translucent) bitmapImage.DecodePixelHeight = 800;
                else bitmapImage.DecodePixelHeight = 720;
                bitmapImage.SetSource(fs);
                fs.Close();
                ImageBrush imageBrush = new ImageBrush();
                imageBrush.ImageSource = bitmapImage;
                panorama.Background = imageBrush;
                panorama.Background.Opacity = 0.7;
            }
            catch { }
        }
        private bool IsPressed = false;
        private void Confirm(object sender, PopUpEventArgs<string, PopUpResult> e)
        {
            IsPressed = false;
        }
        protected override void OnBackKeyPress(System.ComponentModel.CancelEventArgs e)
        {
            base.OnBackKeyPress(e);
            switch (UVEngineNative.UVEngineSettings.ConfirmExit)
            {
                case 1:
                    if (!IsPressed)
                    {
                        e.Cancel = true;
                        IsPressed = true;
                        this.Dispatcher.BeginInvoke(() =>
                        {
                            ToastPrompt confirm = new ToastPrompt();
                            confirm.Height = 80; //设置标题 
                            confirm.Message = UVEngine.Resources.UVEngine.exitcomfirm; //设置正文消息 
                            confirm.FontSize = 30; //设置文本大小(可选) 
                            confirm.TextOrientation = System.Windows.Controls.Orientation.Vertical; //设置呈现为纵向 
                            confirm.Show();
                            confirm.Completed += new EventHandler<PopUpEventArgs<string, PopUpResult>>(Confirm);
                        });
                    }
                    break;
                case 2:
                    if (MessageBox.Show(UVEngine.Resources.UVEngine.exit_cf_mesbox, UVEngine.Resources.UVEngine.confirm, MessageBoxButton.OKCancel) != MessageBoxResult.OK)
                    {
                        e.Cancel = true;
                    }
                    break;
            }
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            NavigationService.Navigate(new Uri("/Import.xaml", UriKind.Relative));
        }


        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            MarketplaceReviewTask mrt = new MarketplaceReviewTask();
            mrt.Show();
        }

        private void ApplicationBarIconButton_Click(object sender, EventArgs e)
        {
            this.NavigationService.Navigate(new Uri("/System/Settings.xaml", UriKind.Relative));
        }

        private void ApplicationBarIconButton_Click_1(object sender, EventArgs e)
        {
            this.NavigationService.Navigate(new Uri("/System/QuickImport.xaml", UriKind.Relative));
        }

        private void About(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new Uri("/AboutPage.xaml", UriKind.Relative));
        }

        private void Panorama_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            //switch (((sender as Panorama).SelectedItem as PanoramaItem).Name)
            //{
            //    case "Online":
            //        this.Dispatcher.BeginInvoke(() =>
            //            {
            //                System.Threading.Thread.Sleep(250);
            //                ApplicationBar.Mode = Microsoft.Phone.Shell.ApplicationBarMode.Minimized;
            //            });
            //        break;
            //    default:
            //        this.Dispatcher.BeginInvoke(() =>
            //            {
            //                System.Threading.Thread.Sleep(250);
            //                ApplicationBar.Mode = Microsoft.Phone.Shell.ApplicationBarMode.Default;
            //            });
            //        break;
            //}
        }

        private void Skydrive(object sender, ManipulationCompletedEventArgs e)
        {
            if (!delta)
            {
                NavigationService.Navigate(new Uri("/System/Skydrive.xaml", UriKind.Relative));
            }
            delta = false;
        }

        private void Announcement(object sender, ManipulationCompletedEventArgs e)
        {
            if (!delta)
            {
                if (XLiveGameManager.CurrentSession != null && XLiveGameManager.CurrentSession.IsValid)
                {
                    if (!appSettings.Contains("confirmed")) { MessageBox.Show(UVEngine.Resources.UVEngine.agecomfirm); appSettings.Add("confirmed", 1); }
                    OpenXLive.Silverlight.XLiveUIManager.ShowAnnouncement();
                }
                else
                {
                    MessageBox.Show(UVEngine.Resources.UVEngine.networkerror,UVEngine.Resources.UVEngine.errormessage ,MessageBoxButton.OK);
                }
            }
            delta = false;
        }
        private void Friends(object sender, ManipulationCompletedEventArgs e)
        {
            if (!delta)
            {
                if (XLiveGameManager.CurrentSession != null && XLiveGameManager.CurrentSession.IsValid)
                {
                    if (!appSettings.Contains("confirmed")) { MessageBox.Show(UVEngine.Resources.UVEngine.agecomfirm); appSettings.Add("confirmed", 1); }
                    OpenXLive.Silverlight.XLiveUIManager.ShowFriend(false);
                }
                else
                {
                    MessageBox.Show(UVEngine.Resources.UVEngine.networkerror, UVEngine.Resources.UVEngine.errormessage, MessageBoxButton.OK);
                }
            }
            delta = false;
        }
        private void Player(object sender, ManipulationCompletedEventArgs e)
        {
            if (!delta)
            {
                if (XLiveGameManager.CurrentSession != null && XLiveGameManager.CurrentSession.IsValid)
                {
                    if (!appSettings.Contains("confirmed")) { MessageBox.Show(UVEngine.Resources.UVEngine.agecomfirm); appSettings.Add("confirmed", 1); }
                    OpenXLive.Silverlight.XLiveUIManager.ShowOnlinePlayer();
                }
                else
                {
                    MessageBox.Show(UVEngine.Resources.UVEngine.networkerror, UVEngine.Resources.UVEngine.errormessage, MessageBoxButton.OK);
                }
            }
            delta = false;
        }
        private void ApplicationBarMenuItem_Click_1(object sender, EventArgs e)
        {
            PhotoChooserTask pct = new PhotoChooserTask();
            pct.Completed += pc_Completed;
            pct.Show();

        }
        void pc_Completed(object sender, PhotoResult e)
        {
            if (e.TaskResult == TaskResult.OK)
            {
                StreamResourceInfo resource = new StreamResourceInfo(e.ChosenPhoto, null);
                using (IsolatedStorageFileStream file = storage.CreateFile("Images\\Main_bg.png"))
                {
                    int chunkSize = 4096;
                    byte[] bytes = new byte[chunkSize];
                    int byteCount;
                    while ((byteCount = resource.Stream.Read(bytes, 0, chunkSize)) > 0)
                    {
                        file.Write(bytes, 0, byteCount);
                    }
                }
                try
                {
                    BitmapImage bitmapImage = new BitmapImage();
                    IsolatedStorageFileStream fs = new IsolatedStorageFileStream("Images\\Main_bg.png", System.IO.FileMode.Open, storage);
                    bitmapImage.DecodePixelHeight = 720;
                    bitmapImage.SetSource(fs);
                    fs.Close();
                    ImageBrush imageBrush = new ImageBrush();
                    imageBrush.ImageSource = bitmapImage;
                    panorama.Background = imageBrush;
                    panorama.Background.Opacity = 0.7;
                }
                catch { }
            }

        }

        private void ApplicationBarMenuItem_Click_2(object sender, EventArgs e)
        {
            App.Current.Terminate();
            //App.naverr = true;
            //throw new Exception();
        }
        private void Test(object sender, EventArgs e)
        {
            NavigationService.Navigate(new Uri("/BenchMark/BenchMark.xaml", UriKind.Relative));
        }

        private void ApplicationBarMenuItem_Click_3(object sender, EventArgs e)
        {
            NavigationService.Navigate(new Uri("/System/FileClear.xaml", UriKind.Relative));
        }
        private void ApplicationBarMenuItem_Click_4(object sender, EventArgs e)
        {
            if (XLiveGameManager.CurrentSession != null && XLiveGameManager.CurrentSession.IsValid)
            {
                if (!appSettings.Contains("confirmed")) { MessageBox.Show(UVEngine.Resources.UVEngine.agecomfirm); appSettings.Add("confirmed", 1); }

                OpenXLive.Silverlight.XLiveUIManager.ShowGameCenter();
            }
            else
            {
                MessageBox.Show(UVEngine.Resources.UVEngine.networkerror, UVEngine.Resources.UVEngine.errormessage, MessageBoxButton.OK);
            }
        }
        private void TImport(object sender, EventArgs e)
        {
            PhotoChooserTask pc = new PhotoChooserTask();
            pc.Completed += pc_Completed_TImport;
            pc.Show();
        }
        private void pc_Completed_TImport(object sender, PhotoResult e)
        {
            QuickImport.pics.Clear();
            if (e.TaskResult == TaskResult.OK)
            {
                MediaLibrary ml = new MediaLibrary();
                PictureCollection pic_c = ml.Pictures;
                
                try
                {
                    for (int i = 0; i < pic_c.Count; i++)
                    {
                        string fname = "";
                        if (e.OriginalFileName.Contains('\\'))
                            fname = e.OriginalFileName.Substring(e.OriginalFileName.LastIndexOf('\\') + 1);
                        else fname = e.OriginalFileName;
                        if (pic_c[i].Name == fname)
                        {
                            QuickImport.pics.Add(pic_c[i]);
                            break;
                        }
                    }
                    //foreach (Picture p in pic_c)
                    //{
                    //    string fname = "";
                    //    if (e.OriginalFileName.Contains('\\'))
                    //        fname = e.OriginalFileName.Substring(e.OriginalFileName.LastIndexOf('\\') + 1);
                    //    else fname = e.OriginalFileName;
                    //    if (p.Name == fname)
                    //    {
                    //        QuickImport.pics.Add(p);
                    //        break;
                    //    }
                    //}

                }
                catch (Exception exception)
                {
                    MessageBox.Show(exception.Message);
                }
                long totalsize = 0;

                foreach (Picture p in QuickImport.pics)
                {
                    totalsize += p.GetImage().Length;
                }
                if (totalsize <= System.IO.IsolatedStorage.IsolatedStorageFile.GetUserStoreForApplication().AvailableFreeSpace)
                {
                    this.Dispatcher.BeginInvoke(() =>
                    {
                        Thread.Sleep(2000);
                        this.NavigationService.Navigate(new Uri("/System/QuickExtract.xaml", UriKind.Relative));
                    });
                }
                else
                {
                    MessageBox.Show(UVEngine.Resources.UVEngine.spacenotenough);
                }
            }
        }
        private void ApplicationBarMenuItem_Click_5(object sender, EventArgs e)
        {
            //NavigationService.Navigate(new Uri("/System/QuickImport.xaml", UriKind.Relative));
            NavigationService.Navigate(new Uri("/TestPage.xaml", UriKind.Relative));
        }
        
    }
}