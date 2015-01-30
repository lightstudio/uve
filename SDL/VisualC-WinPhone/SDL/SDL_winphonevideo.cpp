extern "C" {
#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../src/video/SDL_sysvideo.h"
#include "..\..\src\video\SDL_pixels_c.h"
#include "../src/events/SDL_events_c.h"
#include "../src/render/SDL_sysrender.h"
#include "SDL_syswm.h"
}

#include "../include/SDL_WinRTApp.h"
#include "SDL_winphonevideo.h"
#include "..\..\src\video\windowsrt\SDL_winrtevents_c.h"
#include "..\..\src\video\windowsrt\SDL_winrtmouse.h"

#ifdef CreateWindow
#undef CreateWindow
#endif
#define WINRTVID_DRIVER_NAME "winphone"
static int WINRT_VideoInit(_THIS,void *driverdata);
static int WINRT_InitModes(_THIS);
static int WINRT_SetDisplayMode(_THIS, SDL_VideoDisplay * display, SDL_DisplayMode * mode);
static void WINRT_VideoQuit(_THIS);

/* Window functions */
static int WINRT_CreateWindow(_THIS, SDL_Window * window);
static void WINRT_DestroyWindow(_THIS, SDL_Window * window);
static SDL_bool WINRT_GetWindowWMInfo(_THIS, SDL_Window * window, SDL_SysWMinfo * info);
void WINRT_PumpEvents(_THIS);

static int
WINRT_Available(void)
{
    return (1);
}

static void
WINRT_DeleteDevice(SDL_VideoDevice * device)
{
    SetSDLVideoDevice(NULL);
    SDL_free(device);
}
int SDL_WINRT_UpdateWindowFramebuffer(SDL_VideoDevice *_this ,SDL_Window * window,const SDL_Rect *rects,int numrects)
{
	D3DSharedData *data=(D3DSharedData *)_this->driverdata;
	SDL_Renderer* renderer=data->renderer;
	if(window->surface_valid)
	{
		auto texture=SDL_CreateTextureFromSurface(renderer,window->surface);
		SDL_RenderCopy(renderer,texture,NULL,NULL);
		return 0;
	}
	else 
	{
		return SDL_SetError("surface not valid");
	}
}
static SDL_VideoDevice *
WINRT_CreateDevice(int devindex,void *driverdata)
{
    SDL_VideoDevice *device;

    /* Initialize all variables that we clean on shutdown */
    device = (SDL_VideoDevice *) SDL_calloc(1, sizeof(SDL_VideoDevice));
    if (!device) {
        SDL_OutOfMemory();
        if (device) {
            SDL_free(device);
        }
        return (0);
    }

    /* Set the function pointers */
    device->VideoInit = WINRT_VideoInit;
    device->VideoQuit = WINRT_VideoQuit;
    device->CreateWindow = WINRT_CreateWindow;
    device->DestroyWindow = WINRT_DestroyWindow;
    device->SetDisplayMode = WINRT_SetDisplayMode;
    device->PumpEvents = WINRT_PumpEvents;
    //device->CreateWindowFramebuffer = SDL_WINRT_CreateWindowFramebuffer;
    device->UpdateWindowFramebuffer = SDL_WINRT_UpdateWindowFramebuffer;
    //device->DestroyWindowFramebuffer = SDL_WINRT_DestroyWindowFramebuffer;
    device->GetWindowWMInfo = WINRT_GetWindowWMInfo;
    device->free = WINRT_DeleteDevice;
	device->driverdata=driverdata;

    SetSDLVideoDevice(device);

    return device;
}
void WINRT_PumpEvents(_THIS)
{

}
VideoBootStrap WINPHONE_bootstrap = {
    WINRTVID_DRIVER_NAME, "SDL Windows Phone 8 video driver",
    WINRT_Available, WINRT_CreateDevice
};

int
WINRT_VideoInit(_THIS,void *driverdata)
{
    // TODO, WinRT: consider adding a hack to wait (here) for the app's orientation to finish getting set (before the initial display mode is set up)

    if (WINRT_InitModes(_this) < 0) {
        return -1;
    }

    return 0;
}

static int
WINRT_InitModes(_THIS)
{
    SDL_DisplayMode mode = GetMainDisplayMode();
    if (SDL_AddBasicVideoDisplay(&mode) < 0) {
        return -1;
    }

    SDL_AddDisplayMode(&_this->displays[0], &mode);
    return 0;
}

static int
WINRT_SetDisplayMode(_THIS, SDL_VideoDisplay * display, SDL_DisplayMode * mode)
{
    return 0;
}

void
WINRT_VideoQuit(_THIS)
{
	
}

int
WINRT_CreateWindow(_THIS, SDL_Window * window)
{
    // Make sure that only one window gets created, at least until multimonitor
    // support is added.
    if (HasSDLWindowData())
    {
        SDL_SetError("WinRT only supports one window");
        return -1;
    }

//    SDL_WindowData *data = new SDL_WindowData;
    //if (!data) {
    //    SDL_OutOfMemory();
    //    return -1;
    //}
	window->driverdata = _this->driverdata;
    //data->sdlWindow = window;
	//data->m_backbuffer = GlobalRenderData.SynchronizedTexure;

    /* Make sure the window is considered to be positioned at {0,0},
       and is considered fullscreen, shown, and the like.
    */
    window->x = 0;
    window->y = 0;
    window->flags =
        SDL_WINDOW_FULLSCREEN |
        SDL_WINDOW_SHOWN |
        SDL_WINDOW_BORDERLESS |
        SDL_WINDOW_MAXIMIZED |
        SDL_WINDOW_INPUT_GRABBED;

    /* HACK from DLudwig: The following line of code prevents
       SDL_CreateWindow and SDL_UpdateFullscreenMode from trying to resize
       the window after the call to WINRT_CreateWindow returns.

       This hack should allow a window to be created in virtually any size,
       and more importantly, it allows a window's framebuffer, as created and
       retrieved via SDL_GetWindowSurface, to be in any size.  This can be
       utilized by apps centered around software rendering, such as ports
       of older apps.  The app can have SDL create a framebuffer in any size
       it chooses.  SDL will scale the framebuffer to the native
       screen size on the GPU (via SDL_UpdateWindowSurface).
    */
    _this->displays[0].fullscreen_window = window;

    /* Further prevent any display resizing, and make sure SDL_GetWindowDisplayMode
       can report the correct size of windows, by creating a new display
       mode in the requested size.  To note, if the window is being created in
       the device's native screen size, SDL_AddDisplayMode will do nothing.
    */
    window->fullscreen_mode = GetMainDisplayMode();
    window->fullscreen_mode.w = window->w;
    window->fullscreen_mode.h = window->h;
    SDL_AddDisplayMode(&_this->displays[0], &window->fullscreen_mode);

    /* TODO: Consider removing custom display modes in WINRT_DestroyWindow. */

    /* Make sure the WinRT app's IFramworkView can post events on
       behalf of SDL:
    */
//    SetSDLWindowData(data);

    /* All done! */
    return 0;
}

void
WINRT_DestroyWindow(_THIS, SDL_Window * window)
{
	D3DSharedData * data = (D3DSharedData *) window->driverdata;

    if (HasSDLWindowData() &&
        GetSDLWindowData()->sdlWindow == window)
    {
        SetSDLWindowData(NULL);
    }

    if (data) {
        // Delete the internal window data:
        delete data;
        data = NULL;
    }
}

SDL_bool
WINRT_GetWindowWMInfo(_THIS, SDL_Window * window, SDL_SysWMinfo * info)
{
    D3DSharedData * data = (D3DSharedData *) window->driverdata;

    if (info->version.major <= SDL_MAJOR_VERSION) {
        info->subsystem = SDL_SYSWM_WINDOWSRT;
		info->info.winphone.window = data;
        return SDL_TRUE;
    } else {
        SDL_SetError("Application not compiled with SDL %d.%d\n",
                     SDL_MAJOR_VERSION, SDL_MINOR_VERSION);
        return SDL_FALSE;
    }
    return SDL_FALSE;
}



