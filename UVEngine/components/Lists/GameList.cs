using System;
using System.Collections.ObjectModel;
using System.IO;
using System.IO.IsolatedStorage;
using System.Windows;
using System.Windows.Media.Imaging;
using UVEngineNative;

namespace List
{
    public class GameList
    {
        public String FolderName { get; set; }
        public String GameName { get; set; }
        public BitmapImage IconUri { get; set; }
        public String Company { get; set; }
        public String GameSize { get; set; }
        public String GameMaker { get; set; }
        public GameList(string folderName, string gameName, string iconUri, string company, string gameSize, string gamemaker)
        {

            this.FolderName = folderName;
            this.GameName = gameName;
            this.IconUri = new BitmapImage();
            try
            {
                using (IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication())
                {
                    if (iconUri != "")
                    {
                        using (IsolatedStorageFileStream fs = storage.OpenFile(iconUri, FileMode.Open, FileAccess.Read))
                        {
                            this.IconUri.SetSource(fs);
                        }
                    }
                }
            }
            catch
            {

            }
            this.Company = company;
            this.GameSize = gameSize + "MB";
            this.GameMaker = gamemaker;
        }

    }
    public class GameLists : ObservableCollection<GameList>
    {

        public GameLists()
        {
            IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication();
            string[] list = storage.GetDirectoryNames();
            for (int i = 0; i < list.Length; i++)
            {
                if (storage.FileExists(Path.Combine(list[i], "uve-manifest.uvm")))
                {
                    string GameName = "", Company = "", GameSize = "", GameMaker = "", IconUri = "";
                    string[] info = new string[30];
                    int temp = 0, temp2 = 0;
                    try
                    {
                        IsolatedStorageFileStream location = new IsolatedStorageFileStream(Path.Combine(list[i], "uve-manifest.uvm"), FileMode.Open, storage);
                        StreamReader file = new StreamReader(location);
                        for (int j = 0; !file.EndOfStream; j++)
                        {

                            info[j] = file.ReadLine();
                            temp = j;

                        }
                        location.Close();
                        file.Close();
                    }
                    catch (Exception e)
                    {
                        MessageBox.Show(UVEngine.Resources.UVEngine.listerror + e.Message, UVEngine.Resources.UVEngine.error, MessageBoxButton.OK);

                    }

                    for (int j = 0; j < temp; j++)
                    {
                        if (info[j].StartsWith("[Info]")) temp2 = 1;
                        else if (info[j].StartsWith("[Tile]")) temp2 = 3;
                        else if (temp2 == 1)
                        {
                            if (info[j].StartsWith("GameName=")) GameName = info[j].Remove(0, 9);
                            else if (info[j].StartsWith("Company=")) Company = info[j].Remove(0, 8);
                            else if (info[j].StartsWith("GameMaker=")) GameMaker = info[j].Remove(0, 10);
                            else if (info[j].StartsWith("GameSize=")) GameSize = info[j].Remove(0, 9);

                        }
                        else if (temp2 == 3)
                        {
                            if (info[j].StartsWith("Icon=")) IconUri = Path.Combine(list[i], "Icon", info[j].Remove(0,5));


                        }


                    }

                    Add(new GameList(list[i], GameName, IconUri, Company, GameSize, GameMaker));
                }
                else if (storage.FileExists(Path.Combine(list[i], "uve-manifestEX.uvm")))
                {
                    ManifestEX mex = new ManifestEX(Path.Combine(list[i], "uve-manifestEX.uvm"));
                    Add(new GameList(list[i], mex.GameName, Path.Combine(list[i], mex.IconPath), mex.GameCompany, mex.GameSize, mex.GameMaker));
                }
            }
        }
    }
}
