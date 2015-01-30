using System;
using System.Threading;
using System.Windows.Media.Imaging;
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
using UVEngineNative;

namespace UVEngine
{
    public partial class ONSCL_GameInfoPage : PhoneApplicationPage
    {
        ManifestEX manifestEX;
        static internal NSGameSettings nsg;
        string gamePath;
        IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication();
        bool deleting = false, reserveSave = true; 
        Thread thd;
        public ONSCL_GameInfoPage()
        {
            InitializeComponent();
        }
        private void ShowInfo()
        {
            if (!storage.DirectoryExists(Path.Combine("Shared", "ShellContent", gamePath)))
            {
                storage.CreateDirectory(Path.Combine("Shared", "ShellContent", gamePath));
                storage.CopyFile(Path.Combine(gamePath, manifestEX.TilePath), Path.Combine("Shared", "ShellContent", gamePath, "Tile.png"), true);
            }
            GameNameTextblock.Text = manifestEX.GameName;
            CompanyTextblock.Text = manifestEX.GameCompany;
            SizeTextblock.Text = manifestEX.GameSize + "MB";
            MakerTextblock.Text = manifestEX.GameMaker;
            try
            {
                IsolatedStorageFileStream location = new IsolatedStorageFileStream(Path.Combine(gamePath, manifestEX.IconPath), FileMode.Open, storage);
                BitmapImage bitmapimage = new BitmapImage();
                bitmapimage.SetSource(location);
                IconImage.Source = bitmapimage;
                location.Close();
            }
            catch
            {

            }
        }
        private void Del_Proc()
        {
            this.Dispatcher.BeginInvoke(() =>
            {
                Deleting.IsIndeterminate = true;
            });
            try
            {
                RemovePath(gamePath);
                RemovePath(Path.Combine("Shared", "ShellContent", gamePath));
            }
            catch (Exception exc)
            {
                this.Dispatcher.BeginInvoke(() =>
                {
                    MessageBox.Show(exc.Message);
                });
            }
            this.Dispatcher.BeginInvoke(() =>
            {
                this.NavigationService.GoBack();
            });
        }
        protected override void OnNavigatedTo(System.Windows.Navigation.NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            gamePath = NavigationContext.QueryString["game"];
            manifestEX = new ManifestEX(gamePath + "\\uve-manifestEX.uvm");
            nsg = new NSGameSettings(gamePath);
            switch (nsg.screentype)
            {
                case 1:
                    ratio.IsChecked = true;
                    break;
                case 2:
                    wide.IsChecked = true;
                    break;
            }
            ratio.Checked += RadioButton_Checked_1;
            wide.Checked += RadioButton_Checked_2;
            ShowInfo();

        }
        private void Run(object sender, RoutedEventArgs e)
        {
            if (UVEngineSettings.NoMoreDisplay)
                this.NavigationService.Navigate(new Uri("/ONSCL/Direct3DPage.xaml?game=" + gamePath, UriKind.Relative));
            else
                this.NavigationService.Navigate(new Uri("/System/DonatePage.xaml?game=" + gamePath, UriKind.Relative));
        }
        private void Delete(object sender, RoutedEventArgs e)
        {
            if (MessageBox.Show(UVEngine.Resources.UVEngine.comfirmdelete, UVEngine.Resources.UVEngine.comfirminfo, MessageBoxButton.OKCancel) == MessageBoxResult.OK)
            {
                if (MessageBox.Show(UVEngine.Resources.UVEngine.reservesave, UVEngine.Resources.UVEngine.comfirminfo, MessageBoxButton.OKCancel) == MessageBoxResult.Cancel)
                {
                    reserveSave = false;
                }
                thd = new Thread(new ThreadStart(Del_Proc));
                thd.Start();
                deleting = true;
            }
        }
        private void PIN(object sender, RoutedEventArgs e)
        {
            try
            {
                StandardTileData tile = new StandardTileData
                {
                    Title = manifestEX.GameName,
                    BackgroundImage = new Uri("isostore:/Shared/ShellContent/" + gamePath + "/Tile.png", UriKind.Absolute),
                };
                ShellTile.Create(new Uri("/ONSCL/Direct3DPage.xaml?game=" + gamePath, UriKind.Relative), tile);
            }
            catch (Exception exc)
            {
                MessageBox.Show(UVEngine.Resources.UVEngine.tileerror + exc.Message, UVEngine.Resources.UVEngine.error, MessageBoxButton.OK);

            }
        }
        private void Back(object sender, RoutedEventArgs e)
        {
            this.NavigationService.GoBack();
        }
        protected override void OnBackKeyPress(System.ComponentModel.CancelEventArgs e)
        {
            if (deleting && MessageBox.Show(UVEngine.Resources.UVEngine.deleting, UVEngine.Resources.UVEngine.tip, MessageBoxButton.OKCancel) == MessageBoxResult.Cancel)
            {
                e.Cancel = true;
            }
            base.OnBackKeyPress(e);
        }
        void RemovePath(string RemovePath)
        {
            using (var store = IsolatedStorageFile.GetUserStoreForApplication())
            {
                if (store.DirectoryExists(RemovePath))
                {
                    String[] dirNames = store.GetDirectoryNames(string.Concat(RemovePath, "\\*"));
                    String[] fileNames = store.GetFileNames(string.Concat(RemovePath, "\\*"));
                    if (fileNames.Length > 0)
                    {
                        for (int i = 0; i < fileNames.Length; i++)
                        {
                            if (!(reserveSave &&
                                ((fileNames[i].StartsWith("save") &&
                                fileNames[i].EndsWith(".dat")) ||
                                fileNames[i] == "envdata" ||
                                fileNames[i] == "kidoku.dat" ||
                                fileNames[i] == "gloval.sav")))
                                store.DeleteFile(string.Concat(RemovePath, "\\", fileNames[i]));
                        }
                    }
                    if (dirNames.Length == 0)
                    {
                        store.DeleteDirectory(RemovePath);
                        if (RemovePath.IndexOf("\\") != -1)
                        {
                            RemovePath = RemovePath.Substring(0, RemovePath.LastIndexOf("\\"));
                            this.RemovePath(RemovePath);
                        }
                    }
                    if (dirNames.Length > 0)
                    {
                        for (int i = 0; i < dirNames.Length; i++)
                        {
                            this.RemovePath(string.Concat(RemovePath, "\\", dirNames[i]));
                        }
                    }
                }
            }
        }

        private void RadioButton_Checked_1(object sender, RoutedEventArgs e)
        {
            nsg.screentype = 1;
        }
        private void RadioButton_Checked_2(object sender, RoutedEventArgs e)
        {
            nsg.screentype = 2;
        }
    }
}