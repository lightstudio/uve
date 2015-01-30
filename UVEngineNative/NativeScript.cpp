#include "pch.h"
#include "NativeScript.h"
#include "ONScripter.h"

using namespace Platform;
using namespace std;
using namespace UVEngineNative;
using namespace Windows::Storage;

#define TMP_SCRIPT_BUF_LEN 4096
#define STRING_BUFFER_LENGTH 2048

#define SKIP_SPACE(p) while ( *(p) == ' ' || *(p) == '\t' ) (p)++



NativeScript::NativeScript(String^ gameFolder,UVEDelegate^ uve_delegate)
{
	////this->gameInfoEx=ref new GameInfoEx(gameFolder);
	////this->callbackDelegate=uve_delegate;
	////String^ scriptPath_Absolute=ApplicationData::Current->LocalFolder->Path+L'\\'+gameFolder;
	//////_wfopen_s(&scriptFile,scriptPath_Absolute->Data(),L"r");
	////scriptFile.open(scriptPath_Absolute->Data(),ios::in);
	////this->callbackDelegate->GlobalCallback->DrawText(gameInfoEx->GameName);
	////this->ScriptInit();
	//d3dbg=ref new PhoneDirect3DXamlAppComponent::Direct3DBackground();
	//gamepath_relative=ws2s(gameFolder->Data());
	//ons=new ONScripter();
	//
	//SDL_main(*ons);


}

NativeScript::~NativeScript()
{
	this->scriptFile.close();
	/*delete[] this->scriptFile;*/



	return;
}

void NativeScript::RunScript(double p_X,double p_Y)
{
	
}

void NativeScript::SendCommand(string command,wstring* params,int param_count)
{
	
}
void NativeScript::TouchPoint(double p_X,double p_Y)
{

}

bool NativeScript::BackKey()
{
	return false;
}

void NativeScript::ScriptInit()
{
	/*while(true)
	{
		
		char* line=new char[1000];
		for (int i=0;i<1000;i++)
		{
			line[i]=0;
		}
		scriptFile.getline(line,1000);
		if (line[0]=='*'&&
			line[1]=='d'&&
			line[2]=='e'&&
			line[4]=='f'&&
			line[3]=='i'&&
			line[5]=='n'&&
			line[6]=='e'&&
			line[7]==0)
		{
			delete[] line;
			this->callbackDelegate->GlobalCallback->DrawText("defined");
			break;
		}
		delete[] line;
	}*/
}



