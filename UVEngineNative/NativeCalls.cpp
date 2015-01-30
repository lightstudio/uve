#include "pch.h"
#include "INativeCalls.h"

void playVideoWP(const char *filename)
{
	CALL(PlayExternalVideo(ref new String(s2ws(string(filename)).data())));
}
void logWP(std::string log)
{
	CALL(ErrorLog(ref new String(s2ws(log).data())));
}