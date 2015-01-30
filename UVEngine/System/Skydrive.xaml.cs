using Microsoft.Live;
using Microsoft.Phone.Controls;
using OpenXLive.Silverlight.Controls;
using System;
using System.IO;
using System.Threading;
using System.Windows;
using System.Collections.Generic;
using System.IO.IsolatedStorage;
using ICSharpCode.SharpZipLib.Zip;
using ICSharpCode.SharpZipLib;
using ICSharpCode.SharpZipLib.GZip;

namespace UVEngine
{
    public partial class Skydrive : PhoneApplicationPage
    {
        private IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication();
        private IsolatedStorageSettings iss = IsolatedStorageSettings.ApplicationSettings;
        private LiveConnectClient lcc;
        private ZipOutputStream zip;
        private static LiveConnectSession session;
        string fileLocation;
        string FilePath;
        // 构造函数
        public Skydrive()
        {
            InitializeComponent();
            if (!iss.Contains("FileID"))
                iss.Add("FileID", 0);
            // 用于本地化 ApplicationBar 的示例代码
            //BuildLocalizedApplicationBar();
        }
        public static LiveConnectSession Session
        {
            get
            {
                return Skydrive.session;
            }
        }
        public bool DeCompress(string sourceFilePath, string destinationPath)
        {
            try
            {
                Stream stream = storage.OpenFile(sourceFilePath, System.IO.FileMode.Open);
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
                this.Dispatcher.BeginInvoke(() =>
                {
                    MessageBox.Show(Ex.Message);
                });
            }
            return true;
        }
        public void Packup()
        {
            string[] filenames = storage.GetFileNames("*.xml");
            string[] savnames = storage.GetFileNames("*save?.dat");
            string[] savnames_ = storage.GetFileNames("*save??.dat");
            string[] globalsaves = storage.GetFileNames("*gloval.sav");
            string[] kidoku = storage.GetFileNames("*kidoku.dat");
            string[] envdata = storage.GetFileNames("*envdata");
            string[] ini = storage.GetFileNames("*.ini");
            MemoryStream ms = new MemoryStream();
            IsolatedStorageFileStream file = storage.CreateFile("temp.zip");
            zip = new ZipOutputStream(file);
            try
            {
                //                string[] filenames = Directory.GetFiles(dirPath);
                zip.SetLevel(9);
                byte[] buffer = new byte[4096];
                foreach (string File in filenames)
                {
                    ZipEntry entry = new ZipEntry(Path.GetFileName(File));
                    entry.DateTime = DateTime.Now;
                    zip.PutNextEntry(entry);
                    using (FileStream fs = new IsolatedStorageFileStream(File, FileMode.Open, storage))
                    {
                        int sourceBytes;
                        do
                        {
                            sourceBytes = fs.Read(buffer, 0, buffer.Length);
                            zip.Write(buffer, 0, sourceBytes);
                        } while (sourceBytes > 0);
                    }
                } 
                foreach (string File in savnames)
                {
                    ZipEntry entry = new ZipEntry(Path.GetFileName(File));
                    entry.DateTime = DateTime.Now;
                    zip.PutNextEntry(entry);
                    using (FileStream fs = new IsolatedStorageFileStream(File, FileMode.Open, storage))
                    {
                        int sourceBytes;
                        do
                        {
                            sourceBytes = fs.Read(buffer, 0, buffer.Length);
                            zip.Write(buffer, 0, sourceBytes);
                        } while (sourceBytes > 0);
                    }
                } 
                foreach (string File in savnames_)
                {
                    ZipEntry entry = new ZipEntry(Path.GetFileName(File));
                    entry.DateTime = DateTime.Now;
                    zip.PutNextEntry(entry);
                    using (FileStream fs = new IsolatedStorageFileStream(File, FileMode.Open, storage))
                    {
                        int sourceBytes;
                        do
                        {
                            sourceBytes = fs.Read(buffer, 0, buffer.Length);
                            zip.Write(buffer, 0, sourceBytes);
                        } while (sourceBytes > 0);
                    }
                } 
                foreach (string File in globalsaves)
                {
                    ZipEntry entry = new ZipEntry(Path.GetFileName(File));
                    entry.DateTime = DateTime.Now;
                    zip.PutNextEntry(entry);
                    using (FileStream fs = new IsolatedStorageFileStream(File, FileMode.Open, storage))
                    {
                        int sourceBytes;
                        do
                        {
                            sourceBytes = fs.Read(buffer, 0, buffer.Length);
                            zip.Write(buffer, 0, sourceBytes);
                        } while (sourceBytes > 0);
                    }
                } 
                foreach (string File in kidoku)
                {
                    ZipEntry entry = new ZipEntry(Path.GetFileName(File));
                    entry.DateTime = DateTime.Now;
                    zip.PutNextEntry(entry);
                    using (FileStream fs = new IsolatedStorageFileStream(File, FileMode.Open, storage))
                    {
                        int sourceBytes;
                        do
                        {
                            sourceBytes = fs.Read(buffer, 0, buffer.Length);
                            zip.Write(buffer, 0, sourceBytes);
                        } while (sourceBytes > 0);
                    }
                } 
                foreach (string File in envdata)
                {
                    ZipEntry entry = new ZipEntry(Path.GetFileName(File));
                    entry.DateTime = DateTime.Now;
                    zip.PutNextEntry(entry);
                    using (FileStream fs = new IsolatedStorageFileStream(File, FileMode.Open, storage))
                    {
                        int sourceBytes;
                        do
                        {
                            sourceBytes = fs.Read(buffer, 0, buffer.Length);
                            zip.Write(buffer, 0, sourceBytes);
                        } while (sourceBytes > 0);
                    }
                }
                foreach (string File in ini)
                {
                    ZipEntry entry = new ZipEntry(Path.GetFileName(File));
                    entry.DateTime = DateTime.Now;
                    zip.PutNextEntry(entry);
                    using (FileStream fs = new IsolatedStorageFileStream(File, FileMode.Open, storage))
                    {
                        int sourceBytes;
                        do
                        {
                            sourceBytes = fs.Read(buffer, 0, buffer.Length);
                            zip.Write(buffer, 0, sourceBytes);
                        } while (sourceBytes > 0);
                    }
                }
                zip.Finish();
                zip.Close();
            }
            catch (Exception ex)
            {
                this.Dispatcher.BeginInvoke(() =>
                {
                    MessageBox.Show(UVEngine.Resources.UVEngine.compressionerror + ex.Message);
                });
            }
        }

        private void SignInButton_SessionChanged_1(object sender, Microsoft.Live.Controls.LiveConnectSessionChangedEventArgs e)
        {
            if (e.Status == LiveConnectSessionStatus.Connected)
            {
                lcc = new LiveConnectClient(e.Session);
                session = e.Session;
            }
            else
            {
                lcc = null;
            }
        }

        private async void Button_Click_1(object sender, RoutedEventArgs e)
        {
            //try
            //{
            //    if (storage.FileExists("temp.zip"))
            //        storage.DeleteFile("temp.zip");
            //}
            //catch (Exception ex)
            //{
            //    MessageBox.Show(ex.Message);
            //}
            //this.Dispatcher.BeginInvoke(()=>
            //    {
            if (lcc == null)
            {
                //                MessageBox.Show("首先，你要登陆....");
                this.Dispatcher.BeginInvoke(() =>
                {
                    ToastPrompt toast = new ToastPrompt(); //实例化 
                    toast.Height = 100;
                    //toast.Title = "通知"; //设置标题 
                    toast.Message = UVEngine.Resources.UVEngine.mustsignin; //设置正文消息 
                    toast.FontSize = 30; //设置文本大小(可选) 
                    toast.TextOrientation = System.Windows.Controls.Orientation.Vertical; //设置呈现为纵向 
                    toast.Show();
                });
                return;
            }
            Packup();
            Upload.IsEnabled = false;
            IsolatedStorageFileStream fs = storage.OpenFile("temp.zip", FileMode.Open);
            try
            {
                var progressHandler = new Progress<LiveOperationProgress>((LiveOperationProgress progress) =>
                {
                    this.progressBar.Value = progress.ProgressPercentage;
                });
                LiveOperationResult operationResult = await this.lcc.UploadAsync(@"/me/skydrive", "UVE_backup.zip", fs, OverwriteOption.Overwrite, new System.Threading.CancellationToken(false), progressHandler);
                dynamic result = operationResult.Result;
                string Location = result.source;
                //MessageBox.Show(Location);
                fileLocation = Location;
                iss["FileID"] = Location;
                iss.Save();
                this.Upload.IsEnabled = true;
            }
            catch (LiveConnectException ex)
            {
                MessageBox.Show(ex.Message);
            }
            fs.Close();
            zip.Close();
            this.Dispatcher.BeginInvoke(() =>
            {
                ToastPrompt toast = new ToastPrompt(); //实例化 
                toast.Height = 100;
                //toast.Title = "通知"; //设置标题 
                toast.Message = UVEngine.Resources.UVEngine.backupcomplete; //设置正文消息 
                toast.FontSize = 30; //设置文本大小(可选) 
                toast.TextOrientation = System.Windows.Controls.Orientation.Vertical; //设置呈现为纵向 
                toast.Show();
            });
            //MessageBox.Show("备份已完成....");
            storage.DeleteFile("temp.zip");

                //});
            //try
            //{
            //    if (lcc == null)
            //    {
            //        MessageBox.Show("首先，你要登陆....");
            //        return;
            //    }
            //    if (iss.Contains("FolderID"))
            //    {
            //        FilePath = (string)iss["FolderID"];
            //    }
            //    else
            //    {
            //        try
            //        {
            //            var folderData = new Dictionary<string, object>();
            //            folderData.Add("name", "UVEngine");
            //            LiveOperationResult operationResult =
            //                   await lcc.PostAsync("/me/skydrive", folderData);
            //            dynamic result = operationResult.Result;
            //            FilePath = result.id;
            //            iss.Add("FolderID", FilePath);
            //            iss.Save();
            //            MessageBox.Show(string.Join(" ", "Created folder:", result.name, "ID:", result.id));
            //        }
            //        catch (LiveConnectException exception)
            //        {
            //            MessageBox.Show("创建文件夹时发生错误: " + exception.Message);
            //        }
            //    }
            //}
            //catch (Exception ex)
            //{
            //    MessageBox.Show(ex.Message);
            //}
        }
        protected async override void OnNavigatedTo(System.Windows.Navigation.NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            if (BrowseSkyDrive.FileID != "0")
            {
                fileLocation = BrowseSkyDrive.FileID;
                //                lcc = new LiveConnectClient(CoreConcepts.BrowseSkyDrive.Session);
                lcc = new LiveConnectClient(session);
                //while (lcc == null)
                //{
                //    Thread.Sleep(1000);
                //}
                this.progressBar.Value = 0;
                //               lcc=CoreConcepts.BrowseSkyDrive.
                //if (lcc == null)
                //{
                //    MessageBox.Show("首先，你要登陆....");
                //    return;
                //}
                var progressHandler = new Progress<LiveOperationProgress>((LiveOperationProgress progress) =>
                {
                    this.progressBar.Value = progress.ProgressPercentage;
                });
                try
                {
                    LiveOperationResult operationResult =
                    await lcc.GetAsync(fileLocation);
                    dynamic result = operationResult.Result;
                    //       this.infoTextBlock.Text = string.Join(" ", "File name:", result.name, "ID:", result.id);
                    LiveDownloadOperationResult downloadOperationResult =
                    await this.lcc.DownloadAsync(result.source, new CancellationToken(false), progressHandler);
                    using (Stream downloadStream = downloadOperationResult.Stream)
                    {
                        if (downloadStream != null)
                        {
                            using (IsolatedStorageFileStream file = storage.CreateFile("temp.zip"))
                            {
                                int chunkSize = 4096;
                                byte[] bytes = new byte[chunkSize];
                                int byteCount;

                                while ((byteCount = downloadStream.Read(bytes, 0, chunkSize)) > 0)
                                {
                                    file.Write(bytes, 0, byteCount);
                                }
                                file.Close();
                            }
                                        //                        MessageBox.Show("成功下载备份，正在准备解压……");
                            this.Dispatcher.BeginInvoke(() =>
                                {
                                    ToastPrompt toast = new ToastPrompt(); //实例化 
                                    toast.Height = 100;
                                    //toast.Title = "通知"; //设置标题 
                                    toast.Message = UVEngine.Resources.UVEngine.downloaded; //设置正文消息 
                                    toast.FontSize = 30; //设置文本大小(可选) 
                                    toast.TextOrientation = System.Windows.Controls.Orientation.Vertical; //设置呈现为纵向 
                                    toast.Show();
                                });
                            if (DeCompress("temp.zip", "/"))
                            {
                                storage.DeleteFile("temp.zip");
                                ToastPrompt t = new ToastPrompt(); //实例化 
                                t.Height = 100;
                                //toast.Title = "通知"; //设置标题 
                                t.Message = UVEngine.Resources.UVEngine.restorecompleted; //设置正文消息 
                                t.FontSize = 40; //设置文本大小(可选) 
                                t.TextOrientation = System.Windows.Controls.Orientation.Vertical; //设置呈现为纵向 
                                t.Show();
                                //MessageBox.Show("存档恢复成功");
                            }
                            else
                                MessageBox.Show("Failed");
                        }
                        else
                        {
                            //MessageBox.Show("Stream Null Error");
                            MessageBox.Show("An Error Occurred...");
                        }
                    }
                }
                catch (Exception Ex)
                {
                    MessageBox.Show("以下为错误信息\n" + Ex.Message, "错误", MessageBoxButton.OK);
                }
                this.NavigationService.RemoveBackEntry();
            }
        }
        private void Button_Click_2(object sender, System.Windows.RoutedEventArgs e)
        {
            //this.progressBar.Value = 0;
            if (lcc == null)
            {
                //                MessageBox.Show("首先，你要登陆....");
                this.Dispatcher.BeginInvoke(() =>
                {
                    ToastPrompt toast = new ToastPrompt(); //实例化 
                    toast.Height = 100;
                    //toast.Title = "通知"; //设置标题 
                    toast.Message = UVEngine.Resources.UVEngine.mustsignin; //设置正文消息 
                    toast.FontSize = 30; //设置文本大小(可选) 
                    toast.TextOrientation = System.Windows.Controls.Orientation.Vertical; //设置呈现为纵向 
                    toast.Show();
                });
                return;
            }
            else
            {
                NavigationService.Navigate(new Uri("/System/BrowseSkydrive.xaml", UriKind.Relative));
                //ToastPrompt toast = new ToastPrompt(); //实例化 
                //toast.Height = 100;
                ////toast.Title = "通知"; //设置标题 
                //toast.Message = "正在下载备份"; //设置正文消息 
                //toast.FontSize = 30; //设置文本大小(可选) 
                //toast.TextOrientation = System.Windows.Controls.Orientation.Vertical; //设置呈现为纵向 
                //toast.Show();
                //Download.IsEnabled = false;
            }
            //var progressHandler = new Progress<LiveOperationProgress>((LiveOperationProgress progress) =>
            //{
            //    this.progressBar.Value = progress.ProgressPercentage;
            //});
            //try
            //{
            //    LiveDownloadOperationResult downloadOperationResult =
            //        await this.lcc.DownloadAsync((string)iss["FileID"]/*"/me/skydrive/UVE_backup.zip"*/, new CancellationToken(false), progressHandler);
            //    using (Stream downloadStream = downloadOperationResult.Stream)
            //    {
            //        if (downloadStream != null)
            //        {
            //            //                        MessageBox.Show("成功下载备份，正在准备解压……");
            //            this.Dispatcher.BeginInvoke(() =>
            //                {
            //                    ToastPrompt toast = new ToastPrompt(); //实例化 
            //                    toast.Height = 100;
            //                    //toast.Title = "通知"; //设置标题 
            //                    toast.Message = "成功下载备份，正在解压"; //设置正文消息 
            //                    toast.FontSize = 30; //设置文本大小(可选) 
            //                    toast.TextOrientation = System.Windows.Controls.Orientation.Vertical; //设置呈现为纵向 
            //                    toast.Show();
            //                }
            //            );
            //            using (IsolatedStorageFileStream file = storage.CreateFile("temp.zip"))
            //            {
            //                int chunkSize = 4096;
            //                byte[] bytes = new byte[chunkSize];
            //                int byteCount;

            //                while ((byteCount = downloadStream.Read(bytes, 0, chunkSize)) > 0)
            //                {
            //                    file.Write(bytes, 0, byteCount);
            //                }
            //                file.Close();
            //            }
            //            DeCompress("temp.zip", "/");
            //            ToastPrompt t = new ToastPrompt(); //实例化 
            //            t.Height = 100;
            //            //toast.Title = "通知"; //设置标题 
            //            t.Message = "解压完成"; //设置正文消息 
            //            t.FontSize = 40; //设置文本大小(可选) 
            //            t.TextOrientation = System.Windows.Controls.Orientation.Vertical; //设置呈现为纵向 
            //            t.Show();
            //            storage.DeleteFile("temp.zip");
            //        }
            //        else
            //        {
            //            MessageBox.Show("An Error Occurred...");
            //        }
            //    }
            //}
            //catch (Exception Ex)
            //{
            //    MessageBox.Show("首先请确认你备份过数据....以下为错误信息\n" + Ex.Message, "错误", MessageBoxButton.OK);
            //}
            //finally
            //{
            //    Download.IsEnabled = true;
            //}
        }


    }
}