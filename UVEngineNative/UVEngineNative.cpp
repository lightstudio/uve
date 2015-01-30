#include "pch.h"
#include "UVEngineNative.h"
#include "BasicGameInfo.h"
using namespace UVEngineNative;
using namespace Platform;
using namespace std;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;

#pragma region ImageToolkit
Array<int,1>^ ImageToolkitNative::UnAlpha(const Array<int,1>^ alpha,const Array<int,1>^ unAlpha ,int width,int height)
{
	Array<int,1>^ fin=ref new Array<int,1>(width*height/2);
	
	for (int i = 0; i < height; i++)
    {
		for (int k = 0; k < width / 2; k++)
		{
			uint32 gray = 0;
            int temp;
            int access = i * width / 2 + k;
            uint32 r, g, b;
			uint32 ar,ag,ab;
            temp = unAlpha[access];
			ar=(uint32)temp>>16;
			ag=(uint32)(temp<<16)>>24;
			ab=(uint32)(temp<<24)>>24;
			
            gray = (uint32)alpha[access];
            gray <<= 8;
            gray >>= 8;
            r = (uint32)gray >> 16;
            g = (uint32)(gray << 16) >> 24;
            b = (uint32)(gray << 24) >> 24;
			gray = 255 - r;
			
			if (gray<=5) 
			{
				ar=0;
				ag=0;
				ab=0;
				gray=0;
			}
			else if (ar>=200&&ag>=200&&ab>=200)
			{
				gray=255;

			}
            gray <<= 24;
			int color=(int)ar<<16|(int)ag<<8|(int)ab;          
			fin[access]=(int)gray;
			fin[access]|=(color&0x00ffffff);
        }
     }
	return fin;
}
#pragma endregion
//#pragma region StringToolkit
//String^ StringToolkitNative::GetInside(String^ Input,char16 inside)
//{
//	wstring returned;
//	wstring tmp(Input->Data());
//	bool IsIn = false;
//	for (int i = 0; i < Input->Length(); i++)
//	{
//		if (!IsIn&&tmp[i] == inside)
//		{
//			IsIn = true;
//		}
//		else if (IsIn && tmp[i] == inside)
//		{
//			return ref new String(returned.data());
//		}
//		else if (IsIn)
//		{
//			returned += tmp[i];
//		}
//	}
//	return ref new String(returned.data());
//}
//Array<String^,1>^ StringToolkitNative::CutParam(String^ un, char16 cutby)
//{
//	        bool IsIn = false;
//			wstring tempparam[100];
//            int paramcount = 0;
//			wstring temp(un->Data());
//			for (int i = 0; i < un->Length(); i++)
//            {
//				if (!IsIn && temp[i] == '\"')
//                {
//                    IsIn = true;
//                }
//                else if (IsIn && temp[i] == '\"')
//                {
//                    IsIn = false;
//                }
//                if (!IsIn && temp[i] == cutby)
//                {
//                    paramcount++;
//                }
//                else
//                {
//                    tempparam[paramcount] += temp[i];
//                }
//            }
//			Array<String^,1>^ tmp=ref new Array<String^,1>(paramcount + 1);
//            for (int i = 0; i < paramcount+1; i++)
//            {
//				tmp[i] = ref new String(tempparam[i].c_str());
//            }
//			return tmp;
//}
//String^ StringToolkitNative::GetBefore(String^ input,char16 before)
//{
//	wstring returned;
//	wstring wstr(input->Data());
//	for (int i = 0; i < input->Length(); i++)
//	{
//		if (wstr[i] != before)
//		{
//			returned += wstr[i];
//		}
//		else break;
//	}
//	return ref new String(returned.data());
//}
//String^ StringToolkitNative::GetBetween(String^ input, char16 left, char16 right)
//{
//	wstring w_input(input->Data());
//	wstring returned;
//	bool IsIn = false;
//	for (int i = 0; i < w_input.length(); i++)
//	{
//		if (IsIn)
//		{
//			if (w_input[i] == right) return ref new String(returned.data());
//			else returned += w_input[i];
//		}
//		else
//		{
//			if (w_input[i] == left) IsIn = true;
//		}
//	}
//	return ref new String(returned.data());
//}
//#pragma endregion
//#pragma region internal
//wstring* StringToolkitNative::CutParam(wchar_t* un,wchar_t cutby)
//{
//	bool IsIn = false;
//    wstring* temppara = new wstring[100];
//    for (int i = 0; i < sizeof(temppara); i++)
//    {
//		temppara[i] = L"";
//    }
//    int paramcount = 0;
//    for (int i = 0; i < sizeof(un)/sizeof(un[0]); i++)
//    {
//		if (!IsIn && un[i] == '\"')
//		{
//			IsIn = true;
//		}
//		else if (IsIn && un[i] == '\"')
//		{
//			IsIn = false;
//		}
//		if (!IsIn && un[i] == cutby)
//		{
//			paramcount++;
//		}
//		else
//		{
//			temppara[paramcount] += un[i];
//		}
//	}
//	wstring* toreturn = new wstring[paramcount + 1];
//    for (int i = 0; i < paramcount+1; i++)
//	{
//		toreturn[i] = temppara[i];
//	}
//	return toreturn;
//}
////wstring StringToolkitNative::GetBefore(wchar_t* input,wchar_t before)
////{
////
////}
////wstring StringToolkitNative::GetInside(wchar_t* input,wchar_t inside)
////{
////
////}
////wstring StringToolkitNative::GetBetween(wchar_t* input,wchar_t left,wchar_t right)
////{
////
////}
//#pragma endregion