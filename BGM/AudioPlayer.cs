using System;
using System.Diagnostics;
using System.Windows;
using System.Collections.Generic;
using Microsoft.Phone.BackgroundAudio;


namespace BGM
{
    public class AudioPlayer : AudioPlayerAgent
    {
        /// <remarks>
        /// AudioPlayer 实例可共享同一进程。
        /// 静态字段可用于 AudioPlayer 实例之间共享状态
        /// 或与音频流代理通信。
        /// </remarks>
        /// \

        static AudioPlayer()
        {
            
            // 订阅托管异常处理程序
            Deployment.Current.Dispatcher.BeginInvoke(delegate
            {
                Application.Current.UnhandledException += UnhandledException;
            });
        }

        /// 出现未处理的异常时执行的代码
        private static void UnhandledException(object sender, ApplicationUnhandledExceptionEventArgs e)
        {
            if (Debugger.IsAttached)
            {
                // 出现未处理的异常；强行进入调试器
                Debugger.Break();
            }
        }

        /// <summary>
        /// playstate 更改时调用，但 Error 状态除外(参见 OnError)
        /// </summary>
        /// <param name="player">BackgroundAudioPlayer</param>
        /// <param name="track">在 playstate 更改时播放的曲目</param>
        /// <param name="playState">播放机的新 playstate </param>
        /// <remarks>
        /// 无法取消播放状态更改。即使应用程序
        /// 导致状态自行更改也会提出这些更改，假定应用程序已经选择了回调。
        ///
        /// 值得注意的 playstate 事件:
        /// (a) TrackEnded: 播放器没有当前曲目时激活。代理可设置下一曲目。
        /// (b) TrackReady: 音轨已设置完毕，现在可以播放。
        ///
        /// 只在代理请求完成之后调用一次 NotifyComplete()，包括异步回调。
        /// </remarks>
        protected override void OnPlayStateChanged(BackgroundAudioPlayer player, AudioTrack track, PlayState playState)
        {
            switch (playState)
            {
                case PlayState.TrackEnded:
 //                   player.Track = GetPreviousTrack();
                    player.Track = player.Track;
                    break;
                case PlayState.TrackReady:
                    player.Play();
                    break;
                case PlayState.Shutdown:
                    // TODO: 在此处理关机状态(例如保存状态)
                    break;
                case PlayState.Unknown:
                    break;
                case PlayState.Stopped:
                    break;
                case PlayState.Paused:
                    break;
                case PlayState.Playing:
                    break;
                case PlayState.BufferingStarted:
                    break;
                case PlayState.BufferingStopped:
                    break;
                case PlayState.Rewinding:
                    break;
                case PlayState.FastForwarding:
                    break;
            }

            NotifyComplete();
        }

        /// <summary>
        /// 在用户使用应用程序/系统提供的用户界面请求操作时调用
        /// </summary>
        /// <param name="player">BackgroundAudioPlayer</param>
        /// <param name="track">用户操作期间播放的曲目</param>
        /// <param name="action">用户请求的操作</param>
        /// <param name="param">与请求的操作相关联的数据。
        /// 在当前版本中，此参数仅适合与 Seek 操作一起使用，
        /// 以指明请求的乐曲的位置</param>
        /// <remarks>
        /// 用户操作不自动对系统状态进行任何更改；如果用户操作受支持，
        /// 以便执行用户操作(如果这些操作受支持)。
        ///
        /// 只在代理请求完成之后调用 NotifyComplete() 一次，包括异步回调。
        /// </remarks>
        protected override void OnUserAction(BackgroundAudioPlayer player, AudioTrack track, UserAction action, object param)
        {
            switch (action)
            {
                case UserAction.Play:
                    if (player.PlayerState != PlayState.Playing)
                    {
                        if (player.PlayerState != PlayState.Paused) player.Track = track;
                        else player.Play();
           //             player.Play();
                    }
                    break;
                case UserAction.Stop:
                    player.Stop();
                    break;
                case UserAction.Pause:
                    player.Pause();
                    break;
                case UserAction.FastForward:
                    player.FastForward();
                    break;
                case UserAction.Rewind:
                    player.Rewind();
                    break;
                case UserAction.Seek:
                    player.Position = (TimeSpan)param;
                    break;
                case UserAction.SkipNext:
                    player.Track = GetNextTrack();
                    break;
                case UserAction.SkipPrevious:
                    AudioTrack previousTrack = GetPreviousTrack();
                    if (previousTrack != null)
                    {
                        player.Track = previousTrack;
                    }
                    break;
            }

            NotifyComplete();
        }

        /// <summary>
        /// 实现逻辑以获取下一个 AudioTrack 实例。
        /// 在播放列表中，源可以是文件、Web 请求，等等。
        /// </summary>
        /// <remarks>
        /// AudioTrack URI 确定源，源可以是:
        /// (a) 独立存储器文件(相对 URI，表示独立存储器中的路径)
        /// (b) HTTP URL(绝对 URI)
        /// (c) MediaStreamSource (null)
        /// </remarks>
        /// <returns>AudioTrack 实例，或如果播放完毕，则返回 null</returns>
        private AudioTrack GetNextTrack()
        {
            // TODO: 添加逻辑以获取下一条音轨

            AudioTrack track = null;

            // 指定曲目

            return track;
        }

        /// <summary>
        /// 实现逻辑以获取前一个 AudioTrack 实例。
        /// </summary>
        /// <remarks>
        /// AudioTrack URI 确定源，它可以是:
        /// (a) 独立存储器文件(相对 URI，表示独立存储器中的路径)
        /// (b) HTTP URL(绝对 URI)
        /// (c) MediaStreamSource (null)
        /// </remarks>
        /// <returns>AudioTrack 实例，或如果不允许前一曲目，则返回 null</returns>
        private AudioTrack GetPreviousTrack()
        {
            // TODO: 添加逻辑以获取前一条音轨

            AudioTrack track = null;

            // 指定曲目

            return track;
        }

        /// <summary>
        /// 每次播放出错(如 AudioTrack 未正确下载)时调用
        /// </summary>
        /// <param name="player">BackgroundAudioPlayer</param>
        /// <param name="track">出现错误的曲目</param>
        /// <param name="error">出现的错误</param>
        /// <param name="isFatal">如果为 true，则播放不能继续并且曲目播放将停止</param>
        /// <remarks>
        /// 不保证在所有情况下都调用此方法。例如，如果后台代理程序
        /// 本身具有未处理的异常，则不会回调它来处理它自己的错误。
        /// </remarks>
        protected override void OnError(BackgroundAudioPlayer player, AudioTrack track, Exception error, bool isFatal)
        {
            if (isFatal)
            {
                Abort();
            }
            else
            {
                NotifyComplete();
            }

        }

        /// <summary>
        /// 取消代理请求时调用
        /// </summary>
        /// <remarks>
        /// 取消请求后，代理需要 5 秒钟完成其工作，
        /// 通过调用 NotifyComplete()/Abort()。
        /// </remarks>
        protected override void OnCancel()
        {

        }
    }
}