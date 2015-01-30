#include "SDL_config.h"

#if SDL_VIDEO_DRIVER_WINRT

/* WinRT SDL video driver implementation

   Initial work on this was done by David Ludwig (dludwig@pobox.com), and
   was based off of SDL's "dummy" video driver.
 */

extern "C" {
#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../src/video/SDL_sysvideo.h"
#include "..\..\src\video\SDL_pixels_c.h"
#include "../src/events/SDL_events_c.h"
#include "../src/render/SDL_sysrender.h"
#include "SDL_syswm.h"
}

#include "SDL_WinRTApp.h"
#include "..\..\src\video\windowsrt\SDL_winrtvideo.h"
#include "..\..\src\video\windowsrt\SDL_winrtevents_c.h"
#include "..\..\src\video\windowsrt\SDL_winrtmouse.h"

using namespace Windows::UI::Core;

/* On Windows, windows.h defines CreateWindow */
#ifdef CreateWindow
#undef CreateWindow
#endif

extern SDL_WinRTApp ^ SDL_WinRTGlobalApp;

#define WINRTVID_DRIVER_NAME "winrt"


/* Initialization/Query functions */
static int WINRT_VideoInit(_THIS);
static int WINRT_InitModes(_THIS);
static int WINRT_SetDisplayMode(_THIS, SDL_VideoDisplay * display, SDL_DisplayMode * mode);
static void WINRT_VideoQuit(_THIS);

/* Window functions */
static int WINRT_CreateWindow(_THIS, SDL_Window * window);
static void WINRT_DestroyWindow(_THIS, SDL_Window * window);
static SDL_bool WINRT_GetWindowWMInfo(_THIS, SDL_Window * window, SDL_SysWMinfo * info);


static int
WINRT_Available(void)
{
	return 1;
}
static void
WINRT_DeleteDevice(SDL_VideoDevice * device)
{
	SDL_WinRTGlobalApp->SetSDLVideoDevice(NULL);
    SDL_free(device);
}

static SDL_VideoDevice *
WINRT_CreateDevice(int devindex)
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
    //device->UpdateWindowFramebuffer = SDL_WINRT_UpdateWindowFramebuffer;
    //device->DestroyWindowFramebuffer = SDL_WINRT_DestroyWindowFramebuffer;
    device->GetWindowWMInfo = WINRT_GetWindowWMInfo;
    device->free = WINRT_DeleteDevice;

    SDL_WinRTGlobalApp->SetSDLVideoDevice(device);

    return device;
}
VideoBootStrap WINRT_bootstrap = {
    WINRTVID_DRIVER_NAME, "SDL Windows RT video driver",
    WINRT_Available, WINRT_CreateDevice
};
int
WINRT_VideoInit(_THIS)
{
	if (WINRT_InitModes(_this) < 0) {
        return -1;
    }
    WINRT_InitMouse(_this);

    return 0;
}
static int
WINRT_InitModes(_THIS)
{
	SDL_DisplayMode mode = SDL_WinRTGlobalApp->GetMainDisplayMode();
    if (SDL_AddBasicVideoDisplay(&mode) < 0) {
        return -1;
    }

    SDL_AddDisplayMode(&_this->displays[0], &mode);
    return 0;
}
static int
WINRT_SetDisplayMode(_THIS, SDL_VideoDisplay * display, SDL_DisplayMode * mode)
{

}
void
WINRT_VideoQuit(_THIS)
{

}
int
WINRT_CreateWindow(_THIS, SDL_Window * window)
{

}
void
WINRT_DestroyWindow(_THIS, SDL_Window * window)
{

}
SDL_bool
WINRT_GetWindowWMInfo(_THIS, SDL_Window * window, SDL_SysWMinfo * info)
{

}



#endif