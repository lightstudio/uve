using System;
using System.Reflection;
using System.IO;
using System.Windows;
using OggSharp;
using UVEngineNative;
using Microsoft.Phone.BackgroundAudio;
using UVEngine.AlphaLang;
//using UVE_IronRuby;

namespace UVEngine
{
    public class UVECallBack : ICallback
    {
        //UVE_IronRubyEngine irEngine = new UVE_IronRubyEngine();
        //static public UVE_Native p;
        //public void DrawText(string text)
        //{
        //    //p.Text.Text = text;
        //}
        //public void PlayBGM(string path, string title, string artist, string album)
        //{
        //    BackgroundAudioPlayer.Instance.Track = new AudioTrack(new Uri(path, UriKind.Relative), title, artist, album, null, null, EnabledPlayerControls.Pause);
        //}
        //public void PlayDWave(string path, int num)
        //{
            
        //}
        //public void Print(int effectnum)
        //{

        //}
        //public void InitRuby(string path)
        //{
        //    //irEngine.SetVariable("UVE", this);
        //    //irEngine.SetVariable("UVE_Page", p);
        //    ////irEngine.SetVariable("UVE_Audio", p.audio);
        //    ////irEngine.SetVariable("UVE_Resource", p.res);
        //    //Assembly execAssembly = Assembly.GetExecutingAssembly();
        //    //Assembly exe = Assembly.Load(new AssemblyName("Microsoft.Phone"));
        //    //irEngine.LoadAssembly(execAssembly);
        //    //irEngine.LoadAssembly(exe);
        //    //p.Dispatcher.BeginInvoke(() =>
        //    //{
        //    //    try
        //    //    {
        //    //        string code = new StreamReader(System.IO.IsolatedStorage.IsolatedStorageFile.GetUserStoreForApplication().OpenFile(path, FileMode.Open)).ReadToEnd();
        //    //        irEngine.Execute(code);
        //    //    }
        //    //    catch (Exception e) { MessageBox.Show("IronRuby脚本引擎错误：\n"+e.Message + '\n' + e.StackTrace); }
        //    //});
        //}
        //public void CallRuby(string call)
        //{
            
        //}
        public void LogOutput(string str)
        {
        }
        public void VideoPlay(string path)
        {

        }
        public void ErrorAndExit(int code)
        {
            MessageBox.Show("Exit with error" + code.ToString());
        }
        public void Exit()
        {
            MessageBox.Show("Exit!");
        }
    }
}