﻿#pragma checksum "D:\Projects\UVEngine\UVEngine\MainPage.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "712C0F029528831C82C48E5969028169"
//------------------------------------------------------------------------------
// <auto-generated>
//     此代码由工具生成。
//     运行时版本:4.0.30319.42000
//
//     对此文件的更改可能会导致不正确的行为，并且如果
//     重新生成代码，这些更改将会丢失。
// </auto-generated>
//------------------------------------------------------------------------------

using Microsoft.Phone.Controls;
using System;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Automation.Peers;
using System.Windows.Automation.Provider;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Resources;
using System.Windows.Shapes;
using System.Windows.Threading;


namespace UVEngine {
    
    
    public partial class MainPage : Microsoft.Phone.Controls.PhoneApplicationPage {
        
        internal System.Windows.Controls.Grid LayoutRoot;
        
        internal Microsoft.Phone.Controls.Panorama panorama;
        
        internal Microsoft.Phone.Controls.PanoramaItem gameListItem;
        
        internal System.Windows.Controls.ListBox listBox;
        
        internal Microsoft.Phone.Controls.PanoramaItem Online;
        
        internal System.Windows.Controls.TextBlock backupText;
        
        internal System.Windows.Controls.TextBlock latestNews;
        
        internal System.Windows.Controls.TextBlock onlinePlayer;
        
        internal System.Windows.Controls.TextBlock friends;
        
        private bool _contentLoaded;
        
        /// <summary>
        /// InitializeComponent
        /// </summary>
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        public void InitializeComponent() {
            if (_contentLoaded) {
                return;
            }
            _contentLoaded = true;
            System.Windows.Application.LoadComponent(this, new System.Uri("/UVEngine;component/MainPage.xaml", System.UriKind.Relative));
            this.LayoutRoot = ((System.Windows.Controls.Grid)(this.FindName("LayoutRoot")));
            this.panorama = ((Microsoft.Phone.Controls.Panorama)(this.FindName("panorama")));
            this.gameListItem = ((Microsoft.Phone.Controls.PanoramaItem)(this.FindName("gameListItem")));
            this.listBox = ((System.Windows.Controls.ListBox)(this.FindName("listBox")));
            this.Online = ((Microsoft.Phone.Controls.PanoramaItem)(this.FindName("Online")));
            this.backupText = ((System.Windows.Controls.TextBlock)(this.FindName("backupText")));
            this.latestNews = ((System.Windows.Controls.TextBlock)(this.FindName("latestNews")));
            this.onlinePlayer = ((System.Windows.Controls.TextBlock)(this.FindName("onlinePlayer")));
            this.friends = ((System.Windows.Controls.TextBlock)(this.FindName("friends")));
        }
    }
}
