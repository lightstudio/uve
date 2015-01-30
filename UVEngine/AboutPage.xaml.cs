using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using Microsoft.Phone.Tasks;
using Microsoft.Phone.Marketplace;

namespace UVEngine
{
    public partial class AboutPage : PhoneApplicationPage
    {
        public AboutPage()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            MarketplaceDetailTask mdt = new MarketplaceDetailTask();
            mdt.ContentType = MarketplaceContentType.Applications;
            mdt.ContentIdentifier = "14205072-b8d2-476c-a1c7-37bde5c0e985";
            mdt.Show();
        }
    }
}