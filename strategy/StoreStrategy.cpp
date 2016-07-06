#include "StdAfx.h"
#include <string>
#include "Log.h"
#include "InfoEngine.h"
#include "StoreStrategy.h"
bool StoreStrategy::DoLoad(const std::map<std::string, std::string>& configurePairs)
{
	auto itFileName = configurePairs.find("FileName");
	if (itFileName != configurePairs.end())
	{
		mOutFile.reset(new std::ofstream(itFileName->second, std::ios_base::out | std::ios_base::binary));
	}
	else
	{
		LOG_INFO("Load StoreStrategy %s failed, not specified file name", GetName());
		return false;
	}

	auto itInterval = configurePairs.find("StoreInterval");
	if (itInterval != configurePairs.end())
	{
		mStoreInterval = std::stoi(itInterval->second, 0);
	}
	else
	{
		LOG_INFO("Load StoreStrategy %s failed, not specified StoreInterval", GetName());
		return false;
	}

	(*mOutFile) << "CurrentTime,LowestPrice,LowestPriceTime,LPrice,UPrice\n";
	return true;
}

void StoreStrategy::OnUpdate(size_t updateFields)
{
	if (mOutFile)
	{
		std::string currentTime, currentPriceTime;
		auto pInfoEngine = InfoEngine::GetInstance();
		pInfoEngine->GetInfo(InfoEngine::CURRENT_TIME_RECT_INDEX).ToString(currentTime, InfoEngine::TIME);
		auto& lowestPrice = pInfoEngine->GetInfo(InfoEngine::CURRENT_LOWEST_PRICE_INDEX);
		pInfoEngine->GetInfo(InfoEngine::CURRENT_LOWEST_PRICE_TIME_INDEX).ToString(currentPriceTime, InfoEngine::TIME);
		auto& priceRange = pInfoEngine->GetInfo(InfoEngine::CURRENT_ACCEPTABLE_PRICE_RANGE);

		(*mOutFile) << currentTime << "," << lowestPrice.price << "," << currentPriceTime << "," << priceRange.priceRange.GetLowest() << "," << priceRange.priceRange.GetHighest() << '\n';
	}
}