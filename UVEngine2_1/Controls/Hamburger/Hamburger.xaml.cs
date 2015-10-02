using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Net;
using System.Runtime.CompilerServices;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Navigation;
using Windows.UI.Input;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using UVEngine2_1.Annotations;
using UVEngine2_1.Classes;
using ManipulationCompletedEventArgs = System.Windows.Input.ManipulationCompletedEventArgs;
using ManipulationStartedEventArgs = System.Windows.Input.ManipulationStartedEventArgs;

namespace UVEngine2_1.Controls.Hamburger
{

    public enum HamburgerStatus
    {
        Collapsed,
        Half,
        Full
    }
    public partial class Hamburger : UserControl
    {
        public readonly DependencyProperty StatusProperty = DependencyProperty.Register("Status", typeof(HamburgerStatus),
            typeof(Hamburger),
            new PropertyMetadata(GamesPropertyChangedCallback));

        public static bool Dragging;
        public static double DragStart;

        private static void GamesPropertyChangedCallback(DependencyObject sender, DependencyPropertyChangedEventArgs arg)
        {
            if (!(sender is Hamburger)) return;
            HamburgerRuntime.RuntimeProperties.Status = (HamburgerStatus)arg.NewValue;
        }

        public HamburgerStatus Status
        {
            get { return (HamburgerStatus)GetValue(StatusProperty); }
            set { SetValue(StatusProperty, value); }
        }

        public Hamburger()
        {
            InitializeComponent();
            HamburgerRuntime.RuntimeProperties = new HambugerProperties();
            DataContext = HamburgerRuntime.RuntimeProperties;
            Dragging = false;
        }

        private void OnManipulationStarted(object sender, ManipulationStartedEventArgs e)
        {
            if (e.ManipulationOrigin.X > 10) return;
            Dragging = true;
            DragStart = e.ManipulationOrigin.X;
            IconBackground.RenderTransform = null;
            ExpandButton.RenderTransform = null;
            PageSelector.RenderTransform = null;
        }

        private void OnManipulationDelta(object sender, ManipulationDeltaEventArgs e)
        {
            if (!Dragging) return;
            var nowPosition = DragStart + e.CumulativeManipulation.Translation.X;
            if (nowPosition <= 66) return;
            HamburgerGrid.Width = nowPosition;
            PageSelector.Width = nowPosition;
        }

        private void OnManipulationCompleted(object sender, ManipulationCompletedEventArgs e)
        {

        }
    }

    public class HambugerProperties:INotifyPropertyChanged
    {
        private HamburgerStatus _status;

        public HamburgerStatus Status
        {
            get { return _status; }
            set
            {
                if (_status == value) return;
                _status = value;
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

    public static class HamburgerRuntime
    {
        public static HambugerProperties RuntimeProperties;
    }
}
