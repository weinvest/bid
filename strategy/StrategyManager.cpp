#include "Stdafx.h"
#include <fstream>
#include <string>
#include "StrategyManager.h"
#include "SmartStrategy.h"
#include "ActInTime.h"
#include "StoreStrategy.h"
#include "Log.h"

StrategyManager* StrategyManager::GetInstance()
{
	static StrategyManager gInstance;
	return &gInstance;
}

StrategyManager::StrategyManager()
{
	//auto pStrategy = new SmartStrategy();
	//pStrategy->SetName("SmartStrategy");
	//pStrategy->SetFirstBidTime({11, 40, 47});
	//pStrategy->SetDescription("bid by price change ratio");
	//mStrategies.push_back(pStrategy);
}

IStrategy* StrategyManager::Create(const std::string& type)
{
	if ("SmartStrategy" == type)
	{
		return new SmartStrategy();
	}
	else if ("ActInTime" == type)
	{
		return new ActInTime();
	}
	else if ("StoreStrategy" == type)
	{
		return new StoreStrategy();
	}

	return nullptr;
}

bool StrategyManager::Load(const CString& strategyConf)
{
	std::ifstream inFile(strategyConf, std::ios_base::binary | std::ios_base::in);

	std::string line;
	while (std::getline(inFile, line))
	{
		IStrategy::Trim(line);

		if (!line.empty())
		{
			if ('[' == line.front() && ']' == line.back())
			{
				std::string type = line.substr(1, line.length() - 2);
				auto pStrategy = Create(type);
				if (pStrategy->Load(inFile))
				{
					mStrategies.push_back(pStrategy);
				}
			}
			else if ('#' != line.front())
			{
			}
		}
	}
	return true;
}

