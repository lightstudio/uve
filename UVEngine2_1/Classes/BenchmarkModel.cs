using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using UVEngine2_1.Annotations;

namespace UVEngine2_1.Classes
{
    public class BenchmarkModel : INotifyPropertyChanged
    {
        private double _progress;

        public double Progress
        {
            get { return _progress; }
            set
            {
                if ((_progress - value < 0.01) && (_progress - value > -0.01)) return;
                _progress = value;
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
    public static class BenchmarkRuntime
    {
        public static BenchmarkModel CurrentBenchmark;
    }
}