#pragma once
#include <fstream>
#include <memory>
#include "IStrategy.h"
class StoreStrategy : public IStrategy
{
protected:
	bool DoLoad(const std::map<std::string, std::string>& configurePairs);
	void OnUpdate(size_t updateFields) override;

private:
	std::shared_ptr<std::ofstream> mOutFile;
	int mStoreInterval;
};