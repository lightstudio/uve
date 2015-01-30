//
// pch.h
// 标准系统包含文件的标头。
//
#ifndef PCH_H
#define PCH_H
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cmath>
#include <cstring>
#include <mfapi.h>
#include <mfidl.h>
#include <winnt.h>
#include <winbase.h>
#include <winapifamily.h>
#include <ole2.h>
#include <rpc.h>
#include <rpcndr.h>
#include <windef.h>
#include <minwinbase.h>
#include <minwindef.h>
#include <thread>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <memory>
#include <agile.h>
#define USE_SDL_RENDERER
//#define USE_SMPEG
#define WINDOWS_PHONE_8
#define USE_LUA
#include "str.h"
//#undef stderr
//#define stderr stderr_fp
extern _iobuf* stderr_fp;
extern string gamepath_relative;
#define GAMEPATH (ws2s(Windows::Storage::ApplicationData::Current->LocalFolder->Path->Data())+'\\'+gamepath_relative).data()
#define GAMEPATHSTRING (ws2s(Windows::Storage::ApplicationData::Current->LocalFolder->Path->Data())+'\\'+gamepath_relative)
#define FILEPATH(file) (ws2s(Windows::Storage::ApplicationData::Current->LocalFolder->Path->Data())+'\\'+file).data()
#define ENABLE_1BYTE_CHAR
#endif



