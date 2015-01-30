#pragma once
using namespace Platform;
#define CALL(function) GlobalNativeCallback->inc->function
#define INIT_CALLBACK(INATIVECALLS) GlobalNativeCallback=new NativeCallback(INATIVECALLS)
namespace UVEngineNative
{
	public interface class INativeCalls
	{
	public:
		void ErrorLog(String^ errorContent);
		void ExitCore(int code);
		void PlayExternalVideo(String^ path);
	};
	private class NativeCallback
	{
	public:
		NativeCallback(INativeCalls^ inc)
		{
			this->inc=inc;
		}
		INativeCalls^ inc;
	};
}

extern UVEngineNative::NativeCallback* GlobalNativeCallback;
