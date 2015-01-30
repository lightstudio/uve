#pragma once

namespace OggSharp
{
    ref class Drft
    {
        private: int n;
        private: float trigcache;
        private: int splitcache;
        internal: void backward(float data);
        internal: void init(int n);
        internal: void clear();
        private: static int ntryh;
        private: static float tpi;
        private: static float hsqt2;
        private: static float taui;
        private: static float taur;
        private: static float sqrt2;
        private: static void drfti1(int n, float wa, int index, int ifac);
        private: static void fdrffti(int n, float wsave, int ifac);
        private: static void dradf2(int ido, int l1, float cc, float ch, float wa1, int index);
        private: static void dradf4(int ido, int l1, float cc, float ch, float wa1, int index1, float wa2, int index2, float wa3, int index3);
        private: static void dradfg(int ido, int ip, int l1, int idl1, float cc, float c1, float c2, float ch, float ch2, float wa, int index);
        private: static void drftf1(int n, float c, float ch, float wa, int ifac);
        private: static void dradb2(int ido, int l1, float cc, float ch, float wa1, int index);
        private: static void dradb3(int ido, int l1, float cc, float ch, float wa1, int index1, float wa2, int index2);
        private: static void dradb4(int ido, int l1, float cc, float ch, float wa1, int index1, float wa2, int index2, float wa3, int index3);
        private: static void dradbg(int ido, int ip, int l1, int idl1, float cc, float c1, float c2, float ch, float ch2, float wa, int index);
        private: static void drftb1(int n, float c, float ch, float wa, int index, int ifac);
    };
}

