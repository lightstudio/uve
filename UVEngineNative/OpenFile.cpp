#include "pch.h"
#include "OpenFile.h"
using namespace std;
using namespace Platform;

FILE* OpenFile(String^ filePath)
{
	FILE* fp;
	_wfopen_s(&fp,filePath->Data(),L"r");
	return fp;
}