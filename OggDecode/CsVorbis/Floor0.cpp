#include "pch.h"
#include "Floor0.h"

using namespace System;
using namespace OggSharp;

void Floor0::pack(Platform::Object i, OggSharp::csBuffer^ opb)
{
    OggSharp::InfoFloor0^ info = safe_cast<OggSharp::InfoFloor0^>(i);
    opb->write(info->order, 8);
    opb->write(info->rate, 16);
    opb->write(info->barkmap, 16);
    opb->write(info->ampbits, 6);
    opb->write(info->ampdB, 8);
    opb->write(info->numbooks - 1, 4);
    
    for (int j = 0; j < info->numbooks; j++)
        opb->write(info::books[j], 8);
}

Platform::Object Floor0::unpack(OggSharp::Info^ vi, OggSharp::csBuffer^ opb)
{
    OggSharp::InfoFloor0^ info = ref new OggSharp::InfoFloor0();
    info->order = opb->read(8);
    info->rate = opb->read(16);
    info->barkmap = opb->read(16);
    info->ampbits = opb->read(6);
    info->ampdB = opb->read(8);
    info->numbooks = opb->read(4) + 1;
    
    if ((info->order < 1) || (info->rate < 1) || (info->barkmap < 1) || (info->numbooks < 1))
    {
        return (nullptr);
    }
    
    for (int j = 0; j < info->numbooks; j++)
    {
        info->books[j] = opb->read(8);
        
        if (info->books[j] < 0 || info->books[j] >= vi->books)
        {
            return (nullptr);
        }
    }
    
    return (info);
}

Platform::Object Floor0::look(OggSharp::DspState^ vd, OggSharp::InfoMode^ mi, Platform::Object i)
{
    float scale;
    OggSharp::Info^ vi = vd->vi;
    OggSharp::InfoFloor0^ info = safe_cast<OggSharp::InfoFloor0^>(i);
    OggSharp::LookFloor0^ look = ref new OggSharp::LookFloor0();
    look->m = info->order;
    look->n = vi->blocksizes[mi->blockflag] / 2;
    look->ln = info->barkmap;
    look->vi = info;
    look->lpclook->init(look->ln, look->m);
    scale = look->ln / safe_cast<float>(toBARK(safe_cast<float>((info->rate / 2.0))));
    look->linearmap = new int[look->n];
    
    for (int j = 0; j < look->n; j++)
    {
        int val = safe_cast<int>(Math::Floor(toBARK(safe_cast<float>(((info->rate / 2.0) / look->n * j))) * scale));
        
        if (val >= look->ln)
            val = look::ln;
        look->linearmap[j] = val;
    }
    
    return look;
}

double Floor0::toBARK(float f)
{
    double a, b, c;
    a = 13.1 * Math::Atan(0.00074 * f);
    b = 2.24 * Math::Atan(f * f * 1.85e-8);
    c = 1.0e-4 * f;
    return (a + b + c);
}

Platform::Object^ Floor0::state(Platform::Object^ i)
{
    OggSharp::EchstateFloor0^ state = ref new OggSharp::EchstateFloor0();
    OggSharp::InfoFloor0^ info = safe_cast<OggSharp::InfoFloor0^>(i);
    state->codewords = new int[info->order];
    state->curve = new float[info->barkmap];
    state->frameno = -1;
    return (state);
}

void Floor0::free_info(Platform::Object i)
{
}

void Floor0::free_look(Platform::Object i)
{
}

void Floor0::free_state(Platform::Object vs)
{
}

int Floor0::forward(OggSharp::Block^ vb, Platform::Object i, float fin, float fout, Platform::Object vs)
{
    return 0;
}

int Floor0::inverse(OggSharp::Block^ vb, Platform::Object i, float fout)
{
    OggSharp::LookFloor0^ look = safe_cast<OggSharp::LookFloor0^>(i);
    OggSharp::InfoFloor0^ info = look->vi;
    int ampraw = vb->opb->read(info->ampbits);
    
    if (ampraw > 0)
    {
        int maxval = (1 << info->ampbits) - 1;
        float amp = safe_cast<float>(ampraw) / maxval * info->ampdB;
        int booknum = vb->opb->read(ilog(info->numbooks));
        
        if (booknum != -1 && booknum < info->numbooks)
        {
            Monitor::Enter(this);
            
            {
                if (lsp == nullptr || lsp.Length < look->m)
                {
                    lsp = new float[look->m];
                }
                else
                {
                    for (int j = 0; j < look->m; j++)
                        lsp[j] = 0.0f;
                }
                
                OggSharp::CodeBook^ b = vb->vd->fullbooks[info->books[booknum]];
                float last = 0.0f;
                
                for (int j = 0; j < look->m; j++)
                    fout[j] = 0.0f;
                
                for (int j = 0; j < look->m; j += b->dim)
                {
                    if (b->decodevs(lsp, j, vb->opb, 1, -1) == -1)
                    {
                        for (int k = 0; k < look->n; k++)
                            fout[k] = 0.0f;
                        return (0);
                    }
                }
                
                for (int j = 0; j < look->m; )
                {
                    for (int k = 0; k < b->dim; k++, j++)
                        lsp[j] += last;
                    last = lsp[j - 1];
                }
                
                Lsp::lsp_to_curve(fout, look->linearmap, look->n, look->ln, lsp, look->m, amp, info->ampdB);
                return (1);
            }
            Monitor::Exit(this);
        }
    }
    
    return (0);
}

Platform::Object Floor0::inverse1(OggSharp::Block^ vb, Platform::Object i, Platform::Object memo)
{
    OggSharp::LookFloor0^ look = safe_cast<OggSharp::LookFloor0^>(i);
    OggSharp::InfoFloor0^ info = look->vi;
    float lsp = nullptr;
    
    if (dynamic_cast<float>(memo) != nullptr)
    {
        lsp = safe_cast<float>(memo);
    }
    
    int ampraw = vb->opb->read(info->ampbits);
    
    if (ampraw > 0)
    {
        int maxval = (1 << info->ampbits) - 1;
        float amp = safe_cast<float>(ampraw) / maxval * info->ampdB;
        int booknum = vb->opb->read(ilog(info->numbooks));
        
        if (booknum != -1 && booknum < info->numbooks)
        {
            OggSharp::CodeBook^ b = vb->vd->fullbooks[info->books[booknum]];
            float last = 0.0f;
            
            if (lsp == nullptr || lsp.Length < look->m + 1)
            {
                lsp = new float[look->m + 1];
            }
            else
            {
                for (int j = 0; j < lsp.Length; j++)
                    lsp[j] = 0.0f;
            }
            
            for (int j = 0; j < look->m; j += b->dim)
            {
                if (b->decodev_set(lsp, j, vb->opb, b->dim) == -1)
                {
                    return (nullptr);
                }
            }
            
            for (int j = 0; j < look->m; )
            {
                for (int k = 0; k < b->dim; k++, j++)
                    lsp[j] += last;
                last = lsp[j - 1];
            }
            
            lsp[look->m] = amp;
            return (lsp);
        }
    }
    
    return (nullptr);
}

int Floor0::inverse2(OggSharp::Block^ vb, Platform::Object i, Platform::Object memo, float fout)
{
    OggSharp::LookFloor0^ look = safe_cast<OggSharp::LookFloor0^>(i);
    OggSharp::InfoFloor0^ info = look->vi;
    
    if (memo != nullptr)
    {
        float lsp = safe_cast<float>(memo);
        float amp = lsp[look->m];
        Lsp::lsp_to_curve(fout, look->linearmap, look->n, look->ln, lsp, look->m, amp, info->ampdB);
        return (1);
    }
    
    for (int j = 0; j < look->n; j++)
    {
        fout[j] = 0.0f;
    }
    
    return (0);
}

float Floor0::fromdB(float x)
{
    return safe_cast<float>((Math::Exp((x) * .11512925)));
}

int Floor0::ilog(int v)
{
    int ret = 0;
    
    while (v != 0)
    {
        ret++;
        v = safe_cast<int>((safe_cast<unsigned int>(v) >> 1));
    }
    
    return (ret);
}

void Floor0::lsp_to_lpc(float lsp, float lpc, int m)
{
    int i, j, m2 = m / 2;
    float O = new float[m2];
    float E = new float[m2];
    float A;
    float Ae = new float[m2 + 1];
    float Ao = new float[m2 + 1];
    float B;
    float Be = new float[m2];
    float Bo = new float[m2];
    float temp;
    
    for (i = 0; i < m2; i++)
    {
        O[i] = safe_cast<float>((-2.0 * Math::Cos(lsp[i * 2])));
        E[i] = safe_cast<float>((-2.0 * Math::Cos(lsp[i * 2 + 1])));
    }
    
    for (j = 0; j < m2; j++)
    {
        Ae[j] = 0.0f;
        Ao[j] = 1.0f;
        Be[j] = 0.0f;
        Bo[j] = 1.0f;
    }
    
    Ao[j] = 1.0f;
    Ae[j] = 1.0f;
    
    for (i = 1; i < m + 1; i++)
    {
        A = B = 0.0f;
        
        for (j = 0; j < m2; j++)
        {
            temp = O[j] * Ao[j] + Ae[j];
            Ae[j] = Ao[j];
            Ao[j] = A;
            A += temp;
            temp = E[j] * Bo[j] + Be[j];
            Be[j] = Bo[j];
            Bo[j] = B;
            B += temp;
        }
        
        lpc[i - 1] = (A + Ao[j] + B - Ae[j]) / 2;
        Ao[j] = A;
        Ae[j] = B;
    }
}

void Floor0::lpc_to_curve(float curve, float lpc, float amp, OggSharp::LookFloor0^ l, Platform::String name, int frameno)
{
    float lcurve = new float[Math::Max(l->ln * 2, l->m * 2 + 2)];
    
    if (amp == 0)
    {
        for (int j = 0; j < l->n; j++)
            curve[j] = 0.0f;
        return ;
    }
    
    l->lpclook->lpc_to_curve(lcurve, lpc, amp);
    
    for (int i = 0; i < l->n; i++)
        curve[i] = lcurve[l->linearmap[i]];
}


