#region 引用
using Microsoft.Phone.BackgroundAudio;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Tasks;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Media;
using OggSharp;
using System;
using System.Collections.ObjectModel;
using System.IO;
using System.IO.IsolatedStorage;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Input;
using UVEngineNative;
#endregion


namespace UVEngine
{
    public enum RMENU_State
    {
        STATE_null = 0,
        STATE_menu = 1,
        STATE_save = 2,
        STATE_load = 3,
    }
    public class RMENU
    {
        public string Text;
        public string Command;
        public RMENU(string Text, string Command)
        {
            this.Text = Text;
            this.Command = Command;
        }
    }
    public class RMENU_Collection : Collection<RMENU>
    {
        public int text_height = 30, text_width = 30, text_spacing = 2, line_spacing = 2;
        public bool IsBorder = false, IsBlack = false;
        public string savename = "保存", readname = "读档", dataname = "档案";
        public RMENU_Collection()
        {
            
        }
        public void RMENU_SetWindow(string menusetwindow_params)
        {
            string[] param;
            StringToolkit.CutParam(menusetwindow_params, ',', out param);
            this.text_height = int.Parse(param[0]);
            this.text_width = int.Parse(param[1]);
            this.text_spacing = int.Parse(param[2]);
            this.line_spacing = int.Parse(param[3]);
            if (param[4] == "0") IsBorder = false;
            else IsBorder = true;
            if (param[5] == "0") IsBlack = false;
            else IsBlack = false;
        }
        public void RMENU_SaveName(string savename_params)
        {
            string[] param;
            StringToolkit.CutParam(savename_params, ',', out param);
/*            if (GamePage.useNative)
            {
                StringToolkitNative.GetInside(param[0], '\"', out this.savename);
                StringToolkitNative.GetInside(param[1], '\"', out this.readname);
                StringToolkitNative.GetInside(param[2], '\"', out this.dataname);
            }
            else
            {*/
//                StringToolkit.CutParam(savename_params, ',', out param);
                this.savename = StringToolkit.GetInside(param[0], '\"');
                this.readname = StringToolkit.GetInside(param[1], '\"');
                this.dataname = StringToolkit.GetInside(param[2], '\"');
//            }
        }
        public void RMENU_Add(string rmenu_params)
        {
            string[] param;
            StringToolkit.CutParam(rmenu_params, ',', out param);
            for (int i = 0; i < param.Length / 2; i++)
            {
                Add(new RMENU(StringToolkit.GetInside(param[2 * i], '\"'), param[2 * i + 1]));
            }
        }
    }
}