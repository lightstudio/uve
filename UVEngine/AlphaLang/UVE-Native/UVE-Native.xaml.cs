//using GameResource;
//using Microsoft.Phone.Controls;
//using Microsoft.Phone.Storage;
//using Microsoft.Xna.Framework;
//using System.IO;
//using System.IO.IsolatedStorage;
//using System.Linq;
//using System.Threading.Tasks;
//using System.Windows;
//using System.Windows.Input;
//using System.Windows.Navigation;
//using UVE_Media;
//using UVEngine;
//using UVEngineNative;

//namespace UVEngine.AlphaLang
//{
//    public partial class UVE_Native : PhoneApplicationPage
//    {
//        #region public对象声明
//        public System.Windows.Controls.TextBlock Text;
//        public UVE_Audio audio;
//        public GameRes res = new GameRes();
//        public double resolution_width, resolution_height, zoom, gameratio, screenratio;
//        #endregion

//        #region private对象声明
//        bool IsInitialized = false;
//        string gamefolder;
//        ExternalStorageDevice device;
//        IsolatedStorageSettings iss = IsolatedStorageSettings.ApplicationSettings;
//        //NativeScript nativeScript;
//        GameTimer timer = new GameTimer();
//        int ori = 0;
//        #endregion

//        public UVE_Native()
//        {
//            InitializeComponent();
            
//        }

//        protected override void OnBackKeyPress(System.ComponentModel.CancelEventArgs e)
//        {
//            base.OnBackKeyPress(e);
//            //if (!(e.Cancel = nativeScript.BackKey()))
//            //{
//            //    timer.Update -= timer_Update;
//            //    Touch.FrameReported -= Touch_FrameReported;
//            //    //audio.Dispose();
                
//            //}
//        }

//        async Task Init()
//        {
//            resolution_height = Application.Current.Host.Content.ActualWidth;
//            resolution_width = Application.Current.Host.Content.ActualHeight;
//            UVEDelegate deleg = new UVEDelegate();
//            deleg.SetCallback(new UVECallBack());
//            //if ((int)iss["extstorage"] == 1)
//            //{
//            //    game = new Game(gamefolder, deleg, true);
//            //    await res.GameResInit(gamefolder, true);
//            //}
//            //else
//            //{
//            //    game = new Game(gamefolder, deleg, false);
//            await res.GameResInit(gamefolder, false);
//            //}
//            //device = (await ExternalStorage.GetExternalStorageDevicesAsync()).FirstOrDefault();


//            //audio = new UVE_Audio(gamefolder, nativeScript.gameInfoEx.DataFolder);
//            Text = new System.Windows.Controls.TextBlock();
//            if (res.font != null)
//            {
//                Text.FontSource = res.font.fontsource;
//                Text.FontFamily = new System.Windows.Media.FontFamily(res.font.fontname);
//                res.font.fontStream.Close();
//            }
//            LayoutRoot.Children.Add(Text);

//            //nativeScript = new NativeScript(gamefolder, deleg);

//            #region 事件注册
//            Touch.FrameReported += Touch_FrameReported;
//            OrientationChanged += UVE_Native_OrientationChanged;
//            timer.UpdateInterval = System.TimeSpan.FromMilliseconds(33);
//            timer.Update += timer_Update;
//            FrameworkDispatcher.Update();
//            timer.Start();
//            #endregion

//            //string[] str = new string[1];
//            //str[0] = "arc-test.nsa";
//            //device = (await ExternalStorage.GetExternalStorageDevicesAsync()).FirstOrDefault();
//            //await nsa.NsaFileInit(str, device);
//            //UVEDelegate m_UVEDelegate = new UVEDelegate();
//            //m_UVEDelegate.SetCallback(new UVECallBack());
//            //System.Windows.Media.Imaging.BitmapImage bi = new System.Windows.Media.Imaging.BitmapImage();
//            //Stream s = await nsa.GetFile("pic\\lh\\b7.png");
//            //bi.SetSource(s);
//            //s.Close();
//            //img.Source = bi;
//            //Stream sc = await nsa.GetFile("k\\k (500).ogg");
//            //OggSharp.OggSong song = new OggSharp.OggSong(sc);
//            //song.Play();
            
//        }

//        void timer_Update(object sender, GameTimerEventArgs e)
//        {
//            FrameworkDispatcher.Update();
//        }

//        void UVE_Native_OrientationChanged(object sender, OrientationChangedEventArgs e)
//        {
//            if (e.Orientation == PageOrientation.LandscapeLeft) ori = 0;
//            else ori = 1;
//        }

//        void Touch_FrameReported(object sender, TouchFrameEventArgs e)
//        {
//            //throw new System.NotImplementedException();
//            TouchPoint p=e.GetPrimaryTouchPoint(null);
//            double x = 0, y = 0;
//            if (ori == 0)//左横屏
//            {
//                x = p.Position.Y;
//                y = resolution_height - p.Position.X;
//            }
//            else if (ori == 1)//右横屏
//            {
//                x = resolution_width - p.Position.Y;
//                y = p.Position.X;
//            }
//            //nativeScript.TouchPoint(x, y);
//        }
//        ~UVE_Native()
//        {
//            IsInitialized = false;
//        }
//        //private void Initialize()
//        //{
//        //    NavigationContext.QueryString.TryGetValue("game", out gamefolder);
//        //    inf = new InfoReader(gamefolder);
//        //    audio = new UVE_Audio(gamefolder, inf.Game.DataFolder);
//        //}
        
//        protected override async void OnNavigatedTo(NavigationEventArgs e)
//        {
//            UVECallBack.p = this;
//            base.OnNavigatedTo(e);
//            if (!IsInitialized)
//            {
//                //LayoutRoot.Children.Add(Text);
//                NavigationContext.QueryString.TryGetValue("game", out gamefolder);
//                await Init();
//                //inf = new InfoReader(gamefolder);
//                //Initialize();
//                IsInitialized = true;
//                //string[] str = new string[2];
//                //str[0] = "k\\k (500).ogg";
//                //str[1] = "pic\\bg29_00.png";
//                //await Init(str);
                
//            }
//            else
//            {

//            }
//        }
//    }
//}