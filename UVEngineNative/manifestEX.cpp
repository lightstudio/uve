#include "pch.h"
#include "manifestEX.h"

using namespace Platform;
using namespace UVEngineNative;

ManifestEX::ManifestEX(String^ name)
{
	cini=new CINI(FILEPATH(ws2s(wstring(name->Data()))));
	this->gamename=cini->ReadString("Info","Game",FILEPATH(ws2s(wstring(name->Data()))));
	this->gamecompany=cini->ReadString("Info","Company","UNKNOWN COMPANY");
	this->gamemaker=cini->ReadString("Info","Maker","UNKNOWN AUTHOR");
	this->gamesize=cini->ReadInt("Info","Size",0);
	this->scripttype=cini->ReadString("Script","Type","Standard_NS");
	this->iconpath=cini->ReadString("Icon","Icon","Icon.png");
	this->tilepath=cini->ReadString("Icon","Tile","Icon.png");
	this->intro=cini->ReadString("Intro","GameIntro", "");
}