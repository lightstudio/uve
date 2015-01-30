using System.Collections.ObjectModel;
using System.IO;
using System.IO.IsolatedStorage;
using System.Windows;
using System.Windows.Controls;

namespace UVEngine
{

    public class BTN
    {
        static public string BTN_Pic = "";
        public double l, t, r, b;
        public double VAR_TO_LOAD;
        public Image image = new Image();
        public BTN(Thickness TargetMargin, double VAR_TO_LOAD)
        {
            this.l = TargetMargin.Left;
            this.t = TargetMargin.Top;
            this.r = TargetMargin.Right;
            this.b = TargetMargin.Bottom;
            this.VAR_TO_LOAD = VAR_TO_LOAD;
        }
        public BTN(Thickness TargetMargin, double VAR_TO_LOAD, double CutPoint_X, double CutPoint_Y, double Cut_Width, double Cut_Height, Grid layoutRoot)
        {
            this.l = TargetMargin.Left;
            this.t = TargetMargin.Top;
            this.r = TargetMargin.Right;
            this.b = TargetMargin.Bottom;
            this.VAR_TO_LOAD = VAR_TO_LOAD;
            if (BTN_Pic != "")
            {
                IsolatedStorageFileStream btnpic = new IsolatedStorageFileStream(BTN_Pic, FileMode.Open, IsolatedStorageFile.GetUserStoreForApplication());
                Alpha.CutImage(btnpic, new Size(CutPoint_X, CutPoint_Y), new Size(Cut_Width, Cut_Height), ref this.image);
                layoutRoot.Children.Add(this.image);
                this.image.Margin = TargetMargin;
                this.image.HorizontalAlignment = HorizontalAlignment.Left;
            }
        }
        public bool BTNManip(double x, double y)
        {
            if (x > this.l && x < GamePage.resolution_width - this.r && y > this.t && y < GamePage.resolution_height - this.b) return true;
            else return false;
        }
    }
    public class BTN_Collections : ObservableCollection<BTN>
    {
        public void UnREG_BTN_ALL()
        {
            for (int i = 0; i < this.Count; i++)
            {
                this[i].image = null;
            }
            this.Clear();
            BTN.BTN_Pic = "";
        }
    }
}
