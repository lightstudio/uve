#include "pch.h"
#include "UVEngineSettings.h"
using namespace Platform;
using namespace UVEngineNative;
using namespace std;

void UVEngineSettings::Initialize()
{
	ini=new CINI(FILEPATH("uveconfig.ini"));
}