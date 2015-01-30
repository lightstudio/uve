using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using Microsoft.Xna.Framework.Media;

namespace UVEngine
{
    public partial class SelectImport : PhoneApplicationPage
    {
        Dictionary<string, Picture> pics = new Dictionary<string, Picture>();
        public SelectImport()
        {
            InitializeComponent();
            foreach (Picture p in QuickImport.pics)
            {
                pics.Add(p.Name, p);
            }
            QuickImport.pics.Clear();
        }
        void Checked(object sender, EventArgs e)
        {
            if (!QuickImport.pics.Contains(pics[(sender as CheckBox).Content as string])) QuickImport.pics.Add(pics[(sender as CheckBox).Content as string]);
        }
        void Unchecked(object sender, EventArgs e)
        {
            if (QuickImport.pics.Contains(pics[(sender as CheckBox).Content as string])) QuickImport.pics.Remove(pics[(sender as CheckBox).Content as string]);
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            long totalsize = 0, freespace = System.IO.IsolatedStorage.IsolatedStorageFile.GetUserStoreForApplication().AvailableFreeSpace;
            foreach (Picture p in QuickImport.pics)
            {
                totalsize += p.GetImage().Length;
            }
            if (totalsize <= freespace)
            {
                QuickExtract.sel = true;
                this.NavigationService.Navigate(new Uri("/System/QuickExtract.xaml", UriKind.Relative));
            }
            else
            {
                MessageBox.Show(UVEngine.Resources.UVEngine.spacenotenough);
            }
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            foreach (Picture p in pics.Values)
            {
                if (!QuickImport.pics.Contains(p))
                {
                    QuickImport.pics.Add(p);
                }
            }

            this.NavigationService.GoBack();
        }
        protected override void OnBackKeyPress(System.ComponentModel.CancelEventArgs e)
        {
            foreach (Picture p in pics.Values)
            {
                if (!QuickImport.pics.Contains(p))
                {
                    QuickImport.pics.Add(p);
                }
            }
            base.OnBackKeyPress(e);
        }
        private void listContent_Loaded(object sender, RoutedEventArgs e)
        {
            (sender as CheckBox).Checked += Checked;
            (sender as CheckBox).Unchecked += Unchecked;
        }
    }
}
namespace List
{
    public class Item
    {
        public String Name { get; set; }
        public String Length { get; set; }
        public Item(string name,string length)
        {
            this.Name = name;
            this.Length = length;
        }
    }
    public class Items : ObservableCollection<Item>
    {
        public Items()
        {
            foreach (Picture p in UVEngine.QuickImport.pics)
            {
                Add(new Item(p.Name, (p.GetImage().Length / 1048576).ToString() + "MB"));
            }
        }
    }
}