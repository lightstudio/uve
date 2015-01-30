#include "pch.h"
#include "Game.h"
using namespace std;
using namespace Platform;
using namespace UVEngineNative;

Game::Game(String^ gamepath,UVEDelegate^ callbackDelegate,bool ExEnabled)
{
	
	this->gameInfoEx=ref new GameInfoEx(gamepath,ExEnabled);
	this->callbackDelegate=callbackDelegate;
	
	//this->nativeScript=ref new NativeScript(gamepath,callbackDelegate);
}

void Game::Run(double p_X,double p_Y)
{
	this->nativeScript->RunScript(p_X,p_Y);
}