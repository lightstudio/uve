#pragma once

namespace OggSharp
{
    ref class FuncTime
    {
        public: static OggSharp::FuncTime^ time_P;
        public: virtual void pack(Platform::Object i, OggSharp::csBuffer^ opb);
        public: virtual Platform::Object unpack(OggSharp::Info^ vi, OggSharp::csBuffer^ opb);
        public: virtual Platform::Object look(OggSharp::DspState^ vd, OggSharp::InfoMode^ vm, Platform::Object i);
        public: virtual void free_info(Platform::Object i);
        public: virtual void free_look(Platform::Object i);
        public: virtual int forward(OggSharp::Block^ vb, Platform::Object i);
        public: virtual int inverse(OggSharp::Block^ vb, Platform::Object i, float fin, float fout);
    };
}

