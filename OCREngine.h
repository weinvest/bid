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

	void Load(const std::string& fontRoot);

	bool Add(const std::string& name, CFontTraining* pFontTraining, bool overwrite = false);
	bool Remove(const std::string& name, bool removeFromDisk = false);

	CRecognizer* GetRecognizer(const std::string& name);

	const std::map<std::string, CRecognizer*>& GetAllRecognizer() const { return mRecognizers; }

	bool Has(const std::string& name);

	void Registe(IOCRListener* pListener);
	void UnRegiste(IOCRListener* pListener);
private:
	std::string GetFontPath(const std::string& name);
	void Notify(bool isAdd, const std::string& name, CRecognizer* pRecognizer);

	COCREngine();
	COCREngine(const COCREngine&);

	std::string mFontRootDir;
	std::map<std::string, CRecognizer*> mRecognizers;
	std::set<IOCRListener*> mListeners;
};

