#pragma once

namespace OggSharp
{
    ref class Floor1: public OggSharp::FuncFloor
    {
        private: static int floor1_rangedb;
        private: static int VIF_POSIT;
        public: virtual void pack(Platform::Object i, OggSharp::csBuffer^ opb) override;
        public: virtual Platform::Object unpack(OggSharp::Info^ vi, OggSharp::csBuffer^ opb) override;
        public: virtual Platform::Object look(OggSharp::DspState^ vd, OggSharp::InfoMode^ mi, Platform::Object i) override;
        public: virtual void free_info(Platform::Object i) override;
        public: virtual void free_look(Platform::Object i) override;
        public: virtual void free_state(Platform::Object vs) override;
        public: virtual int forward(OggSharp::Block^ vb, Platform::Object i, float fin, float fout, Platform::Object vs) override;
        public: virtual Platform::Object inverse1(OggSharp::Block^ vb, Platform::Object ii, Platform::Object memo) override;
        private: static int render_point(int x0, int x1, int y0, int y1, int x);
        public: virtual int inverse2(OggSharp::Block^ vb, Platform::Object i, Platform::Object memo, float fout) override;
        private: static float FLOOR_fromdB_LOOKUP;
        private: static void render_line(int x0, int x1, int y0, int y1, float d);
        private: static int ilog(int v);
        private: static int ilog2(int v);
    };
    ref class InfoFloor1
    {
        private: const static int VIF_POSIT;
        private: const static int VIF_CLASS;
        private: const static int VIF_PARTS;
        internal: int partitions;
        internal: int partitionclass;
        internal: int class_dim;
        internal: int class_subs;
        internal: int class_book;
        internal: int class_subbook;
        internal: int mult;
        internal: int postlist;
        internal: float maxover;
        internal: float maxunder;
        internal: float maxerr;
        internal: int twofitminsize;
        internal: int twofitminused;
        internal: int twofitweight;
        internal: float twofitatten;
        internal: int unusedminsize;
        internal: int unusedmin_n;
        internal: int n;
        internal: InfoFloor1();
        internal: void free();
        internal: Platform::Object copy_info();
    };
    ref class LookFloor1
    {
        private: static int VIF_POSIT;
        internal: int sorted_index;
        internal: int forward_index;
        internal: int reverse_index;
        internal: int hineighbor;
        internal: int loneighbor;
        internal: int posts;
        internal: int n;
        internal: int quant_q;
        internal: OggSharp::InfoFloor1^ vi;
        private: void free();
    };
    ref class Lsfit_acc
    {
    };
    ref class EchstateFloor1
    {
    };
}

