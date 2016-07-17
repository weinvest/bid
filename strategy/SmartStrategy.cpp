#include "StdAfx.h"
#include <string>
#include "SmartStrategy.h"
#include "InfoEngine.h"
#include "ActionEngine.h"
#include "Log.h"
SmartStrategy::SmartStrategy()
	:mCleanTime{ 11,29,20,0 }
	,mBidTimes(0)
{}

void SmartStrategy::SetFirstBidTime(BidTime time)
{
	mFirstBidTime = time;
}

bool SmartStrategy::DoLoad(const std::map<std::string, std::string>& configurePairs)
{
	auto itFirstBidTime = configurePairs.find("FirstBidTime");
	if (itFirstBidTime == configurePairs.end())
	{
		return false;
	}

	mFirstBidTime.Parse(itFirstBidTime->second.c_str());
	if (mFirstBidTime < BidTime{ 11, 29, 0, 0 })
	{
		return false;
	}

	auto itFirstMarkUp = configurePairs.find("FirstMarkUp");
	if (itFirstMarkUp == configurePairs.end())
	{
		return false;
	}

	mFirstMarkUp = std::stoi(itFirstMarkUp->second);

	auto itSecondBidTime = configurePairs.find("SecondBidTime");
	if (itSecondBidTime == configurePairs.end())
	{
		return false;
	}

	mSecondBidTime.Parse(itSecondBidTime->second.c_str());
	if (mFirstBidTime < BidTime{ 11, 29, 30, 0 })
	{
		return false;
	}

	auto itSecondMarkUp = configurePairs.find("SecondMarkUp");
	if (itSecondMarkUp == configurePairs.end())
	{
		return false;
	}

	mSecondMarkUp = std::stoi(itSecondMarkUp->second);
	return mFirstBidTime < mSecondBidTime;
}
#include <sstream>
void SmartStrategy::OnUpdate(size_t updateFields)
{
	auto pInfoEngine = InfoEngine::GetInstance();
	auto& timeInfo = pInfoEngine->GetInfo(InfoEngine::CURRENT_TIME_RECT_INDEX);
	auto& curPrice = pInfoEngine->GetInfo(InfoEngine::CURRENT_LOWEST_PRICE_INDEX);

	LOG_INFO("SmartStrategy: %d:%d:%d\n", timeInfo.time.hour, timeInfo.time.minute, timeInfo.time.second);
	if (mSecondBidTime < timeInfo.time && 1 >= mBidTimes)
	{
		mBidTimes = 2;
		ActionEngine::GetInstance()->InputPrice(curPrice.price + mSecondMarkUp);
	}
	else if (mFirstBidTime < timeInfo.time && 0 == mBidTimes)
	{	
		mBidTimes = 1;
		ActionEngine::GetInstance()->InputPrice(curPrice.price + mFirstMarkUp);
	}
	else if (timeInfo.time < mCleanTime)
	{
		mBidTimes = 0;
	}
}