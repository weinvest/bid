#include "StdAfx.h"
#include "SmartStrategy.h"
#include "InfoEngine.h"
#include "ActionEngine.h"
#include "Log.h"
SmartStrategy::SmartStrategy()
	:mBidTimes(0)
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

	return mFirstBidTime < mSecondBidTime;
}
#include <sstream>
void SmartStrategy::OnUpdate(size_t updateFields)
{
	auto pInfoEngine = InfoEngine::GetInstance();
	auto& timeInfo = pInfoEngine->GetInfo(InfoEngine::CURRENT_TIME_RECT_INDEX);
	auto& curPrice = pInfoEngine->GetInfo(InfoEngine::CURRENT_LOWEST_PRICE_INDEX);

	if (mFirstBidTime < timeInfo.time)
	{	
		if (0 == mBidTimes)
		{
			mBidTimes = 1;
			ActionEngine::GetInstance()->InputPrice(curPrice.price + 700);
		}
	}

	if (timeInfo.time.minute == 29 && timeInfo.time.second >= 57)
	{
		mBidTimes = 0;
	}

}