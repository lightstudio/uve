
#include <string>
#include <unordered_map>
#include <sstream>

#include "ppltasks.h"


extern "C" {
#include "SDL_assert.h"
#include "SDL_events.h"
#include "SDL_hints.h"
#include "SDL_log.h"
#include "SDL_stdinc.h"
#include "SDL_render.h"
#include "../src/video/SDL_sysvideo.h"
#include "../src/SDL_hints_c.h"
#include "../src/events/scancodes_windows.h"
#include "../src/events/SDL_mouse_c.h"
#include "../src/events/SDL_keyboard_c.h"
#include "../src/events/SDL_windowevents_c.h"
#include "../src/render/SDL_sysrender.h"
#include "SDL.h"
}
#include "SDL_winphone_videodata.h"
#include "SDL_winphonevideo.h"
#include "../include/SDL_WinRTApp.h"

using namespace concurrency;
using namespace std;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Devices::Input;
using namespace Windows::Graphics::Display;
using namespace Windows::Foundation;
using namespace Windows::System;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
SDL_RData GlobalRData={0};
WinPhone_RenderData GlobalRenderData;
// Compile-time debugging options:
// To enable, uncomment; to disable, comment them out.
//#define LOG_POINTER_EVENTS 1
//#define LOG_WINDOW_EVENTS 1
//#define LOG_ORIENTATION_EVENTS 1
static Uint8
WINRT_GetSDLButtonForPointerPoint(PointerPoint ^ pt);
static void
WINRT_LogPointerEvent(const string & header, PointerPoint^ pt, Point transformedPoint);
// HACK, DLudwig: The C-style main() will get loaded via the app's
// WinRT-styled main(), which is part of SDLmain_for_WinRT.cpp.
// This seems wrong on some level, but does seem to work.
static SDL_Scancode WinRT_Official_Keycodes[] = {
    SDL_SCANCODE_UNKNOWN, // VirtualKey.None -- 0
    SDL_SCANCODE_UNKNOWN, // VirtualKey.LeftButton -- 1
    SDL_SCANCODE_UNKNOWN, // VirtualKey.RightButton -- 2
    SDL_SCANCODE_CANCEL, // VirtualKey.Cancel -- 3
    SDL_SCANCODE_UNKNOWN, // VirtualKey.MiddleButton -- 4
    SDL_SCANCODE_UNKNOWN, // VirtualKey.XButton1 -- 5
    SDL_SCANCODE_UNKNOWN, // VirtualKey.XButton2 -- 6
    SDL_SCANCODE_UNKNOWN, // -- 7
    SDL_SCANCODE_BACKSPACE, // VirtualKey.Back -- 8
    SDL_SCANCODE_TAB, // VirtualKey.Tab -- 9
    SDL_SCANCODE_UNKNOWN, // -- 10
    SDL_SCANCODE_UNKNOWN, // -- 11
    SDL_SCANCODE_CLEAR, // VirtualKey.Clear -- 12
    SDL_SCANCODE_RETURN, // VirtualKey.Enter -- 13
    SDL_SCANCODE_UNKNOWN, // -- 14
    SDL_SCANCODE_UNKNOWN, // -- 15
    SDL_SCANCODE_LSHIFT, // VirtualKey.Shift -- 16
    SDL_SCANCODE_LCTRL, // VirtualKey.Control -- 17
    SDL_SCANCODE_MENU, // VirtualKey.Menu -- 18
    SDL_SCANCODE_PAUSE, // VirtualKey.Pause -- 19
    SDL_SCANCODE_CAPSLOCK, // VirtualKey.CapitalLock -- 20
    SDL_SCANCODE_UNKNOWN, // VirtualKey.Kana or VirtualKey.Hangul -- 21
    SDL_SCANCODE_UNKNOWN, // -- 22
    SDL_SCANCODE_UNKNOWN, // VirtualKey.Junja -- 23
    SDL_SCANCODE_UNKNOWN, // VirtualKey.Final -- 24
    SDL_SCANCODE_UNKNOWN, // VirtualKey.Hanja or VirtualKey.Kanji -- 25
    SDL_SCANCODE_UNKNOWN, // -- 26
    SDL_SCANCODE_ESCAPE, // VirtualKey.Escape -- 27
    SDL_SCANCODE_UNKNOWN, // VirtualKey.Convert -- 28
    SDL_SCANCODE_UNKNOWN, // VirtualKey.NonConvert -- 29
    SDL_SCANCODE_UNKNOWN, // VirtualKey.Accept -- 30
    SDL_SCANCODE_UNKNOWN, // VirtualKey.ModeChange -- 31  (maybe SDL_SCANCODE_MODE ?)
    SDL_SCANCODE_SPACE, // VirtualKey.Space -- 32
    SDL_SCANCODE_PAGEUP, // VirtualKey.PageUp -- 33
    SDL_SCANCODE_PAGEDOWN, // VirtualKey.PageDown -- 34
    SDL_SCANCODE_END, // VirtualKey.End -- 35
    SDL_SCANCODE_HOME, // VirtualKey.Home -- 36
    SDL_SCANCODE_LEFT, // VirtualKey.Left -- 37
    SDL_SCANCODE_UP, // VirtualKey.Up -- 38
    SDL_SCANCODE_RIGHT, // VirtualKey.Right -- 39
    SDL_SCANCODE_DOWN, // VirtualKey.Down -- 40
    SDL_SCANCODE_SELECT, // VirtualKey.Select -- 41
    SDL_SCANCODE_UNKNOWN, // VirtualKey.Print -- 42  (maybe SDL_SCANCODE_PRINTSCREEN ?)
    SDL_SCANCODE_EXECUTE, // VirtualKey.Execute -- 43
    SDL_SCANCODE_UNKNOWN, // VirtualKey.Snapshot -- 44
    SDL_SCANCODE_INSERT, // VirtualKey.Insert -- 45
    SDL_SCANCODE_DELETE, // VirtualKey.Delete -- 46
    SDL_SCANCODE_HELP, // VirtualKey.Help -- 47
    SDL_SCANCODE_0, // VirtualKey.Number0 -- 48
    SDL_SCANCODE_1, // VirtualKey.Number1 -- 49
    SDL_SCANCODE_2, // VirtualKey.Number2 -- 50
    SDL_SCANCODE_3, // VirtualKey.Number3 -- 51
    SDL_SCANCODE_4, // VirtualKey.Number4 -- 52
    SDL_SCANCODE_5, // VirtualKey.Number5 -- 53
    SDL_SCANCODE_6, // VirtualKey.Number6 -- 54
    SDL_SCANCODE_7, // VirtualKey.Number7 -- 55
    SDL_SCANCODE_8, // VirtualKey.Number8 -- 56
    SDL_SCANCODE_9, // VirtualKey.Number9 -- 57
    SDL_SCANCODE_UNKNOWN, // -- 58
    SDL_SCANCODE_UNKNOWN, // -- 59
    SDL_SCANCODE_UNKNOWN, // -- 60
    SDL_SCANCODE_UNKNOWN, // -- 61
    SDL_SCANCODE_UNKNOWN, // -- 62
    SDL_SCANCODE_UNKNOWN, // -- 63
    SDL_SCANCODE_UNKNOWN, // -- 64
    SDL_SCANCODE_A, // VirtualKey.A -- 65
    SDL_SCANCODE_B, // VirtualKey.B -- 66
    SDL_SCANCODE_C, // VirtualKey.C -- 67
    SDL_SCANCODE_D, // VirtualKey.D -- 68
    SDL_SCANCODE_E, // VirtualKey.E -- 69
    SDL_SCANCODE_F, // VirtualKey.F -- 70
    SDL_SCANCODE_G, // VirtualKey.G -- 71
    SDL_SCANCODE_H, // VirtualKey.H -- 72
    SDL_SCANCODE_I, // VirtualKey.I -- 73
    SDL_SCANCODE_J, // VirtualKey.J -- 74
    SDL_SCANCODE_K, // VirtualKey.K -- 75
    SDL_SCANCODE_L, // VirtualKey.L -- 76
    SDL_SCANCODE_M, // VirtualKey.M -- 77
    SDL_SCANCODE_N, // VirtualKey.N -- 78
    SDL_SCANCODE_O, // VirtualKey.O -- 79
    SDL_SCANCODE_P, // VirtualKey.P -- 80
    SDL_SCANCODE_Q, // VirtualKey.Q -- 81
    SDL_SCANCODE_R, // VirtualKey.R -- 82
    SDL_SCANCODE_S, // VirtualKey.S -- 83
    SDL_SCANCODE_T, // VirtualKey.T -- 84
    SDL_SCANCODE_U, // VirtualKey.U -- 85
    SDL_SCANCODE_V, // VirtualKey.V -- 86
    SDL_SCANCODE_W, // VirtualKey.W -- 87
    SDL_SCANCODE_X, // VirtualKey.X -- 88
    SDL_SCANCODE_Y, // VirtualKey.Y -- 89
    SDL_SCANCODE_Z, // VirtualKey.Z -- 90
    SDL_SCANCODE_UNKNOWN, // VirtualKey.LeftWindows -- 91  (maybe SDL_SCANCODE_APPLICATION or SDL_SCANCODE_LGUI ?)
    SDL_SCANCODE_UNKNOWN, // VirtualKey.RightWindows -- 92  (maybe SDL_SCANCODE_APPLICATION or SDL_SCANCODE_RGUI ?)
    SDL_SCANCODE_APPLICATION, // VirtualKey.Application -- 93
    SDL_SCANCODE_UNKNOWN, // -- 94
    SDL_SCANCODE_SLEEP, // VirtualKey.Sleep -- 95
    SDL_SCANCODE_KP_0, // VirtualKey.NumberPad0 -- 96
    SDL_SCANCODE_KP_1, // VirtualKey.NumberPad1 -- 97
    SDL_SCANCODE_KP_2, // VirtualKey.NumberPad2 -- 98
    SDL_SCANCODE_KP_3, // VirtualKey.NumberPad3 -- 99
    SDL_SCANCODE_KP_4, // VirtualKey.NumberPad4 -- 100
    SDL_SCANCODE_KP_5, // VirtualKey.NumberPad5 -- 101
    SDL_SCANCODE_KP_6, // VirtualKey.NumberPad6 -- 102
    SDL_SCANCODE_KP_7, // VirtualKey.NumberPad7 -- 103
    SDL_SCANCODE_KP_8, // VirtualKey.NumberPad8 -- 104
    SDL_SCANCODE_KP_9, // VirtualKey.NumberPad9 -- 105
    SDL_SCANCODE_KP_MULTIPLY, // VirtualKey.Multiply -- 106
    SDL_SCANCODE_KP_PLUS, // VirtualKey.Add -- 107
    SDL_SCANCODE_UNKNOWN, // VirtualKey.Separator -- 108
    SDL_SCANCODE_KP_MINUS, // VirtualKey.Subtract -- 109
    SDL_SCANCODE_UNKNOWN, // VirtualKey.Decimal -- 110  (maybe SDL_SCANCODE_DECIMALSEPARATOR, SDL_SCANCODE_KP_DECIMAL, or SDL_SCANCODE_KP_PERIOD ?)
    SDL_SCANCODE_KP_DIVIDE, // VirtualKey.Divide -- 111
    SDL_SCANCODE_F1, // VirtualKey.F1 -- 112
    SDL_SCANCODE_F2, // VirtualKey.F2 -- 113
    SDL_SCANCODE_F3, // VirtualKey.F3 -- 114
    SDL_SCANCODE_F4, // VirtualKey.F4 -- 115
    SDL_SCANCODE_F5, // VirtualKey.F5 -- 116
    SDL_SCANCODE_F6, // VirtualKey.F6 -- 117
    SDL_SCANCODE_F7, // VirtualKey.F7 -- 118
    SDL_SCANCODE_F8, // VirtualKey.F8 -- 119
    SDL_SCANCODE_F9, // VirtualKey.F9 -- 120
    SDL_SCANCODE_F10, // VirtualKey.F10 -- 121
    SDL_SCANCODE_F11, // VirtualKey.F11 -- 122
    SDL_SCANCODE_F12, // VirtualKey.F12 -- 123
    SDL_SCANCODE_F13, // VirtualKey.F13 -- 124
    SDL_SCANCODE_F14, // VirtualKey.F14 -- 125
    SDL_SCANCODE_F15, // VirtualKey.F15 -- 126
    SDL_SCANCODE_F16, // VirtualKey.F16 -- 127
    SDL_SCANCODE_F17, // VirtualKey.F17 -- 128
    SDL_SCANCODE_F18, // VirtualKey.F18 -- 129
    SDL_SCANCODE_F19, // VirtualKey.F19 -- 130
    SDL_SCANCODE_F20, // VirtualKey.F20 -- 131
    SDL_SCANCODE_F21, // VirtualKey.F21 -- 132
    SDL_SCANCODE_F22, // VirtualKey.F22 -- 133
    SDL_SCANCODE_F23, // VirtualKey.F23 -- 134
    SDL_SCANCODE_F24, // VirtualKey.F24 -- 135
    SDL_SCANCODE_UNKNOWN, // -- 136
    SDL_SCANCODE_UNKNOWN, // -- 137
    SDL_SCANCODE_UNKNOWN, // -- 138
    SDL_SCANCODE_UNKNOWN, // -- 139
    SDL_SCANCODE_UNKNOWN, // -- 140
    SDL_SCANCODE_UNKNOWN, // -- 141
    SDL_SCANCODE_UNKNOWN, // -- 142
    SDL_SCANCODE_UNKNOWN, // -- 143
    SDL_SCANCODE_NUMLOCKCLEAR, // VirtualKey.NumberKeyLock -- 144
    SDL_SCANCODE_SCROLLLOCK, // VirtualKey.Scroll -- 145
    SDL_SCANCODE_UNKNOWN, // -- 146
    SDL_SCANCODE_UNKNOWN, // -- 147
    SDL_SCANCODE_UNKNOWN, // -- 148
    SDL_SCANCODE_UNKNOWN, // -- 149
    SDL_SCANCODE_UNKNOWN, // -- 150
    SDL_SCANCODE_UNKNOWN, // -- 151
    SDL_SCANCODE_UNKNOWN, // -- 152
    SDL_SCANCODE_UNKNOWN, // -- 153
    SDL_SCANCODE_UNKNOWN, // -- 154
    SDL_SCANCODE_UNKNOWN, // -- 155
    SDL_SCANCODE_UNKNOWN, // -- 156
    SDL_SCANCODE_UNKNOWN, // -- 157
    SDL_SCANCODE_UNKNOWN, // -- 158
    SDL_SCANCODE_UNKNOWN, // -- 159
    SDL_SCANCODE_LSHIFT, // VirtualKey.LeftShift -- 160
    SDL_SCANCODE_RSHIFT, // VirtualKey.RightShift -- 161
    SDL_SCANCODE_LCTRL, // VirtualKey.LeftControl -- 162
    SDL_SCANCODE_RCTRL, // VirtualKey.RightControl -- 163
    SDL_SCANCODE_MENU, // VirtualKey.LeftMenu -- 164
    SDL_SCANCODE_MENU, // VirtualKey.RightMenu -- 165
};

//typedef int (*SDL_WinRT_MainFunction)(int, char **);
//static SDL_WinRT_MainFunction SDL_WinRT_main = nullptr;


int gamew = 640, gameh = 480;
int xamlw = 640, xamlh = 480;
int devicew = 640, deviceh = 480;
void SDL_SetDeviceResolution(int w, int h)
{
	devicew = w;
	deviceh = h;
}
void SDL_SetGameResolution(int w, int h)
{
	gamew = w;
	gameh = h;
}
void SDL_SetXamlResolution(int w, int h)
{
	xamlw = w;
	xamlh = h;
}

void PumpEvents()
{
	
}


static void WINRT_SetDisplayOrientationsPreference(const char *name, const char *oldValue, const char *newValue)
{
    SDL_assert(SDL_strcmp(name, SDL_HINT_ORIENTATIONS) == 0);

    // Start with no orientation flags, then add each in as they're parsed
    // from newValue.
    unsigned int orientationFlags = 0;
    std::istringstream tokenizer(newValue);
    while (!tokenizer.eof()) {
        std::string orientationName;
        std::getline(tokenizer, orientationName, ' ');
        if (orientationName == "LandscapeLeft") {
            orientationFlags |= (unsigned int) DisplayOrientations::LandscapeFlipped;
        } else if (orientationName == "LandscapeRight") {
            orientationFlags |= (unsigned int) DisplayOrientations::Landscape;
        } else if (orientationName == "Portrait") {
            orientationFlags |= (unsigned int) DisplayOrientations::Portrait;
        } else if (orientationName == "PortraitUpsideDown") {
            orientationFlags |= (unsigned int) DisplayOrientations::PortraitFlipped;
        }
    }

    // If no valid orientation flags were specified, use a reasonable set of defaults:
    if (!orientationFlags) {
        // TODO, WinRT: consider seeing if an app's default orientation flags can be found out via some API call(s).
        orientationFlags = (unsigned int) ( \
            DisplayOrientations::Landscape |
            DisplayOrientations::LandscapeFlipped |
            DisplayOrientations::Portrait |
            DisplayOrientations::PortraitFlipped);
    }

    // Set the orientation/rotation preferences.  Please note that this does
    // not constitute a 100%-certain lock of a given set of possible
    // orientations.  According to Microsoft's documentation on Windows RT [1]
    // when a device is not capable of being rotated, Windows may ignore
    // the orientation preferences, and stick to what the device is capable of
    // displaying.
    //
    // [1] Documentation on the 'InitialRotationPreference' setting for a
    // Windows app's manifest file describes how some orientation/rotation
    // preferences may be ignored.  See
    // http://msdn.microsoft.com/en-us/library/windows/apps/hh700343.aspx
    // for details.  Microsoft's "Display orientation sample" also gives an
    // outline of how Windows treats device rotation
    // (http://code.msdn.microsoft.com/Display-Orientation-Sample-19a58e93).
    DisplayProperties::AutoRotationPreferences = (DisplayOrientations) orientationFlags;
}

void SDL_OnPointerPressed(Windows::UI::Input::PointerPoint^ p,SDL_Window* wnd )
{

	//WINRT_LogPointerEvent("mouse down",p, TransformCursor(p->Position,wnd));

	if (true)
	{		
		Uint8 button = WINRT_GetSDLButtonForPointerPoint(p);
		if (button) {
			SDL_SendMouseButton(wnd, 0, SDL_PRESSED, button);
		}
		Point transformedPoint;
		transformedPoint.X = p->Position.X * (float)devicew / (((float)xamlh * 4) / 3.0f);
		transformedPoint.Y = p->Position.Y * (float)deviceh / (float)xamlh;
		char a[20];
		sprintf(a, "%d \t %d ", (int)transformedPoint.X, (int)transformedPoint.Y);
		SDL_SendMouseMotion(wnd, 0, 0, (int)transformedPoint.X, (int)transformedPoint.Y);
	}
}
void SDL_BackKeyPressed()
{
	SDL_SendKeyboardKey(1,SDL_SCANCODE_ESCAPE);
	SDL_SendKeyboardKey(0,SDL_SCANCODE_ESCAPE);
}

static Uint8
WINRT_GetSDLButtonForPointerPoint(PointerPoint ^ pt)
{

	return SDL_BUTTON_LEFT;
}

static const char *
WINRT_ConvertPointerUpdateKindToString(PointerUpdateKind kind)
{
    switch (kind)
    {
        case PointerUpdateKind::Other:
            return "Other";
        case PointerUpdateKind::LeftButtonPressed:
            return "LeftButtonPressed";
        case PointerUpdateKind::LeftButtonReleased:
            return "LeftButtonReleased";
        case PointerUpdateKind::RightButtonPressed:
            return "RightButtonPressed";
        case PointerUpdateKind::RightButtonReleased:
            return "RightButtonReleased";
        case PointerUpdateKind::MiddleButtonPressed:
            return "MiddleButtonPressed";
        case PointerUpdateKind::MiddleButtonReleased:
            return "MiddleButtonReleased";
        case PointerUpdateKind::XButton1Pressed:
            return "XButton1Pressed";
        case PointerUpdateKind::XButton1Released:
            return "XButton1Released";
        case PointerUpdateKind::XButton2Pressed:
            return "XButton2Pressed";
        case PointerUpdateKind::XButton2Released:
            return "XButton2Released";
    }

    return "";
}

static void
WINRT_LogPointerEvent(const string & header, PointerPoint^ pt, Point transformedPoint)
{
    SDL_Log("%s: Position={%f,%f}, Transformed Pos={%f, %f}, MouseWheelDelta=%d, FrameId=%d, PointerId=%d, PointerUpdateKind=%s\n",
        header.c_str(),
        pt->Position.X, pt->Position.Y,
        transformedPoint.X, transformedPoint.Y,
        pt->Properties->MouseWheelDelta,
        pt->FrameId,
        pt->PointerId,
        WINRT_ConvertPointerUpdateKindToString(pt->Properties->PointerUpdateKind));
}



static inline int _lround(float arg) {
    if (arg >= 0.0f) {
        return (int)floor(arg + 0.5f);
    } else {
        return (int)ceil(arg - 0.5f);
    }
}

Point TransformCursor(Point rawPosition,SDL_Window* wnd)
{
    Point outputPosition;
	outputPosition.X = rawPosition.X * (((float32)wnd->w) / 800);
	outputPosition.Y = rawPosition.Y * (((float32)wnd->h) / 600);
    return outputPosition;
}

static std::unordered_map<int, SDL_Scancode> WinRT_Unofficial_Keycodes;

static SDL_Scancode
TranslateKeycode(int keycode)
{
    if (WinRT_Unofficial_Keycodes.empty()) {
        /* Set up a table of undocumented (by Microsoft), WinRT-specific,
           key codes: */
        // TODO, WinRT: move content declarations of WinRT_Unofficial_Keycodes into a C++11 initializer list, when possible
        WinRT_Unofficial_Keycodes[220] = SDL_SCANCODE_GRAVE;
        WinRT_Unofficial_Keycodes[222] = SDL_SCANCODE_BACKSLASH;
    }

    /* Try to get a documented, WinRT, 'VirtualKey' first (as documented at
       http://msdn.microsoft.com/en-us/library/windows/apps/windows.system.virtualkey.aspx ).
       If that fails, fall back to a Win32 virtual key.
    */
    // TODO, WinRT: try filling out the WinRT keycode table as much as possible, using the Win32 table for interpretation hints
    //SDL_Log("WinRT TranslateKeycode, keycode=%d\n", (int)keycode);
    SDL_Scancode scancode = SDL_SCANCODE_UNKNOWN;
    if (keycode < SDL_arraysize(WinRT_Official_Keycodes)) {
        scancode = WinRT_Official_Keycodes[keycode];
    }
    if (scancode == SDL_SCANCODE_UNKNOWN) {
        if (WinRT_Unofficial_Keycodes.find(keycode) != WinRT_Unofficial_Keycodes.end()) {
            scancode = WinRT_Unofficial_Keycodes[keycode];
        }
    }
    if (scancode == SDL_SCANCODE_UNKNOWN) {
        if (keycode < SDL_arraysize(windows_scancode_table)) {
            scancode = windows_scancode_table[keycode];
        }
    }
    if (scancode == SDL_SCANCODE_UNKNOWN) {
        SDL_Log("WinRT TranslateKeycode, unknown keycode=%d\n", (int)keycode);
    }
    return scancode;
}


static int SDLCALL RemoveAppSuspendAndResumeEvents(void * userdata, SDL_Event * event)
{
    if (event->type == SDL_WINDOWEVENT)
    {
        switch (event->window.event)
        {
            case SDL_WINDOWEVENT_MINIMIZED:
            case SDL_WINDOWEVENT_RESTORED:
                // Return 0 to indicate that the event should be removed from the
                // event queue:
                return 0;
            default:
                break;
        }
    }

    // Return 1 to indicate that the event should stay in the event queue:
    return 1;
}
SDL_DisplayMode GetMainDisplayMode()
{
    // Create an empty, zeroed-out display mode:
    SDL_DisplayMode mode;
    SDL_zero(mode);

    // Fill in most fields:
    mode.format = SDL_PIXELFORMAT_RGB888;
    mode.refresh_rate = 0;  // TODO, WinRT: see if refresh rate data is available, or relevant (for WinRT apps)
    mode.driverdata = NULL;

    // Calculate the display size given the window size, taking into account
    // the current display's DPI:
    const float currentDPI = Windows::Graphics::Display::DisplayProperties::LogicalDpi; 
    const float dipsPerInch = 96.0f;
	//debug purpose
	mode.w = GlobalWData.w;
	mode.h = GlobalWData.h;

    return mode;
}

const SDL_WindowData * GetSDLWindowData() 
{
    return GlobalRData.m_sdlWindowData;
}

bool HasSDLWindowData() 
{
    return (GlobalRData.m_sdlWindowData != NULL);
}

void SetRelativeMouseMode(bool enable)
{
    GlobalRData.m_useRelativeMouseMode = enable;
}

void SetSDLWindowData(const SDL_WindowData * windowData)
{
    GlobalRData.m_sdlWindowData = windowData;
}

void SetSDLVideoDevice(const SDL_VideoDevice * videoDevice)
{
    GlobalRData.m_sdlVideoDevice = videoDevice;
}
void SDL_OnPointerReleased(Windows::UI::Input::PointerPoint^ p,SDL_Window* wnd)
{
//#if LOG_POINTER_EVENTS
    //WINRT_LogPointerEvent("mouse up", p, TransformCursor(p->Position,wnd));
//#endif

     if (true) 
	 {
		Uint8 button = WINRT_GetSDLButtonForPointerPoint(p);
        if (button) 
		{
			SDL_SendMouseButton(wnd, 0, SDL_RELEASED, button);
        }
    }
}
void SDL_OnPointerMoved(Windows::UI::Input::PointerPoint ^p,SDL_Window* wnd)
{
//#if LOG_POINTER_EVENTS
    //WINRT_LogPointerEvent("pointer moved", p, TransformCursor(p->Position,wnd));
//#endif
    if (true)
    {
		Point transformedPoint;
		transformedPoint.X = p->Position.X * (float)devicew / (((float)xamlh * 4) / 3.0f);
		transformedPoint.Y = p->Position.Y * (float)deviceh / (float)xamlh;
		char a[20];
		sprintf(a,"%d \t %d ",(int )transformedPoint.X,(int )transformedPoint.Y);

        SDL_SendMouseMotion(wnd, 0, 0, (int)transformedPoint.X, (int)transformedPoint.Y);
    }
}
void SDL_OnSuspending()
{

}
void SDL_OnResuming()
{

}
void SDL_CtrlPressed()
{
	SDL_SendKeyboardKey(1,SDL_SCANCODE_RCTRL);
}
void SDL_CtrlReleased()
{
	SDL_SendKeyboardKey(0,SDL_SCANCODE_RCTRL);
}