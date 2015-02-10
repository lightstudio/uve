using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Security.Cryptography;
using System.Xml.Serialization;
using Windows.Storage;
using UVEngine2_1.Annotations;

namespace UVEngine2_1.Classes
{
    [XmlRoot("Game")]
    public class Game
    {
        [XmlElement("Name")]
        public string Name { get; set; }

        [XmlElement("Folder")]
        public string Folder { get; set; }

        [XmlElement("Size")]
        public ulong Size { get; set; }

        [XmlElement("Date")]
        public DateTime Date { get; set; }

        [XmlElement("Hash")]
        public string Hash { get; set; }

        [XmlElement("Icon")]
        public string Icon { get; set; }

        [XmlElement("Cover")]
        public string Cover { get; set; }

        [XmlElement("OPMovie")]
        public string OPMovie { get; set; }

        [XmlElement("ThemeSong")]
        public string ThemeSong { get; set; }
    }

    [XmlRoot("GameList")]
    public class GameList : INotifyPropertyChanged
    {
        private readonly string[] _audioExtensions = {".mp3", ".flac", ",ogg", ".wma"};
        private readonly string[] _coverNames = {"cover"};
        private readonly string[] _iconNames = {"icon"};
        private readonly string[] _imageExtensions = {".jpg", ".png"};
        private readonly string[] _opMovieNames = {"preview", "pv", "op"};
        private readonly string[] _scriptNames = {"0.txt", "00.txt", "nscr_sec.dat", "nscript.___", "nscript.dat"};
        private readonly string[] _themeSongNames = {"theme", "track"};
        private readonly string[] _videoExtensions = {".mp4", ".avi", ".mpg"};
        private ObservableCollection<Game> _games;

        [XmlArray("Games"), XmlArrayItem("Game")]
        public ObservableCollection<Game> Games
        {
            get { return _games; }
            set
            {
                if (_games == value) return;
                _games = value;
                OnPropertyChanged("Games");
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        public static GameList Load()
        {
            var listPath = ApplicationData.Current.LocalFolder.Path + "//System//GameList.xml";
            if (!File.Exists(listPath))
                return new GameList
                {
                    Games = new ObservableCollection<Game>()
                };
            var reader = new StreamReader(listPath);
            var serializer = new XmlSerializer(typeof (GameList));
            var list = (GameList) serializer.Deserialize(reader);
            reader.Close();
            return list;
        }

        public void Save()
        {
            var listPath = ApplicationData.Current.LocalFolder.Path + "//System//GameList.xml";
            var writer = new StreamWriter(listPath, false);
            var serializer = new XmlSerializer(typeof (GameList));
            serializer.Serialize(writer, this);
            writer.Close();
        }

        private async void AddGame(StorageFolder storage)
        {
            var files = await storage.GetFilesAsync();
            var properties = await storage.GetBasicPropertiesAsync();
            var newGame = new Game
            {
                Name = storage.Name,
                Folder = storage.Path,
                Size = properties.Size/1024/1024,
                Date = DateTime.Now,
                Hash = HashScript(files),
                Icon = FilterFile(files, _iconNames, _imageExtensions),
                Cover = FilterFile(files, _coverNames, _imageExtensions),
                OPMovie = FilterFile(files, _opMovieNames, _videoExtensions),
                ThemeSong = FilterFile(files, _themeSongNames, _audioExtensions)
            };
            Games.Add(newGame);
        }

        private string FilterFile(IReadOnlyList<StorageFile> files, string[] names, string[] extensions)
        {
            var filePathEnumerable = from file in files
                where (names.Contains(Path.GetFileNameWithoutExtension(file.Path))) &&
                      (extensions.Contains(file.FileType))
                select file.Path;
            var filePath = filePathEnumerable.FirstOrDefault();
            return filePath;
        }

        private string HashScript(IReadOnlyList<StorageFile> files)
        {
            var sha1Provider = new SHA1Managed();
            var scriptEnumerable = from file in files
                where (_scriptNames.Contains(file.Name))
                select file.Path;
            var scriptPath = scriptEnumerable.FirstOrDefault();
            var scriptReader = new StreamReader(scriptPath);
            var hashedBytes = sha1Provider.ComputeHash(scriptReader.BaseStream);
            return Convert.ToBase64String(hashedBytes);
        }

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            var handler = PropertyChanged;
            if (handler != null) handler(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}