using System;
using System.IO.IsolatedStorage;
namespace UVEngine
{
    public abstract class Saver
    {
        public static void Save(string key,object toSave)
        {
            IsolatedStorageSettings settings = IsolatedStorageSettings.ApplicationSettings;
            if (!settings.Contains(key))
            {
                settings.Add(key, toSave);
            }
            else
            {
                settings[key] = toSave;
            }
        }
        public static object Load(string key)
        {
            IsolatedStorageSettings settings = IsolatedStorageSettings.ApplicationSettings;
            if (!settings.Contains(key))
                throw new Exception("The destination savedata does not exist");
            else
                return settings[key];
        }
    }
}