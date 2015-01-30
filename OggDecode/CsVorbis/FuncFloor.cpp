#include "pch.h"
#include "FuncFloor.h"

using namespace System;
using namespace OggSharp;

OggSharp::FuncFloor^ FuncFloor::floor_P = 
{
    ref new OggSharp::Floor0(), ref new OggSharp::Floor1()
};

void FuncFloor::pack(Platform::Object i, OggSharp::csBuffer^ opb)
{
}

Platform::Object FuncFloor::unpack(OggSharp::Info^ vi, OggSharp::csBuffer^ opb)
{
}

Platform::Object FuncFloor::look(OggSharp::DspState^ vd, OggSharp::InfoMode^ mi, Platform::Object i)
{
}

void FuncFloor::free_info(Platform::Object i)
{
}

void FuncFloor::free_look(Platform::Object i)
{
}

void FuncFloor::free_state(Platform::Object vs)
{
}

int FuncFloor::forward(OggSharp::Block^ vb, Platform::Object i, float fin, float fout, Platform::Object vs)
{
}

Platform::Object FuncFloor::inverse1(OggSharp::Block^ vb, Platform::Object i, Platform::Object memo)
{
}

int FuncFloor::inverse2(OggSharp::Block^ vb, Platform::Object i, Platform::Object memo, float fout)
{
}


