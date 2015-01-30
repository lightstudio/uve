using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Microsoft.Phone.Tasks;

namespace UVEngine
{
    public class ONSCL_Callback:UVEngineNative.INativeCalls
    {
        Direct3DPage d3dp;
        public ONSCL_Callback(Direct3DPage d3dp)
        {
            this.d3dp = d3dp;

        }
        public void ErrorLog(string errorContent)
        {

        }
        public void ExitCore(int code)
        {
            d3dp.Dispatcher.BeginInvoke(() =>
            {
                //if (code!=0)
                //    System.Windows.MessageBox.Show(UVEngine.Resources.UVEngine.exitwitherror + code.ToString());
                //if (d3dp.NavigationService.CanGoBack) d3dp.NavigationService.GoBack();
                //else App.Current.Terminate();
                Application.Current.Terminate();
            });
        }
        public void PlayExternalVideo(string path)
        {
            if (UVEngineNative.ONScripterSettings.playExternVideo)
            {
                try
                {
                    MediaPlayerLauncher mediaPlayerLauncher = new MediaPlayerLauncher();
                    mediaPlayerLauncher.Media = new Uri(path, UriKind.Relative);
                    mediaPlayerLauncher.Location = MediaLocationType.Data;
                    mediaPlayerLauncher.Controls = MediaPlaybackControls.Pause | MediaPlaybackControls.Stop;
                    mediaPlayerLauncher.Orientation = MediaPlayerOrientation.Landscape;
                    mediaPlayerLauncher.Show();
                }
                catch (Exception e)
                {
                    d3dp.Dispatcher.BeginInvoke(() =>
                    {
                        MessageBox.Show(UVEngine.Resources.UVEngine.videoplaybackerror + e.Message + "\n" + e.StackTrace);
                    });
                }
            }
            else
            {
                try
                {
                    d3dp.isMediaPlaying = true;
                    d3dp.Dispatcher.BeginInvoke(() =>
                    {
                        d3dp.me.Source = new Uri(path, UriKind.Relative);
                        d3dp.LayoutRoot.Children.Add(d3dp.me);
                        d3dp.me.Play();
                    });
                    while (d3dp.isMediaPlaying)
                    {
                        System.Threading.Thread.Sleep(100);
                    }
                }
                catch (Exception e)
                {
                    d3dp.Dispatcher.BeginInvoke(() =>
                        {
                            MessageBox.Show(UVEngine.Resources.UVEngine.videoplaybackerror + e.Message + "\n" + e.StackTrace);
                        });
                }
            }
        }
    }
}
