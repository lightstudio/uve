#pragma once

namespace OggSharp
{
    ref class Lpc
    {
        private: OggSharp::Drft^ fft;
        private: int ln;
        private: int m;
        private: static float lpc_from_data(float data, float lpc, int n, int m);
        private: float lpc_from_curve(float curve, float lpc);
        internal: void init(int mapped, int m);
        private: void clear();
        private: static float FAST_HYPOT(float a, float b);
        internal: void lpc_to_curve(float curve, float lpc, float amp);
    };
}

