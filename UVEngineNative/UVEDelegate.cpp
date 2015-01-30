#include "pch.h"
#include "UVEDelegate.h"
using namespace UVEngineNative;
using namespace std;
using namespace Platform;


UVEDelegate::UVEDelegate()
{

}
void UVEDelegate::SetCallback(ICallback^ callback)
{
    GlobalCallback = callback;
}