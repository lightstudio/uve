#include "pch.h"
#include "BenchMem.h"
#include <ppltasks.h>

using namespace concurrency;
using namespace UVEngineNative;
using namespace Platform;
using namespace std;
using namespace Windows::Foundation;

typedef unsigned char byte;
const int MB = 1048576;

IAsyncOperationWithProgress <double, double>^ BenchMem::Bench(int size)
{
	return create_async([size](progress_reporter<double> progress)->double
	{
		clock_t start, finish;
		double duration = 0, Size = (double) size;
		byte source[MB], dest[MB];
		srand((unsigned) time(NULL));

		for (int i = 0; i < size; i++)
		{
			memset(source, rand(), sizeof(source));
			start = clock();
			memcpy(source, dest, MB);
			finish = clock();
			progress.report(i / Size * 100);
			duration += (double) (finish - start) / CLOCKS_PER_SEC;
		}
		// delete source;
		// delete dest;
		return duration;
	});
}