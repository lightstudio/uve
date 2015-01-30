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
using ICSharpCode.SharpZipLib.Zip;

namespace UVEngine
{
    public partial class QuickExtract : PhoneApplicationPage
    {
        IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication();
        Thread extractThread;
        static public bool sel = false;
        public QuickExtract()
        {
            InitializeComponent();
            extractThread = new Thread(new ThreadStart(extractProcess));
            extractThread.Start();
            packageofall.Text = "0/" + QuickImport.pics.Count.ToString();
        }
        void extractProcess()
        {
            int fc = 0;
            foreach (Microsoft.Xna.Framework.Media.Picture pic in QuickImport.pics)
            {
                this.Dispatcher.BeginInvoke(() =>
                {
                    package_extracting.Text = pic.Name;
                });
                Extract(pic.GetImage(), "");
                this.Dispatcher.BeginInvoke(() =>
                {
                    fc++;
                    int filec = QuickImport.pics.Count;
                    double value = 100 / (double)filec;
                    zipFileCount.Value += value;
                    packageofall.Text = fc.ToString() + "/" + filec;
                });
            }
            this.Dispatcher.BeginInvoke(() =>
            {
                //NavigationService.Navigate(new Uri("/MainPage.xaml", UriKind.Relative));
                NavigationService.GoBack();
            });
        }
        public bool Extract(Stream stream, string destinationPath)
        {
            try
            {
                //Stream stream = storage.OpenFile(sourceFilePath, System.IO.FileMode.Open);
                this.Dispatcher.BeginInvoke(() => { file_extracting.Text = UVEngine.Resources.UVEngine.analyzing; });
                byte[] bytes = new byte[4096];
                int position = 0;
                do
                {
                    stream.Read(bytes, 0, 4096);
                    for (int i = 0; i < 4096; i++)
                    {
                        if (bytes[i] == 0x50 &&
                            bytes[i + 1] == 0x4b &&
                            bytes[i + 2] == 0x03 &&
                            bytes[i + 3] == 0x04)
                            goto loopend;
                            position++;
                    }
                }
                while (stream.Position < stream.Length);
                loopend:
                //while (stream.Position != stream.Length)
                //{
                //    if (stream.ReadByte() == 0x50)
                //        if (stream.ReadByte() == 0x4b)
                //            if (stream.ReadByte() == 0x03)
                //                if (stream.ReadByte() == 0x04)
                //                    break;
                //}
                if (stream.Position >= stream.Length) return false;
                stream.Position = position;
                using (ZipInputStream zs = new ZipInputStream(stream))
                {
                    ZipEntry entry = null;
                    
                    while ((entry = zs.GetNextEntry()) != null)
                    {
                        string directoryName = Path.GetDirectoryName(entry.Name);
                        string fileName = Path.GetFileName(entry.Name);

                        if (!string.IsNullOrEmpty(fileName))
                        {
                            storage.CreateDirectory(Path.Combine(destinationPath, directoryName));
                            using (IsolatedStorageFileStream streamWriter = storage.CreateFile(Path.Combine(destinationPath, directoryName, fileName)))
                            {
                                this.Dispatcher.BeginInvoke(() => { file_extracting.Text = fileName; });
                                int size = 1048576;
                                byte[] data = new byte[size];
                                while (true)
                                {
                                    long swpos = streamWriter.Position;
                                    size = zs.Read(data, 0, data.Length);
                                    if (size > 0)
                                    {
                                        this.Dispatcher.BeginInvoke(() =>
                                        {
                                            try
                                            {
                                                fileSizeCount.Value = (double)swpos / (double)zs.Length * 100;
                                                contentFileCount.Value = (double)stream.Position / (double)stream.Length * 100;
                                            }
                                            catch { }
                                        });
                                        streamWriter.Write(data, 0, size);
                                    }
                                    else
                                    {
                                        this.Dispatcher.BeginInvoke(() =>
                                            {
                                                fileSizeCount.Value = 100;
                                            });
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
                    MessageBox.Show(Ex.Message, UVEngine.Resources.UVEngine.error, MessageBoxButton.OK);
                });
            }
            return true;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (MessageBox.Show(UVEngine.Resources.UVEngine.really, UVEngine.Resources.UVEngine.tip, MessageBoxButton.OKCancel) == MessageBoxResult.OK)
            {
                NavigationService.GoBack();
            }
        }
        protected override void OnBackKeyPress(System.ComponentModel.CancelEventArgs e)
        {
            if (MessageBox.Show(UVEngine.Resources.UVEngine.really, UVEngine.Resources.UVEngine.tip, MessageBoxButton.OKCancel) != MessageBoxResult.OK)
            {
                e.Cancel = true;
            }
            base.OnBackKeyPress(e);
        }
    }
}