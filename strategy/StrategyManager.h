#ifndef _STRATEGY_MANAGER_H
#define _STRATEGY_MANAGER_H
#include <vector>
#include <memory>
#include "IStrategy.h"
class StrategyManager
{
public:
	typedef std::vector<IStrategy*> StrategiesT;
	static StrategyManager* GetInstance();

	const StrategiesT& GetStrategies() const { return mStrategies; }

private:
	StrategyManager();
	StrategyManager(const StrategyManager&);

	StrategiesT mStrategies;
};
#endif
