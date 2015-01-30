#include "pch.h"
#include "UVE_DX.h"
#include "Direct3DContentProvider.h"
//#include  "F:/Documents/Open Source Projects/DavidLudwig-sdl-25883bdf3cab/include/SDL_WinRTApp.h"
#include "../include/SDL_WinRTApp.h"
#include <ppltasks.h>
#include "debug.h"
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Microsoft::WRL;
using namespace Windows::Graphics::Display;
using namespace Windows::Phone::Graphics::Interop;
using namespace Windows::Phone::Input::Interop;
using namespace UVEngineNative;
using namespace concurrency;
using namespace Windows::Devices::Sensors;
using namespace Windows::Storage;
extern SDL_Renderer* GlobalRenderer;
string gamepath_relative;
_iobuf* stderr_fp;
//
//static void WINRT_SetDisplayOrientationsPreference(const char *name, const char *oldValue, const char *newValue)
//{
//    SDL_assert(SDL_strcmp(name, SDL_HINT_ORIENTATIONS) == 0);
//
//    // Start with no orientation flags, then add each in as they're parsed
//    // from newValue.
//    unsigned int orientationFlags = 0;
//    std::istringstream tokenizer(newValue);
//    while (!tokenizer.eof()) {
//        std::string orientationName;
//        std::getline(tokenizer, orientationName, ' ');
//        if (orientationName == "LandscapeLeft") {
//            orientationFlags |= (unsigned int) DisplayOrientations::LandscapeFlipped;
//        } else if (orientationName == "LandscapeRight") {
//            orientationFlags |= (unsigned int) DisplayOrientations::Landscape;
//        } else if (orientationName == "Portrait") {
//            orientationFlags |= (unsigned int) DisplayOrientations::Portrait;
//        } else if (orientationName == "PortraitUpsideDown") {
//            orientationFlags |= (unsigned int) DisplayOrientations::PortraitFlipped;
//        }
//    }
//
//    // If no valid orientation flags were specified, use a reasonable set of defaults:
//    if (!orientationFlags) {
//        // TODO, WinRT: consider seeing if an app's default orientation flags can be found out via some API call(s).
//        orientationFlags = (unsigned int) ( \
//            DisplayOrientations::Landscape |
//            DisplayOrientations::LandscapeFlipped |
//            DisplayOrientations::Portrait |
//            DisplayOrientations::PortraitFlipped);
//    }
//
//    // Set the orientation/rotation preferences.  Please note that this does
//    // not constitute a 100%-certain lock of a given set of possible
//    // orientations.  According to Microsoft's documentation on Windows RT [1]
//    // when a device is not capable of being rotated, Windows may ignore
//    // the orientation preferences, and stick to what the device is capable of
//    // displaying.
//    //
//    // [1] Documentation on the 'InitialRotationPreference' setting for a
//    // Windows app's manifest file describes how some orientation/rotation
//    // preferences may be ignored.  See
//    // http://msdn.microsoft.com/en-us/library/windows/apps/hh700343.aspx
//    // for details.  Microsoft's "Display orientation sample" also gives an
//    // outline of how Windows treats device rotation
//    // (http://code.msdn.microsoft.com/Display-Orientation-Sample-19a58e93).
//    DisplayProperties::AutoRotationPreferences = (DisplayOrientations) orientationFlags;
//}


Direct3DInterop::Direct3DInterop(Platform::String^ gamepath) :
m_timer(ref new BasicTimer()),
isLoaded(false)
{
	//gamepath_relative=ws2s(wstring(gamepath->Data()));
	wstring wsPath = wstring(gamepath->Data());
	string s = ws2s(wsPath);
	gamepath_relative = ws2s(wsPath);
	ons = new ONScripter();
	//stderr_fp=fopen((string(GAMEPATH)+"stderr.txt").data(),"rb");
	//SetSDL(); 
	//m_renderer->SetPrivateRenderer(GlobalRenderer);//SIG07
	//data = (D3D11_RenderData *) ((*ons).renderer->driverdata);
	//SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight Portrait PortraitUpsideDown");
	//   SDL_RegisterHintChangedCb(SDL_HINT_ORIENTATIONS, WINRT_SetDisplayOrientationsPreference);

}


IDrawingSurfaceContentProvider^ Direct3DInterop::CreateContentProvider()
{
	ComPtr<Direct3DContentProvider> provider = Make<Direct3DContentProvider>(this);
	return reinterpret_cast<IDrawingSurfaceContentProvider^>(provider.Get());
}

// IDrawingSurfaceManipulationHandler
void Direct3DInterop::SetManipulationHost(DrawingSurfaceManipulationHost^ manipulationHost)
{
	manipulationHost->PointerPressed +=
		ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DInterop::OnPointerPressed);

	manipulationHost->PointerMoved +=
		ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DInterop::OnPointerMoved);

	manipulationHost->PointerReleased +=
		ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DInterop::OnPointerReleased);

}

void Direct3DInterop::BackKeyPressed()
{
	SDL_BackKeyPressed();
	//this->ons->quit();
}

void Direct3DInterop::RenderResolution::set(Windows::Foundation::Size renderResolution)
{
	if (renderResolution.Width != m_renderResolution.Width ||
		renderResolution.Height != m_renderResolution.Height)
	{
		m_renderResolution = renderResolution;

		if (m_renderer)
		{
			m_renderer->UpdateForRenderResolutionChange(m_renderResolution.Width, m_renderResolution.Height);
			RecreateSynchronizedTexture();
		}
	}
}

// 事件处理程序
void Direct3DInterop::OnPointerPressed(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
{
	// 在此处插入代码。
	SDL_OnPointerPressed(args->CurrentPoint, ons->window);
}

void Direct3DInterop::OnPointerMoved(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
{
	// 在此处插入代码。
	SDL_OnPointerMoved(args->CurrentPoint, ons->window);
}

void Direct3DInterop::OnPointerReleased(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
{
	// 在此处插入代码。
	SDL_OnPointerReleased(args->CurrentPoint, ons->window);
}

// 与 Direct3DContentProvider 交互
HRESULT Direct3DInterop::Connect(_In_ IDrawingSurfaceRuntimeHostNative* host)
{
	m_renderer = ref new SDLRenderer();
	m_renderer->Initialize();
	m_renderer->UpdateForWindowSizeChange(WindowBounds.Width, WindowBounds.Height);
	m_renderer->UpdateForRenderResolutionChange(m_renderResolution.Width, m_renderResolution.Height);
	//void* sdata = (void*)&(m_renderer->SharedData);
	if (ons != NULL) ons->renderer = m_renderer->SharedData.renderer;
	// 在呈现器完成初始化后重新启动计时器。
	//m_timer->Reset();
	return S_OK;
}

void Direct3DInterop::Disconnect()
{
	m_renderer = nullptr;
}

HRESULT Direct3DInterop::PrepareResources(_In_ const LARGE_INTEGER* presentTargetTime, _Out_ BOOL* contentDirty)
{
	*contentDirty = true;
	if (!isLoaded)
	{
		auto task = create_async([this]()
		{
			isLoaded = true;
			SDL_main(ons, (void *)(&(m_renderer->SharedData)));
			//fake_main((void *)(&(m_renderer->SharedData)));
		});
	}
	return S_OK;
}

HRESULT Direct3DInterop::GetTexture(_In_ const DrawingSurfaceSizeF* size, _Inout_ IDrawingSurfaceSynchronizedTextureNative** synchronizedTexture, _Inout_ DrawingSurfaceRectF* textureSubRectangle)
{
	m_timer->Update();
	m_renderer->Update(m_timer->Total, m_timer->Delta);
	m_renderer->Render();

	RequestAdditionalFrame();

	return S_OK;
}

ID3D11Texture2D* Direct3DInterop::GetTexture()
{

	return m_renderer->GetTexture();
}

void Direct3DInterop::InitGlobalCallback(INativeCalls^ inc)
{
	GlobalNativeCallback = new NativeCallback(inc);
	const char* stderrPath = FILEPATH(gamepath_relative + "\\stderr.txt");
	stderr_fp = fopen(stderrPath, "rb");
}
void Direct3DInterop::Exited()
{
	m_renderer->Exit();
}
void Direct3DInterop::DeActivated()
{

}
void Direct3DInterop::Activated()
{
	//ons->screen_surface = SDL_SetVideoMode( screen_width, screen_height, screen_bpp, DEFAULT_VIDEO_SURFACE_FLAG );
	//ons->repaintCommand();
}
void Direct3DInterop::CtrlPressed()
{
	SDL_CtrlPressed();
}
void Direct3DInterop::CtrlReleased()
{
	SDL_CtrlReleased();
}
void Direct3DInterop::Quit()
{
	//ons->quit();
	ons->endCommand();
}
void Direct3DInterop::SetONSResolutionAndDisplayMode(int devicew, int deviceh, int xamlw, int xamlh, int flags)
{
	SDL_SetDeviceResolution(devicew, deviceh);
	ons->SetScreenResolution(devicew, deviceh);
	SDL_SetXamlResolution(xamlw, xamlh);
	ons->SetDisplayMode(flags);
}

void Direct3DInterop::Load()
{
	auto task = create_async([this]()
	{
		//ons->system_menu_mode=4;
		//ons->executeSystemCall();
	});
}

void Direct3DInterop::Save()
{
	auto task = create_async([this]()
	{
		//ons->system_menu_mode=3;
		//ons->executeSystemCall();
	});
}