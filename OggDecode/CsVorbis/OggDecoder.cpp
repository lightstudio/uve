#include "pch.h"
#include "OggDecoder.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace OggSharp;

const int OggDecoder::pageSize = 4096;

const int OggDecoder::CHUNKSIZE = 8500;

const int OggDecoder::SEEK_SET = 0;

const int OggDecoder::SEEK_CUR = 1;

const int OggDecoder::SEEK_END = 2;

const int OggDecoder::OV_FALSE = -1;

const int OggDecoder::OV_EOF = -2;

const int OggDecoder::OV_HOLE = -3;

const int OggDecoder::OV_EREAD = -128;

const int OggDecoder::OV_EFAULT = -129;

const int OggDecoder::OV_EIMPL = -130;

const int OggDecoder::OV_EINVAL = -131;

const int OggDecoder::OV_ENOTVORBIS = -132;

const int OggDecoder::OV_EBADHEADER = -133;

const int OggDecoder::OV_EVERSION = -134;

const int OggDecoder::OV_ENOTAUDIO = -135;

const int OggDecoder::OV_EBADPACKET = -136;

const int OggDecoder::OV_EBADLINK = -137;

const int OggDecoder::OV_ENOSEEK = -138;

OggSharp::Info^ OggDecoder::getInfo(int link)
{
    if (link < 0)
    {
        if (decode_ready)
        {
            return vis[current_link];
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        if (link >= links)
        {
            return nullptr;
        }
        else
        {
            return vis[link];
        }
    }
}

OggSharp::Comment^ OggDecoder::getComment(int link)
{
    if (link < 0)
    {
        if (decode_ready)
        {
            return vcs[current_link];
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        if (link >= links)
        {
            return nullptr;
        }
        else
        {
            return vcs[link];
        }
    }
}

int OggDecoder::make_decode_ready()
{
    vd->synthesis_init(vis[0]);
    vb->init(vd);
    decode_ready = true;
    return (0);
}

int OggDecoder::process_packet(int readp)
{
    OggSharp::Page^ og = ref new OggSharp::Page();
    
    while (true)
    {
        if (decode_ready)
        {
            OggSharp::Packet^ op = ref new OggSharp::Packet();
            int result = os->packetout(op);
            long granulepos;
            
            if (result > 0)
            {
                granulepos = op->granulepos;
                
                if (vb->synthesis(op) == 0)
                {
                
                {
                    int oldsamples = vd->synthesis_pcmout(nullptr, nullptr);
                    vd->synthesis_blockin(vb);
                    samptrack += vd->synthesis_pcmout(nullptr, nullptr) - oldsamples;
                    bittrack += op->bytes * 8;
                }
                    
                    if (granulepos != -1 && op->e_o_s == 0)
                    {
                        int link = current_link;
                        int samples;
                        samples = vd->synthesis_pcmout(nullptr, nullptr);
                        granulepos -= samples;
                        
                        for (int i = 0; i < link; i++)
                        {
                            granulepos += pcmlengths[i];
                        }
                        
                        pcm_offset = granulepos;
                    }
                    
                    return (1);
                }
            }
        }
        
        if (readp == 0)
            return (0);
        
        if (get_next_page(og, -1) < 0)
            return (0);
        bittrack += og->header_len * 8;
        
        if (decode_ready)
        {
            if (current_serialno != og->serialno())
            {
                decode_clear();
            }
        }
        
        if (!decode_ready)
        {
            int i;
            current_serialno = og->serialno();
            
            for (i = 0; i < links; i++)
            {
                if (serialnos[i] == current_serialno)
                    break;
            }
            
            if (i == links)
                return (-1);
            current_link = i;
            os->init(current_serialno);
            os->reset();
            make_decode_ready();
        }
        
        os->pagein(og);
    }
}

void OggDecoder::decode_clear()
{
    os->clear();
    vd->clear();
    vb->clear();
    decode_ready = false;
    bittrack = 0.0f;
    samptrack = 0.0f;
}

long OggDecoder::raw_total(int i)
{
    if (i >= links)
        return (-1);
    
    if (i < 0)
    {
        long acc = 0;
        
        for (int j = 0; j < links; j++)
        {
            acc += raw_total(j);
        }
        
        return (acc);
    }
    else
    {
        return (offsets[i + 1] - offsets[i]);
    }
}

long OggDecoder::pcm_total(int i)
{
    if (i >= links)
        return (-1);
    
    if (i < 0)
    {
        long acc = 0;
        
        for (int j = 0; j < links; j++)
        {
            acc += pcm_total(j);
        }
        
        return (acc);
    }
    else
    {
        return (pcmlengths[i]);
    }
}

float OggDecoder::time_total(int i)
{
    if (i >= links)
        return (-1);
    
    if (i < 0)
    {
        float acc = 0;
        
        for (int j = 0; j < links; j++)
        {
            acc += time_total(j);
        }
        
        return (acc);
    }
    else
    {
        return (safe_cast<float>((pcmlengths[i])) / vis[i]->rate);
    }
}

int OggDecoder::raw_seek(int pos)
{
    if (!seekable && pos != 0)
    {
        throw ref new InvalidOperationException("Cannot seek, the stream is not seekable");
    }
    
    if (pos < 0 || pos > offsets[links])
    {
        pcm_offset = -1;
        decode_clear();
        return -1;
    }
    
    pcm_offset = -1;
    decode_clear();
    seek_helper(pos);
    switch (process_packet(1))
    {
        case 0:
        
            pcm_offset = pcm_total(-1);
            return (0);
        
        case -1:
        
            pcm_offset = -1;
            decode_clear();
            return -1;
        
        default: 
        
            if (!seekable)
            {
                break;
            }
            
            break;
    }
    
    while (true)
    {
        switch (process_packet(0))
        {
            case 0:
            
                return (0);
            
            case -1:
            
                pcm_offset = -1;
                decode_clear();
                return -1;
            
            default: 
            
                if (!seekable)
                {
                    break;
                }
                
                break;
        }
    }
}

int OggDecoder::pcm_seek(long pos)
{
    int link = -1;
    long total = pcm_total(-1);
    
    if (pos < 0 || pos > total)
    {
        pcm_offset = -1;
        decode_clear();
        return -1;
    }
    
    for (link = links - 1; link >= 0; link--)
    {
        total -= pcmlengths[link];
        
        if (pos >= total)
            break;
    }

{
    long target = pos - total;
    long end = offsets[link + 1];
    long begin = offsets[link];
    int best = safe_cast<int>(begin);
    OggSharp::Page^ og = ref new OggSharp::Page();
    
    while (begin < end)
    {
        long bisect;
        int ret;
        
        if (end - begin < CHUNKSIZE)
        {
            bisect = begin;
        }
        else
        {
            bisect = (end + begin) / 2;
        }
        
        seek_helper(bisect);
        ret = get_next_page(og, end - bisect);
        
        if (ret == -1)
        {
            end = bisect;
        }
        else
        {
            long granulepos = og->granulepos();
            
            if (granulepos < target)
            {
                best = ret;
                begin = offset;
            }
            else
            {
                end = bisect;
            }
        }
    }
    
    if (raw_seek(best) != 0)
    {
        pcm_offset = -1;
        decode_clear();
        return -1;
    }
}
    
    if (pcm_offset >= pos)
    {
        pcm_offset = -1;
        decode_clear();
        return -1;
    }
    
    if (pos > pcm_total(-1))
    {
        pcm_offset = -1;
        decode_clear();
        return -1;
    }
    
    while (pcm_offset < pos)
    {
        float pcm;
        int target = safe_cast<int>((pos - pcm_offset));
        float _pcm = new float[1][][];
        int samples = vd->synthesis_pcmout(_pcm, _index);
        pcm = _pcm[0];
        
        if (samples > target)
            samples = target;
        vd->synthesis_read(samples);
        pcm_offset += samples;
        
        if (samples < target)
            if (process_packet(1) == 0)
            {
                pcm_offset = pcm_total(-1);
            }
    }
    
    return 0;
}

int OggDecoder::time_seek(float seconds)
{
    int link = -1;
    long pcm_tot = pcm_total(-1);
    float time_tot = time_total(-1);
    
    if (seconds < 0 || seconds > time_tot)
    {
        pcm_offset = -1;
        decode_clear();
        return -1;
    }
    
    for (link = links - 1; link >= 0; link--)
    {
        pcm_tot -= pcmlengths[link];
        time_tot -= time_total(link);
        
        if (seconds >= time_tot)
            break;
    }

{
    long target = safe_cast<long>((pcm_tot + (seconds - time_tot) * vis[link]->rate));
    return (pcm_seek(target));
}
}

long OggDecoder::raw_tell()
{
    return (offset);
}

long OggDecoder::pcm_tell()
{
    return (pcm_offset);
}

float OggDecoder::time_tell()
{
    int link = -1;
    long pcm_tot = 0;
    float time_tot = 0.0f;
    pcm_tot = pcm_total(-1);
    time_tot = time_total(-1);
    
    for (link = links - 1; link >= 0; link--)
    {
        pcm_tot -= pcmlengths[link];
        time_tot -= time_total(link);
        
        if (pcm_offset >= pcm_tot)
            break;
    }
    
    return (safe_cast<float>(time_tot) + safe_cast<float>((pcm_offset - pcm_tot)) / vis[link]->rate);
}

int OggDecoder::clear()
{
    vb->clear();
    vd->clear();
    os->clear();
    
    if (vis != nullptr && links != 0)
    {
        for (int i = 0; i < links; i++)
        {
            vis[i]->clear();
            vcs[i]->clear();
        }
        
        vis = nullptr;
        vcs = nullptr;
    }
    
    if (dataoffsets != nullptr)
        dataoffsets = nullptr;
    
    if (pcmlengths != nullptr)
        pcmlengths = nullptr;
    
    if (serialnos != nullptr)
        serialnos = nullptr;
    
    if (offsets != nullptr)
        offsets = nullptr;
    oy->clear();
    return (0);
}

int OggDecoder::open_seekable()
{
    OggSharp::Info^ initial_i = ref new OggSharp::Info();
    OggSharp::Comment^ initial_c = ref new OggSharp::Comment();
    int serialno;
    long end;
    int ret;
    int dataoffset;
    OggSharp::Page^ og = ref new OggSharp::Page();
    int foo = new int[1];
    ret = fetch_headers(initial_i, initial_c, foo, nullptr);
    serialno = foo[0];
    dataoffset = safe_cast<int>(offset);
    os->clear();
    
    if (ret == -1)
        return (-1);
    offset = input.Position = input.Length;
    end = offset;
    end = get_prev_page(og);
    
    if (og->serialno() != serialno)
    {
        if (bisect_forward_serialno(0, 0, end + 1, serialno, 0) < 0)
        {
            clear();
            return OV_EREAD;
        }
    }
    else
    {
        if (bisect_forward_serialno(0, end, end + 1, serialno, 0) < 0)
        {
            clear();
            return OV_EREAD;
        }
    }
    
    prefetch_all_headers(initial_i, initial_c, dataoffset);
    return (raw_seek(0));
}

int OggDecoder::fetch_headers(OggSharp::Info^ vi, OggSharp::Comment^ vc, int serialno, OggSharp::Page^ og_ptr)
{
    OggSharp::Page^ og = ref new OggSharp::Page();
    OggSharp::Packet^ op = ref new OggSharp::Packet();
    int ret;
    
    if (og_ptr == nullptr)
    {
        ret = get_next_page(og, CHUNKSIZE);
        
        if (ret == OV_EREAD)
            return OV_EREAD;
        
        if (ret < 0)
            return OV_ENOTVORBIS;
        og_ptr = og;
    }
    
    if (serialno != nullptr)
        serialno[0] = og_ptr->serialno();
    os->init(og_ptr->serialno());
    vi->init();
    vc->init();
    int i = 0;
    
    while (i < 3)
    {
        os->pagein(og_ptr);
        
        while (i < 3)
        {
            int result = os->packetout(op);
            
            if (result == 0)
                break;
            
            if (result == -1)
            {
                throw ref new Exception("Corrupt header in logical bitstream.");
                vi->clear();
                vc->clear();
                os->clear();
                return -1;
            }
            
            if (vi->synthesis_headerin(vc, op) != 0)
            {
                throw ref new Exception("Illegal header in logical bitstream.");
                vi->clear();
                vc->clear();
                os->clear();
                return -1;
            }
            
            i++;
        }
        
        if (i < 3)
            if (get_next_page(og_ptr, 1) < 0)
            {
                throw ref new Exception("Missing header in logical bitstream.");
                vi->clear();
                vc->clear();
                os->clear();
                return -1;
            }
    }
    
    return 0;
}

void OggDecoder::prefetch_all_headers(OggSharp::Info^ first_i, OggSharp::Comment^ first_c, int dataoffset)
{
    OggSharp::Page^ og = ref new OggSharp::Page();
    int ret;
    vis = new Info[links];
    vcs = new Comment[links];
    dataoffsets = new long[links];
    pcmlengths = new long[links];
    serialnos = new int[links];
    
    for (int i = 0; i < links; i++)
    {
        if (first_i != nullptr && first_c != nullptr && i == 0)
        {
            vis[i] = first_i;
            vcs[i] = first_c;
            dataoffsets[i] = dataoffset;
        }
        else
        {
            seek_helper(offsets[i]);
            
            if (fetch_headers(vis[i], vcs[i], nullptr, nullptr) == -1)
            {
                throw ref new Exception("Error opening logical bitstream #" + (i + 1) + "\n");
                dataoffsets[i] = -1;
            }
            else
            {
                dataoffsets[i] = offset;
                os->clear();
            }
        }
        
        long end = offsets[i + 1];
        seek_helper(end);
        
        while (true)
        {
            ret = get_prev_page(og);
            
            if (ret == -1)
            {
                throw ref new Exception("Could not find last page of logical " + "bitstream #" + safe_cast<i>(+"\n"));
                vis[i]->clear();
                vcs[i]->clear();
                break;
            }
            
            if (og->granulepos() != -1)
            {
                serialnos[i] = og->serialno();
                pcmlengths[i] = og->granulepos();
                break;
            }
        }
    }
}

void OggDecoder::seek_helper(long offset)
{
    input.Position = offset;
    this->offset = offset;
    oy->reset();
}

int OggDecoder::get_data()
{
    int index = oy->buffer(CHUNKSIZE);
    unsigned char buffer = oy->data;
    int bytes = 0;
    try
    {
        bytes = input.Read(buffer, index, CHUNKSIZE);
    }
    catch (Exception e)
    {
        return OV_EREAD;
    }
    
    oy->wrote(bytes);
    
    if (bytes == -1)
    {
        bytes = 0;
    }
    
    return bytes;
}

int OggDecoder::get_next_page(OggSharp::Page^ page, long boundary)
{
    if (boundary > 0)
        boundary += offset;
    
    while (true)
    {
        int more;
        
        if (boundary > 0 && offset >= boundary)
            return OV_FALSE;
        more = oy->pageseek(page);
        
        if (more < 0)
        {
            offset -= more;
        }
        else
        {
            if (more == 0)
            {
                if (boundary == 0)
                    return OV_FALSE;
                int ret = get_data();
                
                if (ret == 0)
                    return OV_EOF;
                
                if (ret < 0)
                    return OV_EREAD;
            }
            else
            {
                int ret = safe_cast<int>(offset);
                offset += more;
                return ret;
            }
        }
    }
}

int OggDecoder::get_prev_page(OggSharp::Page^ page)
{
    long begin = offset;
    int ret;
    int offst = -1;
    
    while (offst == -1)
    {
        begin -= CHUNKSIZE;
        
        if (begin < 0)
            begin = 0;
        seek_helper(begin);
        
        while (offset < begin + CHUNKSIZE)
        {
            ret = get_next_page(page, begin + CHUNKSIZE - offset);
            
            if (ret == OV_EREAD)
            {
                return OV_EREAD;
            }
            
            if (ret < 0)
            {
                break;
            }
            else
            {
                offst = ret;
            }
        }
    }
    
    seek_helper(offst);
    ret = get_next_page(page, CHUNKSIZE);
    
    if (ret < 0)
    {
        return OV_EFAULT;
    }
    
    return offst;
}

int OggDecoder::bisect_forward_serialno(long begin, long searched, long end, int currentno, int m)
{
    long endsearched = end;
    long next = end;
    OggSharp::Page^ page = ref new OggSharp::Page();
    int ret;
    
    while (searched < endsearched)
    {
        long bisect;
        
        if (endsearched - searched < CHUNKSIZE)
        {
            bisect = searched;
        }
        else
        {
            bisect = (searched + endsearched) / 2;
        }
        
        seek_helper(bisect);
        ret = get_next_page(page, -1);
        
        if (ret == OV_EREAD)
            return OV_EREAD;
        
        if (ret < 0 || page->serialno() != currentno)
        {
            endsearched = bisect;
            
            if (ret >= 0)
                next = ret;
        }
        else
        {
            searched = ret + page->header_len + page->body_len;
        }
    }
    
    seek_helper(next);
    ret = get_next_page(page, -1);
    
    if (ret == OV_EREAD)
        return OV_EREAD;
    
    if (searched >= end || ret == -1)
    {
        links = m + 1;
        offsets = new long[m + 2];
        offsets[m + 1] = searched;
    }
    else
    {
        ret = bisect_forward_serialno(next, offset, end, page->serialno(), m + 1);
        
        if (ret == OV_EREAD)
            return OV_EREAD;
    }
    
    offsets[m] = begin;
    return 0;
}

int OggDecoder::ReadNextPage()
{
    int index = oy->buffer(pageSize);
    int bytes = 0;
    unsigned char buffer = oy->data;
    try
    {
        bytes = input.Read(buffer, index, pageSize);
    }
    catch ()
    {
        throw ;
    }
    
    oy->wrote(bytes);
    
    if (bytes == 0)
        eos = 1;
    return bytes;
}

void OggDecoder::ReadHeader()
{
    oy = ref new OggSharp::SyncState();
    oy->init();
    os = ref new OggSharp::StreamState();
    op = ref new OggSharp::Packet();
    vi = ref new OggSharp::Info();
    vc = ref new OggSharp::Comment();
    vd = ref new OggSharp::DspState();
    vb = ref new OggSharp::Block(vd);
    input.Position = 0;
    open_seekable();
    vi = vis[0];
    vc = vcs[0];
    SampleRate = vi->rate;
    Stereo = (vi->channels > 1);
    Length = this->time_total(-1);
    _index = new int[vi->channels];
    convsize = 4096 / vi->channels;
    Reset();
}

void OggDecoder::ConvertChunk(int bout, int bytesRead, float pcm)
{
    Platform::bool clipflag = false;
    
    for (int i = 0; 