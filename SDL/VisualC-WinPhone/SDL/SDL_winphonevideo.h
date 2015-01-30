#include "SDL_config.h"
#ifndef _SDL_winphonevideo_h
#define _SDL_winphonevideo_h

extern "C" {
#include "../src/video/SDL_sysvideo.h"
}

#include <agile.h>
#include "SDL_D3D11_STDINC.h"
#include "SharedData.h"
typedef struct 
{
    SDL_Window *sdlWindow;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_backbuffer;
}SDL_WindowData;

#endif /* _SDL_winrtvideo_h */

/* vi: set ts=4 sw=4 expandtab: */
