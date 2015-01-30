using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;


namespace UVEngine
{
    public class Button_withText
    {
        System.Windows.Shapes.Rectangle bg = new System.Windows.Shapes.Rectangle();
        TextBlock text = new TextBlock();
        public Button_withText(string name, Thickness th, SolidColorBrush bgcolor, SolidColorBrush textcolor, double bgopacity, double textopacity)
        {
            text.Name = name;
            bg.Margin = th;
            text.Margin = th;
            text.TextAlignment = TextAlignment.Center;
            bg.Fill = bgcolor;
            text.Foreground = textcolor;
            text.Text = name;
            bg.Opacity = bgopacity;
            text.Opacity = textopacity;
        }
        public void AddButton(Grid layoutRoot, EventHandler<System.Windows.Input.ManipulationStartedEventArgs> handler)
        {
            layoutRoot.Children.Add(this.bg);
            layoutRoot.Children.Add(this.text);
            text.ManipulationStarted += handler;

        }
        public void RemoveButton(Grid layoutRoot, EventHandler<System.Windows.Input.ManipulationStartedEventArgs> handler)
        {
            layoutRoot.Children.Remove(this.bg);
            layoutRoot.Children.Remove(this.text);
            text.ManipulationStarted -= handler;

        }
    }
}