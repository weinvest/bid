#pragma once
class CpuTime
{
public:
	CpuTime()
	{ 
		QueryPerformanceCounter(&mStartTime); //获得计数器的时钟频率     
	}

	double GetElapse()
	{
		QueryPerformanceCounter(&mEndTime);   //获得中止值   
		QueryPerformanceFrequency(&mFrequency);
		double diff = (mEndTime.QuadPart - mStartTime.QuadPart) / (double)mFrequency.QuadPart;
		return diff;
	}
	~CpuTime()
	{
	}

	LARGE_INTEGER mStartTime;
	LARGE_INTEGER mEndTime;
	LARGE_INTEGER mFrequency;
};
