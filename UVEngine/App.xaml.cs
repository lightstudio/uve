using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
//using Microsoft.Xna.Framework;
//using Microsoft.Xna.Framework.Content;
//using Microsoft.Xna.Framework.Graphics;
using OpenXLive;
using OpenXLive.Features;
using OpenXLive.Silverlight;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Resources;
using System.Text;
using System.Windows;
using System.Windows.Markup;
using System.Windows.Navigation;
using UVEngine.Resources;
//using UVEngine.ViewModels;
using UVEngineNative;
//using MarketPlaceAuth;
using Microsoft.Xna.Framework.GamerServices;
using System.Windows.Media.Imaging;
using System.IO;
namespace UVEngine
{
    public partial class App : Application
    {
        //private static MainViewModel viewModel = null;

        ///// <summary>
        ///// 视图用于进行绑定的静态 ViewModel。
        ///// </summary>
        ///// <returns>MainViewModel 对象。</returns>
        //public static MainViewModel ViewModel
        //{
        //    get
        //    {
        //        // 延迟创建视图模型，直至需要时
        //        if (viewModel == null)
        //            viewModel = new MainViewModel();

        //        return viewModel;
        //    }
        //}
        /// <summary>
        /// 提供对应用程序的 ContentManager 的访问权限。
        /// </summary>
        //public ContentManager Content { get; private set; }

        ///// <summary>
        ///// 提供对设置为提取 FrameworkDispatcher 的 GameTimer 的访问权限。
        ///// </summary>
        //public GameTimer FrameworkDispatcherTimer { get; private set; }

        /// <summary>
        /// 提供对应用程序的 AppServiceProvider 的访问权限。
        /// </summary>
        //public AppServiceProvider Services { get; private set; }
        /// <summary>
        /// 提供对电话应用程序的根框架的轻松访问。
        /// </summary>
        /// <returns>电话应用程序的根框架。</returns>
        public static PhoneApplicationFrame RootFrame { get; private set; }
        /// <summary>
        /// Application 对象的构造函数。
        /// </summary>
        static public bool landscape = false;
        public App()
        {
            ONScripterSettings.Initialize();
            UVEngineSettings.Initialize();
            if (UVEngineNative.UVEngineSettings.language == "") UVEngine.Resources.UVEngine.Culture = System.Threading.Thread.CurrentThread.CurrentUICulture;
            else System.Threading.Thread.CurrentThread.CurrentUICulture = new System.Globalization.CultureInfo(UVEngineNative.UVEngineSettings.language);
            UnhandledException += Application_UnhandledException;
            InitializeComponent();
            InitializePhoneApplication();
            //InitializeXnaApplication();
            InitializeLanguage();
            GameSession session = XLiveGameManager.CreateSession("GsMmyW5d3a6fr7VbtgRD4SPH");
            session.CreateSessionCompleted += session_CreateSessionCompleted;
            XLiveUIManager.Initialize(this, session);
            session.Open();
            System.IO.IsolatedStorage.IsolatedStorageSettings appSettings = System.IO.IsolatedStorage.IsolatedStorageSettings.ApplicationSettings;
            if (!appSettings.Contains("showDebug")) appSettings.Add("showDebug", 0);
            if ((int)appSettings["showDebug"] == 1)
            {
                Application.Current.Host.Settings.EnableFrameRateCounter = true;
                MemoryDiagnosticsHelper.Start(TimeSpan.FromMilliseconds(100), true);
            }
            PhoneApplicationService.Current.UserIdleDetectionMode = IdleDetectionMode.Disabled;
            Microsoft.Devices.CameraButtons.ShutterKeyPressed
                +=
                delegate
                {
                    if (XLiveGameManager.CurrentSession != null &&
                        XLiveGameManager.CurrentSession.SNSProviders.Count > 0) // Must exist available provider
                    {

                        if (UVEngineSettings.QuickShare)
                        {
                            
                            //App.RootFrame.Navigate(new Uri("/OpenXLive.Silverlight;component/Forms/UpdateStatusPage.xaml", UriKind.Relative));
                            WriteableBitmap bmp;
                            if (landscape) bmp = new WriteableBitmap((int)App.Current.RootVisual.RenderSize.Height, (int)App.Current.RootVisual.RenderSize.Width);
                            else bmp = new WriteableBitmap((int)App.Current.RootVisual.RenderSize.Width, (int)App.Current.RootVisual.RenderSize.Height);
                            bmp.Render(App.Current.RootVisual, null);
                            bmp.Invalidate();
                            MemoryStream stream = new MemoryStream();
                            //var a = System.IO.IsolatedStorage.IsolatedStorageFile.GetUserStoreForApplication().OpenFile("capture.jpg", FileMode.OpenOrCreate);
                            bmp.SaveJpeg(stream, bmp.PixelWidth, bmp.PixelHeight, 0, 100);
                            BitmapImage bi = new BitmapImage();
                            bi.SetSource(stream);
                            OpenXLive.Silverlight.XLiveUIManager.ShowUpdateStatus(bi, "");
                            //capture = stream.ToArray();
                            //OpenXLive.Silverlight.XLiveUIManager.ShowUpdateStatus(new BitmapImage(stream), "");
                        }
                    }
                    else
                    {
                        MessageBox.Show(UVEngine.Resources.UVEngine.openxliveproblem);
                    }
                };
            if (UVEngineSettings.ReqPasswd)
            {
                Guide.BeginShowKeyboardInput
                    (Microsoft.Xna.Framework.PlayerIndex.One,
                    UVEngine.Resources.UVEngine.passwdreq,
                    UVEngine.Resources.UVEngine.passwdmes,
                    "",
                    delegate(IAsyncResult result)
                    {
                        string text = Guide.EndShowKeyboardInput(result);
                        if (text != UVEngineNative.UVEngineSettings.Passwd)
                        {
                            Application.Current.Terminate();
                        }
                    },
                    new object(),
                    true);
            }
            //this.Startup += delegate
            //{
            //    if (UVEngineSettings.ReqPasswd)
            //    {
            //        var passwordInput = new Coding4Fun.Toolkit.Controls.PasswordInputPrompt
            //        {
            //            Title = UVEngine.Resources.UVEngine.passwdreq,
            //            Message = UVEngine.Resources.UVEngine.passwdmes,
                        
            //        };
            //        passwordInput.Completed += new EventHandler<Coding4Fun.Toolkit.Controls.PopUpEventArgs<string, Coding4Fun.Toolkit.Controls.PopUpResult>>(delegate(object sender, Coding4Fun.Toolkit.Controls.PopUpEventArgs<string, Coding4Fun.Toolkit.Controls.PopUpResult> e)
            //        {
            //            if (e.PopUpResult == Coding4Fun.Toolkit.Controls.PopUpResult.Ok)
            //            {
            //                if (e.Result != UVEngineSettings.Passwd)
            //                {
            //                    MessageBox.Show(UVEngine.Resources.UVEngine.wrongpasswd);
            //                    Application.Current.Terminate();
            //                }
            //            }
            //            else
            //            {
            //                Application.Current.Terminate();
            //            }
            //        });
            //        passwordInput.Show();
            //    }
            //};
/*
            // 调试时显示图形分析信息。
            if (Debugger.IsAttached)
            {
                // 显示当前帧速率计数器。
                Application.Current.Host.Settings.EnableFrameRateCounter = true;

                // 显示在每个帧中重绘的应用程序区域。
                //Application.Current.Host.Settings.EnableRedrawRegions = true；

                // 启用非生产分析可视化模式，
                // 该模式显示递交给 GPU 的包含彩色重叠区的页面区域。
                //Application.Current.Host.Settings.EnableCacheVisualization = true；

                // 通过禁用以下对象阻止在调试过程中关闭屏幕
                // 应用程序的空闲检测。
                //  注意: 仅在调试模式下使用此设置。禁用用户空闲检测的应用程序在用户不使用电话时将继续运行
                // 并且消耗电池电量。
                PhoneApplicationService.Current.UserIdleDetectionMode = IdleDetectionMode.Disabled;
            }
            */
            PhoneApplicationService.Current.ApplicationIdleDetectionMode = IdleDetectionMode.Disabled;
        }

        void session_CreateSessionCompleted(object sender, AsyncEventArgs e)
        {
            if (e.Result.ReturnValue)
            {
                // Successful to create game session
            }
            else
            {
                // Error handler
//                MessageBox.Show(e.Result.ErrorMessage);
            }
        }

        // 应用程序启动(例如，从“开始”菜单启动)时执行的代码
        // 此代码在重新激活应用程序时不执行
        private void Application_Launching(object sender, LaunchingEventArgs e)
        {
            

        }

        // 激活应用程序(置于前台)时执行的代码
        // 此代码在首次启动应用程序时不执行
        private void Application_Activated(object sender, ActivatedEventArgs e)
        {
            // 确保正确恢复应用程序状态
            //if (!App.ViewModel.IsDataLoaded)
            //{
            //    App.ViewModel.LoadData();
            //} 
        }

        // 停用应用程序(发送到后台)时执行的代码
        // 此代码在应用程序关闭时不执行
        private void Application_Deactivated(object sender, DeactivatedEventArgs e)
        {
            // 确保所需的应用程序状态在此处保持不变。
        }

        // 应用程序关闭(例如，用户点击“后退”)时执行的代码
        // 此代码在停用应用程序时不执行
        private void Application_Closing(object sender, ClosingEventArgs e)
        {
            if (XLiveGameManager.CurrentSession != null && XLiveGameManager.CurrentSession.IsValid)
            {
                XLiveGameManager.CurrentSession.Close();
            }

        }
        //static public bool naverr = false;
        // 导航失败时执行的代码
        private void RootFrame_NavigationFailed(object sender, NavigationFailedEventArgs e)
        {
            //if (Debugger.IsAttached)
            //{
            //    // 导航已失败；强行进入调试器
            //    Debugger.Break();
            //}
            //else
            //{
            //    naverr = true;
            //}
        }

        // 出现未处理的异常时执行的代码
        private void Application_UnhandledException(object sender, ApplicationUnhandledExceptionEventArgs e)
        {
            if (Debugger.IsAttached)
            {
                // 出现未处理的异常；强行进入调试器
                Debugger.Break();
            }
            else /*if (!naverr)*/
            {
                MessageBox.Show(UVEngine.Resources.UVEngine.fc_text_1 + e.ExceptionObject.Message + UVEngine.Resources.UVEngine.fc_text_2);
            }
            e.Handled = true;
        }

        #region 电话应用程序初始化

        // 避免双重初始化
        private bool phoneApplicationInitialized = false;
        private bool _isResume = false; 
        // 请勿向此方法中添加任何其他代码
        private void InitializePhoneApplication()
        {
            if (phoneApplicationInitialized)
                return;

            // 创建框架但先不将它设置为 RootVisual；这允许初始
            // 屏幕保持活动状态，直到准备呈现应用程序时。
            //RootFrame = new PhoneApplicationFrame();
            RootFrame = new TransitionFrame();
            RootFrame.Navigated += CompleteInitializePhoneApplication;
            RootFrame.Navigating += RootFrame_Navigating;
            RootFrame.UriMapper = new AssociationUriMapper();
            // 处理导航故障
            RootFrame.NavigationFailed += RootFrame_NavigationFailed;

            // 在下一次导航中处理清除 BackStack 的重置请求，
            RootFrame.Navigated += CheckForResetNavigation;

            // 确保我们未再次初始化
            phoneApplicationInitialized = true;
        }

        // 请勿向此方法中添加任何其他代码
        private void CompleteInitializePhoneApplication(object sender, NavigationEventArgs e)
        {
            // 设置根视觉效果以允许应用程序呈现
            if (RootVisual != RootFrame)
                RootVisual = RootFrame;

            // 删除此处理程序，因为不再需要它
            RootFrame.Navigated -= CompleteInitializePhoneApplication;
        }

        private void CheckForResetNavigation(object sender, NavigationEventArgs e)
        {
            // 如果应用程序收到“重置”导航，则需要进行检查
            // 以确定是否应重置页面堆栈
            if (e.NavigationMode == NavigationMode.Reset)
                _isResume = true;
        }
        void RootFrame_Navigating(object sender, NavigatingCancelEventArgs e)
        {
            //当前为程序快速恢复时，框架进行的自动跳转  
            //这里决定是否取消该跳转，让程序恢复到最后浏览页面  
            if (_isResume)
            {
                e.Cancel = true;
//                if (e.Uri.OriginalString.Contains("StartPage.xaml")) //跳转到默认程序启动页面，则取消  
//                    e.Cancel = true;
//                else //不取消，则清除堆栈  
//                    RootFrame.Navigated += ClearBackStackAfterReset;
                _isResume = false;
            }
        }  

        //private void ClearBackStackAfterReset(object sender, NavigationEventArgs e)
        //{
        //    // 取消注册事件，以便不再调用该事件
        //    RootFrame.Navigated -= ClearBackStackAfterReset;

        //    // 只为“新建”(向前)和“刷新”导航清除堆栈
        //    if (e.NavigationMode != NavigationMode.New && e.NavigationMode != NavigationMode.Refresh)
        //        return;

        //    // 为了获得 UI 一致性，请清除整个页面堆栈
        //    while (RootFrame.RemoveBackEntry() != null)
        //    {
        //        ; // 不执行任何操作
        //    }
        //}

        #endregion

        // 初始化应用程序在其本地化资源字符串中定义的字体和排列方向。
        //
        // 若要确保应用程序的字体与受支持的语言相符，并确保
        // 这些语言的 FlowDirection 都采用其传统方向，ResourceLanguage
        // 应该初始化每个 resx 文件中的 ResourceFlowDirection，以便将这些值与以下对象匹配
        // 文件的区域性。例如:
        //
        // AppResources.es-ES.resx
        //    ResourceLanguage 的值应为“es-ES”
        //    ResourceFlowDirection 的值应为“LeftToRight”
        //
        // AppResources.ar-SA.resx
        //     ResourceLanguage 的值应为“ar-SA”
        //     ResourceFlowDirection 的值应为“RightToLeft”
        //
        // 有关本地化 Windows Phone 应用程序的详细信息，请参见 http://go.microsoft.com/fwlink/?LinkId=262072。
        //
        private void InitializeLanguage()
        {
            try
            {
                // 将字体设置为与由以下对象定义的显示语言匹配
                // 每种受支持的语言的 ResourceLanguage 资源字符串。
                //
                // 如果显示出现以下情况，则回退到非特定语言的字体
                // 手机的语言不受支持。
                //
                // 如果命中编译器错误，则表示以下对象中缺少 ResourceLanguage
                // 资源文件。
                //RootFrame.Language = XmlLanguage.GetLanguage(UVEngine.Resources.UVEngine.ResourceLanguage);
                RootFrame.Language = XmlLanguage.GetLanguage(System.Threading.Thread.CurrentThread.CurrentUICulture.ToString());
                // 根据以下条件设置根框架下的所有元素的 FlowDirection
                // 每个以下对象的 ResourceFlowDirection 资源字符串上的
                // 受支持的语言。
                //
                // 如果命中编译器错误，则表示以下对象中缺少 ResourceFlowDirection
                // 资源文件。
                FlowDirection flow = (FlowDirection)Enum.Parse(typeof(FlowDirection), UVEngine.Resources.UVEngine.ResourceFlowDirection);
                RootFrame.FlowDirection = flow;
            }
            catch
            {
                // 如果此处导致了异常，则最可能的原因是
                // ResourceLangauge 未正确设置为受支持的语言
                // 代码或 ResourceFlowDirection 设置为 LeftToRight 以外的值
                // 或 RightToLeft。

                if (Debugger.IsAttached)
                {
                    Debugger.Break();
                }

                throw;
            }
        }

        #region XNA 应用程序初始化

        // 执行应用程序所需的 XNA 类型的初始化。
        //private void InitializeXnaApplication()
        //{
        //    // 创建服务提供程序
        //    Services = new AppServiceProvider();

        //    // 将 SharedGraphicsDeviceManager 作为应用程序的 IGraphicsDeviceService 添加到服务中
        //    foreach (object obj in ApplicationLifetimeObjects)
        //    {
        //        if (obj is IGraphicsDeviceService)
        //            Services.AddService(typeof(IGraphicsDeviceService), obj);
        //    }

        //    // 创建 ContentManager 以便应用程序可以加载预编译的资产
        //    Content = new ContentManager(Services, "Content");

        //    // 创建 GameTimer 以提取 XNA FrameworkDispatcher
        //    FrameworkDispatcherTimer = new GameTimer();
        //    FrameworkDispatcherTimer.FrameAction += FrameworkDispatcherFrameAction;
        //    FrameworkDispatcherTimer.Start();
        //}

        // 每帧提取 FrameworkDispatcher 的事件处理程序。
        // 许多 XNA 事件和特定功能(如 SoundEffect 播放)
        // 都需要 FrameworkDispatcher。
        //private void FrameworkDispatcherFrameAction(object sender, EventArgs e)
        //{
        //    FrameworkDispatcher.Update();
        //}

        #endregion
    }
}