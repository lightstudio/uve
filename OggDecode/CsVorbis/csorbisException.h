#pragma once

namespace OggSharp
{
    public ref class csorbisException: public Exception
    {
        public: csorbisException();
        public: csorbisException(Platform::String s);
    };
}

