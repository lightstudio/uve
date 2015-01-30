#include "pch.h"
#include "Lsp.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace OggSharp;

float Lsp::M_PI = safe_cast<float>((3.1415926539));

void FloatHack::lsp_to_curve(float curve, int map, int n, int ln, float lsp, int m, float amp, float ampoffset)
{
    int i;
    float wdel = M_PI / ln;
    
    for (i = 0; i < m; i++)
        lsp[i] = Lookup::coslook(lsp[i]);
    int m2 = (m / 2) * 2;
    i = 0;
    
    while (i < n)
    {
        OggSharp::Lsp::FloatHack^ fh = ref new OggSharp::Lsp::FloatHack();
        int k = map[i];
        float p = .7071067812f;
        float q = .7071067812f;
        float w = Lookup::coslook(wdel * k);
        int c = safe_cast<int>((safe_cast<unsigned int>(m) >> 1));
        
        for (int j = 0; j < m2; j += 2)
        {
            q *= lsp[j] - w;
            p *= lsp[j + 1] - w;
        }
        
        if ((m & 1) != 0)
        {
            q *= lsp[m - 1] - w;
            q *= q;
            p *= p * (1.0f - w * w);
        }
        else
        {
            q *= q * (1.0f + w);
            p *= p * (1.0f - w);
        }
        
        q = p + q;
        fh->fh_float = q;
        int hx = fh->fh_int;
        int ix = 0x7fffffff & hx;
        int qexp = 0;
        
        if (ix >= 0x7f800000 || (ix == 0))
        {
        }
        else
        {
            if (ix < 0x00800000)
            {
                q *= 3.3554432000e+07F;
                fh->fh_float = q;
                hx = fh->fh_int;
                ix = 0x7fffffff & hx;
                qexp = -25;
            }
            
            qexp += safe_cast<int>(((safe_cast<unsigned int>(ix) >> 23) - 126));
            hx = safe_cast<int>(((hx & 0x807fffff) | 0x3f000000));
            fh->fh_int = hx;
            q = fh->fh_float;
        }
        
        q = Lookup::fromdBlook(amp * Lookup::invsqlook(q) * Lookup::invsq2explook(qexp + m) - ampoffset);
        do
        {
            curve[i] *= q;
            i++;
        }
        while (i < n && map[i] == k)
    }
}


