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
    public partial class DonatePage : PhoneApplicationPage
    {
        string game = "";
        bool loaded = false;
        public DonatePage()
        {
            InitializeComponent();
            this.Loaded += DonatePage_Loaded;
        }

        void DonatePage_Loaded(object sender, RoutedEventArgs e)
        {
            if (UVEngineNative.UVEngineSettings.Donated) nomore.IsEnabled = true;
            loaded = true;
        }
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            if (loaded && UVEngineNative.UVEngineSettings.Donated) nomore.IsEnabled = true;
            game=NavigationContext.QueryString["game"];
            base.OnNavigatedTo(e);
        }
        private void continue_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new Uri("/ONSCL/Direct3DPage.xaml?game=" + game, UriKind.Relative));
        }
        private void nomore_Checked(object sender, RoutedEventArgs e)
        {
            UVEngineNative.UVEngineSettings.NoMoreDisplay = true;
        }
        private void nomore_Unchecked(object sender, RoutedEventArgs e)
        {
            UVEngineNative.UVEngineSettings.NoMoreDisplay = false;
        }
        private void donate_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new Uri("/System/DonateItems.xaml", UriKind.Relative));
        }
    }
}