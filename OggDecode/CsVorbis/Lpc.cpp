#include "pch.h"
#include "Lpc.h"

using namespace System;
using namespace OggSharp;

float Lpc::lpc_from_data(float data, float lpc, int n, int m)
{
    float aut = new float[m + 1];
    float error;
    int i, j;
    j = m + 1;
    
    while (j-- != 0)
    {
        float d = 0.0F;
        
        for (i = j; i < n; i++)
            d += data[i] * data[i - j];
        aut[j] = d;
    }
    
    error = aut[0];
    
    for (i = 0; i < m; i++)
    {
        float r = -aut[i + 1];
        
        if (error == 0)
        {
            for (int k = 0; k < m; k++)
                lpc[k] = 0.0f;
            return 0;
        }
        
        for (j = 0; j < i; j++)
            r -= lpc[j] * aut[i - j];
        r /= error;
        lpc[i] = r;
        
        for (j = 0; j < i / 2; j++)
        {
            float tmp = lpc[j];
            lpc[j] += r * lpc[i - 1 - j];
            lpc[i - 1 - j] += r * tmp;
        }
        
        if (i % 2 != 0)
            lpc[j] += lpc[j] * r;
        error *= safe_cast<float>((1.0 - r * r));
    }
    
    return error;
}

float Lpc::lpc_from_curve(float curve, float lpc)
{
    int n = ln;
    float work = new float[n + n];
    float fscale = safe_cast<float>((.5 / n));
    int i, j;
    
    for (i = 0; i < n; i++)
    {
        work[i * 2] = curve[i] * fscale;
        work[i * 2 + 1] = 0;
    }
    
    work[n * 2 - 1] = curve[n - 1] * fscale;
    n *= 2;
    fft->backward(work);
    
    for (i = 0, j = n / 2; i < n / 2; )
    {
        float temp = work[i];
        work[i++] = work[j];
        work[j++] = temp;
    }
    
    return (lpc_from_data(work, lpc, n, m));
}

void Lpc::init(int mapped, int m)
{
    ln = mapped;
    this->m = m;
    fft->init(mapped * 2);
}

void Lpc::clear()
{
    fft->clear();
}

float Lpc::FAST_HYPOT(float a, float b)
{
    return safe_cast<float>(Math::Sqrt((a) * safe_cast<a>(+(b)) * (b)));
}

void Lpc::lpc_to_curve(float curve, float lpc, float amp)
{
    for (int i = 0; i < ln * 2; i++)
        curve[i] = 0.0f;
    
    if (amp == 0)
        return ;
    
    for (int i = 0; i < m; i++)
    {
        curve[i * 2 + 1] = lpc[i] / (4 * amp);
        curve[i * 2 + 2] = -lpc[i] / (4 * amp);
    }
    
    fft->backward(curve);
    int l2 = ln * 2;
    float unit = safe_cast<float>((1.0 / amp));
    curve[0] = safe_cast<float>((1.0 / (curve[0] * 2 + unit)));
    
    for (int i = 1; i < ln; i++)
    {
        float real = (curve[i] + curve[l2 - i]);
        float imag = (curve[i] - curve[l2 - i]);
        float a = real + unit;
        curve[i] = safe_cast<float>((1.0 / FAST_HYPOT(a, imag)));
    }
}


