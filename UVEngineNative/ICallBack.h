#pragma once
using namespace Platform;
using namespace std;

namespace UVEngineNative
{
	public interface class ICallback
	{
	public:
		//void DrawText(String^ text);
		///*void PlayBGM(String^ path, String^ title, String^ artist, String^ album);
		//void PlayDWave(String^ path, int num);*/
		//void Print(int effectnum);
		///*void SetFont(String^ path);*/
		//void InitRuby(String^ path);
		//void CallRuby(String^ call);
		//void ErrorAndExit(int errorcode);
		//void Exit();
		void LogOutput(String^ str);
		void VideoPlay(String^ path);
		void ErrorAndExit(int code);
		void Exit();
	};

}

