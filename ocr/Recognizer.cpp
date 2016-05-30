#include "StdAfx.h"
#include "Recognizer.h"
#include "ScreenImage.h"

CRecognizer::CRecognizer(void)
	:mSepCount(0)
	,mLCSTable(nullptr)
{
}


CRecognizer::~CRecognizer(void)
{
	delete[] mLCSTable;
}

void CRecognizer::Initialize(KnowledgeT& knowledge, int sepCount)
{
	mKnowledge.swap(knowledge);
	mSepCount = sepCount;
	mLCSTable = new int[GetLCSLength() * GetLCSLength()];
}

bool CRecognizer::Recognize(CString& outValue, CString& outFeature, CScreenImage* pImage)
{
	std::vector<CString> imageFeatures(16);
	pImage->ScanAndSplit(outFeature, imageFeatures, mSepCount);

	for(auto& itFeature = imageFeatures.begin();
		itFeature != imageFeatures.end();
		++itFeature)
	{
		auto itChar = mKnowledge.find(*itFeature);
		if(itChar == mKnowledge.end())
		{
			return false;
		}

		outValue.AppendChar(itChar->second);
	}

	return true;
}

bool CRecognizer::RecognizeEx(CString& outValue, CString& outFeature, CScreenImage* pImage)
{
	std::vector<CString> imageFeatures(16);

	pImage->ScanAndSplit(outFeature, imageFeatures, mSepCount);

	for(auto& itFeature = imageFeatures.begin();
		itFeature != imageFeatures.end();
		++itFeature)
	{
		auto itChar = mKnowledge.find(*itFeature);
		if(itChar == mKnowledge.end())
		{
			auto c = LCSRecognize(*itFeature);
			if(0 != c)
			{
			    outValue.AppendChar(c);
			}
		}
		else
		{
		    outValue.AppendChar(itChar->second);
		}
	}

	return true;
}

int CRecognizer::LCS(const CString& a, const CString& b) const
{
	int m = a.GetLength();
	int n = b.GetLength();

    for(int i = 0; i <= m; ++i)
	{
       GetLCSValue(i,0) = 0;
	}

	for(int j = 0; j <= n; ++j)
	{
       GetLCSValue(0,j) = 0;
	}

    for(int i = 1; i <= m; ++i)
	{
        for(int j = 1; j <=n; ++j)
		{
            if(a.GetAt(i) == b.GetAt(j))
			{
                GetLCSValue(i,j) = GetLCSValue(i-1,j-1) + 1;
			}
            else
			{
                GetLCSValue(i,j) = max(GetLCSValue(i,j-1), GetLCSValue(i-1,j));
			}
		}
	}
    return GetLCSValue(m,n);
}

char CRecognizer::LCSRecognize(CString& feature) const
{
	if(feature.GetLength() > GetLCSLength())
	{
		return 0;
	}

	char c = 0;
	int maxLen = 0;
	for(auto itKnowledge = mKnowledge.begin();
		itKnowledge != mKnowledge.end();
		++itKnowledge)
	{
		auto len = LCS(itKnowledge->first, feature);
		if(len > maxLen && (len > (feature.GetLength() / 2)))
		{
			maxLen = len;
			c = itKnowledge->second;
		}
	}

	return c;
}
