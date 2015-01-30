#include "pch.h"
#include "GameInfoEx.h"
#include "INI.h"
using namespace Platform;
using namespace std;
using namespace UVEngineNative;
using namespace Windows::Storage;




GameInfoEx::GameInfoEx(String^ GameFolder)
{
	this->gamePath=wstring(ApplicationData::Current->LocalFolder->Path->Data())+L'\\'+wstring(GameFolder->Data())+L'\\';//+GameFolder->Data();
	this->manifestPath=gamePath+DEFAULT_MANIFEST_FILE;
	FILE* fp;
	int buffersize=manifestPath.length()+1;
	char* ansi=new char[buffersize];
	WideCharToMultiByte(CP_UTF8,0,manifestPath.data(),-1,ansi,buffersize,NULL,NULL);
	_wfopen_s(&fp,manifestPath.data(),L"r");
	cini=CINI(ansi);
	string name=cini.ReadString("Info","Game","未知游戏");
	this->gameName=s2ws(name);
	string companyname=cini.ReadString("Info","Company","未知公司");
	this->company=s2ws(companyname);
	string maker=cini.ReadString("Info","Maker","未知制作者");
	this->gameMaker=s2ws(maker);
	string size=cini.ReadString("Info","Size","未知大小");
	this->gameSize=s2ws(size);
	string File=cini.ReadString("Script","Main","nscript.dat");
	this->scriptFile=s2ws(File);
	string type=cini.ReadString("Script","Type","NS");
	if (type=="NS")
	{
		this->scriptType=0;
	}
	else if (type=="Ruby")
	{
		this->scriptType=1;
	}
	else
	{
		this->scriptType=-1;//Unknown
	}
	string fontfile=cini.ReadString("Script","Font","default.ttf");
	this->font=s2ws(fontfile);
	string resolution=cini.ReadString("Script","Resolution","800*600");
	this->screenResolution=s2ws(resolution);
	string datafolder=cini.ReadString("Script","DataFolder","data");
	this->dataFolder=s2ws(datafolder);
	fclose(fp);
	
//	wstring path=wstring(ApplicationData::Current->LocalFolder->Path->Data())+this->gameFolder;
	
}