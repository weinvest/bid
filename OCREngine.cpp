#include "StdAfx.h"
#include "OCREngine.h"
#include "Recognizer.h"
#include "FontTraining.h"
#include "FontLoader.h"
COCREngine::COCREngine(void)
{
}


COCREngine::~COCREngine(void)
{
}

COCREngine* COCREngine::GetInstance()
{
	static COCREngine gInstance;
	return &gInstance;
}


void COCREngine::Load(const CString& fontRoot)
{
	mFontRootDir = fontRoot;
	CFileFind ff;
    if (mFontRootDir.Right(1) != "\\")
	{
        mFontRootDir += "\\";
	}

	CString dir = mFontRootDir + "*.gfont";
    
	CFontLoader loader;
    BOOL ret = ff.FindFile(dir);
    while (ret)
    {
        ret = ff.FindNextFile();
		if (!ff.IsDirectory())
        {
			KnowledgeT knowledge;
			int sepCount(0);
			loader.Load(knowledge, sepCount, ff.GetFilePath());

			auto name = ff.GetFileTitle();
			auto newRecognizer = new CRecognizer();
			newRecognizer->Initialize(knowledge, sepCount);
			mRecognizers[name] = newRecognizer;
        }
    }
}

bool COCREngine::Add(const CString& name, CFontTraining* pFontTraining, bool overwrite)
{
	auto itRecognize = mRecognizers.find(name);
	if(itRecognize != mRecognizers.end() && !overwrite)
	{
		return false;
	}
	else
	{
		pFontTraining->Save(GetFontPath(name));
		auto newRecognizer = new CRecognizer();

		newRecognizer->Initialize(pFontTraining->GetKnowledge(), pFontTraining->GetSepCount());
		mRecognizers[name] = newRecognizer;
		Notify(true, name, newRecognizer);
		pFontTraining->Clear();
		return true;
	}
}

bool COCREngine::Has(const CString& name)
{
	return mRecognizers.find(name) != mRecognizers.end();
}

CString COCREngine::GetFontPath(const CString& name)
{
	return mFontRootDir + '\\' + name + ".gfont";
}

bool COCREngine::Remove(const CString& name, bool removeFromDisk)
{
	auto itRecognize = mRecognizers.find(name);
	if(itRecognize == mRecognizers.end())
	{
		return false;
	}

	auto pRecognizer = itRecognize->second;
	mRecognizers.erase(itRecognize);
	Notify(false, name, pRecognizer);
	DeleteFile(GetFontPath(name));
	return true;
}

CRecognizer* COCREngine::GetRecognizer(const CString& name)
{
	auto itRecognize = mRecognizers.find(name);
	if(itRecognize == mRecognizers.end())
	{
		return nullptr;
	}

	return itRecognize->second;
}

void COCREngine::Registe(IOCRListener* pListener)
{
	if(nullptr != pListener)
	{
		mListeners.insert(pListener);
	}
}

void COCREngine::UnRegiste(IOCRListener* pListener)
{
	if(nullptr != pListener)
	{
		mListeners.erase(pListener);
	}
}

void COCREngine::Notify(bool isAdd, const CString& name, CRecognizer* pRecognizer)
{
	if(!isAdd)
	{
		for(auto itListener = mListeners.begin(); itListener != mListeners.end(); ++itListener)
		{
			(*itListener)->OnDelRecognizer(name, pRecognizer);
		}
	}
	else
	{
		for(auto itListener = mListeners.begin(); itListener != mListeners.end(); ++itListener)
		{
			(*itListener)->OnNewRecognizer(name, pRecognizer);
		}
	}
}