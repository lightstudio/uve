using System;
using System.ComponentModel;
using System.IO;
using System.Runtime.CompilerServices;
using System.Security.Cryptography;
using System.Text;
using System.Windows;
using System.Xml.Serialization;
using Windows.Storage;
using Windows.UI;
using UVEngine2_1.Annotations;

namespace UVEngine2_1.Classes
{
    [XmlRoot("Settings")]
    public class Settings : INotifyPropertyChanged
    {
        public enum ColorStates : byte
        {
            System = 0,
            Game = 1,
            Night = 2,
            Custom = 3
        }

        private bool _autoNightMode;
        private ColorStates _colorState;
        private bool _coverView;
        private Color _customColor;
        private int _customColorInt;
        private bool _debugOn;
        private bool _doubleExit;
        private bool _landscapeView;
        private bool _leftHanded;
        private string _password;
        private bool _passwordOn;
        private bool _quickSave;

        [XmlElement("ColorState")]
        public ColorStates ColorState
        {
            get { return _colorState; }
            set
            {
                if (_colorState == value) return;
                _colorState = value;
                OnPropertyChanged("ColorState");
            }
        }

        [XmlIgnore]
        public Color CustomColor
        {
            get { return _customColor; }
            set
            {
                if (_customColor == value) return;
                _customColor = value;
                CustomColorInt = Helpers.ColorConvert(value);
                OnPropertyChanged("CustomColor");
            }
        }

        [XmlElement("CustomColorInt")]
        public int CustomColorInt
        {
            get { return _customColorInt; }
            set
            {
                if (_customColorInt == value) return;
                _customColorInt = value;
                OnPropertyChanged("CustomColorInt");
            }
        }

        [XmlElement("AutoNightMode")]
        public bool AutoNightMode
        {
            get { return _autoNightMode; }
            set
            {
                if (_autoNightMode == value) return;
                _autoNightMode = value;
                OnPropertyChanged("AutoNightMode");
            }
        }

        [XmlElement("CoverView")]
        public bool CoverView
        {
            get { return _coverView; }
            set
            {
                if (_coverView == value) return;
                _coverView = value;
                OnPropertyChanged("CoverView");
            }
        }

        [XmlElement("LandscapeView")]
        public bool LandscapeView
        {
            get { return _landscapeView; }
            set
            {
                if (_landscapeView == value) return;
                _landscapeView = value;
                OnPropertyChanged("LandscapeView");
            }
        }

        [XmlElement("PasswordOn")]
        public bool PasswordOn
        {
            get { return _passwordOn; }
            set
            {
                if (_passwordOn == value) return;
                _passwordOn = value;
                OnPropertyChanged("PasswordOn");
            }
        }

        [XmlElement("Password")]
        public string Password
        {
            get { return _password; }
            set
            {
                if (_password == value) return;
                _password = value;
                OnPropertyChanged("Password");
            }
        }

        [XmlElement("DoubleExit")]
        public bool DoubleExit
        {
            get { return _doubleExit; }
            set
            {
                if (_doubleExit == value) return;
                _doubleExit = value;
                OnPropertyChanged("DoubleExit");
            }
        }

        [XmlElement("LeftHanded")]
        public bool LeftHanded
        {
            get { return _leftHanded; }
            set
            {
                if (_leftHanded == value) return;
                _leftHanded = value;
                OnPropertyChanged("LeftHanded");
            }
        }

        [XmlElement("QuickSave")]
        public bool QuickSave
        {
            get { return _quickSave; }
            set
            {
                if (_quickSave == value) return;
                _quickSave = value;
                OnPropertyChanged("QuickSave");
            }
        }

        [XmlElement("DebugOn")]
        public bool DebugOn
        {
            get { return _debugOn; }
            set
            {
                if (_debugOn == value) return;
                _debugOn = value;
                OnPropertyChanged("DebugOn");
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            var handler = PropertyChanged;
            if (handler != null) handler(this, new PropertyChangedEventArgs(propertyName));
        }

        public static Settings Load()
        {
            var settingsPath = ApplicationData.Current.LocalFolder.Path + "//System//Settings.xml";
            if (!File.Exists(settingsPath))
            {
                var accentColor = (Color) Application.Current.Resources["SystemColorControlAccentColor"];
                return new Settings
                {
                    ColorState = ColorStates.System,
                    CustomColor = accentColor,
                    AutoNightMode = true,
                    CoverView = true,
                    LandscapeView = false,
                    PasswordOn = false,
                    Password = "",
                    DoubleExit = true,
                    LeftHanded = false,
                    QuickSave = true,
                    DebugOn = false
                };
            }
            var reader = new StreamReader(settingsPath);
            var serializer = new XmlSerializer(typeof (Settings));
            var settings = (Settings) serializer.Deserialize(reader);
            reader.Close();
            settings.CustomColor = Helpers.ColorConvert(settings.CustomColorInt);
            return settings;
        }

        public void Save()
        {
            var settingsPath = ApplicationData.Current.LocalFolder.Path + "//System//Settings.xml";
            var writer = new StreamWriter(settingsPath, false);
            var serializer = new XmlSerializer(typeof (Settings));
            serializer.Serialize(writer, this);
            writer.Close();
        }

        public string HashPassword(string rawPassword)
        {
            var sha1Provider = new SHA1Managed();
            var rawBytes = Encoding.Unicode.GetBytes(rawPassword);
            var hashedBytes = sha1Provider.ComputeHash(rawBytes);
            return Convert.ToBase64String(hashedBytes);
        }

        public bool CheckPassword(string rawPassword)
        {
            var sha1Provider = new SHA1Managed();
            var rawBytes = Encoding.Unicode.GetBytes(rawPassword);
            var hashedBytes = sha1Provider.ComputeHash(rawBytes);
            var hashedPassword = Convert.ToBase64String(hashedBytes);
            return hashedPassword == Password;
        }
    }
}