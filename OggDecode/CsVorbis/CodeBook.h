#pragma once

namespace OggSharp
{
    ref class CodeBook
    {
        internal: int dim;
        internal: int entries;
        internal: OggSharp::StaticCodeBook^ c;
        internal: float valuelist;
        internal: int codelist;
        internal: OggSharp::DecodeAux^ decode_tree;
        internal: int encode(int a, OggSharp::csBuffer^ b);
        internal: int errorv(float a);
        internal: int encodev(int best, float a, OggSharp::csBuffer^ b);
        internal: int encodevs(float a, OggSharp::csBuffer^ b, int step, int addmul);
        internal: int t;
        internal: int decodevs_add(float a, int offset, OggSharp::csBuffer^ b, int n);
        internal: int decodev_add(float a, int offset, OggSharp::csBuffer^ b, int n);
        internal: int decodev_set(float a, int offset, OggSharp::csBuffer^ b, int n);
        internal: int decodevv_add(float a, int offset, int ch, OggSharp::csBuffer^ b, int n);
        internal: int decode(OggSharp::csBuffer^ b);
        internal: int decodevs(float a, int index, OggSharp::csBuffer^ b, int step, int addmul);
        internal: int best(float a, int step);
        internal: int besterror(float a, int step, int addmul);
        internal: void clear();
        internal: static float dist(int el, float rref, int index, float b, int step);
        internal: int init_decode(OggSharp::StaticCodeBook^ s);
        internal: static int make_words(int l, int n);
        internal: OggSharp::DecodeAux^ make_decode_tree();
        internal: static int ilog(int v);
    };
    ref class DecodeAux
    {
        internal: int tab;
        internal: int tabl;
        internal: int tabn;
        internal: int ptr0;
        internal: int ptr1;
        internal: int aux;
    };
}

