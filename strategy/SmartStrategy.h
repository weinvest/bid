#ifndef SMART_STRATEGY_H
#define SMART_STRATEGY_H
#include "IStrategy.h"
class SmartStrategy: public IStrategy
{
public:
	SmartStrategy();

	void SetFirstBidTime(BidTime time);
	void SetSecondBidTime(BidTime time);

	void OnUpdate(size_t updateFields) override;

private:
	BidTime mFirstBidTime;
	BidTime mSecondBidTime;
	int mBidTimes;
};
#endif // !SMART_STRATEGY_H