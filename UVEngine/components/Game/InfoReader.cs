using System;
using System.IO;
using System.IO.IsolatedStorage;
using System.Windows;

namespace UVEngine
{
    public class GameInfo
    {
        public string GameName = "", Company = "", GameMaker = "", GameSize = "", Type = "", ScriptFile = "", DataFolder = "", Icon = "", Tile = "", GameFolder = "";
        public int ScreenResolution_x = 0, ScreenResolution_y = 0;
    }
    public class InfoReader
    {
        public GameInfo Game = new GameInfo();
        IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication();
        public InfoReader(string gamefolder)
        {
            string[] info = new string[20];
            int temp = 0, temp2 = 0;
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
            string temp_sr = "", temp_x = "", temp_y = "";
            for (int j = 0; j < temp; j++)
            {

                if (info[j].StartsWith("[Info]")) temp2 = 1;
                else if (info[j].StartsWith("[Script]")) temp2 = 2;
                else if (info[j].StartsWith("[Tile]")) temp2 = 3;
                else if (temp2 == 1)
                {

                    if (info[j].StartsWith("GameName=")) this.Game.GameName = info[j].Replace("GameName=", "");
                    else if (info[j].StartsWith("Company=")) this.Game.Company = info[j].Replace("Company=", "");
                    else if (info[j].StartsWith("GameMaker=")) this.Game.GameMaker = info[j].Replace("GameMaker=", "");
                    else if (info[j].StartsWith("GameSize=")) this.Game.GameSize = info[j].Replace("GameSize=", "");
                    else if (info[j].StartsWith("ScreenResolution="))
                    {
                        temp_sr = info[j].Replace("ScreenResolution=", "");
                        for (int k = 0, y_started = 0; k < temp_sr.Length; k++)
                        {
                            if (y_started == 0)
                            {
                                temp_x = temp_x + temp_sr[k];
                                if (temp_sr[k + 1] == '*')
                                {
                                    k++;
                                    y_started = 1;
                                }
                            }
                            else if (y_started == 1) temp_y = temp_y + temp_sr[k];

                        }
                    }
                }
                else if (temp2 == 2)
                {
                    if (info[j].StartsWith("Type=")) this.Game.Type = info[j].Replace("Type=", "");
                    else if (info[j].StartsWith("DataFolder=")) this.Game.DataFolder = info[j].Replace("DataFolder=", "");
                    else if (info[j].StartsWith("ScriptFile=")) this.Game.ScriptFile = info[j].Replace("ScriptFile=", "");
                }
                else if (temp2 == 3)
                {
                    if (info[j].StartsWith("Tile=")) this.Game.Tile = Path.Combine(gamefolder, "Icon", info[j].Replace("Tile=", ""));
                    else if (info[j].StartsWith("Icon=")) this.Game.Icon = Path.Combine(gamefolder, "Icon", info[j].Replace("Icon=", ""));
                }


            }
            this.Game.ScreenResolution_x = int.Parse(temp_x);
            this.Game.ScreenResolution_y = int.Parse(temp_y);
            this.Game.GameFolder = gamefolder;
        }
    }
}