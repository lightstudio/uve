#pragma once
#include "SDL_D3D11_STDINC.h"
#include <agile.h>
extern "C" 
{
#include "../src/video/SDL_sysvideo.h"
}
#include "../VisualC-WinPhone/SDL/SDL_winphonevideo.h"
typedef struct 
{
	Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> SynchronizedTexure;
}WinPhone_RenderData;
typedef struct 
{
	bool m_windowClosed;
    bool m_windowVisible;
    const SDL_WindowData* m_sdlWindowData;
    const SDL_VideoDevice* m_sdlVideoDevice;
    bool m_useRelativeMouseMode;
}SDL_RData;
extern SDL_RData GlobalRData;


// SDL-specific methods
extern SDL_DisplayMode GetMainDisplayMode();
extern void PumpEvents();
extern const SDL_WindowData * GetSDLWindowData() ;
extern bool HasSDLWindowData() ;
extern void SetRelativeMouseMode(bool enable);
extern void SetSDLWindowData(const SDL_WindowData * windowData);
extern void SetSDLVideoDevice(const SDL_VideoDevice * videoDevice);
extern Windows::Foundation::Point TransformCursor(Windows::Foundation::Point rawPosition,SDL_Window* wnd);
extern WinPhone_RenderData GlobalRenderData;

extern void DECLSPEC SDL_OnPointerPressed(Windows::UI::Input::PointerPoint^ p,SDL_Window* wnd);
extern void DECLSPEC SDL_OnPointerReleased(Windows::UI::Input::PointerPoint^ p,SDL_Window* wnd);
extern void DECLSPEC SDL_OnPointerMoved(Windows::UI::Input::PointerPoint ^p,SDL_Window * wnd);
extern void DECLSPEC SDL_BackKeyPressed();
extern void DECLSPEC SDL_OnSuspending();
extern void DECLSPEC SDL_OnResuming();
extern void DECLSPEC SDL_CtrlPressed();
extern void DECLSPEC SDL_CtrlReleased();
extern void DECLSPEC SDL_SetGameResolution(int w,int h);
extern void DECLSPEC SDL_SetXamlResolution(int w,int h);
extern void DECLSPEC SDL_SetDeviceResolution(int w, int h);
extern int gamew,gameh;
extern int xamlw,xamlh;