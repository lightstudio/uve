using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Storage.Streams;
using Microsoft.Phone.Storage;

namespace UVEngine
{
    public class ExternalStorageHelper : UVEngineNative.IExternalStorageNative
    {
        ExternalStorageFile esf;
        public ExternalStorageHelper(ExternalStorageFile esf)
        {
            this.esf = esf;
        }
        public Windows.Storage.Streams.IInputStream GetNativeStream()
        {
            var action = esf.OpenForReadAsync();
            action.Start();
            action.Wait();
            return action.Result.AsInputStream();
        }
        
    }
}
