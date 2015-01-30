#include "pch.h"
#include "DspState.h"

using namespace System;
using namespace OggSharp;

float DspState::M_PI = 3.1415926539f;

int DspState::VI_TRANSFORMB = 1;

int DspState::VI_WINDOWB = 1;

DspState::DspState()
{
    transform = new Object[2][];
    wnd = new float[2][][][][];
    wnd[0] = new float[2][][][];
    wnd[0][0] = new float[2][][];
    wnd[0][1] = new float[2][][];
    wnd[0][0][0] = new float[2][];
    wnd[0][0][1] = new float[2][];
    wnd[0][1][0] = new float[2][];
    wnd[0][1][1] = new float[2][];
    wnd[1] = new float[2][][][];
    wnd[1][0] = new float[2][][];
    wnd[1][1] = new float[2][][];
    wnd[1][0][0] = new float[2][];
    wnd[1][0][1] = new float[2][];
    wnd[1][1][0] = new float[2][];
    wnd[1][1][1] = new float[2][];
}

int DspState::ilog2(int v)
{
    int ret = 0;
    
    while (v > 1)
    {
        ret++;
        v = safe_cast<int>((safe_cast<unsigned int>(v) >> 1));
    }
    
    return (ret);
}

float DspState::window(int type, int wnd, int left, int right)
{
    float ret = new float[wnd];
    switch (type)
    {
        case 0:
        
        
        {
            int leftbegin = wnd / 4 - left / 2;
            int rightbegin = wnd - wnd / 4 - right / 2;
            
            for (int i = 0; i < left; i++)
            {
                float x = safe_cast<float>(((i + .5) / left * M_PI / 2.0));
                x = safe_cast<float>(Math::Sin(x));
                x *= x;
                x *= safe_cast<float>((M_PI / 2.0));
                x = safe_cast<float>(Math::Sin(x));
                ret[i + leftbegin] = x;
            }
            
            for (int i = leftbegin + left; i < rightbegin; i++)
            {
                ret[i] = 1.0f;
            }
            
            for (int i = 0; i < right; i++)
            {
                float x = safe_cast<float>(((right - i - .5) / right * M_PI / 2.0));
                x = safe_cast<float>(Math::Sin(x));
                x *= x;
                x *= safe_cast<float>((M_PI / 2.0));
                x = safe_cast<float>(Math::Sin(x));
                ret[i + rightbegin] = x;
            }
        }
            
            break;
        
        default: 
        
            return (nullptr);
    }
    
    return (ret);
}

int DspState::init(OggSharp::Info^ vi, Platform::bool encp)
{
    this->vi = vi;
    modebits = ilog2(vi->modes);
    transform[0] = new Object[VI_TRANSFORMB];
    transform[1] = new Object[VI_TRANSFORMB];
    transform[0][0] = ref new OggSharp::Mdct();
    transform[1][0] = ref new OggSharp::Mdct();
    (safe_cast<OggSharp::Mdct^>(transform[0][0]))->init(vi->blocksizes[0]);
    (safe_cast<OggSharp::Mdct^>(transform[1][0]))->init(vi->blocksizes[1]);
    wnd[0][0][0] = new float[VI_WINDOWB][];
    wnd[0][0][1] = wnd[0][0][0];
    wnd[0][1][0] = wnd[0][0][0];
    wnd[0][1][1] = wnd[0][0][0];
    wnd[1][0][0] = new float[VI_WINDOWB][];
    wnd[1][0][1] = new float[VI_WINDOWB][];
    wnd[1][1][0] = new float[VI_WINDOWB][];
    wnd[1][1][1] = new float[VI_WINDOWB][];
    
    for (int i = 0; i < VI_WINDOWB; i++)
    {
        wnd[0][0][0][i] = window(i, vi->blocksizes[0], vi->blocksizes[0] / 2, vi->blocksizes[0] / 2);
        wnd[1][0][0][i] = window(i, vi->blocksizes[1], vi->blocksizes[0] / 2, vi->blocksizes[0] / 2);
        wnd[1][0][1][i] = window(i, vi->blocksizes[1], vi->blocksizes[0] / 2, vi->blocksizes[1] / 2);
        wnd[1][1][0][i] = window(i, vi->blocksizes[1], vi->blocksizes[1] / 2, vi->blocksizes[0] / 2);
        wnd[1][1][1][i] = window(i, vi->blocksizes[1], vi->blocksizes[1] / 2, vi->blocksizes[1] / 2);
    }
    
    fullbooks = new CodeBook[vi->books];
    
    for (int i = 0; i < vi->books; i++)
    {
        fullbooks[i] = ref new OggSharp::CodeBook();
        fullbooks[i]->init_decode(vi->book_param[i]);
    }
    
    pcm_storage = 8192;
    pcm = new float[vi->channels][];

{
    for (int i = 0; i < vi->channels; i++)
    {
        pcm[i] = new float[pcm_storage];
    }
}
    
    lW = 0;
    W = 0;
    centerW = vi->blocksizes[1] / 2;
    pcm_current = centerW;
    mode = new Object[vi->modes];
    
    for (int i = 0; i < vi->modes; i++)
    {
        int mapnum = vi->mode_param[i]->mapping;
        int maptype = vi->map_type[mapnum];
        mode[i] = FuncMapping::mapping_P[maptype]->look(this, vi->mode_param[i], vi->map_param[mapnum]);
    }
    
    return (0);
}

int DspState::synthesis_init(OggSharp::Info^ vi)
{
    init(vi, false);
    pcm_returned = centerW;
    centerW -= vi->blocksizes[W] / 4 + vi->blocksizes[lW] / 4;
    granulepos = -1;
    sequence = -1;
    return (0);
}

DspState::DspState(OggSharp::Info^ vi) : 
    this()
{
    init(vi, false);
    pcm_returned = centerW;
    centerW -= vi->blocksizes[W] / 4 + vi->blocksizes[lW] / 4;
    granulepos = -1;
    sequence = -1;
}

int DspState::synthesis_blockin(OggSharp::Block^ vb)
{
    if (centerW > vi->blocksizes[1] / 2 && pcm_returned > 8192)
    {
        int shiftPCM = centerW - vi->blocksizes[1] / 2;
        shiftPCM = ((pcm_returned < shiftPCM) ? pcm_returned : shiftPCM);
        pcm_current -= shiftPCM;
        centerW -= shiftPCM;
        pcm_returned -= shiftPCM;
        
        if (shiftPCM != 0)
        {
            for (int i = 0; i < vi->channels; i++)
            {
                Array::Copy(pcm[i], shiftPCM, pcm[i], 0, pcm_current);
            }
        }
    }
    
    lW = W;
    W = vb->W;
    nW = -1;
    glue_bits += vb->glue_bits;
    time_bits += vb->time_bits;
    floor_bits += vb->floor_bits;
    res_bits += vb->res_bits;
    
    if (sequence + 1 != vb->sequence)
        granulepos = -1;
    sequence = vb->sequence;

{
    int sizeW = vi->blocksizes[W];
    int _centerW = centerW + vi->blocksizes[lW] / 4 + sizeW / 4;
    int beginW = _centerW - sizeW / 2;
    int endW = beginW + sizeW;
    int beginSl = 0;
    int endSl = 0;
    
    if (endW > pcm_storage)
    {
        pcm_storage = endW + vi->blocksizes[1];
        
        for (int i = 0; i < vi->channels; i++)
        {
            float foo = new float[pcm_storage];
            Array::Copy(pcm[i], 0, foo, 0, pcm[i].Length);
            pcm[i] = foo;
        }
    }
    
    switch (W)
    {
        case 0:
        
            beginSl = 0;
            endSl = vi->blocksizes[0] / 2;
            break;
        
        case 1:
        
            beginSl = vi->blocksizes[1] / 4 - vi->blocksizes[lW] / 4;
            endSl = beginSl + vi->blocksizes[lW] / 2;
            break;
        
    }
    
    for (int j = 0; j < vi->channels; j++)
    {
        int _pcm = beginW;
        int i = 0;
        
        for (i = beginSl; i < endSl; i++)
        {
            pcm[j][_pcm + i] += vb->pcm[j][i];
        }
        
        for (; i < sizeW; i++)
        {
            pcm[j][_pcm + i] = vb->pcm[j][i];
        }
    }
    
    if (granulepos == -1)
    {
        granulepos = vb->granulepos;
    }
    else
    {
        granulepos += (_centerW - centerW);
        
        if (vb->granulepos != -1 && granulepos != vb->granulepos)
        {
            if (granulepos > vb->granulepos && vb->eofflag != 0)
            {
                _centerW = _centerW - safe_cast<int>((granulepos - vb->granulepos));
            }
            
            granulepos = vb->granulepos;
        }
    }
    
    centerW = _centerW;
    pcm_current = endW;
    
    if (vb->eofflag != 0)
        eofflag = 1;
}
    
    return (0);
}

int DspState::synthesis_pcmout(float _pcm, int index)
{
    if (pcm_returned < centerW)
    {
        if (_pcm != nullptr)
        {
            for (int i = 0; i < vi->channels; i++)
            {
                index[i] = pcm_returned;
            }
            
            _pcm[0] = pcm;
        }
        
        return (centerW - pcm_returned);
    }
    
    return (0);
}

int DspState::synthesis_read(int bytes)
{
    if (bytes != 0 && pcm_returned + bytes > centerW)
        return (-1);
    pcm_returned += bytes;
    return (0);
}

void DspState::clear()
{
}


