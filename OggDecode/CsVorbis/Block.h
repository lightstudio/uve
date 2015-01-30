#pragma once

namespace OggSharp
{
    public ref class Block
    {
        internal: float pcm;
        internal: OggSharp::csBuffer^ opb;
        internal: int lW;
        internal: int W;
        internal: int nW;
        internal: int pcmend;
        internal: int mode;
        internal: int eofflag;
        internal: long granulepos;
        internal: long sequence;
        internal: OggSharp::DspState^ vd;
        internal: int glue_bits;
        internal: int time_bits;
        internal: int floor_bits;
        internal: int res_bits;
        public: Block(OggSharp::DspState^ vd);
        public: void init(OggSharp::DspState^ vd);
        public: int clear();
        public: int synthesis(OggSharp::Packet^ op);
    };
}

