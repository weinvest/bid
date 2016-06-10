#include "StdAfx.h"
#include "SmartStrategy.h"
#include "InfoEngine.h"
#include "ActionEngine.h"
SmartStrategy::SmartStrategy()
	:mBidTimes(0)
{}

void SmartStrategy::SetFirstBidTime(BidTime time)
{
	mFirstBidTime = time;
}

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