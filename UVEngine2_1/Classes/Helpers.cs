using System;
using Windows.UI;

namespace UVEngine2_1.Classes
{
    public static class Helpers
    {
        public static int ColorConvert(Color color)
        {
            var argb = (color.A << 24) | (color.R << 16) | (color.G << 8) | (color.B);
            return argb;
        }

        public static Color ColorConvert(int argb)
        {
            var a = (byte) (argb >> 24);
            var r = (byte) (argb >> 16);
            var g = (byte) (argb >> 8);
            var b = (byte) (argb);
            return Color.FromArgb(a, r, g, b);
        }

        public static double GetSaturation(Color color)
        {
            var r = color.R/255.0;
            var g = color.G/255.0;
            var b = color.B/255.0;
            var max = Math.Max(r, Math.Max(g, b));
            var min = Math.Min(r, Math.Min(g, b));
            var sat = (max < 0.001) ? 0.0 : (1.0 - (min/max));
            return sat;
        }
    }
}