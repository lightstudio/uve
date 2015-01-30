using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using System;
using System.IO;
using System.IO.IsolatedStorage;
using System.Threading;
using System.Windows;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;


namespace UVEngine
{
    public partial class GameDetailPage : PhoneApplicationPage
    {
        string gamefolder = "";
        IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication();
        IsolatedStorageSettings appSettings = IsolatedStorageSettings.ApplicationSettings;
        string GameName = "", Company = "", GameSize = "", GameMaker = "", IconUri = "", ScreenResolution = "", ScriptType = "", TileUri = "", Description = "";
        Thread thd;
        sbyte type = 0;//0:NS-simple 1:NS-Full
        bool deleting = false;
        public GameDetailPage()
        {
            InitializeComponent();

        }
        private void ShowInfo()
        {
            if (!storage.DirectoryExists(Path.Combine("Shared", "ShellContent", gamefolder)))
            {
                storage.CreateDirectory(Path.Combine("Shared", "ShellContent", gamefolder));
                storage.CopyFile(TileUri, Path.Combine("Shared", "ShellContent", gamefolder, "Tile.png"), true);
            }
            GameNameTextblock.Text = GameName;
            CompanyTextblock.Text = Company;
            SizeTextblock.Text = GameSize + "MB";
            MakerTextblock.Text = GameMaker;
            try
            {
                IsolatedStorageFileStream location = new IsolatedStorageFileStream(IconUri, FileMode.Open, storage);
                BitmapImage bitmapimage = new BitmapImage();
                bitmapimage.SetSource(location);
                IconImage.Source = bitmapimage;
                location.Close();
            }
            catch
            {

            }
            ResolutionTextblock.Text = ScreenResolution.Replace("*", "×");
            TypeTextblock.Text = ScriptType;
            DescriptionTextblock.Text = Description;
            if (ScriptType == "ns" || ScriptType == "NS")
            {
                TypeTextblock.Text = "NS脚本";
            }
            else if (ScriptType == "Ruby")
            {
                TypeTextblock.Text = "Ruby脚本";
            }
            else
            {
                TypeTextblock.Text = "未知";
            }
            DescriptionTextblock.Text = Description;
        }
        private void ReadInfo()
        {
            string[] info = new string[30];
            int temp = 0, temp2 = 0;
            if (storage.FileExists(Path.Combine(gamefolder, "uve-mafest.uvm")))
            {
                type = 0;
                try
                {
                    IsolatedStorageFileStream location = new IsolatedStorageFileStream(Path.Combine(gamefolder, "uve-manifest.uvm"), FileMode.Open, storage);
                    StreamReader file = new StreamReader(location);
                    for (int j = 0, b = 0; ; j++)
                    {
                        info[j] = file.ReadLine();
                        temp = j;
                        if (b == 1) break;
                        if (file.EndOfStream)
                            b = 1;
                    }
                    location.Close();
                    file.Close();
                }
                catch (Exception e)
                {
                    MessageBox.Show("这个游戏似乎不存在的说~~~\n以下为错误信息:" + e.Message, "错误", MessageBoxButton.OK);
                }
                for (int j = 0; j < temp; j++)
                {

                    if (info[j].StartsWith("[Info]")) temp2 = 1;
                    else if (info[j].StartsWith("[Script]")) temp2 = 2;
                    else if (info[j].StartsWith("[Tile]")) temp2 = 3;
                    else
                    {
                        if (temp2 == 1)
                        {
                            if (info[j].StartsWith("GameName=")) GameName = info[j].Remove(0, 9);
                            else if (info[j].StartsWith("Company=")) Company = info[j].Remove(0, 8);
                            else if (info[j].StartsWith("GameMaker=")) GameMaker = info[j].Remove(0, 10);
                            else if (info[j].StartsWith("GameSize=")) GameSize = info[j].Remove(0, 9);
                            else if (info[j].StartsWith("ScreenResolution=")) ScreenResolution = info[j].Remove(0, 17);
                            else if (info[j].StartsWith("Description=")) Description = info[j].Remove(0, 12);

                        }
                        else if (temp2 == 2)
                        {
                            if (info[j].StartsWith("Type=")) ScriptType = info[j].Replace("Type=", "");
                        }
                        else if (temp2 == 3)
                        {
                            if (info[j].StartsWith("Tile=")) TileUri = Path.Combine(gamefolder, "Icon", info[j].Replace("Tile=", ""));
                            else if (info[j].StartsWith("Icon=")) IconUri = Path.Combine(gamefolder, "Icon", info[j].Replace("Icon=", ""));
                        }
                    }
                }
            }
            else if (storage.FileExists(Path.Combine(gamefolder, "uve-manifestEX_Unicode.uvm")))
            {
                type = 1;
                try
                {
                    IsolatedStorageFileStream location = new IsolatedStorageFileStream(Path.Combine(gamefolder, "uve-manifestEX_Unicode.uvm"), FileMode.Open, storage);
                    StreamReader file = new StreamReader(location);
                    for (int j = 0, b = 0; ; j++)
                    {
                        info[j] = file.ReadLine();
                        temp = j;
                        if (b == 1) break;
                        if (file.EndOfStream)
                            b = 1;
                    }
                    location.Close();
                    file.Close();
                }
                catch (Exception e)
                {
                    MessageBox.Show("这个游戏似乎不存在的说~~~\n以下为错误信息:" + e.Message, "错误", MessageBoxButton.OK);
                }
                for (int j = 0; j < temp; j++)
                {

                    if (info[j].StartsWith("[Info]")) temp2 = 1;
                    else if (info[j].StartsWith("[Script]")) temp2 = 2;
                    else if (info[j].StartsWith("[Icon]")) temp2 = 3;
                    else
                    {
                        if (temp2 == 1)
                        {
                            if (info[j].StartsWith("Game=")) GameName = info[j].Remove(0, 5);
                            else if (info[j].StartsWith("Company=")) Company = info[j].Remove(0, 8);
                            else if (info[j].StartsWith("Maker=")) GameMaker = info[j].Remove(0, 6);
                            else if (info[j].StartsWith("Size=")) GameSize = info[j].Remove(0, 5);
                            else if (info[j].StartsWith("Description=")) Description = info[j].Remove(0, 12);

                        }
                        else if (temp2 == 2)
                        {
                            if (info[j].StartsWith("Type=")) ScriptType = info[j].Remove(0, 5);
                            else if (info[j].StartsWith("Resolution=")) ScreenResolution = info[j].Remove(0, 11);
                        }
                        else if (temp2 == 3)
                        {
                            if (info[j].StartsWith("Tile=")) TileUri = Path.Combine(gamefolder, "Icon", info[j].Remove(0, 5));
                            else if (info[j].StartsWith("Icon=")) IconUri = Path.Combine(gamefolder, "Icon", info[j].Remove(0, 5));
                        }
                    }
                }
            }

        }
        protected override void OnNavigatedTo(System.Windows.Navigation.NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            gamefolder = NavigationContext.QueryString["game"];
            ReadInfo();
            ShowInfo();

        }

        private void Run(object sender, RoutedEventArgs e) 
        {
            if (type == 0)
            {
                this.NavigationService.Navigate(new Uri("/GamePage.xaml?game=" + gamefolder, UriKind.Relative));
                appSettings["temp"] = gamefolder;
            }
            else if (type == 1)
            {
                this.NavigationService.Navigate(new Uri("/AlphaLang/UVE-Native/UVE-Native.xaml?game=" + gamefolder, UriKind.Relative));
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
                DelFile(gamefolder);
                DelFile(Path.Combine("Shared", "ShellContent", gamefolder));
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
        private void GoBack()
        {
            this.NavigationService.GoBack();
        }
        protected override void OnBackKeyPress(System.ComponentModel.CancelEventArgs e)
        {
            base.OnBackKeyPress(e);
            if (deleting) e.Cancel = true;
        }
        private void PIN(object sender, RoutedEventArgs e)
        {
            try
            {
                StandardTileData tile = new StandardTileData
                {
                    Title = GameName,
                    BackgroundImage = new Uri("isostore:/Shared/ShellContent/" + gamefolder + "/Tile.png", UriKind.Absolute),
                };
                if (type == 0)
                {
                    ShellTile.Create(new Uri("/GamePage.xaml?game=" + gamefolder, UriKind.Relative), tile);
                }
                else
                {
                    ShellTile.Create(new Uri("/AlphaLang/UVE-Native/UVE-Native.xaml?game=" + gamefolder, UriKind.Relative), tile);
                }
            }
            catch (Exception exc)
            {
                MessageBox.Show("你似乎已经固定了这个Tile~~~当然不能再固定了……\n以下为错误消息:" + exc.Message, "错误", MessageBoxButton.OK);
                
            }
            
        }
        private void Back(object sender, RoutedEventArgs e)
        {
            if (!deleting) this.NavigationService.GoBack();
        }
        private void Delete(object sender, RoutedEventArgs e)
        {
            if (MessageBox.Show("确实要删除这个游戏吗", "确认信息", MessageBoxButton.OKCancel) == MessageBoxResult.OK)
            {
                if (MessageBox.Show("是否保留存档？", "确认信息", MessageBoxButton.OKCancel) == MessageBoxResult.Cancel)
                {
                    using (var store = IsolatedStorageFile.GetUserStoreForApplication())
                    {
                        try
                        {
                            store.DeleteFile(gamefolder + ".xml");
                        }
                        catch { }
                    }
                }
                thd = new Thread(new ThreadStart(Del_Proc));
                thd.Start();
                deleting = true;
            }
        }
        void DelFile(string unZipFilePath)//unZipFilePath第一次传递的是根目录名
        {
            using (var store = IsolatedStorageFile.GetUserStoreForApplication())
            {
                if (store.DirectoryExists(unZipFilePath))
                {
                    String[] dirNames = store.GetDirectoryNames(string.Concat(unZipFilePath, "\\*"));
                    String[] fileNames = store.GetFileNames(string.Concat(unZipFilePath, "\\*"));
                    if (fileNames.Length > 0)
                    {
                        for (int i = 0; i < fileNames.Length; i++)
                        {
                            store.DeleteFile(string.Concat(unZipFilePath, "\\", fileNames[i]));
                        }
                    }
                    if (dirNames.Length == 0)
                    {
                        store.DeleteDirectory(unZipFilePath);
                        if (unZipFilePath.IndexOf("\\") != -1)
                        {
                            unZipFilePath = unZipFilePath.Substring(0, unZipFilePath.LastIndexOf("\\"));
                            DelFile(unZipFilePath);
                        }
                    }
                    if (dirNames.Length > 0)
                    {
                        for (int i = 0; i < dirNames.Length; i++)
                        {
                            DelFile(string.Concat(unZipFilePath, "\\", dirNames[i]));
                        }
                    }
                }
            }
        }
    }
}