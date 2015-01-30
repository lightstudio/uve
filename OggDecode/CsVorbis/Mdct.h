#pragma once

namespace OggSharp
{
    ref class Mdct
    {
        private: int n;
        private: int log2n;
        private: float trig;
        private: int bitrev;
        private: float scale;
        internal: void init(int n);
        internal: void clear();
        internal: void forward(float fin, float fout);
        private: float _x;
        private: float _w;
        internal: void backward(float fin, float fout);
        internal: float mdct_kernel(float x, float w, int n, int n2, int n4, int n8);
    };
}

