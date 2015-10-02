using System.Windows;
using System.Windows.Navigation;
using Windows.ApplicationModel.Activation;
using Windows.Storage.AccessCache;
using Microsoft.Phone.Controls;

namespace UVEngine2_1.Pages.Import
{
    public partial class FolderImport : PhoneApplicationPage
    {
        public FolderImport()
        {
            InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            var app = Application.Current as App;
            if (app.FolderPickerContinuationArgs != null)
            {
                ContinueFolderPicker(app.FolderPickerContinuationArgs);
            }
        }

        public void ContinueFolderPicker(FolderPickerContinuationEventArgs args)
        {
            StorageApplicationPermissions.FutureAccessList.Add(args.Folder);
        }
    }
}