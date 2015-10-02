#pragma once

using namespace Platform;
using namespace std;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

namespace UVEngineNative
{
	public ref class BenchCPU sealed
	{
	public:
		static IAsyncOperationWithProgress <double, double>^ BenchCPU::CalcPiSingle(int digits);
		static IAsyncOperationWithProgress <double, double>^ BenchCPU::CalcPiMulti(int digits);
	};
}