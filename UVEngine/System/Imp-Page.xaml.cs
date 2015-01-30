using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;

namespace UVEngine
{
    public partial class Imp_Page : PhoneApplicationPage
    {
        public Imp_Page()
        {
            InitializeComponent();
        }
        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            NavigationService.Navigate(new Uri("/Import.xaml", UriKind.Relative));
        }
        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            this.NavigationService.RemoveBackEntry();
            NavigationService.Navigate(new Uri("/MainPage.xaml", UriKind.Relative));
        }
        protected override void OnBackKeyPress(System.ComponentModel.CancelEventArgs e)
        {
            base.OnBackKeyPress(e);
            this.NavigationService.RemoveBackEntry();
            NavigationService.Navigate(new Uri("/MainPage.xaml", UriKind.Relative));

        }
    }
}