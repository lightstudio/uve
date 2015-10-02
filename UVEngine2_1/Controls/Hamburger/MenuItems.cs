using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;
using UVEngine2_1.Annotations;

namespace UVEngine2_1.Controls.Hamburger
{
    public class MenuItem : INotifyPropertyChanged
    {
        private BitmapImage _icon;
        private string _text;
        private Uri _page;
        public BitmapImage Icon
        {
            get { return _icon; }
            set
            {
                if (_icon == value) return;
                _icon = value;
                OnPropertyChanged();
            }
        }

        public string Text
        {
            get { return _text; }
            set
            {
                if (_text == value) return;
                _text = value;
                OnPropertyChanged();
            }
        }

        public Uri Page
        {
            get { return _page; }
            set
            {
                if (_page == value) return;
                _page = value;
                OnPropertyChanged();
            }
        }
        public event PropertyChangedEventHandler PropertyChanged;

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
    public class MenuItemList
    {
        public ObservableCollection<MenuItem> MenuItems { get; set; } 
    }
}
