#pragma once

namespace OggSharp
{
    public ref class csBuffer
    {
        private: static int BUFFER_INCREMENT;
        private: static unsigned int mask;
        private: int ptr;
        private: unsigned char buffer;
        private: int endbit;
        private: int endbyte;
        private: int storage;
        public: void writeinit();
        public: void write(unsigned char s);
        public: void read(unsigned char s, int bytes);
        private: void reset();
        public: void writeclear();
        public: void readinit(unsigned char buf, int start, int bytes);
        public: void write(int vvalue, int bits);
        public: int look(int bits);
        public: int look1();
        public: void adv(int bits);
        public: void adv1();
        public: int read(int bits);
        public: int read1();
        public: int bytes();
        public: int bits();
        public: static int ilog(int v);
        public: unsigned char buf();
        public: csBuffer();
    };
}

