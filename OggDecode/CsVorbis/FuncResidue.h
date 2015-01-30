#pragma once

namespace OggSharp
{
    ref class FuncResidue
    {
        public: static OggSharp::FuncResidue^ residue_P;
        public: virtual void pack(Platform::Object vr, OggSharp::csBuffer^ opb);
        public: virtual Platform::Object unpack(OggSharp::Info^ vi, OggSharp::csBuffer^ opb);
        public: virtual Platform::Object look(OggSharp::DspState^ vd, OggSharp::InfoMode^ vm, Platform::Object vr);
        public: virtual void free_info(Platform::Object i);
        public: virtual void free_look(Platform::Object i);
        public: virtual int forward(OggSharp::Block^ vb, Platform::Object vl, float fin, int ch);
        public: virtual int inverse(OggSharp::Block^ vb, Platform::Object vl, float fin, int nonzero, int ch);
    };
}

