#include "pch.h"
#include "ONScripterSettings.h"

using namespace Platform;
using namespace std;
using namespace UVEngineNative;

void ONScripterSettings::Initialize()
{
	m_ini=new CINI(FILEPATH("nsconfig.ini"));
}