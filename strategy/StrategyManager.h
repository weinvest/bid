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

	bool Load(const CString& strategyConf);

	IStrategy* GetStrategy(const std::string& name);
private:
	IStrategy* Create(const std::string& type);

	StrategyManager();
	StrategyManager(const StrategyManager&);

	StrategiesT mStrategies;
};
#endif
