#pragma once

namespace OggSharp
{
    public ref class DspState
    {
        private: static float M_PI;
        private: static int VI_TRANSFORMB;
        private: static int VI_WINDOWB;
        internal: int analysisp;
        internal: OggSharp::Info^ vi;
        internal: int modebits;
        private: float pcm;
        private: int pcm_storage;
        private: int pcm_current;
        private: int pcm_returned;
        private: float multipliers;
        private: int envelope_storage;
        private: int envelope_current;
        private: int eofflag;
        private: int lW;
        private: int W;
        private: int nW;
        private: int centerW;
        private: long granulepos;
        public: long sequence;
        private: long glue_bits;
        private: long time_bits;
        private: long floor_bits;
        private: long res_bits;
        internal: float wnd;
        internal: Platform::Object transform;
        internal: OggSharp::CodeBook^ fullbooks;
        internal: Platform::Object mode;
        private: unsigned char header;
        private: unsigned char header1;
        private: unsigned char header2;
        public: DspState();
        private: static int ilog2(int v);
        internal: static float window(int type, int wnd, int left, int right);
        private: int init(OggSharp::Info^ vi, Platform::bool encp);
        public: int synthesis_init(OggSharp::Info^ vi);
        private: DspState(OggSharp::Info^ vi);
        public: int synthesis_blockin(OggSharp::Block^ vb);
        public: int synthesis_pcmout(float _pcm, int index);
        public: int synthesis_read(int bytes);
        public: void clear();
    };
}

