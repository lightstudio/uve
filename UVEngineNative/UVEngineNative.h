#pragma once
using namespace Platform;
using namespace std;


namespace UVEngineNative
{
    public ref class ImageToolkitNative sealed
    {
    public:
		static Array<int,1>^ UnAlpha(const Array<int,1>^ alpha, const Array<int,1>^ unAlpha ,int width,int height);
    };
}