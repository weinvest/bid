#include "Stdafx.h"
#include "ActInTime.h"
bool ActInTime::DoLoad(const std::map<std::string, std::string>& configurePairs)
{
	auto itBidTime = configurePairs.find("BidTime");
	if (configurePairs.end() != itBidTime)
	{
		mBidTime.Parse(itBidTime->second.c_str());
	}
	else
	{
		TRACE("ActInTime need configure BidTime.");
		return false;
	}

	auto itVerifyCodeTime = configurePairs.find("InputVerifyCodeTime");
	if (configurePairs.end() != itVerifyCodeTime)
	{
		mInputVerifyCodeTime.Parse(itVerifyCodeTime->second.c_str());
	}
	else
	{
		return false;
	}
}

void ActInTime::OnUpdate(size_t updateFields)
{

}