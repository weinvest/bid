#include "Stdafx.h"
#include "Log.h"
#include <stdarg.h>
Log::Log()
	:mSink("log.txt", std::ios_base::binary | std::ios_base::app)
{
	char szTime[32] = { 0 };
	auto time = CTime::GetCurrentTime();
	sprintf(szTime, "%4d%02d%02d %02d:%02d:%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	mSink << "--------";
	mSink.write(szTime, strlen(szTime));
	mSink << "--------\n";

}

Log::~Log()
{
	mSink.flush();
}

Log* Log::GetInstance()
{
	static Log gInstance;
	return &gInstance;
}

void Log::Info(const char* fmt, ...)
{
	char szLog[4096] = { 0 };
	va_list args;
	va_start(args, fmt);
	vsprintf(szLog, fmt,  args);
	va_end(args);

	mSink.write(szLog, strlen(szLog));
	mSink.flush();
}

void Log::Info0(const char* message)
{
	mSink << message;
	mSink.flush();
}