#pragma once

namespace OggSharp
{
    ref class Lookup
    {
        private: static int COS_LOOKUP_SZ;
        private: static float COS_LOOKUP;
        internal: static float coslook(float a);
        private: static int INVSQ_LOOKUP_SZ;
        private: static float INVSQ_LOOKUP;
        internal: static float invsqlook(float a);
        private: static int INVSQ2EXP_LOOKUP_MIN;
        private: static float INVSQ2EXP_LOOKUP;
        internal: static float invsq2explook(int a);
        private: const static int FROMdB_LOOKUP_SZ;
        private: const static int FROMdB2_LOOKUP_SZ;
        private: const static int FROMdB_SHIFT;
        private: const static int FROMdB2_SHIFT;
        private: const static int FROMdB2_MASK;
        private: static float FROMdB_LOOKUP;
        private: static float FROMdB2_LOOKUP;
        internal: static float fromdBlook(float a);
    };
}

