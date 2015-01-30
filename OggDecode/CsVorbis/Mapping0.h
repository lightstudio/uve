#pragma once

namespace OggSharp
{
    ref class Mapping0: public OggSharp::FuncMapping
    {
        public: virtual void free_info(Platform::Object imap) override;
        public: virtual void free_look(Platform::Object imap) override;
        public: virtual Platform::Object look(OggSharp::DspState^ vd, OggSharp::InfoMode^ vm, Platform::Object m) override;
        public: virtual void pack(OggSharp::Info^ vi, Platform::Object imap, OggSharp::csBuffer^ opb) override;
        public: virtual Platform::Object unpack(OggSharp::Info^ vi, OggSharp::csBuffer^ opb) override;
        private: float pcmbundle;
        private: int zerobundle;
        private: int nonzero;
        private: Platform::Object floormemo;
        public: virtual int inverse(OggSharp::Block^ vb, Platform::Object l) override;
        private: static int ilog2(int v);
    };
    ref class InfoMapping0
    {
        internal: int submaps;
        internal: int chmuxlist;
        internal: int timesubmap;
        internal: int floorsubmap;
        internal: int residuesubmap;
        internal: int psysubmap;
        internal: int coupling_steps;
        internal: int coupling_mag;
        internal: int coupling_ang;
        internal: void free();
    };
    ref class LookMapping0
    {
        internal: OggSharp::InfoMode^ mode;
        internal: OggSharp::InfoMapping0^ map;
        internal: Platform::Object time_look;
        internal: Platform::Object floor_look;
        internal: Platform::Object residue_look;
        internal: OggSharp::FuncTime^ time_func;
        internal: OggSharp::FuncFloor^ floor_func;
        internal: OggSharp::FuncResidue^ residue_func;
        internal: int ch;
    };
}

