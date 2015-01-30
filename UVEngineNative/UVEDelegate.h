#pragma once
#include "ICallback.h"

namespace UVEngineNative
{
    public ref class UVEDelegate sealed
    {
        public:
        UVEDelegate();
        void SetCallback(ICallback^ callback);
        property ICallback^ GlobalCallback;
    };
}

