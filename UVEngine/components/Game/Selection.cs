using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace UVEngine
{
    public class Selection
    {
        public TextBlock SelectionTextBlock = new TextBlock();
        public string TargetLabel;
        public Selection(string SelectionText, string TargetLabel)
        {
            this.SelectionTextBlock.Text = SelectionText;
            this.TargetLabel = TargetLabel;
        }
    }
    public class Selection_Collection : Collection<Selection>
    {
        public void ShowSelection(ref Grid LayoutRoot, TextBlock TextRender, WindowStatus wndst)
        {
            TextRender.Opacity = 0;
            double height = wndst.charheight * GamePage.zoom, x = wndst.textblockx1 * GamePage.zoom, y = wndst.textblocky1 * GamePage.zoom;
            for (int i = 0; i < this.Count; i++)
            {
                LayoutRoot.Children.Add(this[i].SelectionTextBlock);
                this[i].SelectionTextBlock.Margin = new Thickness(x + 20, y + (height + 10) * i + 50, GamePage.resolution_width - wndst.textblockx2 * GamePage.zoom - 20, GamePage.resolution_height - (2 * y + (height + 10) * (i + 2)) - 50);
                this[i].SelectionTextBlock.Foreground = new SolidColorBrush(Colors.White);
            }
        }
        public void FinishSelection(Grid LayoutRoot, TextBlock TextRender)
        {
            TextRender.Opacity = 1;
            for (int i = 0; i < this.Count; i++)
            {
                LayoutRoot.Children.Remove(this[i].SelectionTextBlock);
                this[i].SelectionTextBlock = new TextBlock();
            }
            this.Clear();
        }
    }
}
