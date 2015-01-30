#include "pch.h"
#include "Block.h"

using namespace System;
using namespace OggSharp;

Block::Block(OggSharp::DspState^ vd)
{
    this->vd = vd;
    
    if (vd->analysisp != 0)
    {
        opb->writeinit();
    }
}

void Block::init(OggSharp::DspState^ vd)
{
    this->vd = vd;
}

int Block::clear()
{
    if (vd != nullptr)
    {
        if (vd->analysisp != 0)
        {
            opb->writeclear();
        }
    }
    
    return (0);
}

int Block::synthesis(OggSharp::Packet^ op)
{
    OggSharp::Info^ vi = vd->vi;
    opb->readinit(op->packet_base, op->packet, op->bytes);
    
    if (opb->read(1) != 0)
    {
        return (-1);
    }
    
    int _mode = opb->read(vd->modebits);
    
    if (_mode == -1)
        return (-1);
    mode = _mode;
    W = vi->mode_param[mode]->blockflag;
    
    if (W != 0)
    {
        lW = opb->read(1);
        nW = opb->read(1);
        
        if (nW == -1)
            return (-1);
    }
    else
    {
        lW = 0;
        nW = 0;
    }
    
    granulepos = op->granulepos;
    sequence = op->packetno - 3;
    eofflag = op->e_o_s;
    pcmend = vi->blocksizes[W];
    
    if (pcm.Length < vi->channels)
    {
        pcm = new float[vi->channels][];
    }
    
    for (int i = 0; i < vi->channels; i++)
    {
        if (pcm[i] == nullptr || pcm[i].Length < pcmend)
        {
            pcm[i] = new float[pcmend];
        }
        else
        {
            for (int j = 0; j < pcmend; j++)
            {
                pcm[i][j] = 0;
            }
        }
    }
    
    int type = vi->map_type[vi->mode_param[mode]->mapping];
    return (FuncMapping::mapping_P[type]->inverse(this, vd->mode[mode]));
}


