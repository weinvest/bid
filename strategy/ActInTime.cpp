#include "Stdafx.h"
#include <string>
#include "ActInTime.h"
#include "InfoEngine.h"
#include "ActionEngine.h"
#include "Log.h"
ActInTime::ActInTime()
    :mCleanTime{ 11,29,20,0 }
    ,mBidTimes(0)
{}

bool ActInTime::DoLoad(const std::map<std::string, std::string>& configurePairs)
{
	auto itBidTime = configurePairs.find("BidTime");
	if (configurePairs.end() != itBidTime)
	{
		mBidTime.Parse(itBidTime->second.c_str());
	}
	else
	{
		LOG_INFO0("ActInTime need configure BidTime.");
		return false;
	}

	auto itBidMarkUp = configurePairs.find("BidMarkUp");
	if(configurePairs.end() != itBidMarkUp)
	{
	    mBidMarkUp = std::stoi(itBidMarkUp->second);
	}
	else
	{
	    LOG_INFO0("ActInTime need configure BidMarkUp.");
	    return false;
	}

	return true;
	//auto itVerifyCodeTime = configurePairs.find("InputVerifyCodeTime");
	//if (configurePairs.end() != itVerifyCodeTime)
	//{
	//	mInputVerifyCodeTime.Parse(itVerifyCodeTime->second.c_str());
	//	return true;
	//}
	//else
	//{
	//	return false;
	//}
}

void ActInTime::OnUpdate(size_t updateFields)
{
	auto pInfoEngine = InfoEngine::GetInstance();
	auto& timeInfo = pInfoEngine->GetInfo(InfoEngine::CURRENT_TIME_RECT_INDEX);
	auto& curPrice = pInfoEngine->GetInfo(InfoEngine::CURRENT_LOWEST_PRICE_INDEX);

	LOG_INFO("ActInTime: %d:%d:%d\n", timeInfo.time.hour, timeInfo.time.minute, timeInfo.time.second);
	if ((mBidTime < timeInfo.time) && (0 == mBidTimes))
	{
		mBidTimes = 1;
		ActionEngine::GetInstance()->InputPrice(curPrice.price + mBidMarkUp);
	}
	else if (timeInfo.time < mCleanTime)
	{
		mBidTimes = 0;
	}
}

