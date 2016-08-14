#pragma once
#include "IStrategy.h"
class ActInTime : public IStrategy
{
public:
	ActInTime();
protected:
	bool DoLoad(const std::map<std::string, std::string>& configurePairs);
	void OnUpdate(size_t updateFields) override;
private:
	BidTime mBidTime;
	int mBidMarkUp;
	int mBidTimes;
	BidTime mCleanTime;
	BidTime mInputVerifyCodeTime;
};
