#pragma once
#include "INI.h"
using namespace std;
using namespace Platform;

CINI * ini;
namespace UVEngineNative
{
	public ref class UVEngineSettings sealed
	{
	public:
		static property String^ language
		{
			String^ get()
			{
				return ref new String(s2ws(ini->ReadString("config","Language","")).data());
			}
			void set(String^ value)
			{
				ini->WriteString("config","Language",ws2s(value->Data()));
				ini->WriteFileNow();
			}
		}
		static property bool ReqPasswd
		{
			bool get()
			{
				return ini->ReadBool("config","ReqPasswd",false);
			}
			void set(bool value)
			{
				ini->WriteBool("config","ReqPasswd",value);
				ini->WriteFileNow();
			}
		};
		static property bool translucent
		{
			bool get()
			{
				return ini->ReadBool("config","translucent",true);
			}
			void set(bool value)
			{
				ini->WriteBool("config","translucent",value);
				ini->WriteFileNow();
			}
		};
		static property String^ Passwd
		{
			String^ get()
			{
				return ref new String(s2ws(ini->ReadString("config","Password","")).data());
			}
			void set(String^ value)
			{
				ini->WriteString("config","Password",ws2s(value->Data()));
				ini->WriteFileNow();
			}
		}
		static property bool QuickShare
		{
			bool get()
			{
				return ini->ReadBool("config","QuickShare",true);
			}
			void set(bool value)
			{
				ini->WriteBool("config","QuickShare",value);
				ini->WriteFileNow();
			}
		}
		static property int ConfirmExit
		{
			int get()
			{
				return ini->ReadInt("config","ConfirmExit",0);
			}
			void set(int value)
			{
				ini->WriteInt("config","ConfirmExit",value);
				ini->WriteFileNow();
			}
		}
		static property bool Donated
		{
			bool get()
			{
				return ini->ReadBool("info","donated",false);
			}
			void set(bool value)
			{
				ini->WriteBool("info","donated",value);
				ini->WriteFileNow();
			}
		}
		static property bool NoMoreDisplay
		{
			bool get()
			{
				return ini->ReadBool("info","NoMoreDisplay",true);
			}
			void set(bool value)
			{
				ini->WriteBool("info","NoMoreDisplay",value);
				ini->WriteFileNow();
			}
		}
		static void Initialize();
	};
}