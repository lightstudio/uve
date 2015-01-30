using ICSharpCode.SharpZipLib.Zip;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Tasks;
using System;
using System.IO;
using System.IO.IsolatedStorage;
using System.Threading;
using System.Windows;
using System.Windows.Navigation;
using System.Windows.Resources;


namespace UVEngine
{
    public partial class Import : PhoneApplicationPage
    {
        PhotoChooserTask pc; 
        IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication();
        int import = 0;
        Thread thd;
        PhotoResult result;
        delegate void ProgressDelegate(double i);
        delegate void Error(string Error);
        delegate void Back();
        ProgressDelegate progressDelegate;
        Back back;
        Error er;
        public Import()
        {
            InitializeComponent();
            pc = new PhotoChooserTask();
            pc.Completed += new EventHandler<PhotoResult>(pc_Completed);
        }
        private void SetProgress(double i)
        {
            pb1.Value = 100.00 * i;
            if (i >= 1)
            {
                pb1.Visibility = Visibility.Collapsed;
                Tb.Text = "复制完成，正在解压";
                perf.Visibility = Visibility.Visible;
                perf.Opacity = 1;
                perf.IsIndeterminate = true;
                
            }
        }
        private void GoBack()
        {
            //this.NavigationService.RemoveBackEntry();
            //this.NavigationService.Navigate(new Uri("/MainPage.xaml", UriKind.Relative));
            this.NavigationService.GoBack();
            if (thd.IsAlive) thd.Abort();
        }
        private void ShowError(string Error)
        {
            MessageBox.Show(Error, "错误", MessageBoxButton.OK);
            GoBack();
        }
        void pc_Completed(object sender, PhotoResult e)
        {
            thd = new Thread(new ThreadStart(ThreadProc));
            if (e.TaskResult == TaskResult.OK)
            {
                result = e;
                progressDelegate = SetProgress;
                back = GoBack;
                er = ShowError;
                thd.Start();
            }
            else
            {
                GoBack();
            }
        }
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            if (import == 0)
            {
                pc.Show();
                import = 1;
            }

        }
        private void ThreadProc()
        {
            IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication();
            using (storage = IsolatedStorageFile.GetUserStoreForApplication())
            {
                 StreamResourceInfo resource = new StreamResourceInfo(result.ChosenPhoto, null);
                 using (IsolatedStorageFileStream file = storage.CreateFile("temp.zip"))
                 {
                     int chunkSize = 4096;
                     byte[] bytes = new byte[chunkSize];
                     int byteCount;
                     int count = 0;
                     while ((byteCount = resource.Stream.Read(bytes, 0, chunkSize)) > 0)
                     {
                         count++;
                         file.Write(bytes, 0, byteCount);
                         double i = (double)count * (double)byteCount / (double)resource.Stream.Length;
                         this.Dispatcher.BeginInvoke(progressDelegate, i);
                     }
                 }
                this.Dispatcher.BeginInvoke(progressDelegate, 1);
                try
                {
                    if (!DeCompress("temp.zip", "/"))
                    {
                        storage.DeleteFile("temp.zip");
                        throw new Exception("你似乎选择了一个没有包含数据的图片的说~~不要幻想能够从没有包含数据文件的图片里找到GAL GAME~~回去重新找找吧！（快点感谢我没让程序闪退）"); ;
                    }
                }
                catch (Exception Ex)
                {
                    this.Dispatcher.BeginInvoke(er, Ex.Message);
                }
                this.Dispatcher.BeginInvoke(back);
                //                    this.NavigationService.RemoveBackEntry();
                //                    this.NavigationService.Navigate(new Uri("/MainPage.xaml", UriKind.Relative));
            }


        }
        protected override void OnBackKeyPress(System.ComponentModel.CancelEventArgs e)
        {
            base.OnBackKeyPress(e);
            if (MessageBox.Show("真的要停止复制和解压的过程吗？可能会产生大量垃圾文件！", "警告", MessageBoxButton.OKCancel) != MessageBoxResult.OK)
            {
                e.Cancel = true;
            }
            else
            {
                thd.Abort();

            }
        }
       
        public bool DeCompress(string sourceFilePath, string destinationPath)
        {
            try
            {
                Stream stream = storage.OpenFile(sourceFilePath, System.IO.FileMode.Open);
                while (stream.Position != stream.Length)
                {
                    if (stream.ReadByte() == 0x50)
                        if (stream.ReadByte() == 0x4b)
                            if (stream.ReadByte() == 0x03)
                                if (stream.ReadByte() == 0x04)
                                    break;
                }
                if (stream.Position == stream.Length) return false;
                stream.Position = stream.Position - 4;
                using (ZipInputStream zs = new ZipInputStream(stream))
                {
                    ZipEntry entry = null;
                    //解压缩*.rar文件运行至此处出错：Wrong Local header signature: 0x21726152，解压*.zip文件不出错 
                    while ((entry = zs.GetNextEntry()) != null)
                    {
                        string directoryName = Path.GetDirectoryName(entry.Name);
                        string fileName = Path.GetFileName(entry.Name);

                        if (!string.IsNullOrEmpty(fileName))
                        {
                            storage.CreateDirectory(Path.Combine(destinationPath, directoryName));
                            using (IsolatedStorageFileStream streamWriter = storage.CreateFile(Path.Combine(destinationPath, directoryName, fileName)))
                            {
                                int size = 2048;
                                byte[] data = new byte[size];
                                while (true)
                                {
                                    size = zs.Read(data, 0, data.Length);
                                    if (size > 0)
                                    {
                                        streamWriter.Write(data, 0, size);
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                stream.Close();
            }
            catch (System.Exception Ex)
            {
                this.Dispatcher.BeginInvoke(er, Ex.Message);
                //                MessageBox.Show(Ex.Message, "错误", MessageBoxButton.OK);
            }
            return true;
        }
        
    }
}