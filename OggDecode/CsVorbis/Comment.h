#pragma once

namespace OggSharp
{
    public ref class Comment
    {
        private: static Platform::String _vorbis;
        private: static int OV_EIMPL;
        public: unsigned char user_comments;
        public: int comment_lengths;
        public: int comments;
        public: unsigned char vendor;
        public: void init();
        public: void add(Platform::String comment);
        private: void add(unsigned char comment);
        public: void add_tag(Platform::String tag, Platform::String contents);
        private: static Platform::bool tagcompare(unsigned char s1, unsigned char s2, int n);
        public: Platform::String query(Platform::String tag);
        public: Platform::String query(Platform::String tag, int count);
        private: int query(unsigned char tag, int count);
        internal: int unpack(OggSharp::csBuffer^ opb);
        private: int pack(OggSharp::csBuffer^ opb);
        public: int header_out(OggSharp::Packet^ op);
        internal: void clear();
        public: Platform::String getVendor();
        public: Platform::String getComment(int i);
        public: Platform::String toString();
    };
}

