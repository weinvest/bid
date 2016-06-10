#ifndef _ISTRATEGY_H
#define _ISTRATEGY_H
#include <afxwin.h>
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
protected:
	CString mName;
	CString mDescription;
};
#endif // !_ISTRATEGY_H