#pragma once

using namespace Platform;
using namespace std;
using namespace Windows::Foundation;

namespace UVEngineNative
{
	public ref class BenchMem sealed
	{
	public:
		static IAsyncOperationWithProgress <double, double>^ BenchMem::Bench(int size);
	};
}