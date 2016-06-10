#include "stdafx.h"
#include "StrategyManager.h"
#include "SmartStrategy.h"
StrategyManager* StrategyManager::GetInstance()
{
	static StrategyManager gInstance;
	return &gInstance;
}

StrategyManager::StrategyManager()
{
	auto pStrategy = new SmartStrategy();
	pStrategy->SetName("SmartStrategy");
	pStrategy->SetFirstBidTime({11, 40, 47});
	pStrategy->SetDescription("bid by price change ratio");
	mStrategies.push_back(pStrategy);
}

