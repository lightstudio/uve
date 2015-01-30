#pragma once

namespace OggSharp
{
    ref class FuncFloor
    {
        public: static OggSharp::FuncFloor^ floor_P;
        public: virtual void pack(Platform::Object i, OggSharp::csBuffer^ opb);
        public: virtual Platform::Object unpack(OggSharp::Info^ vi, OggSharp::csBuffer^ opb);
        public: virtual Platform::Object look(OggSharp::DspState^ vd, OggSharp::InfoMode^ mi, Platform::Object i);
        public: virtual void free_info(Platform::Object i);
        public: virtual void free_look(Platform::Object i);
        public: virtual void free_state(Platform::Object vs);
        public: virtual int forward(OggSharp::Block^ vb, Platform::Object i, float fin, float fout, Platform::Object vs);
        public: virtual Platform::Object inverse1(OggSharp::Block^ vb, Platform::Object i, Platform::Object memo);
        public: virtual int inverse2(OggSharp::Block^ vb, Platform::Object i, Platform::Object memo, float fout);
    };
}

