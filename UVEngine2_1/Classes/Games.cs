using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Data.Linq;
using System.Data.Linq.Mapping;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Security.Cryptography;
using Windows.Devices.HumanInterfaceDevice;
using Windows.Storage;
using UVEngine2_1.Annotations;

namespace UVEngine2_1.Classes
{
    [Table]
    public class Game : INotifyPropertyChanged, INotifyPropertyChanging
    {
        private string _cover;
        private DateTime _date;
        private string _folder;
        private string _hash;
        private string _icon;
        private int _id;
        private string _name;
        private string _opMovie;
        private ulong _size;
        private string _themeSong;
        private bool _wideScreen;
        private bool _isSelected;

        [Column(IsPrimaryKey = true, IsDbGenerated = true, DbType = "INT NOT NULL Identity", CanBeNull = false,
            AutoSync = AutoSync.OnInsert)]
        public int ID
        {
            get { return _id; }
            set
            {
                if (_id == value) return;
                OnPropertyChanging();
                _id = value;
                OnPropertyChanged();
            }
        }

        [Column(CanBeNull = false)]
        public string Name
        {
            get { return _name; }
            set
            {
                if (_name == value) return;
                OnPropertyChanging();
                _name = value;
                OnPropertyChanged();
            }
        }

        [Column(CanBeNull = false)]
        public string Folder
        {
            get { return _folder; }
            set
            {
                if (_folder == value) return;
                OnPropertyChanging();
                _folder = value;
                OnPropertyChanged();
            }
        }

        [Column(CanBeNull = false)]
        public ulong Size
        {
            get { return _size; }
            set
            {
                if (_size == value) return;
                OnPropertyChanging();
                _size = value;
                OnPropertyChanged();
            }
        }

        [Column(CanBeNull = false)]
        public DateTime Date
        {
            get { return _date; }
            set
            {
                if (_date == value) return;
                OnPropertyChanging();
                _date = value;
                OnPropertyChanged();
            }
        }

        [Column(CanBeNull = false)]
        public string Hash
        {
            get { return _hash; }
            set
            {
                if (_hash == value) return;
                OnPropertyChanging();
                _hash = value;
                OnPropertyChanged();
            }
        }

        [Column(CanBeNull = true)]
        public string Icon
        {
            get { return _icon; }
            set
            {
                if (_icon == value) return;
                OnPropertyChanging();
                _icon = value;
                OnPropertyChanged();
            }
        }

        [Column(CanBeNull = true)]
        public string Cover
        {
            get { return _cover; }
            set
            {
                if (_cover == value) return;
                OnPropertyChanging();
                _cover = value;
                OnPropertyChanged();
            }
        }

        [Column(CanBeNull = true)]
        public string OPMovie
        {
            get { return _opMovie; }
            set
            {
                if (_opMovie == value) return;
                OnPropertyChanging();
                _opMovie = value;
                OnPropertyChanged();
            }
        }

        [Column(CanBeNull = true)]
        public string ThemeSong
        {
            get { return _themeSong; }
            set
            {
                if (_themeSong == value) return;
                OnPropertyChanging();
                _themeSong = value;
                OnPropertyChanged();
            }
        }

        [Column(CanBeNull = false)]
        public bool WideScreen
        {
            get { return _wideScreen; }
            set
            {
                if (_wideScreen == value) return;
                OnPropertyChanging();
                _wideScreen = value;
                OnPropertyChanged();
            }
        }

        public bool IsSelected
        {
            get { return _isSelected; }
            set
            {
                if (_isSelected == value) return;
                OnPropertyChanging();
                _isSelected = value;
                OnPropertyChanged();
            }
        }
        public event PropertyChangedEventHandler PropertyChanged;
        public event PropertyChangingEventHandler PropertyChanging;

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        protected virtual void OnPropertyChanging([CallerMemberName] string propertyName = null)
        {
            PropertyChanging?.Invoke(this, new PropertyChangingEventArgs(propertyName));
        }
    }

    public class GameList
    {
        public ObservableCollection<Game> Games { get; set; }
    }

    public class GameContext : DataContext
    {
        public static string ConnectionString = "Data Source=isostore:/Games.sdf";
        private readonly string[] _audioExtensions = { ".mp3", ".flac", ",ogg", ".wma" };
        private readonly string[] _coverNames = { "cover" };
        private readonly string[] _iconNames = { "icon" };
        private readonly string[] _imageExtensions = { ".jpg", ".png" };
        private readonly string[] _opMovieNames = { "preview", "pv", "op" };
        private readonly string[] _scriptNames = { "0.txt", "00.txt", "nscr_sec.dat", "nscript.___", "nscript.dat" };
        private readonly string[] _themeSongNames = { "theme", "track" };
        private readonly string[] _videoExtensions = { ".mp4", ".avi", ".mpg" };
        public GameList GamesList;
        public Table<Game> GamesTable;

        public GameContext(string fileOrConnection)
            : base(fileOrConnection)
        {
            if (DatabaseExists() == false)
            {
                CreateDatabase();
            }
            GamesList = new GameList { Games = new ObservableCollection<Game>(GamesTable) };
        }

        public void Save()
        {
            SubmitChanges();
        }

        public Game GetGameByHash(string hash)
        {
            var gameEnumerable = from game in GamesList.Games
                                 where game.Hash == hash
                                 select game;
            return gameEnumerable.FirstOrDefault();
        }

        private async void ImportGame(StorageFolder storage)
        {
            StorageFolder gameFolder;
            if (storage.Path.StartsWith("D:\\"))
            {
                gameFolder = storage;
            }
            else
            {
                var appFolder = await ApplicationData.Current.LocalCacheFolder.CreateFolderAsync(storage.Name);
                await FileIO.CopyFolderAsync(storage, appFolder);
                gameFolder = appFolder;
            }
            var folders = await FileIO.AnalyzeFoldersAsync(gameFolder);
            var rootFolder = folders.Find(folder => folder.ParentFolderID == -1);
            var newGame = new Game
            {
                Name = gameFolder.Name,
                Folder = gameFolder.Path,
                Size = rootFolder.FilesSize,
                Date = DateTime.Now,
                Hash = HashScript(rootFolder.Files),
                Icon = FilterFile(rootFolder.Files, _iconNames, _imageExtensions),
                Cover = FilterFile(rootFolder.Files, _coverNames, _imageExtensions),
                OPMovie = FilterFile(rootFolder.Files, _opMovieNames, _videoExtensions),
                ThemeSong = FilterFile(rootFolder.Files, _themeSongNames, _audioExtensions),
                IsSelected = false
            };
            GamesList.Games.Add(newGame);
            GamesTable.InsertOnSubmit(newGame);
        }

        private static string FilterFile(IReadOnlyList<StorageFile> files, string[] names, string[] extensions)
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
    }

    public static class GameRuntime
    {
        public static GameContext CurrentGames;

        public static GameList DebugGames()
        {
            var debugGameList = new GameList();
            var debugGame1 = new Game() { Name = "Clannad", Date = DateTime.Now };
            var debugGame2 = new Game() { Name = "Little Busters", Date = DateTime.Now };
            debugGameList.Games = new ObservableCollection<Game> { debugGame1, debugGame2 };
            return debugGameList;
        }
    }
}