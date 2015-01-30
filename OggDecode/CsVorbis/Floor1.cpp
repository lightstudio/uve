#include "pch.h"
#include "Floor1.h"

using namespace System;
using namespace OggSharp;

int Floor1::floor1_rangedb = 140;

int Floor1::VIF_POSIT = 63;

void Floor1::pack(Platform::Object i, OggSharp::csBuffer^ opb)
{
    OggSharp::InfoFloor1^ info = safe_cast<OggSharp::InfoFloor1^>(i);
    int count = 0;
    int rangebits;
    int maxposit = info->postlist[1];
    int maxclass = -1;
    opb->write(info->partitions, 5);
    
    for (int j = 0; j < info->partitions; j++)
    {
        opb->write(info->partitionclass[j], 4);
        
        if (maxclass < info->partitionclass[j])
            maxclass = info::partitionclass[j];
    }
    
    for (int j = 0; j < maxclass + 1; j++)
    {
        opb->write(info->class_dim[j] - 1, 3);
        opb->write(info->class_subs[j], 2);
        
        if (info->class_subs[j] != 0)
        {
            opb->write(info->class_book[j], 8);
        }
        
        for (int k = 0; k < (1 << info->class_subs[j]); k++)
        {
            opb->write(info->class_subbook[j][k] + 1, 8);
        }
    }
    
    opb->write(info->mult - 1, 2);
    opb->write(ilog2(maxposit), 4);
    rangebits = ilog2(maxposit);
    
    for (int j = 0, k = 0; j < info->partitions; j++)
    {
        count += info->class_dim[info->partitionclass[j]];
        
        for (; k < count; k++)
        {
            opb->write(info->postlist[k + 2], rangebits);
        }
    }
}

Platform::Object Floor1::unpack(OggSharp::Info^ vi, OggSharp::csBuffer^ opb)
{
    int count = 0, maxclass = -1, rangebits;
    OggSharp::InfoFloor1^ info = ref new OggSharp::InfoFloor1();
    info->partitions = opb->read(5);
    
    for (int j = 0; j < info->partitions; j++)
    {
        info->partitionclass[j] = opb->read(4);
        
        if (maxclass < info->partitionclass[j])
            maxclass = info::partitionclass[j];
    }
    
    for (int j = 0; j < maxclass + 1; j++)
    {
        info->class_dim[j] = opb->read(3) + 1;
        info->class_subs[j] = opb->read(2);
        
        if (info->class_subs[j] < 0)
        {
            info->free();
            return (nullptr);
        }
        
        if (info->class_subs[j] != 0)
        {
            info->class_book[j] = opb->read(8);
        }
        
        if (info->class_book[j] < 0 || info->class_book[j] >= vi->books)
        {
            info->free();
            return (nullptr);
        }
        
        for (int k = 0; k < (1 << info->class_subs[j]); k++)
        {
            info->class_subbook[j][k] = opb->read(8) - 1;
            
            if (info->class_subbook[j][k] < -1 || info->class_subbook[j][k] >= vi->books)
            {
                info->free();
                return (nullptr);
            }
        }
    }
    
    info->mult = opb->read(2) + 1;
    rangebits = opb->read(4);
    
    for (int j = 0, k = 0; j < info->partitions; j++)
    {
        count += info->class_dim[info->partitionclass[j]];
        
        for (; k < count; k++)
        {
            int t = info->postlist[k + 2] = opb->read(rangebits);
            
            if (t < 0 || t >= (1 << rangebits))
            {
                info->free();
                return (nullptr);
            }
        }
    }
    
    info->postlist[0] = 0;
    info->postlist[1] = 1 << rangebits;
    return (info);
}

Platform::Object Floor1::look(OggSharp::DspState^ vd, OggSharp::InfoMode^ mi, Platform::Object i)
{
    int _n = 0;
    int sortpointer = new int[VIF_POSIT + 2];
    OggSharp::InfoFloor1^ info = safe_cast<OggSharp::InfoFloor1^>(i);
    OggSharp::LookFloor1^ look = ref new OggSharp::LookFloor1();
    look->vi = info;
    look->n = info->postlist[1];
    
    for (int j = 0; j < info->partitions; j++)
    {
        _n += info->class_dim[info->partitionclass[j]];
    }
    
    _n += 2;
    look->posts = _n;
    
    for (int j = 0; j < _n; j++)
    {
        sortpointer[j] = j;
    }
    
    int foo;
    
    for (int j = 0; j < _n - 1; j++)
    {
        for (int k = j; k < _n; k++)
        {
            if (info->postlist[sortpointer[j]] > info->postlist[sortpointer[k]])
            {
                foo = sortpointer[k];
                sortpointer[k] = sortpointer[j];
                sortpointer[j] = foo;
            }
        }
    }
    
    for (int j = 0; j < _n; j++)
    {
        look->forward_index[j] = sortpointer[j];
    }
    
    for (int j = 0; j < _n; j++)
    {
        look->reverse_index[look->forward_index[j]] = j;
    }
    
    for (int j = 0; j < _n; j++)
    {
        look->sorted_index[j] = info->postlist[look->forward_index[j]];
    }
    
    switch (info->mult)
    {
        case 1:
        
            look->quant_q = 256;
            break;
        
        case 2:
        
            look->quant_q = 128;
            break;
        
        case 3:
        
            look->quant_q = 86;
            break;
        
        case 4:
        
            look->quant_q = 64;
            break;
        
        default: 
        
            look->quant_q = -1;
            break;
    }
    
    for (int j = 0; j < _n - 2; j++)
    {
        int lo = 0;
        int hi = 1;
        int lx = 0;
        int hx = look->n;
        int currentx = info->postlist[j + 2];
        
        for (int k = 0; k < j + 2; k++)
        {
            int x = info->postlist[k];
            
            if (x > lx && x < currentx)
            {
                lo = k;
                lx = x;
            }
            
            if (x < hx && x > currentx)
            {
                hi = k;
                hx = x;
            }
        }
        
        look->loneighbor[j] = lo;
        look->hineighbor[j] = hi;
    }
    
    return look;
}

void Floor1::free_info(Platform::Object i)
{
}

void Floor1::free_look(Platform::Object i)
{
}

void Floor1::free_state(Platform::Object vs)
{
}

int Floor1::forward(OggSharp::Block^ vb, Platform::Object i, float fin, float fout, Platform::Object vs)
{
    return 0;
}

Platform::Object Floor1::inverse1(OggSharp::Block^ vb, Platform::Object ii, Platform::Object memo)
{
    OggSharp::LookFloor1^ look = safe_cast<OggSharp::LookFloor1^>(ii);
    OggSharp::InfoFloor1^ info = look->vi;
    OggSharp::CodeBook^ books = vb->vd->fullbooks;
    
    if (vb->opb->read(1) == 1)
    {
        int fit_value = nullptr;
        
        if (dynamic_cast<int>(memo) != nullptr)
        {
            fit_value = safe_cast<int>(memo);
        }
        
        if (fit_value == nullptr || fit_value::Length < look->posts)
        {
            fit_value = new int[look->posts];
        }
        else
        {
            for (int i = 0; i < fit_value::Length; i++)
                fit_value[i] = 0;
        }
        
        fit_value[0] = vb->opb->read(ilog(look->quant_q - 1));
        fit_value[1] = vb->opb->read(ilog(look->quant_q - 1));
        
        for (int i = 0, j = 2; i < info->partitions; i++)
        {
            int clss = info->partitionclass[i];
            int cdim = info->class_dim[clss];
            int csubbits = info->class_subs[clss];
            int csub = 1 << csubbits;
            int cval = 0;
            
            if (csubbits != 0)
            {
                cval = books[info->class_book[clss]]->decode(vb->opb);
                
                if (cval == -1)
                {
                    return (nullptr);
                }
            }
            
            for (int k = 0; k < cdim; k++)
            {
                int book = info->class_subbook[clss][cval & (csub - 1)];
                cval = safe_cast<int>((safe_cast<unsigned int>(cval) >> csubbits));
                
                if (book >= 0)
                {
                    if ((fit_value[j + k] = books[book]->decode(vb->opb)) == -1)
                    {
                        return (nullptr);
                    }
                }
                else
                {
                    fit_value[j + k] = 0;
                }
            }
            
            j += cdim;
        }
        
        for (int i = 2; i < look->posts; i++)
        {
            int predicted = render_point(info->postlist[look->loneighbor[i - 2]], info->postlist[look->hineighbor[i - 2]], fit_value[look->loneighbor[i - 2]], fit_value[look->hineighbor[i - 2]], info->postlist[i]);
            int hiroom = look->quant_q - predicted;
            int loroom = predicted;
            int room = ((hiroom < loroom) ? hiroom : loroom) << 1;
            int val = fit_value[i];
            
            if (val != 0)
            {
                if (val >= room)
                {
                    if (hiroom > loroom)
                    {
                        val = val - loroom;
                    }
                    else
                    {
                        val = -1 - (val - hiroom);
                    }
                }
                else
                {
                    if ((val & 1) != 0)
                    {
                        val = safe_cast<int>((-(safe_cast<unsigned int>((val + 1)) >> 1)));
                    }
                    else
                    {
                        val >>= 1;
                    }
                }
                
                fit_value[i] = val + predicted;
                fit_value[look->loneighbor[i - 2]] &= 0x7fff;
                fit_value[look->hineighbor[i - 2]] &= 0x7fff;
            }
            else
            {
                fit_value[i] = predicted | 0x8000;
            }
        }
        
        return (fit_value);
    }
    
    return (nullptr);
}

int Floor1::render_point(int x0, int x1, int y0, int y1, int x)
{
    y0 &= 0x7fff;
    y1 &= 0x7fff;

{
    int dy = y1 - y0;
    int adx = x1 - x0;
    int ady = Math::Abs(dy);
    int err = ady * (x - x0);
    int off = safe_cast<int>((err / adx));
    
    if (dy < 0)
        return (y0 - off);
    return (y0 + off);
}
}

int Floor1::inverse2(OggSharp::Block^ vb, Platform::Object i, Platform::Object memo, float fout)
{
    OggSharp::LookFloor1^ look = safe_cast<OggSharp::LookFloor1^>(i);
    OggSharp::InfoFloor1^ info = look->vi;
    int n = vb->vd->vi->blocksizes[vb->mode] / 2;
    
    if (memo != nullptr)
    {
        int fit_value = safe_cast<int>(memo);
        int hx = 0;
        int lx = 0;
        int ly = fit_value[0] * info->mult;
        
        for (int j = 1; j < look->posts; j++)
        {
            int current = look->forward_index[j];
            int hy = fit_value[current] & 0x7fff;
            
            if (hy == fit_value[current])
            {
                hy *= info->mult;
                hx = info->postlist[current];
                render_line(lx, hx, ly, hy, fout);
                lx = hx;
                ly = hy;
            }
        }
        
        for (int j = hx; j < n; j++)
        {
            fout[j] *= fout[j - 1];
        }
        
        return (1);
    }
    
    for (int j = 0; j < n; j++)
    {
        fout[j] = 0.0f;
    }
    
    return (0);
}

float Floor1::FLOOR_fromdB_LOOKUP = 
{
    1.0649863e-07F, 1.1341951e-07F, 1.2079015e-07F, 1.2863978e-07F, 1.3699951e-07F, 1.4590251e-07F, 1.5538408e-07F, 1.6548181e-07F, 1.7623575e-07F, 1.8768855e-07F, 1.9988561e-07F, 2.128753e-07F, 2.2670913e-07F, 2.4144197e-07F, 2.5713223e-07F, 2.7384213e-07F, 2.9163793e-07F, 3.1059021e-07F, 3.3077411e-07F, 3.5226968e-07F, 3.7516214e-07F, 3.9954229e-07F, 4.2550680e-07F, 4.5315863e-07F, 4.8260743e-07F, 5.1396998e-07F, 5.4737065e-07F, 5.8294187e-07F, 6.2082472e-07F, 6.6116941e-07F, 7.0413592e-07F, 7.4989464e-07F, 7.9862701e-07F, 8.5052630e-07F, 9.0579828e-07F, 9.6466216e-07F, 1.0273513e-06F, 1.0941144e-06F, 1.1652161e-06F, 1.2409384e-06F, 1.3215816e-06F, 1.4074654e-06F, 1.4989305e-06F, 1.5963394e-06F, 1.7000785e-06F, 1.8105592e-06F, 1.9282195e-06F, 2.0535261e-06F, 2.1869758e-06F, 2.3290978e-06F, 2.4804557e-06F, 2.6416497e-06F, 2.8133190e-06F, 2.9961443e-06F, 3.1908506e-06F, 3.3982101e-06F, 3.6190449e-06F, 3.8542308e-06F, 4.1047004e-06F, 4.3714470e-06F, 4.6555282e-06F, 4.9580707e-06F, 5.2802740e-06F, 5.6234160e-06F, 5.9888572e-06F, 6.3780469e-06F, 6.7925283e-06F, 7.2339451e-06F, 7.7040476e-06F, 8.2047000e-06F, 8.7378876e-06F, 9.3057248e-06F, 9.9104632e-06F, 1.0554501e-05F, 1.1240392e-05F, 1.1970856e-05F, 1.2748789e-05F, 1.3577278e-05F, 1.4459606e-05F, 1.5399272e-05F, 1.6400004e-05F, 1.7465768e-05F, 1.8600792e-05F, 1.9809576e-05F, 2.1096914e-05F, 2.2467911e-05F, 2.3928002e-05F, 2.5482978e-05F, 2.7139006e-05F, 2.8902651e-05F, 3.0780908e-05F, 3.2781225e-05F, 3.4911534e-05F, 3.7180282e-05F, 3.9596466e-05F, 4.2169667e-05F, 4.4910090e-05F, 4.7828601e-05F, 5.0936773e-05F, 5.4246931e-05F, 5.7772202e-05F, 6.1526565e-05F, 6.5524908e-05F, 6.9783085e-05F, 7.4317983e-05F, 7.9147585e-05F, 8.4291040e-05F, 8.9768747e-05F, 9.5602426e-05F, 0.00010181521F, 0.00010843174F, 0.00011547824F, 0.00012298267F, 0.00013097477F, 0.00013948625F, 0.00014855085F, 0.00015820453F, 0.00016848555F, 0.00017943469F, 0.00019109536F, 0.00020351382F, 0.00021673929F, 0.00023082423F, 0.00024582449F, 0.00026179955F, 0.00027881276F, 0.00029693158F, 0.00031622787F, 0.00033677814F, 0.00035866388F, 0.00038197188F, 0.00040679456F, 0.00043323036F, 0.00046138411F, 0.00049136745F, 0.00052329927F, 0.00055730621F, 0.00059352311F, 0.00063209358F, 0.00067317058F, 0.00071691700F, 0.00076350630F, 0.00081312324F, 0.00086596457F, 0.00092223983F, 0.00098217216F, 0.0010459992F, 0.0011139742F, 0.0011863665F, 0.0012634633F, 0.0013455702F, 0.0014330129F, 0.0015261382F, 0.0016253153F, 0.0017309374F, 0.0018434235F, 0.0019632195F, 0.0020908006F, 0.0022266726F, 0.0023713743F, 0.0025254795F, 0.0026895994F, 0.0028643847F, 0.0030505286F, 0.0032487691F, 0.0034598925F, 0.0036847358F, 0.0039241906F, 0.0041792066F, 0.0044507950F, 0.0047400328F, 0.0050480668F, 0.0053761186F, 0.0057254891F, 0.0060975636F, 0.0064938176F, 0.0069158225F, 0.0073652516F, 0.0078438871F, 0.0083536271F, 0.0088964928F, 0.009474637F, 0.010090352F, 0.010746080F, 0.011444421F, 0.012188144F, 0.012980198F, 0.013823725F, 0.014722068F, 0.015678791F, 0.016697687F, 0.017782797F, 0.018938423F, 0.020169149F, 0.021479854F, 0.022875735F, 0.024362330F, 0.025945531F, 0.027631618F, 0.029427276F, 0.031339626F, 0.033376252F, 0.035545228F, 0.037855157F, 0.040315199F, 0.042935108F, 0.045725273F, 0.048696758F, 0.051861348F, 0.055231591F, 0.058820850F, 0.062643361F, 0.066714279F, 0.071049749F, 0.075666962F, 0.080584227F, 0.085821044F, 0.091398179F, 0.097337747F, 0.10366330F, 0.11039993F, 0.11757434F, 0.12521498F, 0.13335215F, 0.14201813F, 0.15124727F, 0.16107617F, 0.17154380F, 0.18269168F, 0.19456402F, 0.20720788F, 0.22067342F, 0.23501402F, 0.25028656F, 0.26655159F, 0.28387361F, 0.30232132F, 0.32196786F, 0.34289114F, 0.36517414F, 0.38890521F, 0.41417847F, 0.44109412F, 0.46975890F, 0.50028648F, 0.53279791F, 0.56742212F, 0.60429640F, 0.64356699F, 0.68538959F, 0.72993007F, 0.77736504F, 0.82788260F, 0.88168307F, 0.9389798F, 1.0F
};

void Floor1::render_line(int x0, int x1, int y0, int y1, float d)
{
    int dy = y1 - y0;
    int adx = x1 - x0;
    int ady = Math::Abs(dy);
    int bbase = dy / adx;
    int sy = (((dy < 0)) ? bbase - 1 : bbase + 1);
    int x = x0;
    int y = y0;
    int err = 0;
    ady -= Math::Abs(bbase * adx);
    d[x] *= FLOOR_fromdB_LOOKUP[y];
    
    while (++x < x1)
    {
        err = err + ady;
        
        if (err >= adx)
        {
            err -= adx;
            y += sy;
        }
        else
        {
            y += bbase;
        }
        
        d[x] *= FLOOR_fromdB_LOOKUP[y];
    }
}

int Floor1::ilog(int v)
{
    int ret = 0;
    
    while (v != 0)
    {
        ret++;
        v = safe_cast<int>((safe_cast<unsigned int>(v) >> 1));
    }
    
    return (ret);
}

int Floor1::ilog2(int v)
{
    int ret = 0;
    
    while (v > 1)
    {
        ret++;
        v = safe_cast<int>((safe_cast<unsigned int>(v) >> 1));
    }
    
    return (ret);
}

const int InfoFloor1::VIF_POSIT = 63;

const int InfoFloor1::VIF_CLASS = 16;

const int InfoFloor1::VIF_PARTS = 31;

InfoFloor1::InfoFloor1()
{
    for (int i = 0; i < class_subbook.Length; i++)
    {
        class_subbook[i] = new int[8];
    }
}

void InfoFloor1::free()
{
    partitionclass = nullptr;
    class_dim = nullptr;
    class_subs = nullptr;
    class_book = nullptr;
    class_subbook = nullptr;
    postlist = nullptr;
}

Platform::Object InfoFloor1::copy_info()
{
    OggSharp::InfoFloor1^ info = this;
    OggSharp::InfoFloor1^ ret = ref new OggSharp::InfoFloor1();
    ret->partitions = info->partitions;
    Array::Copy(info->partitionclass, 0, ret->partitionclass, 0, VIF_PARTS);
    Array::Copy(info->class_dim, 0, ret->class_dim, 0, VIF_CLASS);
    Array::Copy(info->class_subs, 0, ret->class_subs, 0, VIF_CLASS);
    Array::Copy(info->class_book, 0, ret->class_book, 0, VIF_CLASS);
    
    for (int j = 0; j < VIF_CLASS; j++)
    {
        Array::Copy(info->class_subbook[j], 0, ret->class_subbook[j], 0, 8);
    }
    
    ret->mult = info->mult;
    Array::Copy(info->postlist, 0, ret->postlist, 0, VIF_POSIT + 2);
    ret->maxover = info->maxover;
    ret->maxunder = info->maxunder;
    ret->maxerr = info->maxerr;
    ret->twofitminsize = info->twofitminsize;
    ret->twofitminused = info->twofitminused;
    ret->twofitweight = info->twofitweight;
    ret->twofitatten = info->twofitatten;
    ret->unusedminsize = info->unusedminsize;
    ret->unusedmin_n = info->unusedmin_n;
    ret->n = info->n;
    return (ret);
}

int LookFloor1::VIF_POSIT = 63;

void LookFloor1::free()
{
    sorted_index = nullptr;
    forward_index = nullptr;
    reverse_index = nullptr;
    hineighbor = nullptr;
    loneighbor = nullptr;
}


