using System;
using System.Globalization;
using System.Windows;
using System.Windows.Data;

namespace UVEngine2_1.Controls.GameSelector
{
    public class IsSelectedConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var isSelected = (bool) value;
            return isSelected ? Visibility.Visible : Visibility.Collapsed;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var isVisible = (Visibility) value;
            return isVisible == Visibility.Visible;
        }
    }
}