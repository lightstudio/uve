#pragma once

namespace OggSharp
{
    public ref class Info
    {
        private: static int OV_EBADPACKET;
        private: static int OV_ENOTAUDIO;
        private: static Platform::String^ _vorbis;
        private: static int VI_TIMEB;
        private: static int VI_FLOORB;
        private: static int VI_RESB;
        private: static int VI_MAPB;
        private: static int VI_WINDOWB;
        public: int version;
        public: int channels;
        public: int rate;
        internal: int bitrate_upper;
        internal: int bitrate_nominal;
        internal: int bitrate_lower;
        internal: int blocksizes;
        internal: int modes;
        internal: int maps;
        internal: int times;
        internal: int floors;
        internal: int residues;
        internal: int books;
        internal: int psys;
        internal: OggSharp::InfoMode^ mode_param;
        internal: int map_type;
        internal: Platform::Object map_param;
        internal: int time_type;
        internal: Platform::Object time_param;
        internal: int floor_type;
        internal: Platform::Object floor_param;
        internal: int residue_type;
        internal: Platform::Object residue_param;
        internal: OggSharp::StaticCodeBook^ book_param;
        internal: OggSharp::PsyInfo^ psy_param;
        public: void init();
        public: void clear();
        private: int unpack_info(OggSharp::csBuffer^ opb);
        private: int unpack_books(OggSharp::csBuffer^ opb);
        public: int synthesis_headerin(OggSharp::Comment^ vc, OggSharp::Packet^ op);
        private: int pack_info(OggSharp::csBuffer^ opb);
        private: int pack_books(OggSharp::csBuffer^ opb);
        public: int blocksize(OggSharp::Packet^ op);
        private: static int ilog2(int v);
        public: Platform::String toString();
    };
}

