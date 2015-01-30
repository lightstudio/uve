#include "pch.h"
#include "Info.h"

using namespace System;
using namespace System::Text;
using namespace OggSharp;

int Info::OV_EBADPACKET = -136;

int Info::OV_ENOTAUDIO = -135;

Platform::String^ Info::_vorbis = "vorbis";

int Info::VI_TIMEB = 1;

int Info::VI_FLOORB = 2;

int Info::VI_RESB = 3;

int Info::VI_MAPB = 1;

int Info::VI_WINDOWB = 1;

void Info::init()
{
    rate = 0;
}

void Info::clear()
{
    for (int i = 0; i < modes; i++)
    {
        mode_param[i] = nullptr;
    }
    
    mode_param = nullptr;
    
    for (int i = 0; i < maps; i++)
    {
        FuncMapping::mapping_P[map_type[i]]->free_info(map_param[i]);
    }
    
    map_param = nullptr;
    
    for (int i = 0; i < times; i++)
    {
        FuncTime::time_P[time_type[i]]->free_info(time_param[i]);
    }
    
    time_param = nullptr;
    
    for (int i = 0; i < floors; i++)
    {
        FuncFloor::floor_P[floor_type[i]]->free_info(floor_param[i]);
    }
    
    floor_param = nullptr;
    
    for (int i = 0; i < residues; i++)
    {
        FuncResidue::residue_P[residue_type[i]]->free_info(residue_param[i]);
    }
    
    residue_param = nullptr;
    
    for (int i = 0; i < books; i++)
    {
        if (book_param[i] != nullptr)
        {
            book_param[i]->clear();
            book_param[i] = nullptr;
        }
    }
    
    book_param = nullptr;
    
    for (int i = 0; i < psys; i++)
    {
        psy_param[i]->free();
    }
}

int Info::unpack_info(OggSharp::csBuffer^ opb)
{
    version = opb->read(32);
    
    if (version != 0)
        return (-1);
    channels = opb->read(8);
    rate = opb->read(32);
    bitrate_upper = opb->read(32);
    bitrate_nominal = opb->read(32);
    bitrate_lower = opb->read(32);
    blocksizes[0] = 1 << opb->read(4);
    blocksizes[1] = 1 << opb->read(4);
    
    if ((rate < 1) || (channels < 1) || (blocksizes[0] < 8) || (blocksizes[1] < blocksizes[0]) || (opb->read(1) != 1))
    {
        clear();
        return (-1);
    }
    
    return (0);
}

int Info::unpack_books(OggSharp::csBuffer^ opb)
{
    books = opb->read(8) + 1;
    
    if (book_param == nullptr || book_param->Length != books)
        book_param = new StaticCodeBook[books];
    
    for (int i = 0; i < books; i++)
    {
        book_param[i] = ref new OggSharp::StaticCodeBook();
        
        if (book_param[i]->unpack(opb) != 0)
        {
            clear();
            return (-1);
        }
    }
    
    times = opb->read(6) + 1;
    
    if (time_type == nullptr || time_type.Length != times)
        time_type = new int[times];
    
    if (time_param == nullptr || time_param.Length != times)
        time_param = new Object[times];
    
    for (int i = 0; i < times; i++)
    {
        time_type[i] = opb->read(16);
        
        if (time_type[i] < 0 || time_type[i] >= VI_TIMEB)
        {
            clear();
            return (-1);
        }
        
        time_param[i] = FuncTime::time_P[time_type[i]]->unpack(this, opb);
        
        if (time_param[i] == nullptr)
        {
            clear();
            return (-1);
        }
    }
    
    floors = opb->read(6) + 1;
    
    if (floor_type == nullptr || floor_type.Length != floors)
        floor_type = new int[floors];
    
    if (floor_param == nullptr || floor_param.Length != floors)
        floor_param = new Object[floors];
    
    for (int i = 0; i < floors; i++)
    {
        floor_type[i] = opb->read(16);
        
        if (floor_type[i] < 0 || floor_type[i] >= VI_FLOORB)
        {
            clear();
            return (-1);
        }
        
        floor_param[i] = FuncFloor::floor_P[floor_type[i]]->unpack(this, opb);
        
        if (floor_param[i] == nullptr)
        {
            clear();
            return (-1);
        }
    }
    
    residues = opb->read(6) + 1;
    
    if (residue_type == nullptr || residue_type.Length != residues)
        residue_type = new int[residues];
    
    if (residue_param == nullptr || residue_param.Length != residues)
        residue_param = new Object[residues];
    
    for (int i = 0; i < residues; i++)
    {
        residue_type[i] = opb->read(16);
        
        if (residue_type[i] < 0 || residue_type[i] >= VI_RESB)
        {
            clear();
            return (-1);
        }
        
        residue_param[i] = FuncResidue::residue_P[residue_type[i]]->unpack(this, opb);
        
        if (residue_param[i] == nullptr)
        {
            clear();
            return (-1);
        }
    }
    
    maps = opb->read(6) + 1;
    
    if (map_type == nullptr || map_type.Length != maps)
        map_type = new int[maps];
    
    if (map_param == nullptr || map_param.Length != maps)
        map_param = new Object[maps];
    
    for (int i = 0; i < maps; i++)
    {
        map_type[i] = opb->read(16);
        
        if (map_type[i] < 0 || map_type[i] >= VI_MAPB)
        {
            clear();
            return (-1);
        }
        
        map_param[i] = FuncMapping::mapping_P[map_type[i]]->unpack(this, opb);
        
        if (map_param[i] == nullptr)
        {
            clear();
            return (-1);
        }
    }
    
    modes = opb->read(6) + 1;
    
    if (mode_param == nullptr || mode_param->Length != modes)
        mode_param = new InfoMode[modes];
    
    for (int i = 0; i < modes; i++)
    {
        mode_param[i] = ref new OggSharp::InfoMode();
        mode_param[i]->blockflag = opb->read(1);
        mode_param[i]->windowtype = opb->read(16);
        mode_param[i]->transformtype = opb->read(16);
        mode_param[i]->mapping = opb->read(8);
        
        if ((mode_param[i]->windowtype >= VI_WINDOWB) || (mode_param[i]->transformtype >= VI_WINDOWB) || (mode_param[i]->mapping >= maps))
        {
            clear();
            return (-1);
        }
    }
    
    if (opb->read(1) != 1)
    {
        clear();
        return (-1);
    }
    
    return (0);
}

int Info::synthesis_headerin(OggSharp::Comment^ vc, OggSharp::Packet^ op)
{
    OggSharp::csBuffer^ opb = ref new OggSharp::csBuffer();
    
    if (op != nullptr)
    {
        opb->readinit(op->packet_base, op->packet, op->bytes);
    
    {
        unsigned char buffer = new byte[6];
        int packtype = opb::read(8);
        opb::read(buffer, 6);
        
        if (buffer[0] != 'v' || buffer[1] != 'o' || buffer[2] != 'r' || buffer[3] != 'b' || buffer[4] != 'i' || buffer[5] != 's')
        {
            return (-1);
        }
        
        switch (packtype)
        {
            case 0x01:
            
                if (op->b_o_s == 0)
                {
                    return (-1);
                }
                
                if (rate != 0)
                {
                    return (-1);
                }
                
                return (unpack_info(opb));
            
            case 0x03:
            
                if (rate == 0)
                {
                    return (-1);
                }
                
                return (vc->unpack(opb));
            
            case 0x05:
            
                if (rate == 0 || vc->vendor == nullptr)
                {
                    return (-1);
                }
                
                return (unpack_books(opb));
            
            default: 
            
                break;
        }
    }
    }
    
    return (-1);
}

int Info::pack_info(OggSharp::csBuffer^ opb)
{
    Encoding AE = Encoding::UTF8;
    unsigned char _vorbis_byt = AE::GetBytes(_vorbis);
    opb->write(0x01, 8);
    opb->write(_vorbis_byt);
    opb->write(0x00, 32);
    opb->write(channels, 8);
    opb->write(rate, 32);
    opb->write(bitrate_upper, 32);
    opb->write(bitrate_nominal, 32);
    opb->write(bitrate_lower, 32);
    opb->write(ilog2(blocksizes[0]), 4);
    opb->write(ilog2(blocksizes[1]), 4);
    opb->write(1, 1);
    return (0);
}

int Info::pack_books(OggSharp::csBuffer^ opb)
{
    Encoding AE = Encoding::UTF8;
    unsigned char _vorbis_byt = AE::GetBytes(_vorbis);
    opb->write(0x05, 8);
    opb->write(_vorbis_byt);
    opb->write(books - 1, 8);
    
    for (int i = 0; i < books; i++)
    {
        if (book_param[i]->pack(opb) != 0)
        {
            return (-1);
        }
    }
    
    opb->write(times - 1, 6);
    
    for (int i = 0; i < times; i++)
    {
        opb->write(time_type[i], 16);
        FuncTime::time_P[time_type[i]]->pack(this->time_param[i], opb);
    }
    
    opb->write(floors - 1, 6);
    
    for (int i = 0; i < floors; i++)
    {
        opb->write(floor_type[i], 16);
        FuncFloor::floor_P[floor_type[i]]->pack(floor_param[i], opb);
    }
    
    opb->write(residues - 1, 6);
    
    for (int i = 0; i < residues; i++)
    {
        opb->write(residue_type[i], 16);
        FuncResidue::residue_P[residue_type[i]]->pack(residue_param[i], opb);
    }
    
    opb->write(maps - 1, 6);
    
    for (int i = 0; i < maps; i++)
    {
        opb->write(map_type[i], 16);
        FuncMapping::mapping_P[map_type[i]]->pack(this, map_param[i], opb);
    }
    
    opb->write(modes - 1, 6);
    
    for (int i = 0; i < modes; i++)
    {
        opb->write(mode_param[i]->blockflag, 1);
        opb->write(mode_param[i]->windowtype, 16);
        opb->write(mode_param[i]->transformtype, 16);
        opb->write(mode_param[i]->mapping, 8);
    }
    
    opb->write(1, 1);
    return (0);
}

int Info::blocksize(OggSharp::Packet^ op)
{
    OggSharp::csBuffer^ opb = ref new OggSharp::csBuffer();
    int mode;
    opb->readinit(op->packet_base, op->packet, op->bytes);
    
    if (opb->read(1) != 0)
    {
        return (OV_ENOTAUDIO);
    }

{
    int modebits = 0;
    int v = modes;
    
    while (v > 1)
    {
        modebits++;
        v = safe_cast<int>((safe_cast<unsigned int>(v) >> 1));
    }
    
    mode = opb::read(modebits);
}
    
    if (mode == -1)
        return (OV_EBADPACKET);
    return (blocksizes[mode_param[mode]->blockflag]);
}

int Info::ilog2(int v)
{
    int ret = 0;
    
    while (v > 1)
    {
        ret++;
        v = safe_cast<int>((safe_cast<unsigned int>(v) >> 1));
    }
    
    return (ret);
}

Platform::String Info::toString()
{
    return "version:" + version.ToString() + ", channels:" + channels.ToString() + ", rate:" + rate.ToString() + ", bitrate:" + bitrate_upper.ToString() + "," + bitrate_nominal.ToString() + "," + bitrate_lower.ToString();
}


