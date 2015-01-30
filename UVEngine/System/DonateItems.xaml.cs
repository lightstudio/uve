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

namespace UVEngine
{
    public partial class DonateItems : PhoneApplicationPage
    {
        public DonateItems()
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
        private void Check()
        {
            var productLicenses = Windows.ApplicationModel.Store.CurrentApp.LicenseInformation.ProductLicenses;
            Completed(productLicenses["Donate"]);
        }
        void Completed(Windows.ApplicationModel.Store.ProductLicense license)
        {
            if (license.IsConsumable && license.IsActive)
            {
                Windows.ApplicationModel.Store.CurrentApp.ReportProductFulfillment(license.ProductId);
                UVEngineNative.UVEngineSettings.Donated = true;
                MessageBox.Show(UVEngine.Resources.UVEngine.thanksdonation);
            }
        }

        private async void Button_Click_1(object sender, RoutedEventArgs e)
        {
            try
            {
                await Windows.ApplicationModel.Store.CurrentApp.RequestProductPurchaseAsync("Donate", false);
                Check();
            }
            catch
            {

            }
        }
    }
}