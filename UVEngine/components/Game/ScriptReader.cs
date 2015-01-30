using System;
using System.IO;
using System.IO.IsolatedStorage;
using System.Windows;

namespace UVEngine
{
    public class ScriptReader
    {
        static public void ReadScript(string scriptPath,out string[] buf)
        {
            string[] temps = new string[2000000];
            int length = 0;
            IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication();
            string temp="";
            try
            {
                IsolatedStorageFileStream location = new IsolatedStorageFileStream(scriptPath, FileMode.Open, storage);
                StreamReader file = new StreamReader(location);
                for (int j = 0,inquotation=0 , ended = 0; ; )
                {
                    string with_comments = "";
                    with_comments = file.ReadLine();
                    temp = "";
                    for (int i = 0; with_comments != null && i < with_comments.Length; i++)
                    {
                        if (inquotation == 0 && with_comments[i] == '\"') inquotation = 1;
                        else if (inquotation == 1 && with_comments[i] == '\"') inquotation = 0;
                        if (inquotation == 0 && with_comments[i] == ';') break;
                        temp = temp + with_comments[i];

                    }
                    if (temp != null && temp.Replace(" ", "") != "")
                    {
                        temps[j] = temp;
                        j++;
                    }
                    if (ended == 1) break;
                    if (file.EndOfStream) ended = 1;
                    length = j;
                }
                buf = new string[length];
                for (int i = 0; i < buf.Length; i++)
                {
                    buf[i] = temps[i];
                }
                location.Close();
                file.Close();
            }
            catch (Exception e)
            {
                MessageBox.Show("脚本文件读取出错：" + e.Message, "错误", MessageBoxButton.OK);
                buf = null;
            }
        }
    }
}