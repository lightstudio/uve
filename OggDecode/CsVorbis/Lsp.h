#pragma once

namespace OggSharp
{
    ref class Lsp
    {
        [StructLayout(LayoutKind::Explicit, Size = 32, CharSet = CharSet::Auto)]
        private ref struct FloatHack
        {
            public: float fh_float;
            public: int fh_int;
        };
        private: static float M_PI;
        internal: static void lsp_to_curve(float curve, int map, int n, int ln, float lsp, int m, float amp, float ampoffset);
    };
}

