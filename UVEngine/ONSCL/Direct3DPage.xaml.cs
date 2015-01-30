using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using UVEngineNative;

namespace UVEngine
{
    public partial class Direct3DPage : PhoneApplicationPage
    {
        internal Direct3DInterop m_d3dBackground = null;
        DrawingSurface drawingSurfacebg = null;
        internal MediaElement me = new MediaElement();
        internal bool isMediaPlaying = false;
        private string gamefolder;
        ONSCL_Callback cb;
        Button_withText exit, skip, save, load, back, forw;
        double screenratio;
        double resolution_width, resolution_height;
        bool IsInitialized = false, ControlPressed = false, QuickButtonShowed = true;
        public Direct3DPage()
        {
            InitializeComponent();
        }
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            NavigationContext.QueryString.TryGetValue("game", out gamefolder);
            //ColumnDefinition render = new ColumnDefinition(), blank = new ColumnDefinition();
            //, button = new ColumnDefinition()
            if (ONSCL_GameInfoPage.nsg == null) ONSCL_GameInfoPage.nsg = new NSGameSettings(gamefolder);

            //LayoutRoot.ColumnDefinitions.Add(render);
            //LayoutRoot.ColumnDefinitions.Add(blank);
            //LayoutRoot.ColumnDefinitions.Add(button);
            App.landscape = true;
            base.OnNavigatedTo(e);
            resolution_height = Application.Current.Host.Content.ActualWidth;
            resolution_width = Application.Current.Host.Content.ActualHeight;
            screenratio = resolution_width / resolution_height;
            if (!IsInitialized)
            {
                drawingSurfacebg = new DrawingSurface();
                drawingSurfacebg.Loaded += drawingSurfacebg_Loaded;
                drawingSurfacebg.ManipulationStarted += drawingSurfacebg_ManipulationStarted;
                RenderArea.Children.Add(drawingSurfacebg);
                me.MediaEnded += me_MediaEnded;
                me.ManipulationCompleted += me_ManipulationCompleted;
                exit = new Button_withText("exit", new Thickness(resolution_width - 48, resolution_height - 48, 0, 0), new SolidColorBrush(Colors.Blue), new SolidColorBrush(Colors.White), 0.4, 0.5);
                skip = new Button_withText("skip", new Thickness(resolution_width - 48, resolution_height - 96, 0, 49), new SolidColorBrush(Colors.Blue), new SolidColorBrush(Colors.White), 0.4, 0.5);
                //save = new Button_withText("save", new Thickness(resolution_width - 48, resolution_height - 144, 0, 97), new SolidColorBrush(Colors.Blue), new SolidColorBrush(Colors.White), 0.4, 0.5);
                //load = new Button_withText("load", new Thickness(resolution_width - 48, resolution_height - 192, 0, 145), new SolidColorBrush(Colors.Blue), new SolidColorBrush(Colors.White), 0.4, 0.5);
                back = new Button_withText("back", new Thickness(resolution_width - 48, resolution_height - 144, 0, 97), new SolidColorBrush(Colors.Blue), new SolidColorBrush(Colors.White), 0.4, 0.5);
                forw = new Button_withText("forw", new Thickness(resolution_width - 48, resolution_height - 192, 0, 145), new SolidColorBrush(Colors.Blue), new SolidColorBrush(Colors.White), 0.4, 0.5);
                ShowQuickButton();
                IsInitialized = true;
            }
            else m_d3dBackground.Activated();
            if (isMediaPlaying)
            {
                me.Stop();
                isMediaPlaying = false;
                LayoutRoot.Children.Remove(me);
            }
        }

        void me_ManipulationCompleted(object sender, System.Windows.Input.ManipulationCompletedEventArgs e)
        {
            if (MessageBox.Show("是否结束视频播放？", "提示", MessageBoxButton.OKCancel) == MessageBoxResult.OK)
            {
                me.Stop();
                isMediaPlaying = false;
                LayoutRoot.Children.Remove(me);
            }
        }

        void me_MediaEnded(object sender, RoutedEventArgs e)
        {
            isMediaPlaying = false;
            LayoutRoot.Children.Remove(me);
        }

        void drawingSurfacebg_ManipulationStarted(object sender, System.Windows.Input.ManipulationStartedEventArgs e)
        {
            if (ControlPressed)
            {
                m_d3dBackground.CtrlReleased();
            }
        }
        protected override void OnNavigatingFrom(NavigatingCancelEventArgs e)
        {
            m_d3dBackground.DeActivated();
            base.OnNavigatingFrom(e);
        }
        private void ButtonClick_buttonwithtext(object sender, System.Windows.Input.ManipulationStartedEventArgs e)
        {
            switch ((sender as TextBlock).Name)
            {
                case "exit":
                    if (MessageBox.Show("真的要退出?", "提示", MessageBoxButton.OKCancel) == MessageBoxResult.OK)
                        m_d3dBackground.Quit();
                    break;
                case "skip":
                    if (ControlPressed)
                    {
                        m_d3dBackground.CtrlReleased();
                    }
                    else
                    {
                        m_d3dBackground.CtrlPressed();
                    }
                    ControlPressed = !ControlPressed;
                    break;
                case "save":
                    m_d3dBackground.Save();
                    break;
                case "load":
                    m_d3dBackground.Load();
                    break;
                case "back":

                    break;
                case "forw":

                    break;
            }
        }
        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            App.landscape = false;
            base.OnNavigatedFrom(e);
        }
        protected override void OnBackKeyPress(System.ComponentModel.CancelEventArgs e)
        {
            //if (MessageBox.Show("是否要退出？", "提示", MessageBoxButton.OKCancel) == MessageBoxResult.OK)
            //{
            //    m_d3dBackground.ONSExit();
            //}
            m_d3dBackground.BackKeyPressed();
            e.Cancel = true;
            base.OnBackKeyPress(e);
        }
        private void drawingSurfacebg_Loaded(object sender, RoutedEventArgs e)
        {
            cb = new ONSCL_Callback(this);
            if (m_d3dBackground == null)
            {
                m_d3dBackground = new Direct3DInterop(gamefolder);
                //横屏
                double deviceh = Math.Floor(Application.Current.Host.Content.ActualWidth * Application.Current.Host.Content.ScaleFactor / 100.0);
                double devicew = Math.Floor(Application.Current.Host.Content.ActualHeight * Application.Current.Host.Content.ScaleFactor / 100.0);
                double xamlh = Application.Current.Host.Content.ActualWidth;
                double xamlw = Application.Current.Host.Content.ActualHeight;
                switch (ONSCL_GameInfoPage.nsg.screentype)
                {
                    case 1:
                        m_d3dBackground.SetONSResolutionAndDisplayMode((int)deviceh * 4 / 3, (int)deviceh, 640, 480, 0xF);
                        drawingSurfacebg.Width = 640;
                        break;
                    case 2:
                        m_d3dBackground.SetONSResolutionAndDisplayMode((int)devicew, (int)devicew * 3 / 4, 800, 600, 0xFF);
                        RenderArea.Margin = new Thickness(0, 0, 0, -200);
                        drawingSurfacebg.Height = 600;
                        drawingSurfacebg.VerticalAlignment = VerticalAlignment.Top;
                        break;
                }

                m_d3dBackground.InitGlobalCallback(cb);
                // 设置窗口边界(以 dip 为单位)
                m_d3dBackground.WindowBounds = new Windows.Foundation.Size(xamlw,xamlh);

                // 设置本机分辨率(以像素为单位)
                m_d3dBackground.NativeResolution = new Windows.Foundation.Size(devicew,deviceh);

                // 将呈现分辨率设置为本机的最大分辨率
                m_d3dBackground.RenderResolution = m_d3dBackground.NativeResolution;

                // 将本机组件挂钩到 DrawingSurfaceBackgroundGrid
                drawingSurfacebg.SetContentProvider(m_d3dBackground.CreateContentProvider());
                drawingSurfacebg.SetManipulationHandler(m_d3dBackground);

            }
        }
        private void ShowQuickButton()
        {
            skip.AddButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
            //save.AddButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
            back.AddButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
            forw.AddButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
            exit.AddButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
            //load.AddButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));

        }
        private void HideQuickButton()
        {
            skip.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
            //save.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
            back.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
            forw.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
            exit.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
            //load.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
        }
        private void GestureListener_Flick(object sender, FlickGestureEventArgs e)
        {
            if (QuickButtonShowed)
            {
                HideQuickButton();
            }
            else
            {
                ShowQuickButton();
            }
            QuickButtonShowed = !QuickButtonShowed;
        }
    }
}