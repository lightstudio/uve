using System.Collections.ObjectModel;
using System.IO;
using System.IO.IsolatedStorage;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Imaging;

namespace UVEngine
{
    public class History
    {
        public string text;
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

        public History(WindowStatus st, string text)
        {
            x = st.x;
            y = st.y;
            charcount = st.charcount;
            linecount = st.linecount;
            charwidth = st.charwidth;
            charheight = st.charheight;
            charspacing = st.charspacing;
            linespacing = st.linespacing;
            textspeed = st.textspeed;
            isthick = st.isthick;
            isshadow = st.isshadow;
            color = st.color;
            textblockx1 = st.textblockx1;
            textblocky1 = st.textblocky1;
            textblockx2 = st.textblockx2;
            textblocky2 = st.textblocky2;
            this.text = text;
        }
    }
    public class TextHistory : ObservableCollection<History>
    {
    }
}