#pragma once

namespace OggSharp
{
    ref class Floor0: public OggSharp::FuncFloor
    {
        public: virtual void pack(Platform::Object i, OggSharp::csBuffer^ opb) override;
        public: virtual Platform::Object unpack(OggSharp::Info^ vi, OggSharp::csBuffer^ opb) override;
        public: virtual Platform::Object look(OggSharp::DspState^ vd, OggSharp::InfoMode^ mi, Platform::Object i) override;
        private: static double toBARK(float f);
        private: Platform::Object^ state(Platform::Object^ i);
        public: virtual void free_info(Platform::Object i) override;
        public: virtual void free_look(Platform::Object i) override;
        public: virtual void free_state(Platform::Object vs) override;
        public: virtual int forward(OggSharp::Block^ vb, Platform::Object i, float fin, float fout, Platform::Object vs) override;
        private: float lsp;
        private: int inverse(OggSharp::Block^ vb, Platform::Object i, float fout);
        public: virtual Platform::Object inverse1(OggSharp::Block^ vb, Platform::Object i, Platform::Object memo) override;
        public: virtual int inverse2(OggSharp::Block^ vb, Platform::Object i, Platform::Object memo, float fout) override;
        private: static float fromdB(float x);
        private: static int ilog(int v);
        private: static void lsp_to_lpc(float lsp, float lpc, int m);
        private: static void lpc_to_curve(float curve, float lpc, float amp, OggSharp::LookFloor0^ l, Platform::String name, int frameno);
    };
    ref class InfoFloor0
    {
        internal: int order;
        internal: int rate;
        internal: int barkmap;
        internal: int ampbits;
        internal: int ampdB;
        internal: int numbooks;
        internal: int books;
    };
    ref class LookFloor0
    {
        internal: int n;
        internal: int ln;
        internal: int m;
        internal: int linearmap;
        internal: OggSharp::InfoFloor0^ vi;
        internal: OggSharp::Lpc^ lpclook;
    };
    ref class EchstateFloor0
    {
        internal: int codewords;
        internal: float curve;
        internal: long frameno;
    };
}

