#ifndef SMART_STRATEGY_H
#define SMART_STRATEGY_H
#include <thread>
#include "IStrategy.h"
class SmartStrategy: public IStrategy
{
public:
	SmartStrategy();

	void SetFirstBidTime(BidTime time);
	void SetSecondBidTime(BidTime time);

	void OnUpdate(size_t updateFields) override;

protected:
	bool DoLoad(const std::map<std::string, std::string>& configurePairs) override;

private:
	BidTime mFirstBidTime;
	BidTime mSecondBidTime;
	int mBidTimes;
	std::mutex mLock;
};
#endif // !SMART_STRATEGY_H