#pragma once

using namespace Platform;
using namespace std;

typedef unsigned char byte;

namespace UVEngineNative
{
	public ref class ImageMeta sealed
	{
	public:
		static Array<int, 1>^ ImageMeta::meta_calculate(const Array<byte, 1>^ decodedImageBGRABytes, int metaCount, int maxError);
	};
}