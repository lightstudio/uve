#include "pch.h"
#include "CodeBook.h"

using namespace System;
using namespace System::Runtime::CompilerServices;
using namespace OggSharp;

int CodeBook::encode(int a, OggSharp::csBuffer^ b)
{
    b->write(codelist[a], c->lengthlist[a]);
    return (c->lengthlist[a]);
}

int CodeBook::errorv(float a)
{
    int bestt = best(a, 1);
    
    for (int k = 0; k < dim; k++)
    {
        a[k] = valuelist[bestt * dim + k];
    }
    
    return (bestt);
}

int CodeBook::encodev(int best, float a, OggSharp::csBuffer^ b)
{
    for (int k = 0; k < dim; k++)
    {
        a[k] = valuelist[best * dim + k];
    }
    
    return (encode(best, b));
}

int CodeBook::encodevs(float a, OggSharp::csBuffer^ b, int step, int addmul)
{
    int best = besterror(a, step, addmul);
    return (encode(best, b));
}

int CodeBook::decodevs_add(float a, int offset, OggSharp::csBuffer^ b, int n)
{
    int step = n / dim;
    int entry;
    int i, j, o;
    
    if (t.Length < step)
    {
        t = new int[step];
    }
    
    for (i = 0; i < step; i++)
    {
        entry = decode(b);
        
        if (entry == -1)
            return (-1);
        t[i] = entry * dim;
    }
    
    for (i = 0, o = 0; i < dim; i++, o += step)
    {
        for (j = 0; j < step; j++)
        {
            a[offset + o + j] += valuelist[t[j] + i];
        }
    }
    
    return (0);
}

int CodeBook::decodev_add(float a, int offset, OggSharp::csBuffer^ b, int n)
{
    int i, j, k, entry;
    int t;
    
    if (dim > 8)
    {
        for (i = 0; i < n; )
        {
            entry = decode(b);
            
            if (entry == -1)
                return (-1);
            t = entry * dim;
            
            for (j = 0; j < dim; )
            {
                a[offset + (i++)] += valuelist[t + (j++)];
            }
        }
    }
    else
    {
        for (i = 0; i < n; )
        {
            entry = decode(b);
            
            if (entry == -1)
                return (-1);
            t = entry * dim;
            j = 0;
            
            for (k = 0; k < dim; k++)
            {
                a[offset + (i++)] += valuelist[t + (j++)];
            }
        }
    }
    
    return (0);
}

int CodeBook::decodev_set(float a, int offset, OggSharp::csBuffer^ b, int n)
{
    int i, j, entry;
    int t;
    
    for (i = 0; i < n; )
    {
        entry = decode(b);
        
        if (entry == -1)
            return (-1);
        t = entry * dim;
        
        for (j = 0; j < dim; )
        {
            a[offset + i++] = valuelist[t + (j++)];
        }
    }
    
    return (0);
}

int CodeBook::decodevv_add(float a, int offset, int ch, OggSharp::csBuffer^ b, int n)
{
    int i, j, entry;
    int chptr = 0;
    
    for (i = offset / ch; i < (offset + n) / ch; )
    {
        entry = decode(b);
        
        if (entry == -1)
            return (-1);
        int t = entry * dim;
        
        for (j = 0; j < dim; j++)
        {
            a[chptr][i] += valuelist[t + j];
            chptr++;
            
            if (chptr == ch)
            {
                chptr = 0;
                i++;
            }
        }
    }
    
    return (0);
}

int CodeBook::decode(OggSharp::csBuffer^ b)
{
    int ptr = 0;
    OggSharp::DecodeAux^ t = decode_tree;
    int lok = b->look(t->tabn);
    
    if (lok >= 0)
    {
        ptr = t->tab[lok];
        b->adv(t->tabl[lok]);
        
        if (ptr <= 0)
        {
            return -ptr;
        }
    }
    
    do
    {
        switch (b->read1())
        {
            case 0:
            
                ptr = t->ptr0[ptr];
                break;
            
            case 1:
            
                ptr = t->ptr1[ptr];
                break;
            
            case -1:
            ;
            
            default: 
            
                return (-1);
        }
    }
    while (ptr > 0)
    
    return (-ptr);
}

int CodeBook::decodevs(float a, int index, OggSharp::csBuffer^ b, int step, int addmul)
{
    int entry = decode(b);
    
    if (entry == -1)
        return (-1);
    switch (addmul)
    {
        case -1:
        
            for (int i = 0, o = 0; i < dim; i++, o += step)
                a[index + o] = valuelist[entry * dim + i];
            break;
        
        case 0:
        
            for (int i = 0, o = 0; i < dim; i++, o += step)
                a[index + o] += valuelist[entry * dim + i];
            break;
        
        case 1:
        
            for (int i = 0, o = 0; i < dim; i++, o += step)
                a[index + o] *= valuelist[entry * dim + i];
            break;
        
        default: 
        
            break;
    }
    
    return (entry);
}

int CodeBook::best(float a, int step)
{
    OggSharp::EncodeAuxNearestMatch^ nt = c->nearest_tree;
    OggSharp::EncodeAuxThreshMatch^ tt = c->thresh_tree;
    int ptr = 0;
    
    if (tt != nullptr)
    {
        int index = 0;
        
        for (int k = 0, o = step * (dim - 1); k < dim; k++, o -= step)
        {
            int i;
            
            for (i = 0; i < tt->threshvals - 1; i++)
            {
                if (a[o] < tt->quantthresh[i])
                {
                    break;
                }
            }
            
            index = (index * tt->quantvals) + tt->quantmap[i];
        }
        
        if (c->lengthlist[index] > 0)
        {
            return (index);
        }
    }
    
    if (nt != nullptr)
    {
        while (true)
        {
            double cc = 0.0;
            int p = nt->p[ptr];
            int q = nt->q[ptr];
            
            for (int k = 0, o = 0; k < dim; k++, o += step)
            {
                cc += (valuelist[p + k] - valuelist[q + k]) * (a[o] - (valuelist[p + k] + valuelist[q + k]) * .5);
            }
            
            if (cc > 0.0)
            {
                ptr = -nt->ptr0[ptr];
            }
            else
            {
                ptr = -nt->ptr1[ptr];
            }
            
            if (ptr <= 0)
                break;
        }
        
        return (-ptr);
    }

{
    int besti = -1;
    float best = 0.0f;
    int e = 0;
    
    for (int i = 0; i < entries; i++)
    {
        if (c->lengthlist[i] > 0)
        {
            float _this = dist(dim, valuelist, e, a, step);
            
            if (besti == -1 || _this < best)
            {
                best = _this;
                besti = i;
            }
        }
        
        e += dim;
    }
    
    return (besti);
}
}

int CodeBook::besterror(float a, int step, int addmul)
{
    int bestt = best(a, step);
    switch (addmul)
    {
        case 0:
        
            for (int i = 0, o = 0; i < dim; i++, o += step)
                a[o] -= valuelist[bestt * dim + i];
            break;
        
        case 1:
        
            for (int i = 0, o = 0; i < dim; i++, o += step)
            {
                float val = valuelist[bestt * dim + i];
                
                if (val == 0)
                {
                    a[o] = 0;
                }
                else
                {
                    a[o] /= val;
                }
            }
            
            break;
        
    }
    
    return (bestt);
}

void CodeBook::clear()
{
}

float CodeBook::dist(int el, float rref, int index, float b, int step)
{
    float acc = safe_cast<float>(0.0);
    
    for (int i = 0; i < el; i++)
    {
        float val = (rref[index + i] - b[i * step]);
        acc += val * val;
    }
    
    return (acc);
}

int CodeBook::init_decode(OggSharp::StaticCodeBook^ s)
{
    c = s;
    entries = s->entries;
    dim = s->dim;
    valuelist = s->unquantize();
    decode_tree = make_decode_tree();
    
    if (decode_tree == nullptr)
    {
        clear();
        return (-1);
    }
    
    return (0);
}

int CodeBook::make_words(int l, int n)
{
    int marker = new int[33];
    int r = new int[n];
    
    for (int i = 0; i < n; i++)
    {
        int length = l[i];
        
        if (length > 0)
        {
            int entry = marker[length];
            
            if (length < 32 && (safe_cast<unsigned int>(entry) >> length) != 0)
            {
                return (nullptr);
            }
            
            r[i] = entry;
        
        {
            for (int j = length; j > 0; j--)
            {
                if ((marker[j] & 1) != 0)
                {
                    if (j == 1)
                        marker[1]++;
                    else
                        marker[j] = marker[j - 1] << 1;
                    break;
                }
                
                marker[j]++;
            }
        }
            
            for (int j = length + 1; j < 33; j++)
            {
                if ((safe_cast<unsigned int>(marker[j]) >> 1) == entry)
                {
                    entry = marker[j];
                    marker[j] = marker[j - 1] << 1;
                }
                else
                {
                    break;
                }
            }
        }
    }
    
    for (int i = 0; i < n; i++)
    {
        int temp = 0;
        
        for (int j = 0; j < l[i]; j++)
        {
            temp <<= 1;
            temp = safe_cast<int>((safe_cast<unsigned int>(temp) | (safe_cast<unsigned int>(r[i]) >> j) & 1));
        }
        
        r[i] = temp;
    }
    
    return (r);
}

OggSharp::DecodeAux^ CodeBook::make_decode_tree()
{
    int top = 0;
    OggSharp::DecodeAux^ t = ref new OggSharp::DecodeAux();
    int ptr0 = t->ptr0 = new int[entries * 2];
    int ptr1 = t->ptr1 = new int[entries * 2];
    int codelist = make_words(c->lengthlist, c->entries);
    
    if (codelist == nullptr)
        return (nullptr);
    t->aux = entries * 2;
    
    for (int i = 0; i < entries; i++)
    {
        if (c->lengthlist[i] > 0)
        {
            int ptr = 0;
            int j;
            
            for (j = 0; j < c->lengthlist[i] - 1; j++)
            {
                int bit = safe_cast<int>(((safe_cast<unsigned int>(codelist[i]) >> j) & 1));
                
                if (bit == 0)
                {
                    if (ptr0[ptr] == 0)
                    {
                        ptr0[ptr] = ++top;
                    }
                    
                    ptr = ptr0[ptr];
                }
                else
                {
                    if (ptr1[ptr] == 0)
                    {
                        ptr1[ptr] = ++top;
                    }
                    
                    ptr = ptr1[ptr];
                }
            }
            
            if (((safe_cast<unsigned int>(codelist[i]) >> j) & 1) == 0)
            {
                ptr0[ptr] = -i;
            }
            else
            {
                ptr1[ptr] = -i;
            }
        }
    }
    
    t->tabn = ilog(entries) - 4;
    
    if (t->tabn < 5)
        t.tabn = 5;
    int n = 1 << t->tabn;
    t->tab = new int[n];
    t->tabl = new int[n];
    
    for (int i = 0; i < n; i++)
    {
        int p = 0;
        int j = 0;
        
        for (j = 0; j < t->tabn && (p > 0 || j == 0); j++)
        {
            if ((i & (1 << j)) != 0)
            {
                p = ptr1[p];
            }
            else
            {
                p = ptr0[p];
            }
        }
        
        t->tab[i] = p;
        t->tabl[i] = j;
    }
    
    return (t);
}

int CodeBook::ilog(int v)
{
    int ret = 0;
    
    while (v != 0)
    {
        ret++;
        v = safe_cast<int>((safe_cast<unsigned int>(v) >> 1));
    }
    
    return (ret);
}


