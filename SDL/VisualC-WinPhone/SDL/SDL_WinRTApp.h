#pragma once
#include <SDL_D3D11_STDINC.h>
#include <agile.h>
extern "C" 
{
#include "../src/video/SDL_sysvideo.h"
}
#include "SDL_winphonevideo.h"
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
ref class SDL_WinRTApp sealed 
{
public:
    SDL_WinRTApp();
    

internal:
    // SDL-specific methods
    SDL_DisplayMode GetMainDisplayMode();
    void PumpEvents();
    const SDL_WindowData * GetSDLWindowData() const;
    bool HasSDLWindowData() const;
    void SetRelativeMouseMode(bool enable);
    void SetSDLWindowData(const SDL_WindowData * windowData);
    void SetSDLVideoDevice(const SDL_VideoDevice * videoDevice);
    Windows::Foundation::Point TransformCursor(Windows::Foundation::Point rawPosition);
};
extern WinPhone_RenderData GlobalRenderData;

extern void DECLSPEC OnPointerPressed(Windows::UI::Input::PointerPoint^ p);
extern void DECLSPEC OnPointerReleased(Windows::UI::Input::PointerPoint^ p);
extern void DECLSPEC OnPointerMoved(Windows::UI::Input::PointerPoint ^p);