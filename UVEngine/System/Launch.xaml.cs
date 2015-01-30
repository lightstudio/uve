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
    public partial class Launch : PhoneApplicationPage
    {
        public Launch()
        {
            InitializeComponent();
        }
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            if (NavigationContext.QueryString.ContainsKey("LAUNCHURI"))
            {
                string navuri;
                if ((navuri = NavigationContext.QueryString["LAUNCHURI"]) != "/Protocol?encodedLaunchUri=uve:")
                {
                    this.Dispatcher.BeginInvoke(() =>
                    {
                        System.Threading.Thread.Sleep(500);
                        navuri = navuri.Remove(0, 31);
                        this.NavigationService.Navigate(new Uri(navuri, UriKind.Relative));
                        this.NavigationService.RemoveBackEntry();
                    });
                }
            }
            else if (NavigationContext.QueryString.ContainsKey("uri"))
            {
                string navuri = NavigationContext.QueryString["LAUNCHURI"];
                this.Dispatcher.BeginInvoke(() =>
                {
                    System.Threading.Thread.Sleep(500);
                    this.NavigationService.Navigate(new Uri(navuri, UriKind.Relative));
                    this.NavigationService.RemoveBackEntry();
                });
            }
        }
    }
}