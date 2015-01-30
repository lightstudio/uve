#include "UVEDelegate.h"
#include "GameInfoEx.h"
#include "NativeScripter.h"
using namespace Platform;
using namespace std;
using namespace Windows::Storage;


namespace UVEngineNative
{
	public ref class NativeScript sealed
	{
	public:
		property GameInfoEx^ gameInfoEx;
		static property UVEDelegate^ callbackDelegate;
		void RunScript(double p_X,double p_Y);
		void TouchPoint(double p_X,double p_Y);
		bool BackKey();
		//property PhoneDirect3DXamlAppComponent::Direct3DBackground^ d3dbg;
		NativeScript(String^ gameFolder,UVEDelegate^ uve_delegate);
	internal:
		property ONScripter* ons;
	private:
		~NativeScript();
		void ScriptInit();
		void SendCommand(string command,wstring* params,int param_count);
		ifstream scriptFile;
	};
}

void Exit(int code)
{
	if (code)
	{
		UVEngineNative::NativeScript::callbackDelegate->GlobalCallback->ErrorAndExit(code);
	}
	else
	{
		UVEngineNative::NativeScript::callbackDelegate->GlobalCallback->Exit();
	}
}