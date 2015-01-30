using Microsoft.Phone.Controls;
using System;
using System.IO;
using System.IO.IsolatedStorage;
using System.Windows;
using System.Windows.Controls;

namespace UVEngine
{
    public partial class FileClear : PhoneApplicationPage
    {
        IsolatedStorageFile isf = IsolatedStorageFile.GetUserStoreForApplication();
        IsolatedStorageSettings iss = IsolatedStorageSettings.ApplicationSettings;
        public FileClear()
        {
            if (!isf.DirectoryExists("Images"))
            {
                isf.CreateDirectory("Images");
                isf.CreateDirectory("Images\\icons");
                System.Windows.Resources.StreamResourceInfo res = Application.GetResourceStream(new Uri("Icon\\folder-pic.png",UriKind.Relative));
                using (IsolatedStorageFileStream isfs = isf.CreateFile("Images\\icons\\folder.png"))
                {
                    int chunkSize = 4096;
                    byte[] bytes = new byte[chunkSize];
                    int byteCount;
                    int count = 0;
                    while ((byteCount = res.Stream.Read(bytes, 0, chunkSize)) > 0)
                    {
                        count++;
                        isfs.Write(bytes, 0, byteCount);
                    }
                }
                res.Stream.Close();
                res = Application.GetResourceStream(new Uri("Icon\\txt.png", UriKind.Relative));
                using (IsolatedStorageFileStream isfs = isf.CreateFile("Images\\icons\\file.png"))
                {
                    int chunkSize = 4096;
                    byte[] bytes = new byte[chunkSize];
                    int byteCount;
                    int count = 0;
                    while ((byteCount = res.Stream.Read(bytes, 0, chunkSize)) > 0)
                    {
                        count++;
                        isfs.Write(bytes, 0, byteCount);
                    }
                }
                res.Stream.Close();
            }
            else if (!isf.DirectoryExists("Images\\icons"))
            {
                isf.CreateDirectory("Images\\icons");
                System.Windows.Resources.StreamResourceInfo res = Application.GetResourceStream(new Uri("Icon\\folder-pic.png", UriKind.Relative));
                using (IsolatedStorageFileStream isfs = isf.CreateFile("Images\\icons\\folder.png"))
                {
                    int chunkSize = 4096;
                    byte[] bytes = new byte[chunkSize];
                    int byteCount;
                    int count = 0;
                    while ((byteCount = res.Stream.Read(bytes, 0, chunkSize)) > 0)
                    {
                        count++;
                        isfs.Write(bytes, 0, byteCount);
                    }
                }
                res.Stream.Close();
                res = Application.GetResourceStream(new Uri("Icon\\txt.png", UriKind.Relative));
                using (IsolatedStorageFileStream isfs = isf.CreateFile("Images\\icons\\file.png"))
                {
                    int chunkSize = 4096;
                    byte[] bytes = new byte[chunkSize];
                    int byteCount;
                    int count = 0;
                    while ((byteCount = res.Stream.Read(bytes, 0, chunkSize)) > 0)
                    {
                        count++;
                        isfs.Write(bytes, 0, byteCount);
                    }
                }
                res.Stream.Close();
            }
            InitializeComponent();
            Microsoft.Phone.Controls.TiltEffect.TiltableItems.Add(typeof(Microsoft.Phone.Controls.MenuItem));
            if (!iss.Contains("mentioned"))
            {
                //MessageBox.Show("本功能仅供删除部分垃圾文件，比如未解压完成的temp.zip，或者删除游戏时删除失败的残留文件，请不要乱删东西，否则会丢失数据的说……");
                MessageBox.Show(UVEngine.Resources.UVEngine.aboutfileclear);
                iss.Add("mentioned", 0);
                iss.Save();
            }
        }
        private bool delta = false, deleting = false;
        private void Touch(object sender, System.Windows.Input.ManipulationDeltaEventArgs e)
        {
            delta = true;
        }
        private void Choose(object sender, System.Windows.Input.ManipulationCompletedEventArgs e)
        {
            if (delta)
            {
                delta = false;
            }
            else
            {
                if (MessageBox.Show(UVEngine.Resources.UVEngine.confirmdelete, UVEngine.Resources.UVEngine.comfirminfo, MessageBoxButton.OKCancel) == MessageBoxResult.OK)
                {
                    List.FileInfo IsFolder = (List.FileInfo)(sender as Grid).Tag;
                    perfbar.IsIndeterminate = true;
                    if (IsFolder.isFolder)
                    {
                        this.Dispatcher.BeginInvoke(() =>
                        {
                            deleting = true;
                            DelFile(IsFolder.Name);
                            System.Threading.Thread.Sleep(1000);
                            list.ItemsSource = new List.FileList();
                            list.InvalidateArrange();
                            perfbar.IsIndeterminate = false;
                            deleting = false;
                        });
                    }
                    else
                    {
                        this.Dispatcher.BeginInvoke(() =>
                            {
                                isf.DeleteFile(IsFolder.Name);
                                System.Threading.Thread.Sleep(1000);
                                list.ItemsSource = new List.FileList();
                                list.InvalidateArrange();
                                perfbar.IsIndeterminate = false;
                            });
                    }
                }
            }
        }
        void DelFile(string unZipFilePath)
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
        protected override void OnBackKeyPress(System.ComponentModel.CancelEventArgs e)
        {
            base.OnBackKeyPress(e);
            if (deleting)
            {
                MessageBox.Show(UVEngine.Resources.UVEngine.removing);
            }
        }
    }
}