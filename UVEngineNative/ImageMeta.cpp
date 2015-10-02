#include "pch.h"
#include "ImageMeta.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef unsigned char byte;

using namespace UVEngineNative;
using namespace Platform;
using namespace std;

inline int GetDistance(int b, int g, int r){
	int value = b*b + g*g + r*r;
	return value;
}

inline byte Average(byte* list, int length){
	long long sum = 0;
	for (int i = 0; i < length; i++){
		sum += list[i];
	}
	return (byte)
		(sum / length);
}

inline int IndexOfMin(int * arr, int count) {
	double min;
	int index = 0;
	min = arr[0];
	for (int i = 0; i < count; i++)	{
		if (arr[i] < min) {
			min = arr[i];
			index = i;
		}
	}
	return index;
}

#define SafeDelete(ptr) if(ptr){ delete ptr; ptr=NULL;}

struct metaunit{
	//byte *Rs, *Gs, *Bs;
	unsigned long long Rs, Gs, Bs;
	byte r, g, b;
	int count;
	inline void init() {
		count = 0;
		Rs = 0;
		Gs = 0;
		Bs = 0;
	}
	inline void attach(byte b, byte g, byte r) {
		Rs += r;
		Bs += b;
		Gs += g;
		count++;
	}
	inline void clear(){
		Rs = 0;
		Gs = 0;
		Bs = 0;
		this->count = 0;
	}
	inline void destroy(){
		//SafeDelete(Rs);
		//SafeDelete(Gs);
		//SafeDelete(Bs);
	}
};

Array<int, 1>^ ImageMeta::meta_calculate(const Array<byte, 1>^ decodedImageBGRABytes, int metaCount, int maxError)
{
	Array<int, 1>^ out_colors = ref new Array<int, 1>(metaCount);
	metaunit* points = new metaunit[metaCount];
	int * distance = new int[metaCount];
	int pixelCount = decodedImageBGRABytes->Length / 4;
	metaunit current;

	byte b, g, r;

	for (int i = 0; i < metaCount; i++)	{
		points[i].init();
		srand((int)time(NULL));
		points[i].r = rand() % 256;
		points[i].g = rand() % 256;
		points[i].b = rand() % 256;
	}
	auto calc = true;
	while (calc) {
		for (int i = 0; i < pixelCount; i++) {
			//clear the array
			for (int j = 0; j < metaCount; j++)	{
				distance[j] = 0;
			}
			byte b = decodedImageBGRABytes[i * 4];
			byte g = decodedImageBGRABytes[i * 4 + 1];
			byte r = decodedImageBGRABytes[i * 4 + 2];
			for (auto pointCount = 0; pointCount < metaCount; pointCount++)	{
				current = points[pointCount];
				byte currentB = current.b;
				byte currentG = current.g;
				byte currentR = current.r;
				distance[pointCount] = GetDistance(b - currentB, g - currentG, r - currentR);
			}
			points[IndexOfMin(distance, metaCount)].attach(b, g, r);
		}
		for (int p = 0; p < metaCount; p++)	{
			current = points[p];
			//auto bArray = current.Bs;
			//auto gArray = current.Gs;
			//auto rArray = current.Rs;
			auto colorCount = current.count;
			if (colorCount == 0)
			{
				b = (byte)(rand() % 256);
				g = (byte)(rand() % 256);
				r = (byte)(rand() % 256);
			}
			else
			{
				b = current.Bs / colorCount;
				g = current.Gs / colorCount;
				r = current.Rs / colorCount;
			}
			if (abs(b - current.b) <= maxError&&abs(g - current.g) <= maxError&&abs(r - current.r) <= maxError){
				calc = false;
			}

			points[p].r = r;
			points[p].g = g;
			points[p].b = b;
			points[p].clear();
		}
	}
	for (int i = 0; i < metaCount; i++)
	{
		int b = points[i].b;
		int g = points[i].g;
		int r = points[i].r;
		int a = 255;
		b <<= 24;
		g <<= 16;
		r <<= 8;
		out_colors[i] = b | g | r | a;
		points[i].destroy();
	}
	delete points;
	delete distance;
	return out_colors;
}