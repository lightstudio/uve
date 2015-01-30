#include "pch.h"
#include "Buffer.h"

using namespace System;
using namespace OggSharp;

int csBuffer::BUFFER_INCREMENT = 256;

unsigned int csBuffer::mask = 
{
    0x00000000, 0x00000001, 0x00000003, 0x00000007, 0x0000000f, 0x0000001f, 0x0000003f, 0x0000007f, 0x000000ff, 0x000001ff, 0x000003ff, 0x000007ff, 0x00000fff, 0x00001fff, 0x00003fff, 0x00007fff, 0x0000ffff, 0x0001ffff, 0x0003ffff, 0x0007ffff, 0x000fffff, 0x001fffff, 0x003fffff, 0x007fffff, 0x00ffffff, 0x01ffffff, 0x03ffffff, 0x07ffffff, 0x0fffffff, 0x1fffffff, 0x3fffffff, 0x7fffffff, 0xffffffff
};

void csBuffer::writeinit()
{
    buffer = new byte[BUFFER_INCREMENT];
    ptr = 0;
    buffer[0] = safe_cast<unsigned char>('\0');
    storage = BUFFER_INCREMENT;
}

void csBuffer::write(unsigned char s)
{
    for (int i = 0; i < s::Length; i++)
    {
        if (s[i] == 0)
            break;
        write(s[i], 8);
    }
}

void csBuffer::read(unsigned char s, int bytes)
{
    int i = 0;
    
    while (bytes-- != 0)
    {
        s[i++] = safe_cast<unsigned char>((read(8)));
    }
}

void csBuffer::reset()
{
    ptr = 0;
    buffer[0] = safe_cast<unsigned char>('\0');
    endbit = endbyte = 0;
}

void csBuffer::writeclear()
{
    buffer = nullptr;
}

void csBuffer::readinit(unsigned char buf, int start, int bytes)
{
    ptr = start;
    buffer = buf;
    endbit = endbyte = 0;
    storage = bytes;
}

void csBuffer::write(int vvalue, int bits)
{
    if (endbyte + 4 >= storage)
    {
        unsigned char foo = new byte[storage + BUFFER_INCREMENT];
        Array::Copy(buffer, 0, foo, 0, storage);
        buffer = foo;
        storage += BUFFER_INCREMENT;
    }
    
    vvalue = safe_cast<int>((safe_cast<unsigned int>(vvalue) & mask[bits]));
    bits += endbit;
    buffer[ptr] |= safe_cast<unsigned char>((vvalue << endbit));
    
    if (bits >= 8)
    {
        buffer[ptr + 1] = safe_cast<unsigned char>((safe_cast<unsigned int>(vvalue) >> (8 - endbit)));
        
        if (bits >= 16)
        {
            buffer[ptr + 2] = safe_cast<unsigned char>((safe_cast<unsigned int>(vvalue) >> (16 - endbit)));
            
            if (bits >= 24)
            {
                buffer[ptr + 3] = safe_cast<unsigned char>((safe_cast<unsigned int>(vvalue) >> (24 - endbit)));
                
                if (bits >= 32)
                {
                    if (endbit > 0)
                        buffer[ptr + 4] = safe_cast<unsigned char>((safe_cast<unsigned int>(vvalue) >> (32 - endbit)));
                    else
                        buffer[ptr + 4] = 0;
                }
            }
        }
    }
    
    endbyte += bits / 8;
    ptr += bits / 8;
    endbit = bits & 7;
}

int csBuffer::look(int bits)
{
    int ret;
    unsigned int m = mask[bits];
    bits += endbit;
    
    if (endbyte + 4 >= storage)
    {
        if (endbyte + (bits - 1) / 8 >= storage)
            return (-1);
    }
    
    ret = ((buffer[ptr]) & 0xff) >> endbit;
    
    if (bits > 8)
    {
        ret |= ((buffer[ptr + 1]) & 0xff) << (8 - endbit);
        
        if (bits > 16)
        {
            ret |= ((buffer[ptr + 2]) & 0xff) << (16 - endbit);
            
            if (bits > 24)
            {
                ret |= ((buffer[ptr + 3]) & 0xff) << (24 - endbit);
                
                if ((bits > 32) && (endbit != 0))
                {
                    ret |= ((buffer[ptr + 4]) & 0xff) << (32 - endbit);
                }
            }
        }
    }
    
    ret = safe_cast<int>((m & ret));
    return (ret);
}

int csBuffer::look1()
{
    if (endbyte >= storage)
        return (-1);
    return ((buffer[ptr] >> endbit) & 1);
}

void csBuffer::adv(int bits)
{
    bits += endbit;
    ptr += bits / 8;
    endbyte += bits / 8;
    endbit = bits & 7;
}

void csBuffer::adv1()
{
    ++endbit;
    
    if (endbit > 7)
    {
        endbit = 0;
        ptr++;
        endbyte++;
    }
}

int csBuffer::read(int bits)
{
    int ret;
    unsigned int m = mask[bits];
    bits += endbit;
    
    if (endbyte + 4 >= storage)
    {
        ret = -1;
        
        if (endbyte + (bits - 1) / 8 >= storage)
        {
            ptr += bits / 8;
            endbyte += bits / 8;
            endbit = bits & 7;
            return (ret);
        }
    }
    
    ret = ((buffer[ptr]) & 0xff) >> endbit;
    
    if (bits > 8)
    {
        ret |= ((buffer[ptr + 1]) & 0xff) << (8 - endbit);
        
        if (bits > 16)
        {
            ret |= ((buffer[ptr + 2]) & 0xff) << (16 - endbit);
            
            if (bits > 24)
            {
                ret |= ((buffer[ptr + 3]) & 0xff) << (24 - endbit);
                
                if ((bits > 32) && (endbit != 0))
                {
                    ret |= ((buffer[ptr + 4]) & 0xff) << (32 - endbit);
                }
            }
        }
    }
    
    ret &= safe_cast<int>(m);
    ptr += bits / 8;
    endbyte += bits / 8;
    endbit = bits & 7;
    return (ret);
}

int csBuffer::read1()
{
    int ret;
    
    if (endbyte >= storage)
    {
        ret = -1;
        endbit++;
        
        if (endbit > 7)
        {
            endbit = 0;
            ptr++;
            endbyte++;
        }
        
        return (ret);
    }
    
    ret = (buffer[ptr] >> endbit) & 1;
    endbit++;
    
    if (endbit > 7)
    {
        endbit = 0;
        ptr++;
        endbyte++;
    }
    
    return (ret);
}

int csBuffer::bytes()
{
    return (endbyte + (endbit + 7) / 8);
}

int csBuffer::bits()
{
    return (endbyte * 8 + endbit);
}

int csBuffer::ilog(int v)
{
    int ret = 0;
    
    while (v > 0)
    {
        ret++;
        v >>= 1;
    }
    
    return (ret);
}

unsigned char csBuffer::buf()
{
    return (buffer);
}

csBuffer::csBuffer()
{
}


