#pragma once
#include "INI.h"
using namespace Platform;
using namespace std;

namespace UVEngineNative
{
	public ref class ManifestEX sealed
	{
	public:
		ManifestEX(String^ name);
		property String^ GameName
		{
			String^ get()
			{
				return ref new String(s2ws(this->gamename).data());
			}
		}		
		property String^ GameCompany
		{
			String^ get()
			{
				return ref new String(s2ws(this->gamecompany).data());
			}
		}
		property String^ GameMaker
		{
			String^ get()
			{
				return ref new String(s2ws(this->gamemaker).data());
			}
		}
		property String^ GameSize
		{
			String^ get()
			{
				return (this->gamesize/1048576).ToString();
			}
		}
		property String^ ScriptType
		{
			String^ get()
			{
				return ref new String(s2ws(this->scripttype).data());
			}
		}
		property String^ IconPath
		{
			String^ get()
			{
				return ref new String(s2ws(this->iconpath).data());
			}
		}
		property String^ TilePath
		{
			String^ get()
			{
				return ref new String(s2ws(this->tilepath).data());
			}
		}
		property String^ Intro
		{
			String^ get()
			{
				return ref new String(s2ws(this->intro).data());
			}
		}
	private:
		string gamename;
		string gamecompany;
		string gamemaker;
		int gamesize;
		string scripttype;
		string iconpath;
		string tilepath;
		string intro;
		CINI* cini;
	};
}