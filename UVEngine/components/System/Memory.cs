using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Windows.Controls.Primitives;
using System.Windows.Threading;
using Microsoft.Phone.Info;
using System.Diagnostics;
using System.Collections.Generic;

namespace UVEngine
{
    public class MemoryDiagnostics
    {

    }
    /// <summary>
    /// Helper class for showing current memory usage
    /// </summary>
    public static class MemoryDiagnosticsHelper
    {
        static Popup popup;
        static TextBlock currentMemoryKB;
        static TextBlock currentMemoryMB;
        static TextBlock peakMemoryBlock;
        static DispatcherTimer timer;
        static bool forceGc;
        static long MAX_MEMORY = Microsoft.Phone.Info.DeviceStatus.ApplicationMemoryUsageLimit; // 90MB, per marketplace
        static int lastSafetyBand = -1; // to avoid needless changes of colour

        const long MAX_CHECKPOINTS = 10; // adjust as needed
        static Queue<MemoryCheckpoint> recentCheckpoints;

        static bool alreadyFailedPeak = false; // to avoid endless Asserts

        /// <summary>
        /// Starts the memory diagnostic timer and shows the counter
        /// </summary>
        /// <param name="timespan">The timespan between counter updates</param>
        /// <param name="forceGc">Whether or not to force a GC before collecting memory stats</param>
        public static void Start(TimeSpan timespan, bool forceGc)
        {
            if (timer == null)
            {
                MemoryDiagnosticsHelper.forceGc = forceGc;
                recentCheckpoints = new Queue<MemoryCheckpoint>();

                StartTimer(timespan);
                ShowPopup();
            }
        }

        /// <summary>
        /// Stops the timer and hides the counter
        /// </summary>
        public static void Stop()
        {
            if (popup != null)
            {
                HidePopup();
                StopTimer();
                recentCheckpoints = null;
            }
        }

        /// <summary>
        /// Add a checkpoint to the system to help diagnose failures. Ignored in retail mode
        /// </summary>
        /// <param name="text">Text to describe the most recent thing that happened</param>
        public static void Checkpoint(string text)
        {
            if (recentCheckpoints == null) return;
            if (recentCheckpoints.Count >= MAX_CHECKPOINTS - 1) recentCheckpoints.Dequeue();
            recentCheckpoints.Enqueue(new MemoryCheckpoint(text, GetCurrentMemoryUsage()));
        }

        /// <summary>
        /// Recent checkpoints stored by the app; will always be empty in retail mode
        /// </summary>
        public static IEnumerable<MemoryCheckpoint> RecentCheckpoints
        {
            get
            {
                if (recentCheckpoints == null) yield break;

                foreach (MemoryCheckpoint checkpoint in recentCheckpoints) yield return checkpoint;
            }
        }

        /// <summary>
        /// Gets the current memory usage, in bytes. Returns zero in non-debug mode
        /// </summary>
        /// <returns>Current usage</returns>
        public static long GetCurrentMemoryUsage()
        {
            // don't use DeviceExtendedProperties for release builds (requires a capability)
            return (long)DeviceExtendedProperties.GetValue("ApplicationCurrentMemoryUsage");
        }

        /// <summary>
        /// Gets the peak memory usage, in bytes. Returns zero in non-debug mode
        /// </summary>
        /// <returns>Peak memory usage</returns>
        public static long GetPeakMemoryUsage()
        {
            // don't use DeviceExtendedProperties for release builds (requires a capability)
            return (long)DeviceExtendedProperties.GetValue("ApplicationPeakMemoryUsage");
        }

        private static void ShowPopup()
        {
            popup = new Popup();
            double fontSize = (double)Application.Current.Resources["PhoneFontSizeSmall"] - 2;
            Brush foreground = (Brush)Application.Current.Resources["PhoneForegroundBrush"];
            StackPanel sp = new StackPanel { Orientation = Orientation.Horizontal, Background = (Brush)Application.Current.Resources["PhoneSemitransparentBrush"] };
            currentMemoryKB = new TextBlock { Text = "---", FontSize = fontSize, Foreground = foreground };
            peakMemoryBlock = new TextBlock { Text = "", FontSize = fontSize, Foreground = foreground, Margin = new Thickness(5, 0, 0, 0) };
            sp.Children.Add(currentMemoryKB);
            //sp.Children.Add(new TextBlock { Text = " kb", FontSize = fontSize, Foreground = foreground });
            sp.Children.Add(peakMemoryBlock);

            currentMemoryMB = new TextBlock { Text = "---", FontSize = fontSize, Foreground = foreground };
            sp.Children.Add(currentMemoryMB);

            sp.RenderTransform = new CompositeTransform { Rotation = 90, TranslateX = 480, TranslateY = 480, CenterX = 0, CenterY = 0 };
            popup.Child = sp;
            popup.IsOpen = true;
        }

        private static void StartTimer(TimeSpan timespan)
        {
            timer = new DispatcherTimer();
            timer.Interval = timespan;
            timer.Tick += new EventHandler(timer_Tick);
            timer.Start();
        }

        static void timer_Tick(object sender, EventArgs e)
        {
            if (forceGc) GC.Collect();

            UpdateCurrentMemoryUsage();
            UpdatePeakMemoryUsage();
        }

        private static void UpdatePeakMemoryUsage()
        {
            if (alreadyFailedPeak) return;

            long peak = GetPeakMemoryUsage();
            if (peak >= MAX_MEMORY)
            {
                alreadyFailedPeak = true;
                Checkpoint("*MEMORY USAGE FAIL*");
                peakMemoryBlock.Text = "FAIL!";
                peakMemoryBlock.Foreground = new SolidColorBrush(Colors.Red);
                if (Debugger.IsAttached) Debug.Assert(false, "Peak memory condition violated");
            }
        }

        private static void UpdateCurrentMemoryUsage()
        {
            long mem = GetCurrentMemoryUsage();
            currentMemoryKB.Text = string.Format("{0:N}", mem / 1024) + "KB  ";
            currentMemoryMB.Text = string.Format("{0:f}", mem / 1024.00 / 1024.00) + "MB";
            int safetyBand = GetSafetyBand(mem);
            if (safetyBand != lastSafetyBand)
            {
                currentMemoryKB.Foreground = GetBrushForSafetyBand(safetyBand);
                lastSafetyBand = safetyBand;
            }
        }

        private static Brush GetBrushForSafetyBand(int safetyBand)
        {
            switch (safetyBand)
            {
                case 0:
                    return new SolidColorBrush(Colors.Green);

                case 1:
                    return new SolidColorBrush(Colors.Orange);

                default:
                    return new SolidColorBrush(Colors.Red);
            }
        }

        private static int GetSafetyBand(long mem)
        {
            double percent = (double)mem / (double)MAX_MEMORY;
            if (percent <= 0.75) return 0;

            if (percent <= 0.90) return 1;

            return 2;
        }

        private static void StopTimer()
        {
            timer.Stop();
            timer = null;
        }

        private static void HidePopup()
        {
            popup.IsOpen = false;
            popup = null;
        }
    }

    /// <summary>
    /// Holds checkpoint information for diagnosing memory usage
    /// </summary>
    public class MemoryCheckpoint
    {
        /// <summary>
        /// Creates a new instance
        /// </summary>
        /// <param name="text">Text for the checkpoint</param>
        /// <param name="memoryUsage">Memory usage at the time of the checkpoint</param>
        internal MemoryCheckpoint(string text, long memoryUsage)
        {
            Text = text;
            MemoryUsage = memoryUsage;
        }

        /// <summary>
        /// The text associated with this checkpoint
        /// </summary>
        public string Text { get; private set; }

        /// <summary>
        /// The memory usage at the time of the checkpoint
        /// </summary>
        public long MemoryUsage { get; private set; }
    }


}