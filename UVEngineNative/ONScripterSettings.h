#pragma once
#include "INI.h"
using namespace Platform;
using namespace std;

static CINI* m_ini;

namespace UVEngineNative
{
	public ref class ONScripterSettings sealed
	{
	public:
		static property bool playExternVideo
		{
			bool get()
			{
				return m_ini->ReadBool("config","extVideo",false);
			}
			void set(bool value)
			{
				m_ini->WriteBool("config","extVideo",value);
				m_ini->WriteFileNow();
			}
		}
		static property bool logOutput
		{
			bool get()
			{
				return m_ini->ReadBool("config","logOutput",false);
			}
			void set(bool value)
			{
				m_ini->WriteBool("config","logOutput",value);
				m_ini->WriteFileNow();
			}
		}
		static void Initialize();
	};
}