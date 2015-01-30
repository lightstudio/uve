using System;
using System.Collections.ObjectModel;
using System.IO;
using System.IO.IsolatedStorage;
using System.Windows;
using System.Windows.Media.Imaging;
using UVEngineNative;

namespace List
{
    public class FileInfo
    {
        public String Name;
        public bool isFolder;


    }
    public class File
    {
        public String Name { get; set; }
        public FileInfo info { get; set; }
        public String IsFolder { get; set; }
        public String IsGame { get; set; }
        public BitmapImage FileIcon { get; set; }
        public File(string Name, bool IsFolder, bool IsGame, string IconUri)
        {
            this.FileIcon = new BitmapImage();
            this.Name = Name;
            this.info = new FileInfo();
            this.info.Name = Name;
            if (IsFolder)
            {
                this.IsFolder = UVEngine.Resources.UVEngine.folder;
                this.info.isFolder = true;                
                if (this.Name == "Shared")
                {
                    this.Name = UVEngine.Resources.UVEngine.shared;
                    this.IsFolder = UVEngine.Resources.UVEngine.sharedusage;
                    this.info.isFolder = true;
                }
                else if (this.Name == "PlatformData")
                {
                    this.Name = UVEngine.Resources.UVEngine.platformdata;
                    this.IsFolder = UVEngine.Resources.UVEngine.systemfolder;
                    this.info.isFolder = true;
                }
                else if (this.Name == "Images")
                {
                    this.Name = UVEngine.Resources.UVEngine.piccache;
                    this.IsFolder = UVEngine.Resources.UVEngine.systemfolder;
                    this.info.isFolder = true;
                }
            }
            else
            {
                if (this.Name == "__ApplicationSettings")
                {
                    this.Name = UVEngine.Resources.UVEngine.appsettings;
                    this.IsFolder = UVEngine.Resources.UVEngine.importantfile;
                    this.info.isFolder = false;
                } 
                if (this.Name == "bench")
                {
                    this.Name = UVEngine.Resources.UVEngine.benchremain;
                    this.IsFolder = UVEngine.Resources.UVEngine.cache;
                    this.info.isFolder = false;
                }
                else if (this.Name == "app.config")
                {
                    this.Name = UVEngine.Resources.UVEngine.appconfig;
                    this.IsFolder = UVEngine.Resources.UVEngine.importantfile;
                    this.info.isFolder = false;

                }
                else if (this.Name == "app.config")
                {
                    this.IsFolder = UVEngine.Resources.UVEngine.importantfile;
                    this.info.isFolder = false;
                }
                else if (this.Name == "XLiveGuide.xml")
                {
                    this.Name = UVEngine.Resources.UVEngine.openxliveconfig;
                    this.IsFolder = UVEngine.Resources.UVEngine.importantfile;
                    this.info.isFolder = false;
                }
                else if (this.Name.EndsWith(".xml")||this.Name.EndsWith(".bin"))
                {
                    this.IsFolder = UVEngine.Resources.UVEngine.gamesavefile;
                    this.info.isFolder = false;
                }
                else
                {
                    this.IsFolder = UVEngine.Resources.UVEngine.file;
                    this.info.isFolder = false;
                }
            }
            if (IsGame)
            {
                this.IsGame = UVEngine.Resources.UVEngine.gamefolder;
                try
                {
                    using (IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication())
                    {
                        if (IconUri != "")
                        {
                            using (IsolatedStorageFileStream fs = storage.OpenFile(IconUri, FileMode.Open, FileAccess.Read))
                            {
                                this.FileIcon.SetSource(fs);
                            }
                        }
                    }
                }
                catch
                {

                }
            }
            else
            {
                this.IsGame = UVEngine.Resources.UVEngine.otherfolder;
                try
                {
                    if (IsFolder)
                    {
                        using (var folder = new IsolatedStorageFileStream("Images\\icons\\folder.png", FileMode.Open, IsolatedStorageFile.GetUserStoreForApplication()))
                        {
                            this.FileIcon.SetSource(folder);

                        }
                    }
                    else
                    {
                        using (var file = new IsolatedStorageFileStream("Images\\icons\\file.png", FileMode.Open, IsolatedStorageFile.GetUserStoreForApplication()))
                        {
                            this.FileIcon.SetSource(file);

                        }


                    }
                }
                catch
                {

                }
            }
        }
    }
    public class FileList : ObservableCollection<File>
    {
        public FileList()
        {
            IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication();
            string[] folderlist = storage.GetDirectoryNames(), filelist = storage.GetFileNames();
            string[] info = new string[30];
            string IconUri = "";
            int temp = 0, temp2 = 0;

                for (int i = 0; i < folderlist.Length; i++)
                {
                    bool IsGame = false;
                    if (storage.FileExists(Path.Combine(folderlist[i], "uve-manifest.uvm")))
                    {
                        IsGame = true;
                        IsolatedStorageFileStream location = new IsolatedStorageFileStream(Path.Combine(folderlist[i], "uve-manifest.uvm"), FileMode.Open, storage);
                        StreamReader file = new StreamReader(location);
                        for (int j = 0; !file.EndOfStream; j++)
                        {

                            info[j] = file.ReadLine();
                            temp = j;

                        }
                        for (int j = 0; j < temp; j++)
                        {
                            if (info[j].StartsWith("[Info]")) temp2 = 1;
                            else if (info[j].StartsWith("[Tile]")) temp2 = 3;
                            else
                            {
                                if (temp2 == 3)
                                {
                                    if (info[j].StartsWith("Icon=")) IconUri = Path.Combine(folderlist[i], "Icon", info[j].Remove(0, 5));


                                }
                            }

                        }
                        location.Close();
                        file.Close();
                    }
                    else if (storage.FileExists(Path.Combine(folderlist[i], "uve-manifestEX.uvm")))
                    {
                        IsGame = true;
                        ManifestEX mex = new ManifestEX(Path.Combine(folderlist[i], "uve-manifestEX.uvm"));
                        IconUri = Path.Combine(folderlist[i], mex.IconPath);

                    }
                    Add(new File(folderlist[i], true, IsGame, IconUri));
                }



            for (int i = 0; i < filelist.Length; i++)
            {
                Add(new File(filelist[i], false, false, ""));
            }
        }
    }
}