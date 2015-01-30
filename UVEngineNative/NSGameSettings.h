#pragma once
#define FORCE_FULL_SCREEN 0
#define ORIGINAL_SCREEN 1
#define WIDE_SCREEN 2
using namespace Platform;


namespace UVEngineNative
{
	public ref class NSGameSettings sealed
	{
	public:
		NSGameSettings(String^ gameDirectory);
		property int screentype
		{
			int get()
			{
				return nsgameConfigINI->ReadInt("config","ScreenType",1);
			}
			void set(int value)
			{
				nsgameConfigINI->WriteInt("config","ScreenType",value);
				nsgameConfigINI->WriteFileNow();
			}
		}
	private:
		CINI* nsgameConfigINI;
	};
}