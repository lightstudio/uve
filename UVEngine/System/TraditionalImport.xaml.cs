using System;
using System.Threading;
using System.IO;
using System.IO.IsolatedStorage;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using Microsoft.Phone.Tasks;
using Microsoft.Xna.Framework.Media;
using ICSharpCode.SharpZipLib.Zip;

namespace UVEngine
{
    public partial class TraditionalImport : PhoneApplicationPage
    {
        PhotoChooserTask pc;
        int selected = 0;
        public TraditionalImport()
        {
            selected = 0;
            InitializeComponent();
            QuickImport.pics.Clear();
            pc = new PhotoChooserTask();
            pc.Completed += pc_Completed;
            this.Loaded += TraditionalImport_Loaded;
        }

        void pc_Completed(object sender, PhotoResult e)
        {
            if (e.TaskResult == TaskResult.OK)
            {
                MediaLibrary ml = new MediaLibrary();
                PictureCollection pic_c = ml.Pictures;
                foreach (Picture p in pic_c)
                {
                    var fname = e.OriginalFileName.Substring(e.OriginalFileName.LastIndexOf('\\') + 1);
                    if (p.Name == fname)
                    {
                        QuickImport.pics.Add(p);
                        selected = 1;
                        break;
                    }
                }
                long totalsize = 0;
                foreach (Picture p in QuickImport.pics)
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
            else
            {
                this.NavigationService.GoBack();
            }
        }
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            if (selected == 1)
            {
                this.NavigationService.GoBack();
            }
            else if (selected == 0)
            {
                selected = -1;
                pc.Show();
                
            }
            else if (selected == -1)
            {

            }
            base.OnNavigatedTo(e);
        }
        void TraditionalImport_Loaded(object sender, RoutedEventArgs e)
        {
        }
    }
}