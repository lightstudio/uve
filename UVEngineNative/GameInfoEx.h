#include "BasicGameInfo.h"
#include "INI.h"
#include "ScriptParser.h"

using namespace Platform;
using namespace std;

#define DEFAULT_MANIFEST_FILE L"uve-manifestEX.uvm"
#define DEFAULT_FONT_NAME L"default.ttf"
#define DEFAULT_SCRIPT_FILE L"nscript.dat"
#define DEFAULT_RES_PATH L"arc"

namespace UVEngineNative
{
	public ref class GameInfoEx sealed
	{
	public:		
		property int scriptType;//脚本类型
		property String^ ManifestPath 
		{
			String^ get(){ return ref new String(manifestPath.data()); }
			void set(String^ value)
			{
				this->manifestPath= wstring(value->Data());
			}
			
		}
		property String^ GameName
		{
			String^ get(){ return ref new String(gameName.data()); }
			void set(String^ value)
			{
				this->gameName= wstring(value->Data());
			}
		}
		property String^ Company
		{
			String^ get(){ return ref new String(company.data()); }
			void set(String^ value)
			{
				this->company= wstring(value->Data());
			}
		}
		property String^ GameMaker
		{
			String^ get(){ return ref new String(gameMaker.data()); }
			void set(String^ value)
			{
				this->gameMaker= wstring(value->Data());
			}
		}
		property String^ GameSize
		{
			String^ get(){ return ref new String(gameSize.data()); }
			void set(String^ value)
			{
				this->gameSize= wstring(value->Data());
			}
		}
		property String^ ScriptFile
		{
			String^ get(){ return ref new String(scriptFile.data()); }
			void set(String^ value)
			{
				this->scriptFile= wstring(value->Data());
			}
		}
		property String^ DataFolder
		{
			String^ get(){ return ref new String(dataFolder.data()); }
			void set(String^ value)
			{
				this->dataFolder= wstring(value->Data());
			}
		}
		property String^ GamePath
		{
			String^ get(){ return ref new String(gamePath.data()); }
			void set(String^ value)
			{
				this->gamePath= wstring(value->Data());
			}
		}
		property String^ Font
		{
			String^ get(){ return ref new String(font.data());}
			void set(String^ value)
			{
				this->font=wstring(value->Data());
			}
		}
		property String^ ScreenResolution
		{
			String^ get(){ return ref new String(screenResolution.data());}
			void set(String^ value)
			{
				this->screenResolution=wstring(value->Data());
			}
		}
		GameInfoEx(String^ GameFolder);
	internal:
		enum SCT
		{
			SCT_NS=0,
			SCT_RUBY=1
		};
		property wstring manifestPath; //manifest绝对路径
		property wstring gameName; //游戏名称
		property wstring company; //制作公司
		property wstring gameMaker; //移植者/制作者
		property wstring gameSize; //游戏大小
		property wstring scriptFile; //入口脚本文件相对路径
		property wstring dataFolder; //资源文件夹相对路径
		property wstring gamePath; //游戏绝对路径
		property wstring font; //字体相对路径
		property wstring screenResolution; //游戏分辨率 width*height
	private:
		CINI cini;
	};

}