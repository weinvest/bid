#pragma once
#include "Common.h"
#include "IOCRListener.h"
#include <set>

class CRecognizer;
class CFontTraining;
class COCREngine
{
public:
	~COCREngine(void);

	static COCREngine* GetInstance();

	void Load(const CString& fontRoot);

	bool Add(const CString& name, CFontTraining* pFontTraining, bool overwrite = false);
	bool Remove(const CString& name, bool removeFromDisk = false);

	CRecognizer* GetRecognizer(const CString& name);

	const std::map<CString, CRecognizer*>& GetAllRecognizer() const { return mRecognizers; }

	bool Has(const CString& name);

	void Registe(IOCRListener* pListener);
	void UnRegiste(IOCRListener* pListener);
private:
	CString GetFontPath(const CString& name);
	void Notify(bool isAdd, const CString& name, CRecognizer* pRecognizer);

	COCREngine();
	COCREngine(const COCREngine&);

	CString mFontRootDir;
	std::map<CString, CRecognizer*> mRecognizers;
	std::set<IOCRListener*> mListeners;
};

