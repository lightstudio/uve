//
// pch.cpp
// 包含标准标头并生成预编译标头。
//

#include "pch.h"
#include "INativeCalls.h"
#include "gbk2utf16.h"
UVEngineNative::NativeCallback* GlobalNativeCallback;
Coding2UTF16*coding2utf16 = new GBK2UTF16();