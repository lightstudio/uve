#include "pch.h"
#include "FuncMapping.h"

using namespace System;
using namespace OggSharp;

OggSharp::FuncMapping^ FuncMapping::mapping_P = 
{
    ref new OggSharp::Mapping0()
};

void FuncMapping::pack(OggSharp::Info^ info, Platform::Object imap, OggSharp::csBuffer^ buffer)
{
}

Platform::Object FuncMapping::unpack(OggSharp::Info^ info, OggSharp::csBuffer^ buffer)
{
}

Platform::Object FuncMapping::look(OggSharp::DspState^ vd, OggSharp::InfoMode^ vm, Platform::Object m)
{
}

void FuncMapping::free_info(Platform::Object imap)
{
}

void FuncMapping::free_look(Platform::Object imap)
{
}

int FuncMapping::inverse(OggSharp::Block^ vd, Platform::Object lm)
{
}


