#include "pch.h"
#include "Mapping0.h"

using namespace System;
using namespace OggSharp;

void Mapping0::free_info(Platform::Object imap)
{
}

void Mapping0::free_look(Platform::Object imap)
{
}

Platform::Object Mapping0::look(OggSharp::DspState^ vd, OggSharp::InfoMode^ vm, Platform::Object m)
{
    OggSharp::Info^ vi = vd->vi;
    OggSharp::LookMapping0^ looks = ref new OggSharp::LookMapping0();
    OggSharp::InfoMapping0^ info = looks->map = safe_cast<OggSharp::InfoMapping0^>(m);
    looks->mode = vm;
    looks->time_look = new Object[info->submaps];
    looks->floor_look = new Object[info->submaps];
    looks->residue_look = new Object[info->submaps];
    looks->time_func = new FuncTime[info->submaps];
    looks->floor_func = new FuncFloor[info->submaps];
    looks->residue_func = new FuncResidue[info->submaps];
    
    for (int i = 0; i < info->submaps; i++)
    {
        int timenum = info->timesubmap[i];
        int floornum = info->floorsubmap[i];
        int resnum = info->residuesubmap[i];
        looks->time_func[i] = FuncTime::time_P[vi->time_type[timenum]];
        looks->time_look[i] = looks->time_func[i]->look(vd, vm, vi->time_param[timenum]);
        looks->floor_func[i] = FuncFloor::floor_P[vi->floor_type[floornum]];
        looks->floor_look[i] = looks->floor_func[i]->look(vd, vm, vi->floor_param[floornum]);
        looks->residue_func[i] = FuncResidue::residue_P[vi->residue_type[resnum]];
        looks->residue_look[i] = looks->residue_func[i]->look(vd, vm, vi->residue_param[resnum]);
    }
    
    if (vi->psys != 0 && vd->analysisp != 0)
    {
    }
    
    looks->ch = vi->channels;
    return (looks);
}

void Mapping0::pack(OggSharp::Info^ vi, Platform::Object imap, OggSharp::csBuffer^ opb)
{
    OggSharp::InfoMapping0^ info = safe_cast<OggSharp::InfoMapping0^>(imap);
    
    if (info->submaps > 1)
    {
        opb->write(1, 1);
        opb->write(info->submaps - 1, 4);
    }
    else
    {
        opb->write(0, 1);
    }
    
    if (info->coupling_steps > 0)
    {
        opb->write(1, 1);
        opb->write(info->coupling_steps - 1, 8);
        
        for (int i = 0; i < info->coupling_steps; i++)
        {
            opb->write(info->coupling_mag[i], ilog2(vi->channels));
            opb->write(info->coupling_ang[i], ilog2(vi->channels));
        }
    }
    else
    {
        opb->write(0, 1);
    }
    
    opb->write(0, 2);
    
    if (info->submaps > 1)
    {
        for (int i = 0; i < vi->channels; i++)
            opb->write(info::chmuxlist[i], 4);
    }
    
    for (int i = 0; i < info->submaps; i++)
    {
        opb->write(info->timesubmap[i], 8);
        opb->write(info->floorsubmap[i], 8);
        opb->write(info->residuesubmap[i], 8);
    }
}

Platform::Object Mapping0::unpack(OggSharp::Info^ vi, OggSharp::csBuffer^ opb)
{
    OggSharp::InfoMapping0^ info = ref new OggSharp::InfoMapping0();
    
    if (opb->read(1) != 0)
    {
        info->submaps = opb->read(4) + 1;
    }
    else
    {
        info->submaps = 1;
    }
    
    if (opb->read(1) != 0)
    {
        info->coupling_steps = opb->read(8) + 1;
        
        for (int i = 0; i < info->coupling_steps; i++)
        {
            int testM = info->coupling_mag[i] = opb->read(ilog2(vi->channels));
            int testA = info->coupling_ang[i] = opb->read(ilog2(vi->channels));
            
            if (testM < 0 || testA < 0 || testM == testA || testM >= vi->channels || testA >= vi->channels)
            {
                info->free();
                return (nullptr);
            }
        }
    }
    
    if (opb->read(2) > 0)
    {
        info->free();
        return (nullptr);
    }
    
    if (info->submaps > 1)
    {
        for (int i = 0; i < vi->channels; i++)
        {
            info->chmuxlist[i] = opb->read(4);
            
            if (info->chmuxlist[i] >= info->submaps)
            {
                info->free();
                return (nullptr);
            }
        }
    }
    
    for (int i = 0; i < info->submaps; i++)
    {
        info->timesubmap[i] = opb->read(8);
        
        if (info->timesubmap[i] >= vi->times)
        {
            info->free();
            return (nullptr);
        }
        
        info->floorsubmap[i] = opb->read(8);
        
        if (info->floorsubmap[i] >= vi->floors)
        {
            info->free();
            return (nullptr);
        }
        
        info->residuesubmap[i] = opb->read(8);
        
        if (info->residuesubmap[i] >= vi->residues)
        {
            info->free();
            return (nullptr);
        }
    }
    
    return info;
}

int Mapping0::inverse(OggSharp::Block^ vb, Platform::Object l)
{
    Monitor::Enter(this);
    
    {
        OggSharp::DspState^ vd = vb->vd;
        OggSharp::Info^ vi = vd->vi;
        OggSharp::LookMapping0^ look = safe_cast<OggSharp::LookMapping0^>(l);
        OggSharp::InfoMapping0^ info = look->map;
        OggSharp::InfoMode^ mode = look->mode;
        int n = vb->pcmend = vi->blocksizes[vb->W];
        float window = vd->wnd[vb->W][vb->lW][vb->nW][mode->windowtype];
        
        if (pcmbundle == nullptr || pcmbundle.Length < vi->channels)
        {
            pcmbundle = new float[vi->channels][];
            nonzero = new int[vi->channels];
            zerobundle = new int[vi->channels];
            floormemo = new Object[vi->channels];
        }
        
        for (int i = 0; i < vi->channels; i++)
        {
            float pcm = vb->pcm[i];
            int submap = info->chmuxlist[i];
            floormemo[i] = look->floor_func[submap]->inverse1(vb, look->floor_look[submap], floormemo[i]);
            
            if (floormemo[i] != nullptr)
            {
                nonzero[i] = 1;
            }
            else
            {
                nonzero[i] = 0;
            }
            
            for (int j = 0; j < n / 2; j++)
            {
                pcm[j] = 0;
            }
        }
        
        for (int i = 0; i < info->coupling_steps; i++)
        {
            if (nonzero[info->coupling_mag[i]] != 0 || nonzero[info->coupling_ang[i]] != 0)
            {
                nonzero[info->coupling_mag[i]] = 1;
                nonzero[info->coupling_ang[i]] = 1;
            }
        }
        
        for (int i = 0; i < info->submaps; i++)
        {
            int ch_in_bundle = 0;
            
            for (int j = 0; j < vi->channels; j++)
            {
                if (info->chmuxlist[j] == i)
                {
                    if (nonzero[j] != 0)
                    {
                        zerobundle[ch_in_bundle] = 1;
                    }
                    else
                    {
                        zerobundle[ch_in_bundle] = 0;
                    }
                    
                    pcmbundle[ch_in_bundle++] = vb->pcm[j];
                }
            }
            
            look->residue_func[i]->inverse(vb, look->residue_look[i], pcmbundle, zerobundle, ch_in_bundle);
        }
        
        for (int i = info->coupling_steps - 1; i >= 0; i--)
        {
            float pcmM = vb->pcm[info->coupling_mag[i]];
            float pcmA = vb->pcm[info->coupling_ang[i]];
            
            for (int j = 0; j < n / 2; j++)
            {
                float mag = pcmM[j];
                float ang = pcmA[j];
                
                if (mag > 0)
                {
                    if (ang > 0)
                    {
                        pcmM[j] = mag;
                        pcmA[j] = mag - ang;
                    }
                    else
                    {
                        pcmA[j] = mag;
                        pcmM[j] = mag + ang;
                    }
                }
                else
                {
                    if (ang > 0)
                    {
                        pcmM[j] = mag;
                        pcmA[j] = mag + ang;
                    }
                    else
                    {
                        pcmA[j] = mag;
                        pcmM[j] = mag - ang;
                    }
                }
            }
        }
        
        for (int i = 0; i < vi->channels; i++)
        {
            float pcm = vb->pcm[i];
            int submap = info->chmuxlist[i];
            look->floor_func[submap]->inverse2(vb, look->floor_look[submap], floormemo[i], pcm);
        }
        
        for (int i = 0; i < vi->channels; i++)
        {
            float pcm = vb->pcm[i];
            (safe_cast<OggSharp::Mdct^>(vd->transform[vb->W][0]))->backward(pcm, pcm);
        }
        
        for (int i = 0; i < vi->channels; i++)
        {
            float pcm = vb->pcm[i];
            
            if (nonzero[i] != 0)
            {
                for (int j = 0; j < n; j++)
                {
                    pcm[j] *= window[j];
                }
            }
            else
            {
                for (int j = 0; j < n; j++)
                {
                    pcm[j] = 0.0f;
                }
            }
        }
        
        return (0);
    }
    Monitor::Exit(this);
}

int Mapping0::ilog2(int v)
{
    int ret = 0;
    
    while (v > 1)
    {
        ret++;
        v = safe_cast<int>((safe_cast<unsigned int>(v) >> 1));
    }
    
    return (ret);
}

void InfoMapping0::free()
{
    chmuxlist = nullptr;
    timesubmap = nullptr;
    floorsubmap = nullptr;
    residuesubmap = nullptr;
    psysubmap = nullptr;
    coupling_mag = nullptr;
    coupling_ang = nullptr;
}


