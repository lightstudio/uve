using System;
using System.Threading.Tasks;
using Windows.Graphics.Imaging;
using Windows.Storage.Streams;
using Windows.UI;
using UVEngineNative;

namespace UVEngine2_1.Classes
{
    public static class Helpers
    {
        public static int ColorConvert(Color color)
        {
            return (color.B << 24) | (color.G << 16) | (color.R << 8) | (color.A);
        }

        public static Color ColorConvert(int bgra)
        {
            return Color.FromArgb(
                (byte)bgra, (byte)(bgra >> 8), (byte)(bgra >> 16), (byte)(bgra >> 24));
        }

        public static double GetSaturation(Color color)
        {
            var max = Math.Max(color.R, Math.Max(color.G, color.B)) / 255.0;
            var min = Math.Min(color.R, Math.Min(color.G, color.B)) / 255.0;

            return (max < 0.001) ? 0.0 : (1.0 - (min / max));
        }

        public static async Task<Color[]> ImageMetaRT(IRandomAccessStream imageStream, int metaCount, int maxError)
        {
            var colors = new Color[metaCount];
            var imgDecoder = await BitmapDecoder.CreateAsync(imageStream);
            var imgProvider = await imgDecoder.GetPixelDataAsync(
                BitmapPixelFormat.Bgra8, BitmapAlphaMode.Straight,
                new BitmapTransform(), ExifOrientationMode.IgnoreExifOrientation,
                ColorManagementMode.DoNotColorManage);
            var imageBytes = imgProvider.DetachPixelData();
            var colorsInt = ImageMeta.meta_calculate(imageBytes, metaCount, maxError);
            for (byte index = 0; index < metaCount; index++)
            {
                colors[index] = ColorConvert(colorsInt[index]);
            }
            return colors;
        }
    }
}