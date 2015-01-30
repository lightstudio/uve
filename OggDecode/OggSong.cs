#region Imports

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Media;

#endregion Imports

namespace OggSharp
{
    /// <summary>
    /// Decodes an ogg file and plays it as a song
    /// Usage:
    /// OggSong menuMusic = new OggSong(TitleContainer.OpenStream("Content/Music/MainMenuMusic.ogg"));
    ///menuMusic.Play();
    ///menuMusic.Pitch = 1.0f; // speed it up!
    ///menuMusic.Pan = 0.1f; // move it from left to right speakers
    ///float lengthInSeconds = menuMusic.Length; // how many seconds long is this song?
    ///float position = menuMusic.Position; // where in the song are we?
    ///menuMusic.Position = 5.0f; // seek 5 seconds into the song
    /// for sound effects, does not support playing multiple simultaneous times with the same OggSong yet...
    ///OggSong explosion = new OggSong(TitleContainer.OpenStream("Content/Sounds/Explosion.ogg"));
    ///explosion.Play();
    /// </summary>
    public class OggSong
    {
        private OggDecoder decoder;
        private int bufferCount;
        private DynamicSoundEffectInstance effect;
        private IEnumerator<PCMChunk> enumerator;

        private void BufferNeeded(object sender, EventArgs e)
        {
            lock (this)
            {
                if (enumerator == null)
                {
                    MediaState = MediaState.Stopped;
                    return;
                }
                else if (bufferCount == -1)
                {
                    if (!enumerator.MoveNext())
                    {
                        return;
                    }

                    bufferCount = enumerator.Current.Length;
                }

                for (int i = 0; i < 2; i++)
                {
                    byte[] buffer = enumerator.Current.Bytes;
                    effect.SubmitBuffer(buffer, 0, bufferCount);

                    if (enumerator.MoveNext())
                    {
                        bufferCount = enumerator.Current.Length;
                    }
                    else
                    {
                        enumerator.Dispose();
                        decoder.Reset();
                        if (Repeat)
                        {
                            enumerator = decoder.GetEnumerator();
                        }
                        else
                        {
                            enumerator = null;
                        }
                        bufferCount = -1;
                        break;
                    }
                }
            }
        }

        public OggSong(Stream stream) : this(stream, true) { }

        public OggSong(Stream stream, bool seekable)
        {
            decoder = new OggDecoder();
            decoder.Initialize(stream, seekable);
            effect = new DynamicSoundEffectInstance(decoder.SampleRate, (decoder.Stereo ? AudioChannels.Stereo : AudioChannels.Mono));
            effect.BufferNeeded += BufferNeeded;
            bufferCount = -1;
        }
        public void Dispose()
        {
            if (decoder != null)
            {
                decoder.Dispose();
                decoder = null;
            }
            if (effect != null)
            {
                effect.Dispose();
                effect = null;
            }
            if (enumerator != null)
            {
                enumerator.Dispose();
                enumerator = null;
            }
            bufferCount = 0;
        }
        public void Play()
        {
            lock (this)
            {
                if (MediaState == MediaState.Playing)
                {
                    return;
                }

                MediaState = MediaState.Playing;

                if (enumerator == null)
                {
                    enumerator = decoder.GetEnumerator();
                }

                bufferCount = -1;
                effect.Stop();
                effect.Play();
            }
        }

        public void Pause()
        {
            if (MediaState == MediaState.Paused)
            {
                return;
            }

            MediaState = MediaState.Paused;
            effect.Pause();
        }

        public void Resume()
        {
            if (MediaState != MediaState.Paused)
            {
                return;
            }

            MediaState = MediaState.Playing;
            effect.Resume();
        }

        public void Stop()
        {
            lock (this)
            {
                if (MediaState == MediaState.Stopped || bufferCount == -1)
                {
                    return;
                }

                MediaState = MediaState.Stopped;
                effect.Stop();
                if (enumerator != null)
                {
                    enumerator.Dispose();
                    enumerator = null;
                }
                decoder.Reset();
                bufferCount = -1;
            }
        }

        public bool Repeat
        {
            get;
            set;
        }

        public float Volume
        {
            get { return effect.Volume; }
            set { effect.Volume = value; }
        }

        public float Pan
        {
            get { return effect.Pan; }
            set { effect.Pan = value; }
        }

        public float Pitch
        {
            get { return effect.Pitch; }
            set { effect.Pitch = value; }
        }

        public MediaState MediaState
        {
            get;
            private set;
        }

        public float Position
        {
            get { lock (this) { return decoder.Position; } }
            set { lock (this) { decoder.Position = value; } }
        }

        public float Length
        {
            get { lock (this) { return decoder.Length; } }
        }
    }
}
