#include "pch.h"
#include "csorbisException.h"

using namespace System;
using namespace OggSharp;

csorbisException::csorbisException() : 
    Exception()
{
}

csorbisException::csorbisException(Platform::String s) : 
    Exception("csorbis: " + s)
{
}


