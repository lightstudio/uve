using namespace Platform;
using namespace std;

namespace UVEngineNative
{
	public ref class BasicGameInfo sealed
	{
	public:
		property int scriptType;
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
		property String^ Type
		{
			String^ get(){ return ref new String(type.data()); }
			void set(String^ value)
			{
				this->type= wstring(value->Data());
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
		property String^ Icon
		{
			String^ get(){ return ref new String(icon.data()); }
			void set(String^ value)
			{
				this->icon= wstring(value->Data());
			}
		}
		property String^ Tile
		{
			String^ get(){ return ref new String(tile.data()); }
			void set(String^ value)
			{
				this->tile= wstring(value->Data());
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
		BasicGameInfo(String^ GameFolder);
	internal:
		BasicGameInfo(wstring GameFolder);
	private:
		wstring manifestPath;
		wstring gameName;
		wstring company;
		wstring gameMaker;
		wstring gameSize;
		wstring type;
		wstring scriptFile;
		wstring dataFolder;
		wstring icon;
		wstring tile;
		wstring gamePath;
	};
}