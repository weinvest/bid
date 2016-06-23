#pragma once
#include <fstream>
class Log
{
public:
	static Log* GetInstance();
	~Log();

	void Info(const char* fmt, ...);
	void Info0(const char* message);
private:
	Log();
	Log(const Log&);

	std::ofstream mSink;
};

#define LOG_INFO(fmt, ...) Log::GetInstance()->Info(fmt, __VA_ARGS__)
#define LOG_INFO0(fmt) Log::GetInstance()->Info0(fmt)