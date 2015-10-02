using System;
using System.Threading;
using System.Windows;
using Microsoft.Phone.Tasks;
using UVEngineNative;

namespace UVEngine2_1.Pages.GamePage
{
    //public class ONSCallback : INativeCalls
    //{
    //    private readonly ONS _onsPage;

    //    public ONSCallback(ONS onsPage)
    //    {
    //        _onsPage = onsPage;
    //    }

    //    public void ErrorLog(string errorContent)
    //    {
    //    }

    //    public void ExitCore(int code)
    //    {
    //        _onsPage.Dispatcher.BeginInvoke(() =>
    //        {
    //            //if (code!=0)
    //            //    System.Windows.MessageBox.Show(UVEngine.Resources.UVEngine.exitwitherror + code.ToString());
    //            //if (d3dp.NavigationService.CanGoBack) d3dp.NavigationService.GoBack();
    //            //else App.Current.Terminate();
    //            Application.Current.Terminate();
    //        });
    //    }

    //    public void PlayExternalVideo(string path)
    //    {
    //        if (ONScripterSettings.playExternVideo)
    //        {
    //            try
    //            {
    //                var mediaPlayerLauncher = new MediaPlayerLauncher();
    //                mediaPlayerLauncher.Media = new Uri(path, UriKind.Relative);
    //                mediaPlayerLauncher.Location = MediaLocationType.Data;
    //                mediaPlayerLauncher.Controls = MediaPlaybackControls.Pause | MediaPlaybackControls.Stop;
    //                mediaPlayerLauncher.Orientation = MediaPlayerOrientation.Landscape;
    //                mediaPlayerLauncher.Show();
    //            }
    //            catch (Exception e)
    //            {
    //                _onsPage.Dispatcher.BeginInvoke(() => { MessageBox.Show(e.Message + "\n" + e.StackTrace); });
    //            }
    //        }
    //        else
    //        {
    //            try
    //            {
    //                d3dp.isMediaPlaying = true;
    //                d3dp.Dispatcher.BeginInvoke(() =>
    //                {
    //                    d3dp.me.Source = new Uri(path, UriKind.Relative);
    //                    d3dp.LayoutRoot.Children.Add(d3dp.me);
    //                    d3dp.me.Play();
    //                });
    //                while (d3dp.isMediaPlaying)
    //                {
    //                    Thread.Sleep(100);
    //                }
    //            }
    //            catch (Exception e)
    //            {
    //                d3dp.Dispatcher.BeginInvoke(() => { MessageBox.Show(e.Message + "\n" + e.StackTrace); });
    //            }
    //        }
    //    }
    //}
}