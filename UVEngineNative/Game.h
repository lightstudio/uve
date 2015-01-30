#include "NativeScript.h"

namespace UVEngineNative
{
	public ref class Game sealed
	{
	public:
		property UVEDelegate^ callbackDelegate
		{
			UVEDelegate^ get()
			{
				return this->nativeScript->callbackDelegate;
			}
			void set(UVEDelegate^ value)
			{
				this->nativeScript->callbackDelegate=value;
			}
		}
		property GameInfoEx^ gameInfoEx
		{
			GameInfoEx^ get()
			{
				return this->nativeScript->gameInfoEx;
			}
			void set (GameInfoEx^ value)
			{
				this->nativeScript->gameInfoEx=value;
			}
		}
		property NativeScript^ nativeScript;
		Game(String^ gamepath,UVEDelegate^ callbackDelegate,bool ExEnabled);
		void Run(double p_X,double p_Y);
	internal:

	private:
		
	};
}