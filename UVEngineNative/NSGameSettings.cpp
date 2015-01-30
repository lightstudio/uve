#include "pch.h"
#include "INI.h"
#include "NSGameSettings.h"
using namespace Platform;
using namespace UVEngineNative;

NSGameSettings::NSGameSettings(String^ gameDirectory)
{
	this->nsgameConfigINI=new CINI(FILEPATH((ws2s(gameDirectory->Data())+"\\nsgameconfig.ini").data()));
}