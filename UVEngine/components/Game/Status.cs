using Microsoft.Phone.BackgroundAudio;
using Microsoft.Xna.Framework.Media;
using System;
using System.IO;
using System.IO.IsolatedStorage;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Imaging;

namespace UVEngine
{
    public class WindowStatus
    {
        IsolatedStorageFile isf = IsolatedStorageFile.GetUserStoreForApplication();
        public int x = 8, y = 16, charcount = 20, linecount = 23, charwidth = 26, charheight = 26, charspacing = 0, linespacing = 20, textblockx1 = 0, textblocky1 = 0, textblockx2 = 639, textblocky2 = 479;
        public double textspeed = 20;
        public bool isthick = true, isshadow = true;
        public string color = "#999999";
        public void SetWindow(ref TextBlock txb, ref Image textboximage)
        {
            double left, top, right, bottom;
            left = this.x * GamePage.zoom;
            top = this.y * GamePage.zoom;
            right = GamePage.resolution_width - ((this.charspacing + this.charwidth) * charcount) * GamePage.zoom - this.x * GamePage.zoom;
            bottom = GamePage.resolution_height - ((this.linespacing + this.charheight) * linecount) * GamePage.zoom - this.y * GamePage.zoom;
            txb.FontSize = charwidth * GamePage.zoom - 1;
            txb.Margin = new Thickness(left, top, right, bottom);
            if (color.StartsWith("#"))
            {

            }
            else if (color.StartsWith(":a;") || color.StartsWith(":b;") || color.StartsWith(":c;"))
            {
                color = color.Remove(0, 3);
                BitmapImage textbox = new BitmapImage();
                string path = Path.Combine(GamePage.inf.Game.GameFolder, GamePage.inf.Game.DataFolder, color);
                try
                {
                    if (color.EndsWith("png"))
                    {
                        textbox.SetSource(new IsolatedStorageFileStream(path, FileMode.Open, isf));
                        textboximage.Source = textbox;
                    }
                    else if (color.EndsWith("jpg"))
                    {
                        Alpha.AlphaBlend(new IsolatedStorageFileStream(path, FileMode.Open, isf), ref textboximage);
                    }
                    textboximage.HorizontalAlignment = HorizontalAlignment.Left;
                    textboximage.Opacity = 1;
                    textboximage.Margin = new Thickness(textblockx1 * GamePage.zoom, textblocky1 * GamePage.zoom, GamePage.resolution_width - textblockx2, GamePage.resolution_height - textblocky2);
                }
                catch
                {

                }

            }
        }
    }
    public class TextBlockStatus
    {
        TextBlock tb = new TextBlock();
        public Thickness margin;
        public string Content;
        public double FontSize;
    }
    public class ImageStatus
    {
        public bool IsAlpha = false;
        public Thickness Margin = new Thickness(0, 0, 0, 0);
        public string Path = "";
        public double Opacity = 0;
        public HorizontalAlignment Alignment = HorizontalAlignment.Left;
    }
    public class GameState
    {
        public bool IsBGM, IsOgg;
        public string meuri;
        public bool isSaved = false;
        public int cmdcount;
        public double[] var_num;
        public string[] var_str;
        public ImageStatus[] img;
        public ImageStatus bg, textboximage;
        public DateTime savetime;
        public string lastsetwindow;
        public TextBlockStatus tbs = new TextBlockStatus();
        public GameState()
        {
        }
        public GameState(GamePage p)
        {
            this.lastsetwindow = p.lastsetwindow;
            isSaved = true;
            bool isbgm = false, isogg = false;
            string uri = "";
            savetime = DateTime.Now;
            cmdcount = p.cmdcount;
            //this.img = p.imgState;
            this.img = new ImageStatus[1000];
            for (int i = 0; i < 1000; i++)
            {
                if (p.img[i].Opacity != 0) this.img[i] = p.imgState[i];
                else { this.img[i] = new ImageStatus(); }
            }
            if (p.bg.Opacity != 0) this.bg = p.bgState;
            else { this.bg = new ImageStatus(); }
            this.textboximage = p.textboximageState;
            tbs.Content = p.TextRender.Text;
            tbs.margin = p.TextRender.Margin;
            tbs.FontSize = p.TextRender.FontSize;
            var_num = new double[GamePage.script.num.Length];
            var_str = new string[GamePage.script.str.Length];
            for (int i = 0; i < var_num.Length; i++)
            {
                var_num[i] = GamePage.script.num[i].GetValue();
            }
            for (int i = 0; i < var_str.Length; i++)
            {
                var_str[i] = GamePage.script.str[i].GetVar();
            }
            if (p.playing && p.mp3looping)
            {
                isbgm = false;
                isogg = false;
                uri = p.currentbgm;
            }
            else if (BackgroundAudioPlayer.Instance != null)
            {
                try
                {
                    if (BackgroundAudioPlayer.Instance.Track != null)
                    {
                        isbgm = true;
                        uri = p.currentbgm;
                    }
                }
                catch { isbgm = false; uri = ""; }
            }
            else if (p.bgm_ogg != null && (p.bgm_ogg.MediaState != MediaState.Stopped && p.bgm_ogg.MediaState != MediaState.Paused))
            {
                isogg = true;
                uri = p.currentbgm;
            }
            this.IsBGM = isbgm;
            this.IsOgg = isogg;
            this.meuri = uri;
        }
    }

}
