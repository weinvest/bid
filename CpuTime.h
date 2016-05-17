#pragma once
class CpuTime
{
public:
	CpuTime()
	{ 
		QueryPerformanceCounter(&mStartTime); //��ü�������ʱ��Ƶ��     
	}

	double GetElapse()
	{
		QueryPerformanceCounter(&mEndTime);   //�����ֵֹ   
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
