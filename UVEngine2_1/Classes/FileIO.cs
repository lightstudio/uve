using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Storage;
using ThreadPool = Windows.System.Threading.ThreadPool;

namespace UVEngine2_1.Classes
{
    public static class FileIO
    {
        private const int MaxBufferSize = 1048576;
        private static int _index;
        private static ulong _finishedSize, _totalSize;
        private static List<Folder> _folders;

        public static async Task<List<Folder>> AnalyzeFoldersAsync(StorageFolder toList)
        {
            _index = 0;
            _folders = new List<Folder>();
            await AnalyzeFoldersIterate(toList, -1);
            return _folders;
        }

        private static async Task AnalyzeFoldersIterate(StorageFolder toList, int parentFolderID)
        {
            ulong filesSize = 0;
            var folders = await toList.GetFoldersAsync();
            var files = await toList.GetFilesAsync();
            foreach (var file in files)
            {
                var properties = await file.GetBasicPropertiesAsync();
                filesSize += properties.Size;
            }
            var subFolder = new Folder
            {
                ID = _index,
                ParentFolderID = parentFolderID,
                Name = toList.Name,
                Files = files,
                FilesSize = filesSize
            };
            _index++;
            _folders.Add(subFolder);
            if (folders.Count == 0) return;
            foreach (var folder in folders)
            {
                await AnalyzeFoldersIterate(folder, subFolder.ID);
            }
        }

        public static ulong GetFolderSize(List<Folder> folders)
        {
            return folders.Aggregate<Folder, ulong>(0, (current, folder) => current + folder.FilesSize);
        }

        private static async Task CopyFolderTask(StorageFolder source, StorageFolder destination,
            CancellationToken cancellationToken, IProgress<double> progress)
        {
            var sourceList = await AnalyzeFoldersAsync(source);
            _totalSize = GetFolderSize(sourceList);
            await CopyFolderIterate(sourceList, 0, destination, cancellationToken, progress);
        }

        private static async Task CopyFolderIterate(List<Folder> list, int folderID, StorageFolder destination,
            CancellationToken cancellationToken, IProgress<double> progress)
        {
            var rootFolder = list.Find(folder => folder.ID == folderID);
            foreach (var file in rootFolder.Files)
            {
                var buffer = new byte[MaxBufferSize];
                var readStream = await file.OpenStreamForReadAsync();
                var writeFile = await destination.CreateFileAsync(file.Name);
                var writeStream = await writeFile.OpenStreamForWriteAsync();
                while (true)
                {
                    int buffersize;
                    buffersize = (readStream.Length - readStream.Position) < MaxBufferSize
                        ? (int) (readStream.Length - readStream.Position)
                        : MaxBufferSize;
                    var readSize = readStream.Read(buffer, 0, buffersize);
                    if (readSize > 0)
                    {
                        writeStream.Write(buffer, 0, buffersize);
                        await ThreadPool.RunAsync(delegate
                        {
                            _finishedSize += (ulong) buffersize;
                            progress.Report(_finishedSize/(double) _totalSize*100.0);
                        });
                    }
                    else
                    {
                        break;
                    }
                }
                readStream.Close();
                writeStream.Close();
            }
            var subFolders = list.FindAll(folder => folder.ParentFolderID == folderID);
            if (!subFolders.Any()) return;
            foreach (var folder in subFolders)
            {
                if (cancellationToken.IsCancellationRequested) break;
                var newFolder = await destination.CreateFolderAsync(folder.Name);
                await CopyFolderIterate(list, folder.ID, newFolder, cancellationToken, progress);
            }
        }

        private static async Task MoveFolderTask(StorageFolder source, StorageFolder destination,
            CancellationToken cancellationToken, IProgress<double> progress)
        {
            var sourceList = await AnalyzeFoldersAsync(source);
            _totalSize = GetFolderSize(sourceList);
            await MoveFolderIterate(sourceList, 0, destination, cancellationToken, progress);
        }

        private static async Task MoveFolderIterate(List<Folder> list, int folderID, StorageFolder destination,
            CancellationToken cancellationToken, IProgress<double> progress)
        {
            var rootFolder = list.Find(folder => folder.ID == folderID);
            foreach (var file in rootFolder.Files)
            {
                var properties = await file.GetBasicPropertiesAsync();
                var fileSize = properties.Size;
                await file.MoveAsync(destination);
                await ThreadPool.RunAsync(delegate
                {
                    _finishedSize += fileSize;
                    progress.Report(_finishedSize/(double) _totalSize*100.0);
                });
            }
            var subFolders = list.FindAll(folder => folder.ParentFolderID == folderID);
            if (!subFolders.Any()) return;
            foreach (var folder in subFolders)
            {
                if (cancellationToken.IsCancellationRequested) break;
                var newFolder = await destination.CreateFolderAsync(folder.Name);
                await MoveFolderIterate(list, folder.ID, newFolder, cancellationToken, progress);
            }
        }

        public static IAsyncActionWithProgress<double> CopyFolderAsync(StorageFolder source,
            StorageFolder destination)
        {
            _finishedSize = 0;
            return AsyncInfo.Run(
                (CancellationToken cancellationToken, IProgress<double> progress) =>
                    CopyFolderTask(source, destination, cancellationToken, progress));
        }

        public static IAsyncActionWithProgress<double> MoveFolderAsync(StorageFolder source, StorageFolder destination)
        {
            _finishedSize = 0;
            return AsyncInfo.Run(
                (CancellationToken cancellationToken, IProgress<double> progress) =>
                    MoveFolderTask(source, destination, cancellationToken, progress));
        }

        public struct Folder
        {
            public IReadOnlyList<StorageFile> Files;
            public ulong FilesSize;
            public int ID;
            public string Name;
            public int ParentFolderID;
        }
    }
}