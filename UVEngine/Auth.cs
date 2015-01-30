using System;
using System.IO;
using System.Windows;
using System.Runtime.InteropServices;

namespace MarketPlaceAuthInternal
{
    static public class Auth
    {
        /// <summary>
        /// 判断应用是否为从市场安装
        /// </summary>
        /// <returns>从市场安装:true ;开发者部署:false</returns>
        static public bool GetInstallState()
        {
            try
            {
                //尝试使用正常方式打开文件流
                System.Windows.Resources.StreamResourceInfo res = Application.GetResourceStream(new Uri("Auth.txt", UriKind.Relative));
                res.Stream.Close();
            }
            catch
            {
                //Auth.txt被删除，判断XAP已被修改
                return false;
            }
            FileStream fs;
            try
            {
                //尝试使用FileStream打开文件
                fs = new FileStream("Auth.txt", FileMode.Open);
                return false;
            }
            catch
            {
                //FileStream无权限
                return true;
            }
        }
    }
}
