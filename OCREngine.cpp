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


void COCREngine::Load(const std::string& fontRoot)
{
	mFontRootDir = fontRoot;
	CFileFind ff;
    if (mFontRootDir.back() != '\\')
	{
        mFontRootDir += "\\";
	}

	std::string dir = mFontRootDir + "*.gfont";
    
	CFontLoader loader;
    BOOL ret = ff.FindFile(CString(dir.c_str()));
    while (ret)
    {
        ret = ff.FindNextFile();
		if (!ff.IsDirectory())
        {
			KnowledgeT knowledge;
			int sepCount(0);

			std::string filePath = CW2A(ff.GetFilePath());
			loader.Load(knowledge, sepCount, filePath);

			std::string name = CW2A(ff.GetFileTitle());
			auto newRecognizer = new CRecognizer();
			newRecognizer->Initialize(knowledge, sepCount);
			mRecognizers[name] = newRecognizer;
        }
    }
}

bool COCREngine::Add(const std::string& name, CFontTraining* pFontTraining, bool overwrite)
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

bool COCREngine::Has(const std::string& name)
{
	return mRecognizers.find(name) != mRecognizers.end();
}

std::string COCREngine::GetFontPath(const std::string& name)
{
	return mFontRootDir + '\\' + name + ".gfont";
}

bool COCREngine::Remove(const std::string& name, bool removeFromDisk)
{
	auto itRecognize = mRecognizers.find(name);
	if(itRecognize == mRecognizers.end())
	{
		return false;
	}

	auto pRecognizer = itRecognize->second;
	mRecognizers.erase(itRecognize);
	Notify(false, name, pRecognizer);
	DeleteFile(CString(GetFontPath(name).c_str()));
	return true;
}

CRecognizer* COCREngine::GetRecognizer(const std::string& name)
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

void COCREngine::Notify(bool isAdd, const std::string& name, CRecognizer* pRecognizer)
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