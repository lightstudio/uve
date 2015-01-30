#include "pch.h"
#include "FuncResidue.h"

using namespace System;
using namespace OggSharp;

OggSharp::FuncResidue^ FuncResidue::residue_P = 
{
    ref new OggSharp::Residue0(), ref new OggSharp::Residue1(), ref new OggSharp::Residue2()
};

void FuncResidue::pack(Platform::Object vr, OggSharp::csBuffer^ opb)
{
}

Platform::Object FuncResidue::unpack(OggSharp::Info^ vi, OggSharp::csBuffer^ opb)
{
}

Platform::Object FuncResidue::look(OggSharp::DspState^ vd, OggSharp::InfoMode^ vm, Platform::Object vr)
{
}

void FuncResidue::free_info(Platform::Object i)
{
}

void FuncResidue::free_look(Platform::Object i)
{
}

int FuncResidue::forward(OggSharp::Block^ vb, Platform::Object vl, float fin, int ch)
{
}

int FuncResidue::inverse(OggSharp::Block^ vb, Platform::Object vl, float fin, int nonzero, int ch)
{
}


