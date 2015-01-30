#pragma once

namespace OggSharp
{
    ref class FuncMapping
    {
        public: static OggSharp::FuncMapping^ mapping_P;
        public: virtual void pack(OggSharp::Info^ info, Platform::Object imap, OggSharp::csBuffer^ buffer);
        public: virtual Platform::Object unpack(OggSharp::Info^ info, OggSharp::csBuffer^ buffer);
        public: virtual Platform::Object look(OggSharp::DspState^ vd, OggSharp::InfoMode^ vm, Platform::Object m);
        public: virtual void free_info(Platform::Object imap);
        public: virtual void free_look(Platform::Object imap);
        public: virtual int inverse(OggSharp::Block^ vd, Platform::Object lm);
    };
}

