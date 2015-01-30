#include "pch.h"
#include "Comment.h"

using namespace System;
using namespace System::Text;
using namespace OggSharp;

Platform::String Comment::_vorbis = "vorbis";

int Comment::OV_EIMPL = -130;

void Comment::init()
{
    user_comments = nullptr;
    comments = 0;
    vendor = nullptr;
}

void Comment::add(Platform::String comment)
{
    Encoding AE = Encoding::UTF8;
    unsigned char comment_byt = AE::GetBytes(comment);
    add(comment_byt);
}

void Comment::add(unsigned char comment)
{
    unsigned char foo = new byte[comments + 2][];
    
    if (user_comments != nullptr)
    {
        Array::Copy(user_comments, 0, foo, 0, comments);
    }
    
    user_comments = foo;
    int goo = new int[comments + 2];
    
    if (comment_lengths != nullptr)
    {
        Array::Copy(comment_lengths, 0, goo, 0, comments);
    }
    
    comment_lengths = goo;
    unsigned char bar = new byte[comment::Length + 1];
    Array::Copy(comment, 0, bar, 0, comment::Length);
    user_comments[comments] = bar;
    comment_lengths[comments] = comment::Length;
    comments++;
    user_comments[comments] = nullptr;
}

void Comment::add_tag(Platform::String tag, Platform::String contents)
{
    if (contents == nullptr)
        contents = "";
    add(tag + "=" + contents);
}

Platform::bool Comment::tagcompare(unsigned char s1, unsigned char s2, int n)
{
    int c = 0;
    unsigned char u1, u2;
    
    while (c < n)
    {
        u1 = s1[c];
        u2 = s2[c];
        
        if (u1 >= 'A')
            u1 = safe_cast<unsigned char>((u1 - 'A' + 'a'));
        
        if (u2 >= 'A')
            u2 = safe_cast<unsigned char>((u2 - 'A' + 'a'));
        
        if (u1 != u2)
        {
            return false;
        }
        
        c++;
    }
    
    return true;
}

Platform::String Comment::query(Platform::String tag)
{
    return query(tag, 0);
}

Platform::String Comment::query(Platform::String tag, int count)
{
    Encoding AE = Encoding::UTF8;
    unsigned char tag_byt = AE::GetBytes(tag);
    int foo = query(tag_byt, count);
    
    if (foo == -1)
        return nullptr;
    unsigned char comment = user_comments[foo];
    
    for (int i = 0; i < comment_lengths[foo]; i++)
    {
        if (comment[i] == '=')
        {
            wchar_t comment_uni = AE::GetChars(comment);
            return ref new Platform::String(comment_uni, i + 1, comment_lengths[foo] - (i + 1));
        }
    }
    
    return nullptr;
}

int Comment::query(unsigned char tag, int count)
{
    int i = 0;
    int found = 0;
    int taglen = tag::Length;
    unsigned char fulltag = new byte[taglen + 2];
    Array::Copy(tag, 0, fulltag, 0, tag::Length);
    fulltag[tag::Length] = safe_cast<unsigned char>('=');
    
    for (i = 0; i < comments; i++)
    {
        if (tagcompare(user_comments[i], fulltag, taglen))
        {
            if (count == found)
            {
                return i;
            }
            else
            {
                found++;
            }
        }
    }
    
    return -1;
}

int Comment::unpack(OggSharp::csBuffer^ opb)
{
    int vendorlen = opb->read(32);
    
    if (vendorlen < 0)
    {
        clear();
        return (-1);
    }
    
    vendor = new byte[vendorlen + 1];
    opb->read(vendor, vendorlen);
    comments = opb->read(32);
    
    if (comments < 0)
    {
        clear();
        return (-1);
    }
    
    user_comments = new byte[comments + 1][];
    comment_lengths = new int[comments + 1];
    
    for (int i = 0; i < comments; i++)
    {
        int len = opb->read(32);
        
        if (len < 0)
        {
            clear();
            return (-1);
        }
        
        comment_lengths[i] = len;
        user_comments[i] = new byte[len + 1];
        opb->read(user_comments[i], len);
    }
    
    if (opb->read(1) != 1)
    {
        clear();
        return (-1);
    }
    
    return (0);
}

int Comment::pack(OggSharp::csBuffer^ opb)
{
    Platform::String temp = "Xiphophorus libVorbis I 20000508";
    Encoding AE = Encoding::UTF8;
    unsigned char temp_byt = AE::GetBytes(temp);
    unsigned char _vorbis_byt = AE::GetBytes(_vorbis);
    opb->write(0x03, 8);
    opb->write(_vorbis_byt);
    opb->write(temp::Length, 32);
    opb->write(temp_byt);
    opb->write(comments, 32);
    
    if (comments != 0)
    {
        for (int i = 0; i < comments; i++)
        {
            if (user_comments[i] != nullptr)
            {
                opb->write(comment_lengths[i], 32);
                opb->write(user_comments[i]);
            }
            else
            {
                opb->write(0, 32);
            }
        }
    }
    
    opb->write(1, 1);
    return (0);
}

int Comment::header_out(OggSharp::Packet^ op)
{
    OggSharp::csBuffer^ opb = ref new OggSharp::csBuffer();
    opb->writeinit();
    
    if (pack(opb) != 0)
        return OV_EIMPL;
    op->packet_base = new byte[opb->bytes()];
    op->packet = 0;
    op->bytes = opb->bytes();
    Array::Copy(opb->buf(), 0, op->packet_base, 0, op->bytes);
    op->b_o_s = 0;
    op->e_o_s = 0;
    op->granulepos = 0;
    return 0;
}

void Comment::clear()
{
    for (int i = 0; i < comments; i++)
        user_comments[i] = nullptr;
    user_comments = nullptr;
    vendor = nullptr;
}

Platform::String Comment::getVendor()
{
    Encoding AE = Encoding::UTF8;
    wchar_t vendor_uni = AE::GetChars(vendor);
    return ref new Platform::String(vendor_uni);
}

Platform::String Comment::getComment(int i)
{
    Encoding AE = Encoding::UTF8;
    
    if (comments <= i)
        return nullptr;
    wchar_t user_comments_uni = AE::GetChars(user_comments[i]);
    return ref new Platform::String(user_comments_uni);
}

Platform::String Comment::toString()
{
    Encoding AE = Encoding::UTF8;
    Platform::String long_string = "Vendor: " + ref new Platform::String(AE::GetChars(vendor));
    
    for (int i = 0; i < comments; i++)
        long_string = long_string + "\nComment: " + ref new Platform::String(AE::GetChars(user_comments[i]));
    long_string = long_string + "\n";
    return long_string;
}


