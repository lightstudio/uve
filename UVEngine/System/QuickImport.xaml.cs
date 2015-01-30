using System;
using System.IO;
using System.IO.IsolatedStorage;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using System.Windows.Media.Imaging;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using Microsoft.Xna.Framework.Media;
using ICSharpCode.SharpZipLib.Zip;

namespace UVEngine
{

    public partial class QuickImport : PhoneApplicationPage
    {
        IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication();
        static public ObservableCollection<Picture> pics = new ObservableCollection<Picture>();
        public QuickImport()
        {
            InitializeComponent();
            pics.Clear();
            MediaLibrary ml = new MediaLibrary();
            tip.Text += (storage.AvailableFreeSpace / 1048576).ToString() + "MB";
            PictureCollection pic_c = ml.Pictures;
            this.Dispatcher.BeginInvoke(() =>
                {
                    foreach (Picture pic in pic_c)
                    {
                        if (pic.Album.Name.ToUpperInvariant() == "UVE")
                        {
                            pics.Add(pic);
                        }
                    }
                    if (pics.Count == 0)
                    {
                        items.Text = UVEngine.Resources.UVEngine.nopicfound;
                    }
                    else
                    {
                        impButton.IsEnabled = true;
                        selButton.IsEnabled = true;
                        foreach (Picture pic in pics)
                        {
                            items.Text += pic.Name + "    "+(pic.GetImage().Length/1048576).ToString() + "MB" + '\n';
                        }
                    }
                });
        }
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new Uri("/System/SelectImport.xaml", UriKind.Relative));
        }
        private void Button_Click_1(object sender, RoutedEventArgs e)
        {            
            long totalsize = 0;
            foreach (Picture p in pics)
            {
                totalsize += p.GetImage().Length;
            }
            if (totalsize <= System.IO.IsolatedStorage.IsolatedStorageFile.GetUserStoreForApplication().AvailableFreeSpace)
            {
                this.NavigationService.Navigate(new Uri("/System/QuickExtract.xaml", UriKind.Relative));
            }
            else
            {
                MessageBox.Show(UVEngine.Resources.UVEngine.spacenotenough);
            }
        }
    }
}