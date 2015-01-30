#include "pch.h"
#include "FuncTime.h"

using namespace System;
using namespace OggSharp;

OggSharp::FuncTime^ FuncTime::time_P = 
{
    ref new OggSharp::Time0()
};

void FuncTime::pack(Platform::Object i, OggSharp::csBuffer^ opb)
{
}

Platform::Object FuncTime::unpack(OggSharp::Info^ vi, OggSharp::csBuffer^ opb)
{
}

Platform::Object FuncTime::look(OggSharp::DspState^ vd, OggSharp::InfoMode^ vm, Platform::Object i)
{
}

void FuncTime::free_info(Platform::Object i)
{
}

void FuncTime::free_look(Platform::Object i)
{
}

int FuncTime::forward(OggSharp::Block^ vb, Platform::Object i)
{
}

int FuncTime::inverse(OggSharp::Block^ vb, Platform::Object i, float fin, float fout)
{
}


