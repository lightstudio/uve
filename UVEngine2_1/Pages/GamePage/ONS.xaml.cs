using System;
using System.Windows;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using UVEngine2_1.Classes;
using UVEngineNative;
using Size = Windows.Foundation.Size;

namespace UVEngine2_1.Pages.GamePage
{
    //public partial class ONS : PhoneApplicationPage
    //{
    //    private Game _game;
    //    private string _gameHash;
    //    private ONSCallback _onsCallback;
    //    private Direct3DInterop _onsInterop;
    //    private int _scaleFactor;
    //    private double _xamlHeight, _xamlWidth, _deviceHeight, _deviceWidth, _screenRatio;

    //    public ONS()
    //    {
    //        InitializeComponent();
    //    }

    //    protected override void OnNavigatedTo(NavigationEventArgs e)
    //    {
    //        NavigationContext.QueryString.TryGetValue("Hash", out _gameHash);

    //        _game = GameRuntime.CurrentGames.GetGameByHash(_gameHash);
    //        _scaleFactor = Application.Current.Host.Content.ScaleFactor;
    //        _xamlHeight = Application.Current.Host.Content.ActualWidth;
    //        _xamlWidth = Application.Current.Host.Content.ActualHeight;
    //        _deviceHeight = Math.Floor(_xamlHeight*_scaleFactor/100.0);
    //        _deviceWidth = Math.Floor(_xamlWidth*_scaleFactor/100.0);
    //        _screenRatio = _xamlHeight/_xamlWidth;
    //        base.OnNavigatedTo(e);
    //    }

    //    private void ONSSurface_OnLoaded(object sender, RoutedEventArgs e)
    //    {
    //        _onsCallback = new ONSCallback(this);
    //        if (_onsInterop != null) return;
    //        _onsInterop = new Direct3DInterop(_game.Folder);
    //        switch (_game.WideScreen)
    //        {
    //            case false:
    //                _onsInterop.SetONSResolutionAndDisplayMode((int) _deviceHeight*4/3, (int) _deviceHeight, 640, 480,
    //                    0xF);
    //                ONSSurface.Width = 640;
    //                break;
    //            case true:
    //                _onsInterop.SetONSResolutionAndDisplayMode((int) _deviceWidth, (int) _deviceWidth*3/4, 800, 600,
    //                    0xFF);
    //                ONSSurface.Margin = new Thickness(0, 0, 0, -200);
    //                ONSSurface.Height = 600;
    //                ONSSurface.VerticalAlignment = VerticalAlignment.Top;
    //                break;
    //        }
    //        _onsInterop.InitGlobalCallback(_onsCallback);
    //        _onsInterop.WindowBounds = new Size(_xamlWidth, _xamlHeight);
    //        _onsInterop.NativeResolution = new Size(_deviceWidth, _deviceHeight);
    //        _onsInterop.RenderResolution = _onsInterop.NativeResolution;
    //        ONSSurface.SetContentProvider(_onsInterop.CreateContentProvider());
    //        ONSSurface.SetManipulationHandler(_onsInterop);
    //    }
    //}
}