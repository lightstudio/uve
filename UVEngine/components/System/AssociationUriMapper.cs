using System;
using System.Windows.Navigation;
using Windows.Phone.Storage.SharedAccess;

namespace UVEngine
{
    class AssociationUriMapper : UriMapperBase
    {
        private string tempUri;

        public override Uri MapUri(Uri uri)
        {
            tempUri = uri.ToString();

            // Protocol association launch for contoso.
            if (tempUri.Contains("uve"))
            {
                return new Uri("/System/Launch.xaml?LAUNCHURI=" + tempUri, UriKind.Relative);
            }
            else if (tempUri.Contains("/FileTypeAssociation"))
            {
                int fileIDIndex = tempUri.IndexOf("fileToken=") + 10;
                string fileID = tempUri.Substring(fileIDIndex);
                string incomingFileName = SharedStorageAccessManager.GetSharedFileName(fileID);
                string incomingFileType = System.IO.Path.GetExtension(incomingFileName);
                return new Uri("/System/OpenFile.xaml?fileToken=" + fileID + "&fileType=" + incomingFileType, UriKind.Relative);
                
            }

            // Include the original URI with the mapping to the main page.
            return uri;
        }
    }
}