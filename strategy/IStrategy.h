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

	const CString& GetName() const { return mName; }
	void SetName(const CString& name) { mName = name; }

	const CString& GetDescription() const { return mDescription; }
	void SetDescription(const CString& description) { mDescription = description; }

	bool Load(std::istream& s);
	static void Trim(std::string& s);

protected:
	virtual bool DoLoad(const std::map<std::string, std::string>& configurePairs);

	CString mName;
	CString mDescription;
};
#endif // !_ISTRATEGY_H