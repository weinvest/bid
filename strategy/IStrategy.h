#ifndef _ISTRATEGY_H
#define _ISTRATEGY_H
#include <afxwin.h>
#include <istream>
#include <map>
#include "InfoEngine.h"
class IStrategy: public IInfoListener
{
public:
	IStrategy(){}
	virtual ~IStrategy(){}

	const std::string& GetName() const { return mName; }
	void SetName(const std::string& name) { mName = name; }

	const std::string& GetDescription() const { return mDescription; }
	void SetDescription(const std::string& description) { mDescription = description; }

	bool Load(std::istream& s);
	static void Trim(std::string& s);

protected:
	virtual bool DoLoad(const std::map<std::string, std::string>& configurePairs);

	std::string mName;
	std::string mDescription;
};
#endif // !_ISTRATEGY_H