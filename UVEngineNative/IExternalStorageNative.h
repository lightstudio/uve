#pragma once
using namespace Windows::Storage;
using namespace Platform;

namespace UVEngineNative
{
	public interface class IExternalStorageNative
	{
		Windows::Storage::Streams::IInputStream^ GetNativeStream();
	};
}