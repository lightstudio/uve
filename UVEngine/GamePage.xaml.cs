#region 引用
using Microsoft.Phone.BackgroundAudio;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Tasks;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Media;
using OggSharp;
using System;
using System.IO;
using System.IO.IsolatedStorage;
using System.Runtime.Serialization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
#endregion

namespace UVEngine
{
    public partial class GamePage : PhoneApplicationPage
    {
        #region 对象声明
        public string lastsetwindow = "";
        DataContractSerializer ser;
        static public RMENU_State rs = RMENU_State.STATE_null;
        int ori = 0; // 0:左横屏 1:右横屏
        int clear = 0;
        static public int savenumber = 10;
        public TextHistory textHistory = new TextHistory();
        public BTN_Collections btn_collections = new BTN_Collections();
        public Selection_Collection selection_collection = new Selection_Collection();
        public bool btnwaiting = false;
        public double btn_VAR_to_load = 0;
        public WindowStatus wndst = new WindowStatus();
        public ImageStatus[] imgState = new ImageStatus[1000];
        public ImageStatus bgState = new ImageStatus();
        public ImageStatus textboximageState = new ImageStatus();
        static public WindowStatus lastst = new WindowStatus();
        public bool showTextEffect;
        public int cmdcount;
        public bool IsLoaded = false;
        IsolatedStorageSettings appSettings = IsolatedStorageSettings.ApplicationSettings;
        IsolatedStorageFile isf = IsolatedStorageFile.GetUserStoreForApplication();
        string gamefolder = "";
        TextBlock text = new TextBlock();
        System.Windows.Shapes.Rectangle opacitymask = new System.Windows.Shapes.Rectangle();
        static public Script script;
        bool backed = false;
        static public InfoReader inf;
        public Storyboard waittimer = new Storyboard();
        //public Storyboard textrendtimer = new Storyboard(), skipper = new Storyboard();
        private GameTimer t_rendtimer = new GameTimer(), skip_timer = new GameTimer();
        public bool rending = false, waiting = false, selecting = false;
        public bool skipping = false;
        public string ToShow;
        public Button_withText skip, back, forw, save, load, exit;
        static public double resolution_width, resolution_height, zoom, gameratio, screenratio;
        public Image[] img = new Image[1000];
        public bool IsBGShowed = false, mp3looping = false, click_button = false;
        public bool playing = false;
        public MediaElement me = new MediaElement();
        public OggSong[] dwave_ogg = new OggSong[100];
        public OggSong bgm_ogg, dwave_ogg_u;
        public string currentbgm;
        public SoundEffect[] dwave_wav = new SoundEffect[100];
        public SoundEffectInstance[] dwave_wav_inst = new SoundEffectInstance[100];
        public SoundEffectInstance DwaveInstance_u;
        public SoundEffect dwave_wav_u;
        public Image bg = new Image();
        public Image textboximage = new Image();
        public bool backing = false;
        public int currentbackcount = 0;
        static public bool allowError = true, useNative = true;
        public Save saver;
        public TextBlock[] rmenu;
        #endregion
        public void Select(object sender, ManipulationCompletedEventArgs e)
        {
            foreach (Selection sel in selection_collection)
            {
                if (sel.SelectionTextBlock.Text == (sender as TextBlock).Text)
                {
                    Label.GotoLabel(sel.TargetLabel, script.lab, ref cmdcount);
                    selecting = false;
                    break;
                }

            }
            try
            {
                while (cmdcount < script.scriptmain.Length && CmdExec(ref script.scriptmain[cmdcount++], ref script.num, ref script.str, ref script.sub)) ;
            }
            catch (Exception exc)
            {
                if (!allowError)
                {
                    MessageBox.Show("脚本错误\n线程:1(0x01)\n错误信息:" + exc.Message + "\n脚本行数:" + (cmdcount - 1) + "\n脚本内容:" + script.scriptmain[cmdcount - 1], "脚本错误信息", MessageBoxButton.OK);

                }
            }   
            for (int i = 0; i < selection_collection.Count; i++)
            {
                selection_collection[i].SelectionTextBlock.ManipulationCompleted -= new EventHandler<ManipulationCompletedEventArgs>(Select);
            }
            selection_collection.FinishSelection(LayoutRoot, TextRender);
        }
        public void RendText(string text, ref TextBlock TextRender, ref int clear, ref Storyboard rend)
        {
            if (clear == 0)
            {
                if (!skipping && showTextEffect)
                {
                    ToShow += text;
                    rending = true;
                    rend.Begin();
                }
                else
                {
                    TextRender.Text += text;
                }
            }
            else
            {
                if (!skipping && showTextEffect)
                {
                    TextRender.Text = "";
                    ToShow += text;
                    rending = true;
                    rend.Begin();
                }
                else
                {
                    TextRender.Text = text;
                }
                clear = 0;
            }
        }
        public void RendText(string text, ref TextBlock TextRender, ref int clear, ref GameTimer timer)
        {
            if (clear == 0)
            {
                if (!skipping && showTextEffect)
                {
                    ToShow += text;
                    rending = true;
                    //rend.Begin();
                    timer.Start();
                }
                else
                {
                    TextRender.Text += text;
                }
            }
            else
            {
                if (!skipping && showTextEffect)
                {
                    TextRender.Text = "";
                    ToShow += text;
                    rending = true;
                    //rend.Begin();
                    timer.Start();
                }
                else
                {
                    TextRender.Text = text;
                }
                clear = 0;
            }
        }
        public void Rend(object sender, EventArgs e)
        {
            if (0 < ToShow.Length && rending)
            {
                if (ToShow[0] != ' ')
                    TextRender.Text += ToShow[0];
                ToShow = ToShow.Remove(0, 1);
                //textrendtimer.Begin();
            }
            else
            {
                rending = false;
            }
        }
        private void ButtonClick_buttonwithtext(object sender, System.Windows.Input.ManipulationStartedEventArgs e)
        {
            if (!btnwaiting&&!selecting)
            {
                click_button = true;
                switch ((sender as TextBlock).Name)
                {
                    case "skip":
                        if (skipping) skipping = false;
                        else
                        {
                            
                            //skipper.Duration = TimeSpan.FromMilliseconds(100);
                            //skipper.Completed += new EventHandler(Skipper);
                            skip_timer.Start();
                            skipping = true;
                            //skipper.Begin();
                        }
                        break;
                    case "save":
                        ShowSave();
                        break;
                    case "load":
                        ShowLoad();
                        break;
                    case "back":
                    readhistory:
                        currentbackcount--;
                        if (backing && currentbackcount >= 0)
                        {
                            textHistory[currentbackcount].SetWindow(ref TextRender, ref textboximage);
                            TextRender.Text = textHistory[currentbackcount].text;
                        }
                        else if (!backing)
                        {
                            backing = true;
                            currentbackcount = textHistory.Count - 1;
                            goto readhistory;
                        }
                        break;
                    case "forw":
                        if (backing && ++currentbackcount < textHistory.Count)
                        {
                            //forwhistory:
                            textHistory[currentbackcount].SetWindow(ref TextRender, ref textboximage);
                            TextRender.Text = textHistory[currentbackcount].text;
                        }
                        else
                        {
                            backing = false;
                        }
                        break;
                    case "exit":
                        if (MessageBox.Show("确实要强制退出游戏吗？", "消息", MessageBoxButton.OKCancel) == MessageBoxResult.OK)
                        {
                            MessageBox.Show("脚本主线程已结束，返回值为259", "信息", MessageBoxButton.OK);
                            IsBGShowed = false;
                            for (int i = 0; i < 1000; i++)
                            {
                                LayoutRoot.Children.Remove(img[i]);
                                img[i] = null;
                            }
                            for (int i = 0; i < 100; i++)
                            {
                                if (dwave_ogg[i] != null) dwave_ogg[i].Stop();
                                if (dwave_wav[i] != null) dwave_wav[i].Dispose();
                            }
                            /*if (btnwaiting)*/ Touch.FrameReported -= new TouchFrameEventHandler(Touch_FrameReported);
                            btnwaiting = false;
                            if (bgm_ogg != null) bgm_ogg.Stop();
                            LayoutRoot.Children.Remove(TextRender);
                            LayoutRoot.Children.Remove(bg);
                            LayoutRoot.Children.Remove(textboximage);
                            me.MediaEnded -= new RoutedEventHandler(Media_Ended);
                            LayoutRoot.Children.Remove(me);
                            playing = false;
                            mp3looping = false;
                            skipping = false;
                            BackgroundAudioPlayer.Instance.Close();
                            cmdcount = 0;
                            this.NavigationService.GoBack();
                        }
                        break;
                }
            }
        }
        private void Media_Ended(object sender, EventArgs e)
        {
            if (mp3looping)
            {
                me.Play();
            }
            else
                playing = false;
        }

        private void TextRendUpdate(object sender, GameTimerEventArgs args)
        {
            if (0 < ToShow.Length && rending)
            {
                if (ToShow[0] != ' ')
                    TextRender.Text += ToShow[0];
                ToShow = ToShow.Remove(0, 1);
            }
            else
            {
                rending = false;
                t_rendtimer.Stop();
            }
        }
        private void SkipUpdate(object sender, GameTimerEventArgs args)
        {
            if (cmdcount < script.scriptmain.Length && skipping && !btnwaiting)
            {
                this.Dispatcher.BeginInvoke(() =>
                {
                tr_s:
                    try
                    {
                        while (CmdExec(ref script.scriptmain[cmdcount++], ref script.num, ref script.str, ref script.sub)) ;
                        textHistory.Add(new History(wndst, TextRender.Text + ToShow));
                    }
                    catch (Exception exc)
                    {
                        if (!allowError)
                        {
                            MessageBox.Show("脚本错误\n线程:1(0x01)\n错误信息:" + exc.Message + "\n脚本行数:" + (cmdcount - 1) + "\n脚本内容:" + script.scriptmain[cmdcount - 1], "脚本错误信息", MessageBoxButton.OK);
                            skipping = false;
                        }
                        else goto tr_s;
                    }
                });
            }
            else
            {
                skipping = false;
                skip_timer.Stop();
            }
        }
        public GamePage()
        {
            waittimer.Completed += new EventHandler(Wait_Completed);
            OrientationChanged += new EventHandler<OrientationChangedEventArgs>(About_OrientationChanged);
            InitializeComponent();
            //textrendtimer.Duration = TimeSpan.FromMilliseconds(wndst.textspeed);
            //textrendtimer.Completed += new EventHandler(Rend);
            GameTimer gameTimer = new GameTimer();
            gameTimer.UpdateInterval = TimeSpan.FromMilliseconds(33);
            gameTimer.Update += delegate { try { FrameworkDispatcher.Update(); } catch { } };
            gameTimer.Start();
            
            FrameworkDispatcher.Update();

            t_rendtimer.UpdateInterval = TimeSpan.FromMilliseconds(wndst.textspeed);
            t_rendtimer.Update += new EventHandler<GameTimerEventArgs>(TextRendUpdate);
            skip_timer.UpdateInterval = TimeSpan.FromMilliseconds(100);
            skip_timer.Update += new EventHandler<GameTimerEventArgs>(SkipUpdate);

            //no.Click += new RoutedEventHandler(ButtonClick);
            //yes.Click += new RoutedEventHandler(ButtonClick);
            if ((int)appSettings["useNative"] == 0) useNative = false;
            if ((int)appSettings["allowError"] == 0) allowError = false;
        }
        private void Wait_Completed(object sender, EventArgs e)
        {
            waiting = false;
            try
            {
                while (cmdcount < script.scriptmain.Length && CmdExec(ref script.scriptmain[cmdcount++], ref script.num, ref script.str, ref script.sub)) ;
                textHistory.Add(new History(wndst, TextRender.Text + ToShow));
            }
            catch (Exception exc)
            {
                if (!allowError)
                {
                    MessageBox.Show("脚本错误\n线程:1(0x01)\n错误信息:" + exc.Message + "\n脚本行数:" + (cmdcount - 1) + "\n脚本内容:" + script.scriptmain[cmdcount - 1], "脚本错误信息", MessageBoxButton.OK);

                }
            }
        }
        //private void Skipper(object sender, EventArgs e)
        //{

        //    if (cmdcount < script.scriptmain.Length && skipping && !btnwaiting)
        //    {
        //        this.Dispatcher.BeginInvoke(() =>
        //        {
        //        tr_s:
        //            try
        //            {
        //                while (CmdExec(ref script.scriptmain[cmdcount++], ref script.num, ref script.str, ref script.sub)) ;
        //                textHistory.Add(new History(wndst, TextRender.Text + ToShow));
        //                skipper.Begin();
        //            }
        //            catch (Exception exc)
        //            {
        //                if (!allowError)
        //                {
        //                    MessageBox.Show("脚本错误\n线程:1(0x01)\n错误信息:" + exc.Message + "\n脚本行数:" + (cmdcount - 1) + "\n脚本内容:" + script.scriptmain[cmdcount - 1], "脚本错误信息", MessageBoxButton.OK);
        //                    skipping = false;
        //                }
        //                else goto tr_s;
        //            }
        //        });
        //    }
        //    else
        //    {
        //        skipping = false;
        //    }
        //}
        private void About_OrientationChanged(object sender, OrientationChangedEventArgs e)
        {
            if (e.Orientation == PageOrientation.LandscapeLeft) ori = 0;
            else ori = 1;

        }
        private void RestoreSave(int count)
        {
            if (saver.gs[count].cmdcount != 0)
            {
                this.cmdcount = saver.gs[count].cmdcount;
                for (int i = 0; i < saver.gs[count].img.Length; i++)
                {
                    img[i].Opacity = 0;
                    //                img[i] = new Image();
                    if (saver.gs[count].img[i].Path != "")
                    {
                        img[i].Opacity = 1;
                        if (saver.gs[count].img[i].IsAlpha)
                        {
                            try
                            {
                                Alpha.AlphaBlend(new IsolatedStorageFileStream(saver.gs[count].img[i].Path, FileMode.Open, isf), ref img[i]);
                            }
                            catch { }
                            img[i].HorizontalAlignment = HorizontalAlignment.Left;
                            img[i].Opacity = saver.gs[count].img[i].Opacity;
                            img[i].Margin = saver.gs[count].img[i].Margin;
                            imgState[i].Alignment = HorizontalAlignment.Left;
                            imgState[i].IsAlpha = true;
                            imgState[i].Opacity = img[i].Opacity;
                            imgState[i].Path = saver.gs[count].img[i].Path;
                            imgState[i].Margin = img[i].Margin;
                        }
                        else
                        {
                            BitmapImage bi = new BitmapImage();
                            try
                            {
                                bi.SetSource(new IsolatedStorageFileStream(saver.gs[count].img[i].Path, FileMode.Open, isf));
                                img[i].Source = bi;
                            }
                            catch { }
                            img[i].HorizontalAlignment = HorizontalAlignment.Left;
                            img[i].Opacity = saver.gs[count].img[i].Opacity;
                            img[i].Margin = saver.gs[count].img[i].Margin;
                            imgState[i].Alignment = HorizontalAlignment.Left;
                            imgState[i].IsAlpha = false;
                            imgState[i].Opacity = img[i].Opacity;
                            imgState[i].Path = saver.gs[count].img[i].Path;
                            imgState[i].Margin = img[i].Margin;
                        }
                    }
                }
                if (saver.gs[count].lastsetwindow != "")
                {
                    lastsetwindow = saver.gs[count].lastsetwindow;
                    string[] param;
                    StringToolkit.CutParam(lastsetwindow, ',', out param);
                    wndst.x = int.Parse(param[0]);
                    wndst.y = int.Parse(param[1]);
                    wndst.charcount = int.Parse(param[2]);
                    wndst.linecount = int.Parse(param[3]);
                    wndst.charwidth = int.Parse(param[4]);
                    wndst.charheight = int.Parse(param[5]);
                    wndst.charspacing = int.Parse(param[6]);
                    wndst.linespacing = int.Parse(param[7]);
                    wndst.textspeed = double.Parse(param[8]);
                    if (int.Parse(param[9]) == 0) wndst.isthick = false;
                    else wndst.isthick = true;
                    if (int.Parse(param[10]) == 0) wndst.isshadow = false;
                    else wndst.isthick = true;
                    if (param.Length > 11)
                    {
                        wndst.color = param[11].Replace("\"", "");
                        if (param.Length > 12)
                        {
                            wndst.textblockx1 = int.Parse(param[12]);
                            wndst.textblocky1 = int.Parse(param[13]);
                            if (param.Length > 14)
                            {
                                wndst.textblockx2 = int.Parse(param[14]);
                                wndst.textblocky2 = int.Parse(param[15]);
                            }
                            else
                            {
                                wndst.textblockx2 = (int)resolution_width;
                                wndst.textblocky2 = (int)resolution_height;
                            }
                        }
                    }
                    wndst.SetWindow(ref TextRender, ref textboximage);

                }


                TextRender.Text = saver.gs[count].tbs.Content;
                TextRender.Margin = saver.gs[count].tbs.margin;
                TextRender.FontSize = saver.gs[count].tbs.FontSize;
                clear = 1;
                try
                {
                    if (saver.gs[count].bg.Path != "")
                    {
                        BitmapImage bgs = new BitmapImage();
                        bgs.SetSource(new IsolatedStorageFileStream(saver.gs[count].bg.Path, FileMode.Open, isf));
                        bg.Source = bgs;
                        bgState = saver.gs[count].bg;
                    }
                }
                catch
                {

                }
                try { if (saver.gs[count].textboximage.Path != "") { if (!saver.gs[count].textboximage.IsAlpha) { BitmapImage bgs = new BitmapImage(); bgs.SetSource(new IsolatedStorageFileStream(saver.gs[count].textboximage.Path, FileMode.Open, isf)); textboximage.Source = bgs; textboximage.Margin = saver.gs[count].textboximage.Margin; textboximage.HorizontalAlignment = HorizontalAlignment.Left; } else { Alpha.AlphaBlend(new IsolatedStorageFileStream(saver.gs[count].textboximage.Path, FileMode.Open, isf), ref textboximage); textboximage.Margin = saver.gs[count].textboximage.Margin; textboximage.HorizontalAlignment = HorizontalAlignment.Left; textboximageState = saver.gs[count].textboximage; } } }
                catch { }


                try
                {
                    currentbgm = saver.gs[count].meuri;
                    if (saver.gs[count].IsBGM)
                    {
                        BackgroundAudioPlayer.Instance.Track = new AudioTrack(new Uri(saver.gs[count].meuri, UriKind.Relative), "背景音乐", inf.Game.GameName, null, null, null, EnabledPlayerControls.Pause);
                        
                    }
                    else if (saver.gs[count].IsOgg)
                    {
                        bgm_ogg = new OggSong(new IsolatedStorageFileStream(saver.gs[count].meuri, FileMode.Open, isf));
                        bgm_ogg.Repeat = true;
                        playing = true;
                    }
                    else
                    {
                        me.SetSource(new IsolatedStorageFileStream(saver.gs[count].meuri, FileMode.Open, isf));
                        me.Play();
                        playing = true;
                        mp3looping = true;
                    }
                }
                catch
                {
                }
            }
            else
            {
                MessageBox.Show("你似乎不小心点到空存档了的说。。", "提醒", MessageBoxButton.OK);

            }
        }
        private void RMENU_Click(object sender, System.Windows.Input.ManipulationCompletedEventArgs e)
        {
            string text = (sender as TextBlock).Text, command = "";
            switch (rs)
            {
                case RMENU_State.STATE_null:

                    break;
                case RMENU_State.STATE_save:
                    command += text[2];
                    if (text[3] != ' ') command += text[3];
                    if (MessageBox.Show("真的要将存档保存在" + command + "号存档上吗?", "确认", MessageBoxButton.OKCancel) == MessageBoxResult.OK)
                    {
                        int count = int.Parse(command) - 1;
                        saver.gs[count] = new GameState(this);
                        Save();
//                        appSettings[gamefolder] = saver;
//                        appSettings.Save();
                        ShowSave();
                    }
                    break;
                case RMENU_State.STATE_load:
                    //Restore status here
                    command += text[2];
                    if (text[3] != ' ') command += text[3];
                    if (MessageBox.Show("真的要从" + command + "号存档上读取吗?", "确认", MessageBoxButton.OKCancel) == MessageBoxResult.OK)
                    {
                        int count = int.Parse(command) - 1;
                        RestoreSave(count);
                        HideRMENU();
                    }
                    
                    break;
                case RMENU_State.STATE_menu:
                    foreach (RMENU m in script.rmenu)
                    {
                        if (m.Text == text)
                        {
                            command = m.Command;
                            break;
                        }
                    }
                    switch (command)
                    {
                        case "save":
                            ShowSave();
                            break;
                        case "load":
                            ShowLoad();
                            break;
                        case "lookback":

                            HideRMENU();
                            break;
                        case "skip":
                            //skipper.Duration = TimeSpan.FromMilliseconds(10);
                            //skipper.Completed += new EventHandler(Skipper);
                            skip_timer.Start();
                            skipping = true;
                            //skipper.Begin();
                            HideRMENU();
                            break;
                        case "windowerase":

                            HideRMENU();
                            break;
                        case "reset":
                            IsBGShowed = false;
                            for (int i = 0; i < 1000; i++)
                            {
                                img[i].Source = null;
                            }
                            for (int i = 0; i < 100; i++)
                            {
                                if (dwave_ogg[i] != null) dwave_ogg[i].Stop();
                                if (dwave_wav[i] != null) dwave_wav[i].Dispose();
                            }
                            me.Stop();

                            //                    script = null;
                            ReadScript();
                            /*if (btnwaiting)*/ Touch.FrameReported -= new TouchFrameEventHandler(Touch_FrameReported);
                            btnwaiting = false;
                            if (bgm_ogg != null) bgm_ogg.Stop();
                            LayoutRoot.Children.Remove(TextRender);
                            bg.Source = null;
                            LayoutRoot.Children.Remove(textboximage);
                            me.MediaEnded -= new RoutedEventHandler(Media_Ended);
                            LayoutRoot.Children.Remove(me);
                            playing = false;
                            mp3looping = false;
                            skipping = false;
                            BackgroundAudioPlayer.Instance.Close();
                            cmdcount = 0;
                            TextRender.Text = "";
                            HideRMENU();
                            break;
                        case "end":
                            MessageBox.Show("脚本主线程已结束，返回值为0", "信息", MessageBoxButton.OK);
                            IsBGShowed = false;
                            for (int i = 0; i < 1000; i++)
                            {
                                //                        LayoutRoot.Children.Remove(img[i]);
                                //                        img[i] = null;
                                img[i].Source = null;
                            }
                            for (int i = 0; i < 100; i++)
                            {
                                if (dwave_ogg[i] != null) dwave_ogg[i].Stop();
                                if (dwave_wav[i] != null) dwave_wav[i].Dispose();
                            }
                            /*if (btnwaiting)*/ Touch.FrameReported -= new TouchFrameEventHandler(Touch_FrameReported);
                            btnwaiting = false;
                            if (bgm_ogg != null) bgm_ogg.Stop();
                            LayoutRoot.Children.Remove(TextRender);
                            LayoutRoot.Children.Remove(bg);
                            LayoutRoot.Children.Remove(textboximage);
                            me.MediaEnded -= new RoutedEventHandler(Media_Ended);
                            LayoutRoot.Children.Remove(me);
                            playing = false;
                            mp3looping = false;
                            skipping = false;
                            BackgroundAudioPlayer.Instance.Close();
                            cmdcount = 0;
                            this.NavigationService.GoBack();
                            
                            HideRMENU();
                            break;
                    }
                    break;
                default:

                    break;
            }
        }
        private void ShowRMENU()
        {
            rs = RMENU_State.STATE_menu;
            rmenu = new TextBlock[script.rmenu.Count];
            for (int i = 0; i < rmenu.Length; i++) rmenu[i] = new TextBlock();
            double x = 0, y = 0;
            for (int i = 0; i < script.rmenu.Count; i++)
            {
                rmenu[i].Text = script.rmenu[i].Text;
                rmenu[i].FontSize = script.rmenu.text_width * zoom - 1;
                rmenu[i].Margin = new Thickness(x, y, resolution_width - x - (script.rmenu.text_width + script.rmenu.text_spacing) * script.rmenu[i].Text.Length * zoom, resolution_height - y - script.rmenu.text_width);
                LayoutRoot.Children.Add(rmenu[i]); 
                rmenu[i].Foreground = new SolidColorBrush(Colors.White);
                rmenu[i].ManipulationCompleted += new EventHandler<ManipulationCompletedEventArgs>(RMENU_Click);
                y += ((double)script.rmenu.line_spacing + (double)script.rmenu.text_height) * zoom;
            }
        }
        private void ShowSave()
        {
            rs = RMENU_State.STATE_save;
            if (backed) foreach (TextBlock txb in rmenu)
                {
                    try
                    {
                        txb.ManipulationCompleted -= new EventHandler<ManipulationCompletedEventArgs>(RMENU_Click);
                        LayoutRoot.Children.Remove(txb);
                    }
                    catch
                    {

                    }
                }
            else
            {
                skip.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                save.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                forw.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                back.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                load.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                exit.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                LayoutRoot.ManipulationCompleted -= new EventHandler<System.Windows.Input.ManipulationCompletedEventArgs>(LayoutRoot_ManipulationCompleted_1);
                /*if (btnwaiting) */Touch.FrameReported -= new TouchFrameEventHandler(Touch_FrameReported);
                LayoutRoot.Children.Add(opacitymask);
                opacitymask.Margin = new Thickness(0, 0, 0, 0);
                opacitymask.Fill = new SolidColorBrush(Colors.Black);
                opacitymask.Opacity = 0.7;
                backed = true;
            }
            rmenu = new TextBlock[GamePage.savenumber+1];
            for (int i = 0; i < rmenu.Length; i++) rmenu[i] = new TextBlock();
            double x = 0, y = 0;
            rmenu[0].Text = script.rmenu.savename;
            rmenu[0].FontSize = script.rmenu.text_width * zoom - 1;
            rmenu[0].Margin = new Thickness(x, y, resolution_width - x - (script.rmenu.text_width + script.rmenu.text_spacing) * rmenu[0].Text.Length * zoom, resolution_height - y - script.rmenu.text_width);
            LayoutRoot.Children.Add(rmenu[0]);
            rmenu[0].Foreground = new SolidColorBrush(Colors.White);
            y += ((double)script.rmenu.line_spacing + (double)script.rmenu.text_height) * zoom;
            for (int i = 1; i < GamePage.savenumber+1; i++)
            {
                if (saver.gs[i-1].isSaved)
                {
                    rmenu[i].Text = "存档"+Convert.ToString(i)+" 记录时间："+saver.gs[i-1].savetime.ToString();
                }
                else
                {
                    rmenu[i].Text = "存档" + Convert.ToString(i) + " 无数据";
                }
                rmenu[i].FontSize = script.rmenu.text_width * zoom - 1;
                rmenu[i].Margin = new Thickness(x, y, resolution_width - x - (script.rmenu.text_width + script.rmenu.text_spacing) * rmenu[i].Text.Length * zoom, resolution_height - y - script.rmenu.text_width);
                LayoutRoot.Children.Add(rmenu[i]);
                rmenu[i].Foreground = new SolidColorBrush(Colors.White);
                rmenu[i].ManipulationCompleted += new EventHandler<ManipulationCompletedEventArgs>(RMENU_Click);
                y += ((double)script.rmenu.line_spacing + (double)script.rmenu.text_height) * zoom;
            }
        }
        private void ShowLoad()
        {
            rs = RMENU_State.STATE_load;
            if (backed) foreach (TextBlock txb in rmenu)
                {
                    try
                    {
                        txb.ManipulationCompleted -= new EventHandler<ManipulationCompletedEventArgs>(RMENU_Click);
                        LayoutRoot.Children.Remove(txb);
                    }
                    catch
                    {

                    }
                }
            else
            {
                skip.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                save.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                forw.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                back.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                load.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                exit.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                LayoutRoot.ManipulationCompleted -= new EventHandler<System.Windows.Input.ManipulationCompletedEventArgs>(LayoutRoot_ManipulationCompleted_1);
                /*if (btnwaiting)*/ Touch.FrameReported -= new TouchFrameEventHandler(Touch_FrameReported);
                LayoutRoot.Children.Add(opacitymask);
                opacitymask.Margin = new Thickness(0, 0, 0, 0);
                opacitymask.Fill = new SolidColorBrush(Colors.Black);
                opacitymask.Opacity = 0.7;
                backed = true;
            }
            rmenu = new TextBlock[GamePage.savenumber + 1];
            for (int i = 0; i < rmenu.Length; i++) rmenu[i] = new TextBlock();
            double x = 0, y = 0;
            rmenu[0].Text = script.rmenu.readname;
            rmenu[0].FontSize = script.rmenu.text_width * zoom - 1;
            rmenu[0].Margin = new Thickness(x, y, resolution_width - x - (script.rmenu.text_width + script.rmenu.text_spacing) * rmenu[0].Text.Length * zoom, resolution_height - y - script.rmenu.text_width);
            LayoutRoot.Children.Add(rmenu[0]);
            rmenu[0].Foreground = new SolidColorBrush(Colors.White);
            y += ((double)script.rmenu.line_spacing + (double)script.rmenu.text_height) * zoom;
            for (int i = 1; i < GamePage.savenumber + 1; i++)
            {
                if (saver.gs[i - 1].isSaved)
                {
                    rmenu[i].Text = "存档" + Convert.ToString(i) + " 记录时间：" + saver.gs[i - 1].savetime.ToString();
                }
                else
                {
                    rmenu[i].Text = "存档" + Convert.ToString(i) + " 无数据";
                }
                rmenu[i].FontSize = script.rmenu.text_width * zoom - 1;
                rmenu[i].Margin = new Thickness(x, y, resolution_width - x - (script.rmenu.text_width + script.rmenu.text_spacing) * rmenu[i].Text.Length * zoom, resolution_height - y - script.rmenu.text_width);
                LayoutRoot.Children.Add(rmenu[i]);
                rmenu[i].Foreground = new SolidColorBrush(Colors.White);
                rmenu[i].ManipulationCompleted += new EventHandler<ManipulationCompletedEventArgs>(RMENU_Click);
                y += ((double)script.rmenu.line_spacing + (double)script.rmenu.text_height) * zoom;
            }
        }
        private void HideRMENU()
        {
            rs = RMENU_State.STATE_null;
            foreach (TextBlock txb in rmenu)
            {
                try
                {
                    txb.ManipulationCompleted -= new EventHandler<ManipulationCompletedEventArgs>(RMENU_Click);
                    LayoutRoot.Children.Remove(txb);
                }
                catch
                {

                }
            }
            try
            {
                skip.AddButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                save.AddButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                forw.AddButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                back.AddButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                load.AddButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                exit.AddButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                backed = false;
                LayoutRoot.Children.Remove(opacitymask);
                LayoutRoot.ManipulationCompleted += new EventHandler<System.Windows.Input.ManipulationCompletedEventArgs>(LayoutRoot_ManipulationCompleted_1);
                /*if (btnwaiting)*/ Touch.FrameReported += new TouchFrameEventHandler(Touch_FrameReported);
                opacitymask.Opacity = 0;
            }
            catch
            {

            }
        }
        protected override void OnBackKeyPress(System.ComponentModel.CancelEventArgs e)
        {
            base.OnBackKeyPress(e);
            e.Cancel = true;
            if (!backed)
            {
                /*                yes.Content = "退出";
                                yes.Name = "yes";
                                no.Content = "不";
                                no.Name = "no";
                                text.Text = "真的要退出吗？";
                                text.Margin = new Thickness(30, 10, 0, 0);
                                text.Foreground = new SolidColorBrush(Colors.White);
                                yes.Margin = new Thickness(0, 50, 660, 360);
                                yes.Foreground = new SolidColorBrush(Colors.White);
                                yes.BorderBrush = new SolidColorBrush(Colors.White);
                                no.Foreground = new SolidColorBrush(Colors.White);
                                no.BorderBrush = new SolidColorBrush(Colors.White);
                                yes.Width = 140;
                                no.Margin = new Thickness(140, 50, 520, 360);
                                no.Width = 140;
                                LayoutRoot.Children.Add(text);
                                LayoutRoot.Children.Add(yes);
                                LayoutRoot.Children.Add(no);*/
                skip.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                save.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                forw.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                back.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                load.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                exit.RemoveButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                LayoutRoot.ManipulationCompleted -= new EventHandler<System.Windows.Input.ManipulationCompletedEventArgs>(LayoutRoot_ManipulationCompleted_1);
                /*if (btnwaiting)*/ Touch.FrameReported -= new TouchFrameEventHandler(Touch_FrameReported);
                LayoutRoot.Children.Add(opacitymask);
                opacitymask.Margin = new Thickness(0, 0, 0, 0);
                opacitymask.Fill = new SolidColorBrush(Colors.Black);
                opacitymask.Opacity = 0.7;
                ShowRMENU();
                backed = true;
            }
            else
            {
/*                LayoutRoot.Children.Remove(yes);
                LayoutRoot.Children.Remove(no);
                LayoutRoot.Children.Remove(text);*/
                HideRMENU();
            }

        }
        private void ReadScript()
        {
            script = new Script(inf.Game);
            script.ParseScript();
        }
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            App.landscape = true;
            base.OnNavigatedTo(e);
            if ((int)appSettings["textEffect"] == 1) showTextEffect = true;
            else showTextEffect = false;
            resolution_height = Application.Current.Host.Content.ActualWidth;
            resolution_width = Application.Current.Host.Content.ActualHeight;
            NavigationContext.QueryString.TryGetValue("game", out gamefolder);

            
            //FileOperation fo = new FileOperation();
            //MessageBox.Show(fo.GetFileContent(gamefolder+"\\uve-manifest.uvm"));

            if (btnwaiting) Touch.FrameReported += new TouchFrameEventHandler(Touch_FrameReported);
            inf = new InfoReader(gamefolder);
            //            thd = new Thread(new ThreadStart(ReadScript));
            //            thd.Start();
            //            Thread.Sleep(5000);
            if (!IsLoaded)
            {
                saver = new Save();
                savenumber = 9;
                ReadScript(); //占用主线程时间较长
                gameratio = (double)inf.Game.ScreenResolution_x / (double)inf.Game.ScreenResolution_y;
                screenratio = resolution_width / resolution_height;
                if (gameratio <= screenratio) zoom = resolution_height / inf.Game.ScreenResolution_y;
                else zoom = resolution_width / inf.Game.ScreenResolution_x;
                exit = new Button_withText("exit", new Thickness(resolution_width - 48, resolution_height - 48, 0, 0), new SolidColorBrush(Colors.Blue), new SolidColorBrush(Colors.White), 0.4, 0.5);
                skip = new Button_withText("skip", new Thickness(resolution_width - 48, resolution_height - 96, 0, 49), new SolidColorBrush(Colors.Blue), new SolidColorBrush(Colors.White), 0.4, 0.5);
                save = new Button_withText("save", new Thickness(resolution_width - 48, resolution_height - 144, 0, 97), new SolidColorBrush(Colors.Blue), new SolidColorBrush(Colors.White), 0.4, 0.5);
                load = new Button_withText("load", new Thickness(resolution_width - 48, resolution_height - 192, 0, 145), new SolidColorBrush(Colors.Blue), new SolidColorBrush(Colors.White), 0.4, 0.5);
                back = new Button_withText("back", new Thickness(resolution_width - 48, resolution_height - 240, 0, 193), new SolidColorBrush(Colors.Blue), new SolidColorBrush(Colors.White), 0.4, 0.5);
                forw = new Button_withText("forw", new Thickness(resolution_width - 48, resolution_height - 288, 0, 241), new SolidColorBrush(Colors.Blue), new SolidColorBrush(Colors.White), 0.4, 0.5);

                bg = new Image();
                LayoutRoot.Children.Add(bg);
                LayoutRoot.Children.Remove(TextRender);
                for (int i = 0; i < 1000; i++)
                {
                    img[999 - i] = new Image();
                    LayoutRoot.Children.Add(img[999 - i]);
                }
                for (int i = 0; i < imgState.Length; i++) imgState[i] = new ImageStatus();
                bool temp = false;
            tr_f:
                try
                {
                    while (cmdcount < script.scriptmain.Length && (temp = CmdExec(ref script.scriptmain[cmdcount++], ref script.num, ref script.str, ref script.sub))) ;

                }
                catch (Exception exc)
                {
                    if (!allowError)
                    {
                        MessageBox.Show("脚本错误\n线程:1(0x01)\n错误信息:" + exc.Message + "\n脚本行数:" + (cmdcount - 1) + "\n脚本内容:" + script.scriptmain[cmdcount - 1], "脚本错误信息", MessageBoxButton.OK);

                    }
                    else if (!temp) goto tr_f;

                }
                skip.AddButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                save.AddButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                back.AddButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                forw.AddButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                exit.AddButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                load.AddButton(LayoutRoot, new EventHandler<System.Windows.Input.ManipulationStartedEventArgs>(ButtonClick_buttonwithtext));
                IsLoaded = true;
            }
            else if (playing)
            {
                try
                {
                    me.SetSource(new IsolatedStorageFileStream(currentbgm, FileMode.Open, isf));
                    me.Play();
                }
                catch { }
            }
            try
            {
                BackgroundAudioPlayer.Instance.Play();
            }
            catch { }
        }
        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            App.landscape = false;
            Touch.FrameReported -= new TouchFrameEventHandler(Touch_FrameReported);
            base.OnNavigatedFrom(e);
            if (playing)
            {
                me.Pause();
            }
            try
            {
                if (BackgroundAudioPlayer.Instance.CanPause)
                    BackgroundAudioPlayer.Instance.Pause();
            }
            catch
            {

            }
        }
        private void Touch_FrameReported(object sender, TouchFrameEventArgs e)
        {
            Touch.FrameReported -= new TouchFrameEventHandler(Touch_FrameReported);
            TouchPoint point = e.GetPrimaryTouchPoint(null);
            this.Dispatcher.BeginInvoke(() =>
            {
                if (btnwaiting)
                {
                    btnwaiting = false;
                    double x = 0, y = 0;
                    bool IsButton = false;
                    if (ori == 0)//左横屏
                    {
                        x = point.Position.Y;
                        y = GamePage.resolution_height - point.Position.X;
                    }
                    else if (ori == 1)//右横屏
                    {
                        x = GamePage.resolution_width - point.Position.Y;
                        y = point.Position.X;
                    }
                    foreach (BTN btn in btn_collections)
                    {
                        if (btn.BTNManip(x, y))
                        {
                            script.num[(int)btn_VAR_to_load] = new VAR_NUM(btn.VAR_TO_LOAD);
                            btn_collections.Clear();
                            IsButton = true;
                            while (cmdcount < script.scriptmain.Length && CmdExec(ref script.scriptmain[cmdcount++], ref script.num, ref script.str, ref script.sub)) ;
                            break;
                        }
                    }
                    if (!IsButton)
                    {
                        script.num[(int)btn_VAR_to_load] = new VAR_NUM(0);
                        btn_collections.Clear();
                        try
                        {
                            while (cmdcount < script.scriptmain.Length && CmdExec(ref script.scriptmain[cmdcount++], ref script.num, ref script.str, ref script.sub)) ;
                        }
                        catch (Exception exc)
                        {
                            if (!allowError)
                            {
                                MessageBox.Show("脚本错误\n线程:1(0x01)\n错误信息:" + exc.Message + "\n脚本行数:" + (cmdcount - 1) + "\n脚本内容:" + script.scriptmain[cmdcount - 1], "脚本错误信息", MessageBoxButton.OK);

                            }
                        }
                    }
                }
            });
        }
        private void LayoutRoot_ManipulationCompleted_1(object sender, System.Windows.Input.ManipulationCompletedEventArgs e)
        {
            this.Dispatcher.BeginInvoke(() =>
            {
                if (!btnwaiting)
                {
                    if (!click_button && !selecting)
                    {
                        backing = false;
                        if (!skipping && !waiting)
                        {
                            if (rending)
                            {
                                rending = false;
                                TextRender.Text += ToShow;
                                ToShow = "";
                            }
                            else
                            {
                            tr_m:
                                try
                                {
                                    while (cmdcount < script.scriptmain.Length && CmdExec(ref script.scriptmain[cmdcount++], ref script.num, ref script.str, ref script.sub)) ;
                                    textHistory.Add(new History(wndst, TextRender.Text + ToShow));
                                }
                                catch (Exception exc)
                                {
                                    if (!allowError)
                                    {
                                        MessageBox.Show("脚本错误\n线程:1(0x01)\n错误信息:" + exc.Message + "\n脚本行数:" + (cmdcount - 1) + "\n脚本内容:" + script.scriptmain[cmdcount - 1], "脚本错误信息", MessageBoxButton.OK);

                                    }
                                    else goto tr_m;
                                }
                            }
                        }
                        else
                        {
                            skipping = false;
                        }
                    }
                    else
                    {
                        click_button = false;
                    }
                }
                else
                {


                }
            });
        }
        private void Save()
        {
            //MemoryStream stream = new MemoryStream();
            //IsolatedStorageFileStream serstream = new IsolatedStorageFileStream(gamefolder + ".bin", FileMode.OpenOrCreate, isf);
            //ser.Serialize(saver, stream);
            //serstream.Position = 0;
            //stream.Position = 0;
            //int chunkSize = 4096;
            //byte[] bytes = new byte[chunkSize];
            //int byteCount;
            //while ((byteCount = stream.Read(bytes, 0, chunkSize)) > 0)
            //{
            //    serstream.Write(bytes, 0, byteCount);
            //}
            //serstream.Close();
            MemoryStream ms = new MemoryStream();
            ser.WriteObject(ms, saver);
            ms.Position = 0;
            using (IsolatedStorageFileStream file = isf.CreateFile(gamefolder + ".xml"))
            {
                int chunkSize = 4096;
                byte[] bytes = new byte[chunkSize];
                int byteCount;

                while ((byteCount = ms.Read(bytes, 0, chunkSize)) > 0)
                {
                    file.Write(bytes, 0, byteCount);
                }
                file.Close();
            }
            ms.Close();

        }
        private Save Load()
        {
            Save returned=new Save();
            try
            {
                System.Xml.XmlReader reader = System.Xml.XmlReader.Create(new IsolatedStorageFileStream(gamefolder + ".xml", FileMode.OpenOrCreate, isf));
                returned = (ser.ReadObject(reader)) as Save;
            }
            catch { }
            //Stream serstream = new IsolatedStorageFileStream(gamefolder + ".bin", FileMode.Open, isf);
            //returned = (Save)ser.Deserialize(serstream);
            //serstream.Close();
            return returned;
        }
        #region 指令解析
        public bool CmdExec(ref string cmd, ref VAR_NUM[] num, ref VAR_STR[] str, ref Sub[] sub)
        {
            ///<summary>
            /// 用本方法来执行单行命令
            /// return true:继续执行
            /// return false:等待用户交互
            ///</summary>
            if (!IsBGShowed)
            {
                saver = new Save();
                ser = new DataContractSerializer(Type.GetTypeFromHandle(Type.GetTypeHandle(saver)));
                textboximage = new Image();
                LayoutRoot.Children.Add(textboximage);
                LayoutRoot.Children.Add(TextRender);
                LayoutRoot.Children.Add(me);
                me.MediaEnded += new RoutedEventHandler(Media_Ended);
                IsBGShowed = true; 
                isf = IsolatedStorageFile.GetUserStoreForApplication();
                if (isf.FileExists(gamefolder + ".xml"))
                {
                    saver = Load();
                    //System.Xml.XmlReader reader = System.Xml.XmlReader.Create(new IsolatedStorageFileStream(gamefolder + ".xml", FileMode.OpenOrCreate, isf));
                    //saver = (ser.ReadObject(reader)) as Save;
                }
                else
                {
                    Save();
                }

                //if (isf.FileExists(gamefolder + ".bin"))
                //{
                //    saver = Load();
                //}
                //else
                //{
                //    Save();
                //}



                 //if (appSettings.Contains(gamefolder))
                //{
                //    saver = (Save)appSettings[gamefolder];
                //}
                //else
                //{
                //    appSettings.Add(gamefolder, saver);
                //    appSettings.Save();
                //}
            }
            if (Text.IsText(cmd))
            {
                #region 文字输出
                int tempstringcount = 0;
                string tempstring = "";
                for (; tempstringcount < cmd.Length; tempstringcount++)
                {



                    if (cmd[tempstringcount] == '@')
                    {
                        if (tempstringcount != cmd.Length - 1)
                        {
                            cmdcount--;
                            string temp = "";
                            for (int i = tempstringcount + 1; i < cmd.Length; i++)
                            {
                                temp += cmd[i];
                            }
                            cmd = temp;
                            //RendText(tempstring, ref TextRender, ref clear, ref textrendtimer);
                            RendText(tempstring, ref TextRender, ref clear, ref t_rendtimer);
                        }
                        else
                        {
                            //                            tempstring += cmd[tempstringcount];
                            //RendText(tempstring + '\n', ref TextRender, ref clear, ref textrendtimer);
                            RendText(tempstring + '\n', ref TextRender, ref clear, ref t_rendtimer);
                        }
                        tempstring = "";
                        return false;
                    }
                    else if (cmd[tempstringcount] == '\\')
                    {
                        if (tempstringcount != cmd.Length - 1)
                        {
                            cmdcount--;
                            string temp = "";
                            for (int i = tempstringcount + 1; i < cmd.Length; i++)
                            {
                                temp += cmd[i];
                            }
                            cmd = temp;
                            //RendText(tempstring, ref TextRender, ref clear, ref textrendtimer);
                            RendText(tempstring, ref TextRender, ref clear, ref t_rendtimer);
                            clear = 1;
                        }
                        else
                        {
                            //RendText(tempstring + '\n', ref TextRender, ref clear, ref textrendtimer);
                            RendText(tempstring + '\n', ref TextRender, ref clear, ref t_rendtimer);

                            clear = 1;
                        }
                        tempstring = "";
                        return false;
                    }
                    else if (tempstringcount == cmd.Length - 1)
                    {
                        tempstring = tempstring + cmd[tempstringcount];
                        //RendText(tempstring + '\n', ref TextRender, ref clear, ref textrendtimer);
                        RendText(tempstring + '\n', ref TextRender, ref clear, ref t_rendtimer);
                        tempstring = "";
                        return true;
                    }
                    else
                    {
                        tempstring += cmd[tempstringcount];
                    }


                }

                #endregion
            }
            else
            {
                if (cmd[0] != '*')
                    return CmdExec(cmd, new string[0]);
            }
            return true;
        }

        #endregion
        #region Sub指令解析
        public void CmdExec(string[] subcmd, ref Grid LayoutRoot, ref TextBlock TextRender, ref VAR_NUM[] num, ref VAR_STR[] str, string[] subparam, ref Sub[] sub, ref Image[] img)
        {
            ///<summary>
            /// 用本方法来执行sub
            ///</summary>
            for (int j = 0; j < subcmd.Length; j++)
            {
                subcmd[j] = subcmd[j].Replace("\t", "");
                CmdExec(subcmd[j], subparam);
                #region 废弃代码
                /*if (StringToolkit.GetBefore(subcmd[j], ' ') == "getparam")
                {
                    StringToolkit.CutParam(subcmd[j].Remove(0, 9), ',', out subparams);
                    for (int k = 0; k < subparams.Length; k++)
                    {
                        if (subparams[k][0] == '%')
                        {
                            subparams[k] = subparams[k].Remove(0, 1);
                            try
                            {
                                num[int.Parse(subparams[k])] = new VAR_NUM(Convert.ToDouble(subparam[k]));
                            }
                            catch
                            {
                                foreach (DEF def in script.defas)
                                {
                                    if (!def.IsString && def.def == subparams[k])
                                    {
                                        num[Convert.ToInt32((double)def.VAR)] = new VAR_NUM(Convert.ToDouble(subparam[k]));
                                    }
                                }
                            }
                        }
                        else if (subparams[k][0] == '$')
                        {
                            subparams[k] = subparams[k].Remove(0, 1);
                            try
                            {
                                str[int.Parse(subparams[k])] = new VAR_STR(subparam[k]);
                            }
                            catch
                            {
                                foreach (DEF def in script.defas)
                                {
                                    if (def.IsString && def.def == subparams[k])
                                    {
                                        str[Convert.ToInt32((double)def.VAR)] = new VAR_STR(subparam[k]);
                                    }
                                }
                            }
                        }
                    }
                }
                else if (StringToolkit.GetBefore(subcmd[j], ' ') == "if")
                {
                    StringToolkit.CutParam(subcmd[j].Remove(0, 3), ' ', out subparams);
                    if (Condition.JudgeCondition(subparams[0], num))
                    {
                        if (StringToolkit.GetBefore(subcmd[j].Remove(0, 4 + subparams[0].Length), ' ') == "setwindow")
                        {
                            StringToolkit.CutParam(subcmd[j].Remove(0, 14 + subparams[0].Length), ',', out subparams);
                            for (int i = 0; i < subparams.Length; i++)
                            {
                                subparams[i] = StringToolkit.CalcStr(subparams[i], str, num);
                            }
                            wndst.x = int.Parse(subparams[0]);
                            wndst.y = int.Parse(subparams[1]);
                            wndst.charcount = int.Parse(subparams[2]);
                            wndst.linecount = int.Parse(subparams[3]);
                            wndst.charwidth = int.Parse(subparams[4]);
                            wndst.charheight = int.Parse(subparams[5]);
                            wndst.charspacing = int.Parse(subparams[6]);
                            wndst.linespacing = int.Parse(subparams[7]);
                            wndst.textspeed = double.Parse(subparams[8]);
                            if (int.Parse(subparams[9]) == 0) wndst.isthick = false;
                            else wndst.isthick = true;
                            if (int.Parse(subparams[10]) == 0) wndst.isshadow = false;
                            else wndst.isthick = true;
                            wndst.color = subparams[11].Replace("\"", "");
                            if (subparams.Length > 12)
                            {
                                wndst.textblockx1 = int.Parse(subparams[12]);
                                wndst.textblocky1 = int.Parse(subparams[13]);
                                if (subparams.Length > 14)
                                {
                                    wndst.textblockx2 = int.Parse(subparams[14]);
                                    wndst.textblocky2 = int.Parse(subparams[15]);
                                }
                                else
                                {
                                    wndst.textblockx2 = (int)resolution_width;
                                    wndst.textblocky2 = (int)resolution_height;
                                }
                            }
                            wndst.SetWindow(ref TextRender,ref textboximage);
                        }
                        else if (StringToolkit.GetBefore(subcmd[j].Remove(0, 4 + subparams[0].Length), ' ') == "csp")
                        {

                            StringToolkit.CutParam(subcmd[j].Remove(0, 3), ' ', out subparams);
                            StringToolkit.CutParam(subcmd[j].Remove(0, 8 + subparams[0].Length), ',', out subparams);
                            for (int i = 0; i < subparams.Length; i++)
                            {
                                subparams[i] = StringToolkit.CalcStr(subparams[i], str, num);
                            }
                            int csp = int.Parse(subparams[0]);
                            if (csp != -1)
                            {
                                img[csp].Opacity = 0;
                            }
                            else
                            {
                                for (int i = 0; i < 1000; i++)
                                {
                                    img[i].Opacity = 0;
                                }
                            }
                        }
                    }
                }
                else if (StringToolkit.GetBefore(subcmd[j], ' ') == "setwindow")
                {
                    StringToolkit.CutParam(subcmd[j].Remove(0, 10), ',', out subparams);
                    for (int i = 0; i < subparams.Length; i++)
                    {
                        subparams[i] = StringToolkit.CalcStr(subparams[i], str, num);
                    }
                    wndst.x = int.Parse(subparams[0]);
                    wndst.y = int.Parse(subparams[1]);
                    wndst.charcount = int.Parse(subparams[2]);
                    wndst.linecount = int.Parse(subparams[3]);
                    wndst.charwidth = int.Parse(subparams[4]);
                    wndst.charheight = int.Parse(subparams[5]);
                    wndst.charspacing = int.Parse(subparams[6]);
                    wndst.linespacing = int.Parse(subparams[7]);
                    wndst.textspeed = int.Parse(subparams[8]);
                    if (int.Parse(subparams[9]) == 0) wndst.isthick = false;
                    else wndst.isthick = true;
                    if (int.Parse(subparams[10]) == 0) wndst.isshadow = false;
                    else wndst.isthick = true;
                    wndst.color = subparams[11].Replace("\"", "");
                    if (subparams.Length > 12)
                    {
                        wndst.textblockx1 = int.Parse(subparams[12]);
                        wndst.textblocky1 = int.Parse(subparams[13]);
                        if (subparams.Length > 14)
                        {
                            wndst.textblockx2 = int.Parse(subparams[14]);
                            wndst.textblocky2 = int.Parse(subparams[15]);
                        }
                    }
                    wndst.SetWindow(ref TextRender,ref textboximage);
                }
                else if (StringToolkit.GetBefore(subcmd[j], ' ') == "msp")
                {
                    string[] param;
                    StringToolkit.CutParam(subcmd[j].Remove(0, 4), ',', out param);
                    int imgcount, c_x, c_y;
                    for (int i = 0; i < param.Length; i++)
                    {
                        param[i] = StringToolkit.CalcStr(param[i], str, num);
                    }
                    imgcount = int.Parse(param[0]);
                    c_x = int.Parse(param[1]);
                    c_y = int.Parse(param[2]);
                    double l = img[imgcount].Margin.Left, t = img[imgcount].Margin.Top, r = img[imgcount].Margin.Right, b = img[imgcount].Margin.Bottom;
                    img[imgcount].Margin = new Thickness(l + c_x * zoom, t + c_y * zoom, r - c_x * zoom, b - c_y * zoom);
                }
                else if (StringToolkit.GetBefore(subcmd[j], ' ') == "amsp")
                {
                    string[] param;
                    StringToolkit.CutParam(subcmd[j].Remove(0, 5), ',', out param);
                    int imgcount, to_x, to_y;
                    double p_x, p_y;
                    for (int i = 0; i < param.Length; i++)
                    {
                        param[i] = StringToolkit.CalcStr(param[i], str, num);
                    }
                    imgcount = int.Parse(param[0]);
                    to_x = int.Parse(param[1]);
                    to_y = int.Parse(param[2]);
                    p_x = img[imgcount].Width;
                    p_y = img[imgcount].Height;
                    double l = img[imgcount].Margin.Left, t = img[imgcount].Margin.Top, r = img[imgcount].Margin.Right, b = img[imgcount].Margin.Bottom;
                    img[imgcount].Margin = new Thickness(to_x * zoom, to_y * zoom, resolution_width - p_x - to_x * zoom, resolution_height - p_y - to_y * zoom);
                }
                else if (StringToolkit.GetBefore(subcmd[j], ' ') == "lsp")
                {
                    string[] param;
                    string path;
                    StringToolkit.CutParam(subcmd[j].Remove(0, 4), ',', out param);
                    for (int i = 0; i < param.Length; i++)
                    {
                        param[i] = StringToolkit.CalcStr(param[i], str, num);
                    }
                    int imgcount;
                    double x = 0, y = 0, opacity = 1, image_x = 0, image_y = 0;
                    imgcount = int.Parse(param[0]);
                    BitmapImage tempimg = new BitmapImage();
                    path = Path.Combine(inf.Game.GameFolder, inf.Game.DataFolder, StringToolkit.GetInside(param[1], '\"').Replace(":a;", ""));

                    if (param.Length >= 3)
                    {
                        x = double.Parse(param[2]);
                        y = double.Parse(param[3]);

                        if (param.Length >= 5)
                        {
                            opacity = double.Parse(param[4]) / 100;
                        }


                    }

                    if (path.EndsWith(".png") || path.EndsWith(".PNG"))
                    {
                        try
                        {
                            PNGTools.PNGImageResolution(new IsolatedStorageFileStream(path, FileMode.Open, isf), ref image_x, ref image_y);
                            tempimg.SetSource(new IsolatedStorageFileStream(path, FileMode.Open, isf));
                            img[imgcount].Source = tempimg;
                            img[imgcount].HorizontalAlignment = HorizontalAlignment.Left;
                            img[imgcount].Opacity = 1;
                            img[imgcount].Margin = new Thickness(x * zoom, y * zoom, resolution_width - (x + image_x) * zoom, resolution_height - (y + image_y) * zoom);
                        }
                        catch
                        {

                        }
                    }
                    else
                    {
                        try
                        {
                            Alpha.AlphaBlend(new IsolatedStorageFileStream(path, FileMode.Open, isf), ref img[imgcount]);
                            img[imgcount].HorizontalAlignment = HorizontalAlignment.Left;
                            img[imgcount].Opacity = 1;
                            img[imgcount].Margin = new Thickness(x * zoom, y * zoom, resolution_width - img[imgcount].Width - x * zoom, resolution_height - img[imgcount].Height - y * zoom);
                        }
                        catch
                        {

                        }
                    }



                }*/
#endregion
            }
        }
        #endregion
        #region 通用指令解析
        public bool CmdExec(string cmd, string[] sbparam)
        {
            string[] param;
            string path;

            cmd = cmd.Replace("/", "\\");
            switch (StringToolkit.GetBefore(cmd, ' '))
            {
                case "systemcall":
                    switch (cmd.Remove(0, 11))
                    {
                        case "save":
                            ShowSave();
                            break;
                        case "load":
                            ShowLoad();
                            break;
                        case "rmenu":
                            ShowRMENU();
                            break;
                        case "windowerase":

                            break;
                        case "skip":

                            break;
                        case "lookback":

                            break;
                        case "reset":
                            IsBGShowed = false;
                            for (int i = 0; i < 1000; i++)
                            {
                                img[i].Source = null;

                            }
                            for (int i = 0; i < 100; i++)
                            {
                                if (dwave_ogg[i] != null) dwave_ogg[i].Stop();
                                if (dwave_wav[i] != null) dwave_wav[i].Dispose();
                            }
                            me.Stop();
                            ReadScript();
                            btnwaiting = false;
                            if (bgm_ogg != null) bgm_ogg.Stop();
                            LayoutRoot.Children.Remove(TextRender);
                            //                    LayoutRoot.Children.Remove(bg);
                            bg.Source = null;
                            LayoutRoot.Children.Remove(textboximage);
                            me.MediaEnded -= new RoutedEventHandler(Media_Ended);
                            LayoutRoot.Children.Remove(me);
                            playing = false;
                            mp3looping = false;
                            skipping = false;
                            BackgroundAudioPlayer.Instance.Close();
                            cmdcount = 0;
                            break;
                    }
                    break;
                case "bg":
                    StringToolkit.CutParam(cmd.Remove(0, 3), ',', out param);
                    if (param[0].Contains("\""))
                    {
                        bg.Opacity = 1;
                        param[0] = StringToolkit.CalcStr(param[0], script.str, script.num);
                        BitmapImage bg_bi = new BitmapImage();
                        path = Path.Combine(inf.Game.GameFolder, inf.Game.DataFolder, param[0]);
                        bgState.Alignment = HorizontalAlignment.Left;
                        try
                        {
                            bg_bi.SetSource(new IsolatedStorageFileStream(path, FileMode.Open, isf));
                            bg.Source = bg_bi;
                            bg.HorizontalAlignment = HorizontalAlignment.Left;
                            bgState.Path = path;
                            bgState.IsAlpha = false;
                            bgState.Opacity = 1;
                            bgState.Margin = new Thickness(0, 0, 0, 0);
                        }
                        catch (Exception exc)
                        {
                            if (!allowError)
                            {
                                MessageBox.Show("脚本错误\n线程:1(0x01)\n错误信息:" + exc.Message + "\n脚本行数:" + (cmdcount - 1) + "\n脚本内容:" + script.scriptmain[cmdcount - 1], "脚本错误信息", MessageBoxButton.OK);

                            }
                        }
                    }
                    else
                    {
                        if (param[0] == "black")
                        {
                            bg.Opacity = 0;
                        }
                    }
                    break;
                case "bgm":
                    if ((int)appSettings["bgmOut"] == 1)
                    {
                        StringToolkit.CutParam(cmd.Remove(0, 4), ',', out param);
                        for (int i = 0; i < param.Length; i++)
                        {
                            param[i] = StringToolkit.CalcStr(param[i], script.str, script.num);
                        }
                        me.Stop();
                        if (bgm_ogg != null && bgm_ogg.MediaState == MediaState.Playing) bgm_ogg.Stop();
                        string AudioUri;
                        AudioUri = Path.Combine(inf.Game.GameFolder, inf.Game.DataFolder, param[0]);
                        currentbgm = AudioUri;
                        if (AudioUri.EndsWith(".ogg") || AudioUri.EndsWith(".OGG"))
                        {
                            if ((int)appSettings["audioBackground"] == 1) if (BackgroundAudioPlayer.Instance.PlayerState == PlayState.Playing) BackgroundAudioPlayer.Instance.Stop();
                            bgm_ogg = new OggSong(new IsolatedStorageFileStream(AudioUri, FileMode.Open, isf));
                            bgm_ogg.Repeat = true;
                            bgm_ogg.Play();
                        }
                        else
                        {
                            if ((int)appSettings["audioBackground"] == 1)
                            {
                                if (BackgroundAudioPlayer.Instance.PlayerState == PlayState.Playing) BackgroundAudioPlayer.Instance.Stop();
                                BackgroundAudioPlayer.Instance.Track = new AudioTrack(new Uri(AudioUri, UriKind.Relative), "背景音乐", inf.Game.GameName, null, null, null, EnabledPlayerControls.Pause);
                                //BackgroundAudioPlayer.Instance.Play();
                            }
                            else
                            {
                                me.SetSource(new IsolatedStorageFileStream(AudioUri, FileMode.Open, isf));
                                mp3looping = true;
                                me.Play();
                            }
                        }
                    }
                    break;
                case "bgmstop":
                    me.Stop();
                    if (BackgroundAudioPlayer.Instance.PlayerState == PlayState.Playing) BackgroundAudioPlayer.Instance.Stop();
                    if (bgm_ogg != null && bgm_ogg.MediaState == MediaState.Playing) bgm_ogg.Stop();
                    break;
                case "defbtn":
                    BTN.BTN_Pic = Path.Combine(inf.Game.GameFolder, inf.Game.DataFolder, StringToolkit.GetInside(cmd.Remove(0, 7), '\"'));
                    break;
                case "btn":
                    StringToolkit.CutParam(cmd.Remove(0, 4), ',', out param);
                    for (int i = 0; i < param.Length; i++)
                    {
                        param[i] = StringToolkit.CalcStr(param[i], script.str, script.num);
                    }
                    double b_x, b_y, b_width, b_height, b_num, cut_x, cut_y;
                    b_num = double.Parse(param[0]);
                    b_x = double.Parse(param[1]);
                    b_y = double.Parse(param[2]);
                    b_width = double.Parse(param[3]);
                    b_height = double.Parse(param[4]);
                    cut_x = double.Parse(param[5]);
                    cut_y = double.Parse(param[6]);
                    btn_collections.Add(new BTN(new Thickness(b_x * zoom, b_y * zoom, GamePage.resolution_width - (b_x + b_width) * zoom, resolution_height - (b_y + b_height) * zoom), b_num, cut_x, cut_y, cut_x, cut_y, LayoutRoot));
                    break;
                case "exbtn_d":
                    //Ignore
                    break;
                case "exbtn":
                    StringToolkit.CutParam(cmd.Remove(0, 6), ',', out param);
                    for (int i = 0; i < param.Length; i++)
                    {
                        param[i] = StringToolkit.CalcStr(param[i], script.str, script.num);
                    }
                    int count_btn = int.Parse(param[0]);
                    btn_collections.Add(new BTN(img[count_btn].Margin, double.Parse(param[1])));
                    break;
                case "btnwait":
                    btn_VAR_to_load = int.Parse(cmd.Remove(0, 9));
                    btnwaiting = true;
                    Touch.FrameReported += new TouchFrameEventHandler(Touch_FrameReported);
                    return false;
                case "wait":
                    waiting = true;
                    skipping = false;
                    waittimer.Duration = TimeSpan.FromMilliseconds(double.Parse(cmd.Remove(0, 5)));
                    waittimer.Begin();
                    return false;
                case "click":

                    return false;
                case "end":
                    MessageBox.Show("脚本主线程已结束，返回值为0", "信息", MessageBoxButton.OK);
                    IsBGShowed = false;
                    for (int i = 0; i < 1000; i++)
                    {
                        LayoutRoot.Children.Remove(img[i]);
                        img[i] = null;
                    }
                    for (int i = 0; i < 100; i++)
                    {
                        if (dwave_ogg[i] != null) dwave_ogg[i].Stop();
                        if (dwave_wav[i] != null) dwave_wav[i].Dispose();
                    }
                    /*if (btnwaiting)*/ Touch.FrameReported -= new TouchFrameEventHandler(Touch_FrameReported);
                    btnwaiting = false;
                    if (bgm_ogg != null) bgm_ogg.Stop();
                    LayoutRoot.Children.Remove(TextRender);
                    LayoutRoot.Children.Remove(bg);
                    LayoutRoot.Children.Remove(textboximage);
                    me.MediaEnded -= new RoutedEventHandler(Media_Ended);
                    LayoutRoot.Children.Remove(me);
                    playing = false;
                    mp3looping = false;
                    skipping = false;
                    BackgroundAudioPlayer.Instance.Close();
                    cmdcount = 0;
                    this.NavigationService.GoBack();
                    return false;
                case "csp":
                    StringToolkit.CutParam(cmd.Remove(0, 4), ',', out param);
                    int csp;
                    for (int i = 0; i < param.Length; i++)
                    {
                        param[i] = StringToolkit.CalcStr(param[i], script.str, script.num);
                    }
                    csp = int.Parse(param[0]);

                    if (csp != -1)
                    {
                        img[csp].Opacity = 0;
                        imgState[csp].Path = "";
                    }
                    else
                    {
                        for (int i = 0; i < 1000; i++)
                        {
                            img[i].Opacity = 0;
                            imgState[i].Path = "";
                        }
                    }
                    break;
                case "mov":
                    StringToolkit.CutParam(cmd.Remove(0, 4), ',', out param);
                    if (param[0][0] == '%')
                    {
                        VAR_NUM tmp = new VAR_NUM(0);
                        try
                        {
                            tmp = new VAR_NUM(double.Parse(param[1]));
                        }
                        catch
                        {
                            
                        }
                        script.num[int.Parse(param[0].Remove(0, 1))] = tmp;
                    }
                    else
                    {

                    }
                    break;
                case "stop":
                    for (int i = 0; i < dwave_ogg.Length; i++)
                    {
                        if (dwave_ogg[i]!=null) dwave_ogg[i].Stop();
                    }
                    for (int i = 0; i < dwave_wav.Length; i++)
                    {
                        if (dwave_wav[i]!=null) dwave_wav[i].Dispose();
                    }
                    if (dwave_ogg_u != null) dwave_ogg_u.Stop();
                    if (dwave_wav_u != null) dwave_wav_u.Dispose();
                    try
                    {
                        BackgroundAudioPlayer.Instance.Close();
                    }
                    catch
                    {

                    }
                    me.Stop();
                    break;
                case "mesbox":
                    MessageBox.Show(StringToolkit.GetInside(cmd, '\"'));
                    break;
                case "lsp":
                    StringToolkit.CutParam(cmd.Remove(0, 4), ',', out param);
                    int imgcount;
                    double x = 0, y = 0, opacity = 1, image_x = 0, image_y = 0;

                    for (int i = 0; i < param.Length; i++)
                    {
                        param[i] = StringToolkit.CalcStr(param[i], script.str, script.num);
                    }
                    imgcount = int.Parse(param[0]);
                    BitmapImage tempimg = new BitmapImage();
                    try
                    {
                        path = Path.Combine(inf.Game.GameFolder, inf.Game.DataFolder, param[1].Replace(":a;", "").Replace("\"", ""));

                        if (param.Length >= 3)
                        {
                            x = double.Parse(param[2]);
                            y = double.Parse(param[3]);

                            if (param.Length >= 5)
                            {
                                opacity = double.Parse(param[4]) / 100;
                            }


                        }

                        if (param[1].StartsWith(":a;") && (path.EndsWith("jpg") || path.EndsWith("JPG")))
                        {
                            Alpha.AlphaBlend(new IsolatedStorageFileStream(path, FileMode.Open, isf), ref img[imgcount]);
                            img[imgcount].HorizontalAlignment = HorizontalAlignment.Left;
                            img[imgcount].Opacity = 1;
                            img[imgcount].Margin = new Thickness(x * zoom, y * zoom, resolution_width - img[imgcount].Width / 2 - x * zoom, resolution_height - img[imgcount].Height - y * zoom);
                            imgState[imgcount].Alignment = HorizontalAlignment.Left;
                            imgState[imgcount].Opacity = 1;
                            imgState[imgcount].Margin = new Thickness(x * zoom, y * zoom, resolution_width - img[imgcount].Width / 2 - x * zoom, resolution_height - img[imgcount].Height - y * zoom);
                            imgState[imgcount].IsAlpha = true;
                            imgState[imgcount].Path = path;
                        }
                        else
                        {
                            IsolatedStorageFileStream fs = new IsolatedStorageFileStream(path, FileMode.Open, isf);
                            if (path.EndsWith("jpg") || path.EndsWith("JPG"))
                            {
                                Size jpgsize = new Size();
                                JPGTools.getJpgSize(fs, out jpgsize);
                                image_x = jpgsize.Width;
                                image_y = jpgsize.Height;
                            }
                            else
                            {
                                PNGTools.PNGImageResolution(fs, ref image_x, ref image_y);
                            }
                            tempimg.SetSource(fs);
                            img[imgcount].Source = tempimg;
                            img[imgcount].HorizontalAlignment = HorizontalAlignment.Left;
                            img[imgcount].Opacity = 1;
                            img[imgcount].Margin = new Thickness(x * zoom, y * zoom, resolution_width - (x + image_x) * zoom, resolution_height - (y + image_y) * zoom);
                            imgState[imgcount].Alignment = HorizontalAlignment.Left;
                            imgState[imgcount].Opacity = 1;
                            imgState[imgcount].Margin = new Thickness(x * zoom, y * zoom, resolution_width - (x + image_x) * zoom, resolution_height - (y + image_y) * zoom);
                            imgState[imgcount].Path = path;
                            imgState[imgcount].IsAlpha = false;
                            fs.Close();
                        }
                    }
                    catch (Exception exc)
                    {
                        if (!allowError)
                        {
                            MessageBox.Show("脚本错误\n线程:1(0x01)\n错误信息:" + exc.Message + "\n脚本行数:" + (cmdcount - 1) + "\n脚本内容:" + script.scriptmain[cmdcount - 1], "脚本错误信息", MessageBoxButton.OK);
                        }
                    }
                    break;
                case "print":
                    //ignore
                    break;
                case "if":
                    StringToolkit.CutParam(cmd.Remove(0, 3), ' ', out param);
                    if (Condition.JudgeCondition(param[0], script.num))
                    {
                        string if_s = cmd.Remove(0, 3 + param[0].Length + 1);
                        string[] if_c;
                        StringToolkit.CutParam(if_s, ':', out if_c);
                        CmdExec(if_c, ref LayoutRoot, ref TextRender, ref script.num, ref script.str, new string[0], ref script.sub, ref img);
                    }
                    break;
                case "mp3":
                    if ((int)appSettings["mp3Out"] == 1)
                    {
                        StringToolkit.CutParam(cmd.Remove(0, 4), ',', out param);
                        for (int i = 0; i < param.Length; i++)
                        {
                            param[i] = StringToolkit.CalcStr(param[i], script.str, script.num);
                        }
                        mp3looping = false;
                        playing = true;
                        path = Path.Combine(inf.Game.GameFolder, inf.Game.DataFolder, param[0]);
                        try
                        {
                        me.SetSource(new IsolatedStorageFileStream(path, FileMode.Open, isf));
                        me.Play();
                        }
                        catch (Exception exc)
                        {
                            if (!allowError)
                            {
                                MessageBox.Show("脚本错误\n线程:1(0x01)\n错误信息:" + exc.Message + "\n脚本行数:" + (cmdcount - 1) + "\n脚本内容:" + script.scriptmain[cmdcount - 1], "脚本错误信息", MessageBoxButton.OK);

                            }
                        }
                    }
                    break;
                case "mp3loop":
                    if ((int)appSettings["mp3Out"] == 1)
                    {
                        StringToolkit.CutParam(cmd.Remove(0, 8), ',', out param);
                        for (int i = 0; i < param.Length; i++)
                        {
                            param[i] = StringToolkit.CalcStr(param[i], script.str, script.num);
                        }
                        mp3looping = true;
                        playing = true;
                        path = Path.Combine(inf.Game.GameFolder, inf.Game.DataFolder, param[0]);
                        currentbgm = path;
                        try
                        {
                            me.SetSource(new IsolatedStorageFileStream(path, FileMode.Open, isf));
                            me.Play();
                        }
                        catch (Exception exc)
                        {
                            if (!allowError)
                            {
                                MessageBox.Show("脚本错误\n线程:1(0x01)\n错误信息:" + exc.Message + "\n脚本行数:" + (cmdcount - 1) + "\n脚本内容:" + script.scriptmain[cmdcount - 1], "脚本错误信息", MessageBoxButton.OK);

                            }
                        }
                    }
                    break;
                case "mpegplay":
                    if ((int)appSettings["videoPlay"] == 1)
                    {

                        StringToolkit.CutParam(cmd.Remove(0, 9), ',', out param);
                        for (int i = 0; i < param.Length; i++)
                        {
                            param[i] = StringToolkit.CalcStr(param[i], script.str, script.num);
                        }
                        path = Path.Combine(inf.Game.GameFolder, inf.Game.DataFolder, param[0]);

                        MediaPlayerLauncher mediaPlayerLauncher = new MediaPlayerLauncher();
                        mediaPlayerLauncher.Media = new Uri(path, UriKind.Relative);
                        mediaPlayerLauncher.Location = MediaLocationType.Data;
                        mediaPlayerLauncher.Controls = MediaPlaybackControls.Pause | MediaPlaybackControls.Stop;
                        mediaPlayerLauncher.Orientation = MediaPlayerOrientation.Landscape;
                        mediaPlayerLauncher.Show();

                    }
                    break;
                case "goto":
                    Label.GotoLabel(cmd.Remove(0, 5), script.lab, ref cmdcount);
                    break;
                case "skip":
                    cmdcount += int.Parse(cmd.Remove(0, 5)) - 1; //Not Fully Tested;
                    break;
                case "select":
                    StringToolkit.CutParam(cmd.Remove(0, 7), ',', out param);
                    for (int i = 0; i < param.Length / 2; i++)
                    {
                        selection_collection.Add(new Selection(StringToolkit.GetInside(param[2 * i], '\"'), param[2 * i + 1]));
                    }
                    selection_collection.ShowSelection(ref LayoutRoot, TextRender, wndst);
                    selecting = true;
                    skipping = false;
                    for (int i = 0; i < selection_collection.Count; i++)
                    {
                        selection_collection[i].SelectionTextBlock.ManipulationCompleted += new EventHandler<ManipulationCompletedEventArgs>(Select);
                    }
                    break;
                case "dwave":
                    if ((int)appSettings["dwaveOut"] == 1)
                    {
                        StringToolkit.CutParam(cmd.Remove(0, 6), ',', out param);
                        for (int i = 0; i < param.Length; i++)
                        {
                            param[i] = StringToolkit.CalcStr(param[i], script.str, script.num);
                        }
                        if (param.Length == 1)
                        {
                            string p = Path.Combine(inf.Game.GameFolder, inf.Game.DataFolder, param[0]);
                            if (dwave_ogg_u != null && dwave_ogg_u.MediaState == MediaState.Playing) dwave_ogg_u.Pause();
                            if (DwaveInstance_u != null && DwaveInstance_u.State == SoundState.Playing) DwaveInstance_u.Stop();
                            if (p.EndsWith(".ogg") || p.EndsWith(".OGG"))
                            {
                                dwave_ogg_u = new OggSong(new IsolatedStorageFileStream(p, FileMode.Open, isf));
                                dwave_ogg_u.Repeat = false;
                                dwave_ogg_u.Play();
                            }
                            else if (p.EndsWith(".wav") || p.EndsWith(".WAV"))
                            {
                                dwave_wav_u = SoundEffect.FromStream(new IsolatedStorageFileStream(p, FileMode.Open, isf));
                                DwaveInstance_u = dwave_wav_u.CreateInstance();
                                DwaveInstance_u.IsLooped = false;
                                DwaveInstance_u.Play();
                            }
                            else if ((int)appSettings["onsTrans"] == 1)
                            {
                                p = p.Remove(p.Length - 4, 4) + ".ogg";
                                if (dwave_ogg_u != null && dwave_ogg_u.MediaState == MediaState.Playing) dwave_ogg_u.Stop();
                                try
                                {
                                    dwave_ogg_u = new OggSong(new IsolatedStorageFileStream(p, FileMode.Open, isf));
                                    dwave_ogg_u.Repeat = false;
                                    dwave_ogg_u.Play();
                                }
                                catch (Exception exc)
                                {
                                    if (!allowError)
                                    {
                                        MessageBox.Show("脚本错误\n线程:1(0x01)\n错误信息:" + exc.Message + "\n脚本行数:" + (cmdcount - 1) + "\n脚本内容:" + script.scriptmain[cmdcount - 1], "脚本错误信息", MessageBoxButton.OK);

                                    }
                                }
                            }



                        }
                        else
                        {
                            int count = int.Parse(param[0]);
                            string p = Path.Combine(inf.Game.GameFolder, inf.Game.DataFolder, param[1]);
                            if (dwave_ogg[count] != null && dwave_ogg[count].MediaState == MediaState.Playing) dwave_ogg[count].Pause();
                            if (dwave_wav_inst[count] != null && dwave_wav_inst[count].State == SoundState.Playing) dwave_wav_inst[count].Stop();
                            if (p.EndsWith(".ogg") || p.EndsWith(".OGG"))
                            {
                                dwave_ogg[count] = new OggSong(new IsolatedStorageFileStream(p, FileMode.Open, isf));
                                dwave_ogg[count].Repeat = false;
                                dwave_ogg[count].Play();
                            }
                            else if (p.EndsWith(".wav") || p.EndsWith(".WAV"))
                            {
                                dwave_wav[count] = SoundEffect.FromStream(new IsolatedStorageFileStream(p, FileMode.Open, isf));
                                dwave_wav_inst[count] = dwave_wav[count].CreateInstance();
                                dwave_wav_inst[count].IsLooped = false;
                                dwave_wav_inst[count].Play();
                            }
                            else if ((int)appSettings["onsTrans"] == 1)
                            {
                                p = p.Remove(p.Length - 4, 4) + ".ogg";
                                if (dwave_ogg[count] != null && dwave_ogg[count].MediaState == MediaState.Playing) dwave_ogg[count].Stop();
                                try
                                {
                                    dwave_ogg[count] = new OggSong(new IsolatedStorageFileStream(p, FileMode.Open, isf));
                                    dwave_ogg[count].Repeat = false;
                                    dwave_ogg[count].Play();
                                }
                                catch (Exception exc)
                                {
                                    if (!allowError)
                                    {
                                        MessageBox.Show("脚本错误\n线程:1(0x01)\n错误信息:" + exc.Message + "\n脚本行数:" + (cmdcount - 1) + "\n脚本内容:" + script.scriptmain[cmdcount - 1], "脚本错误信息", MessageBoxButton.OK);

                                    }
                                }

                            }

                        }
                    }
                    break;
                case "dwaveloop":
                    if ((int)appSettings["dwaveOut"] == 1)
                    {
                        StringToolkit.CutParam(cmd.Remove(0, 10), ',', out param);
                        for (int i = 0; i < param.Length; i++)
                        {
                            param[i] = StringToolkit.CalcStr(param[i], script.str, script.num);
                        }
                        int lcount = int.Parse(param[0]);
                        string lp = Path.Combine(inf.Game.GameFolder, inf.Game.DataFolder, param[1]);
                        if (dwave_wav_inst[lcount] != null && dwave_wav_inst[lcount].State == SoundState.Playing) dwave_wav_inst[lcount].Stop();
                        if (dwave_ogg[lcount] != null && dwave_ogg[lcount].MediaState == MediaState.Playing) dwave_ogg[lcount].Pause();
                        if (lp.EndsWith(".ogg"))
                        {
                            dwave_ogg[lcount] = new OggSong(new IsolatedStorageFileStream(lp, FileMode.Open, isf));
                            dwave_ogg[lcount].Repeat = true;
                            dwave_ogg[lcount].Play();
                        }
                        else if (lp.EndsWith(".wav"))
                        {
                            dwave_wav[lcount] = SoundEffect.FromStream(new IsolatedStorageFileStream(lp, FileMode.Open, isf));
                            dwave_wav_inst[lcount] = dwave_wav[lcount].CreateInstance();
                            dwave_wav_inst[lcount].IsLooped = false;
                            dwave_wav_inst[lcount].Play();
                        }
                        else if ((int)appSettings["onsTrans"] == 1)
                        {
                            lp = lp.Remove(lp.Length - 4, 4) + ".ogg";
                            if (dwave_ogg[lcount] != null && dwave_ogg[lcount].MediaState == MediaState.Playing) dwave_ogg[lcount].Stop();
                            try
                            {
                                dwave_ogg[lcount] = new OggSong(new IsolatedStorageFileStream(lp, FileMode.Open, isf));
                                dwave_ogg[lcount].Repeat = true;
                                dwave_ogg[lcount].Play();
                            }
                            catch (Exception exc)
                            {
                                if (!allowError)
                                {
                                    MessageBox.Show("脚本错误\n线程:1(0x01)\n错误信息:" + exc.Message + "\n脚本行数:" + (cmdcount - 1) + "\n脚本内容:" + script.scriptmain[cmdcount - 1], "脚本错误信息", MessageBoxButton.OK);

                                }

                            }
                        }
                    }
                    break;
                case "dwavestop":
                    StringToolkit.CutParam(cmd.Remove(0, 10), ',', out param);

                    int stopcount = int.Parse(param[0]);
                    if (dwave_ogg[stopcount] != null && dwave_ogg[stopcount].MediaState == MediaState.Playing) dwave_ogg[stopcount].Stop();
                    if (dwave_wav_inst[stopcount] != null && dwave_wav_inst[stopcount].State == SoundState.Playing) dwave_wav_inst[stopcount].Stop();

                    break;
                case "msp":
                    StringToolkit.CutParam(cmd.Remove(0, 4), ',', out param);
                    int imgc, c_x, c_y;
                    for (int i = 0; i < param.Length; i++)
                    {
                        param[i] = StringToolkit.CalcStr(param[i], script.str, script.num);
                    }
                    imgc = int.Parse(param[0]);
                    c_x = int.Parse(param[1]);
                    c_y = int.Parse(param[2]);
                    double l = img[imgc].Margin.Left, t = img[imgc].Margin.Top, r = img[imgc].Margin.Right, b = img[imgc].Margin.Bottom;
                    img[imgc].Margin = new Thickness(l + c_x * zoom, t + c_y * zoom, r - c_x * zoom, b - c_y * zoom);
                    imgState[imgc].Margin = img[imgc].Margin;
                    break;
                case "amsp":
                    StringToolkit.CutParam(cmd.Remove(0, 5), ',', out param);
                    int c, to_x, to_y;
                    double p_x, p_y;
                    for (int i = 0; i < param.Length; i++)
                    {
                        param[i] = StringToolkit.CalcStr(param[i], script.str, script.num);
                    }
                    c = int.Parse(param[0]);
                    to_x = int.Parse(param[1]);
                    to_y = int.Parse(param[2]);
                    p_x = img[c].Width;
                    p_y = img[c].Height;
                    l = img[c].Margin.Left;
                    t = img[c].Margin.Top;
                    r = img[c].Margin.Right;
                    b = img[c].Margin.Bottom;
                    img[c].Margin = new Thickness(to_x * zoom, to_y * zoom, resolution_width - p_x - to_x * zoom, resolution_height - p_y - to_y * zoom);
                    imgState[c].Margin = img[c].Margin;
                    break;
                case "texton":
                    textboximage.Opacity = 1;
                    TextRender.Opacity = 1;
                    return false;
                case "textoff":
                    TextRender.Opacity = 0;
                    textboximage.Opacity = 0;
                    return false;
                case "setwindow":
                    lastsetwindow = cmd.Remove(0, 10);
                    StringToolkit.CutParam(cmd.Remove(0, 10), ',', out param);

                    wndst.x = int.Parse(param[0]);
                    wndst.y = int.Parse(param[1]);
                    wndst.charcount = int.Parse(param[2]);
                    wndst.linecount = int.Parse(param[3]);
                    wndst.charwidth = int.Parse(param[4]);
                    wndst.charheight = int.Parse(param[5]);
                    wndst.charspacing = int.Parse(param[6]);
                    wndst.linespacing = int.Parse(param[7]);
                    wndst.textspeed = double.Parse(param[8]);
                    if (int.Parse(param[9]) == 0) wndst.isthick = false;
                    else wndst.isthick = true;
                    if (int.Parse(param[10]) == 0) wndst.isshadow = false;
                    else wndst.isthick = true;
                    if (param.Length > 11)
                    {
                        wndst.color = param[11].Replace("\"", "");
                        if (param.Length > 12)
                        {
                            wndst.textblockx1 = int.Parse(param[12]);
                            wndst.textblocky1 = int.Parse(param[13]);
                            if (param.Length > 14)
                            {
                                wndst.textblockx2 = int.Parse(param[14]);
                                wndst.textblocky2 = int.Parse(param[15]);
                            }
                            else
                            {
                                wndst.textblockx2 = (int)resolution_width;
                                wndst.textblocky2 = (int)resolution_height;
                            }
                        }
                    }
                    wndst.SetWindow(ref TextRender, ref textboximage);
                    textboximageState = new ImageStatus();

                        textboximageState.Path = Path.Combine(GamePage.inf.Game.GameFolder, GamePage.inf.Game.DataFolder, wndst.color);
                        if (wndst.color.EndsWith("png")||wndst.color.EndsWith("PNG"))
                        {
                            textboximageState.IsAlpha = false;
                        }
                        else if (wndst.color.EndsWith("jpg") || wndst.color.EndsWith("JPG"))
                        {
                            textboximageState.IsAlpha = true;
                        }
                    textboximageState.Alignment = HorizontalAlignment.Left;
                    textboximageState.Opacity = 1;
                    textboximageState.Margin = textboximage.Margin;
                    break;
                case "textspeed":
                    wndst.textspeed = double.Parse(cmd.Remove(0, 10));
                    break;
                case "getparam":
                    StringToolkit.CutParam(cmd.Remove(0, 9), ',', out param);
                    for (int k = 0; k < param.Length; k++)
                    {
                        if (param[k][0] == '%')
                        {
                            param[k] = param[k].Remove(0, 1);
                            try
                            {
                                script.num[int.Parse(param[k])] = new VAR_NUM(Convert.ToDouble(sbparam[k]));
                            }
                            catch
                            {
                                foreach (DEF def in script.defas)
                                {
                                    if (!def.IsString && def.def == param[k])
                                    {
                                        script.num[Convert.ToInt32((double)def.VAR)] = new VAR_NUM(Convert.ToDouble(def.VAR));
                                    }
                                }
                            }
                        }
                        else if (param[k][0] == '$')
                        {
                            param[k] = param[k].Remove(0, 1);
                            try
                            {
                                script.str[int.Parse(param[k])] = new VAR_STR(sbparam[k]);
                            }
                            catch
                            {
                                foreach (DEF def in script.defas)
                                {
                                    if (def.IsString && def.def == param[k])
                                    {
                                        script.str[Convert.ToInt32((double)def.VAR)] = new VAR_STR((string)def.VAR);
                                    }
                                }
                            }
                        }
                    }
                    break;
                case "reset":
                    IsBGShowed = false;
                    for (int i = 0; i < 1000; i++)
                    {
                        img[i].Source = null;

                    }
                    for (int i = 0; i < 100; i++)
                    {
                        if (dwave_ogg[i] != null) dwave_ogg[i].Stop();
                        if (dwave_wav[i] != null) dwave_wav[i].Dispose();
                    }
                    /*if (btnwaiting)*/ Touch.FrameReported -= new TouchFrameEventHandler(Touch_FrameReported);
                    btnwaiting = false;
                    me.Stop();
                    ReadScript();
                    if (bgm_ogg != null) bgm_ogg.Stop();
                    LayoutRoot.Children.Remove(TextRender);
//                    LayoutRoot.Children.Remove(bg);
                    bg.Source = null;
                    LayoutRoot.Children.Remove(textboximage);
                    me.MediaEnded -= new RoutedEventHandler(Media_Ended);
                    LayoutRoot.Children.Remove(me);
                    playing = false;
                    mp3looping = false;
                    skipping = false;
                    BackgroundAudioPlayer.Instance.Close();
                    cmdcount = 0;
                    break;
                default:
                    foreach (Sub s in script.sub)
                    {
                        if (StringToolkit.GetBefore(cmd, ' ') == s.name/*cmd.StartsWith(s.name + " ")*/)
                        {
                            string[] sparam;
                            if (cmd.Length > s.name.Length + 1) StringToolkit.CutParam(cmd.Remove(0, s.name.Length + 1)/*cmd.Replace(s.name+" ","")*/, ',', out sparam);
                            else sparam = new string[0];
                            for (int i = 0; i < sparam.Length; i++)
                            {
                                if (sparam[i][0] == '%')
                                {
                                    sparam[i] = Convert.ToString(script.num[int.Parse(sparam[i].Remove(0, 1))].GetValue());
                                }
                                else if (sparam[i][0] == '$')
                                {
                                    sparam[i] = script.str[int.Parse(sparam[i].Remove(0, 1))].GetVar();
                                }
                            }
                            CmdExec(s.subbuf, ref LayoutRoot, ref TextRender, ref s.num, ref s.str, sparam, ref script.sub, ref img);
                            return true;
                        }
                    }
                    break;
            }
            return true;

        }
        #endregion
        #region 资源小工具
        string GetSoundUri(string scruri)
        {
            ///<summary>
            /// scruri:去掉了引号的资源路径
            ///</summary>
            return Path.Combine(inf.Game.GameFolder, inf.Game.DataFolder, scruri);
        }
        void LoadImage(string scruri, ref Image PicBox)
        {
            ///<summary>
            /// scruri:去掉了引号的资源路径
            ///</summary>
            string path = "";
            string p = StringToolkit.GetBetween(scruri, ':', ';');
            if (p != "")
            {
                scruri = scruri.Remove(0, p.Length + 2);
            }
            if (p == "a" && (scruri.EndsWith("jpg") || scruri.EndsWith("JPG")))
            {
                path = Path.Combine(inf.Game.GameFolder, inf.Game.DataFolder, scruri);
                Alpha.AlphaBlend(new IsolatedStorageFileStream(path, FileMode.Open, isf), ref PicBox);

            }
            else
            {
                BitmapImage tempimg = new BitmapImage();
                tempimg.SetSource(new IsolatedStorageFileStream(path, FileMode.Open, isf));
                PicBox.Source = tempimg;

            }
        }

        #endregion
    }
}