using System;
using System.Collections.Generic;
using System.IO;
using System.IO.IsolatedStorage;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

namespace UVEngine
{
    public class Sub
    {
        public string name;
        public VAR_NUM[] num = new VAR_NUM[1000];
        public VAR_STR[] str = new VAR_STR[1000];
        public string[] subbuf;
        public Sub(string subname,Script scr)
        {
            for (int i = 0; i < scr.buf.Length; i++)
            {
                if (scr.buf[i] == "*" + subname)
                {
                    this.name = subname;
                    i++;
                    string[] temp = new string[1000];
                    int k = 0;
                    for (int j = i; j < scr.buf.Length && scr.buf[j] != "return"; j++, k++)
                    {
                        temp[k] = scr.buf[j];
                    }
                    subbuf = new string[k];
                    for (int j = 0; j < subbuf.Length; j++)
                    {
                        subbuf[j] = temp[j];
                    }
                }
            }
        }
    }
}
