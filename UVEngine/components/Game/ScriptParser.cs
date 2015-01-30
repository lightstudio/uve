using System;
using System.Collections.ObjectModel;
using System.IO;
using System.IO.IsolatedStorage;
using System.Windows;
using System.Text;


namespace UVEngine
{
    public class Text
    {
        static public bool IsText(string input)
        {
            string temp = "";
            if (input != null)
            {
                temp = temp + input[0];
                if (temp.Length == Encoding.UTF8.GetByteCount(temp))
                {
                    if (input.Length >= 2)
                    {
                        temp += input[1];
                        if (temp.Length == Encoding.UTF8.GetByteCount(temp))
                        {
                            return false;
                        }
                        else return true;
                    }
                    else return false;
                }
                else
                {
                    return true;
                }
            }
            return true;
        }

    }
    public class DEF
    {
        public string def;
        public bool IsString;
        public object VAR;
        public DEF(string def,bool IsString,object VAR)
        {
            this.def = def;
            this.IsString = IsString;
            this.VAR = VAR;
        }
    }
    public class DEFAS : ObservableCollection<DEF>
    {



    }
    public class Script
    {
        public string[] buf;
        public string[] scriptmain;
        public int CommandCount = 0;
        public Label[] lab;
        public VAR_NUM[] num = new VAR_NUM[1000];
        public VAR_STR[] str = new VAR_STR[1000];
        public Sub[] sub;
        public DEFAS defas = new DEFAS();
        public RMENU_Collection rmenu = new RMENU_Collection();
        public int tempcmdcount = 0;
        public void ParseScript()
        {
            for (int i = 0; i < 1000; i++)
            {
                num[i] = new VAR_NUM(0);
                str[i] = new VAR_STR("");
            }
            int tempsubcount = 0;
            Sub[] tempsub = new Sub[1000];
            for (int i = 0, area = 0; i < buf.Length; i++)
            {
                if (StringToolkit.GetBefore(buf[i], ' ') == "*define") area = 1;
                else if (StringToolkit.GetBefore(buf[i], ' ') == "game")
                {
                    area = 0;
                    this.sub = new Sub[tempsubcount];
                    for (int j = 0; j < tempsubcount; j++)
                    {
                        this.sub[j] = tempsub[j];
                    }
                }
                else if (StringToolkit.GetBefore(buf[i], ' ') == "*start") area = 2;

                if (area == 1)
                {
                    switch (StringToolkit.GetBefore(buf[i], ' '))
                    {
                        case "defsub":
                            tempsub[tempsubcount] = new Sub(buf[i].Remove(0, 7), this);
                            tempsubcount++;
                            break;
                        case "numalias":
                            string[] param;
                            StringToolkit.CutParam(buf[i].Remove(0, 9), ',', out param);
                            defas.Add(new DEF(param[0], false, double.Parse(param[1])));
                            break;
                        case "stralias":
                            StringToolkit.CutParam(buf[i].Remove(0, 9), ',', out param);
                            defas.Add(new DEF(param[0], true, param[1]));
                            break;
                        case "menusetwindow":
                            rmenu.RMENU_SetWindow(buf[i].Remove(0, 14));
                            break;
                        case "rmenu":
                            rmenu.Clear();
                            rmenu.RMENU_Add(buf[i].Remove(0, 6));
                            break;
                        case "savename":
                            rmenu.RMENU_SaveName(buf[i].Remove(0, 9));
                            break;
                        case "savenumber":
                            GamePage.savenumber = int.Parse(buf[i].Remove(0, 11));
                            break;
                    }
                }
                else if (area == 2)
                {
                    int mainscriptcount = 0;
                    string[] tempscr = new string[2000000];
                    for (; i < buf.Length; i++, mainscriptcount++)
                    {
                        tempscr[mainscriptcount] = buf[i];
                    }
                    scriptmain = new string[mainscriptcount];
                    for (int j = 0; j < scriptmain.Length; j++)
                    {
                        scriptmain[j] = tempscr[j];
                    }
                    Label.GetLabel(this, ref lab);
                }
            }
        }
        public Script(GameInfo info)
        {
            ScriptReader.ReadScript(Path.Combine(info.GameFolder,info.ScriptFile), out buf);
           
        }
    }
}