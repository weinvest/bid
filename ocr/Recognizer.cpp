#include "StdAfx.h"
#include "Recognizer.h"
#include "ScreenImage.h"

CRecognizer::CRecognizer(void)
	:mSepCount(0)
	,mLCSTable(nullptr)
	, mNextAviableKey(0)
{
}


CRecognizer::~CRecognizer(void)
{
	delete[] mLCSTable;
}

void CRecognizer::Initialize(KnowledgeT& knowledge, int sepCount)
{
	for (const auto& feature : knowledge)
	{
		auto& key = mKeyPool[mNextAviableKey++];
		key = feature.first.vertical;
		mKnowledge.insert(std::make_pair(key.c_str(), feature));
	}

	mSepCount = sepCount;
	mLCSTable = new int[GetLCSLength() * GetLCSLength()];
}

bool CRecognizer::Recognize(std::string& outValue, Feature& inFeature)
{
	const char RECONGIZE_FAIL_CHAR = '\0';
	auto equalRange = mKnowledge.equal_range(inFeature.vertical);
	if (equalRange.first != equalRange.second)
	{
		auto it = equalRange.first;
		++it;
		if (it == equalRange.second)
		{
			outValue.push_back(equalRange.first->second.second);
			return true;
		}

		for (auto it = equalRange.first; it != equalRange.second; ++it)
		{
			if (0 == it->second.first.CompareHorizon(inFeature))
			{
				outValue.push_back(it->second.second);
				return true;
			}
		}

		int nMaxMatchLen = 0;
		auto itSelect = equalRange.second;
		for (auto it = equalRange.first; it != equalRange.second; ++it)
		{
			auto nMatchLen = LCS(it->second.first.horizon, it->second.first.horizonLength, inFeature.horizon, inFeature.horizonLength);
			if (nMatchLen > nMatchLen)
			{
				nMaxMatchLen = nMatchLen;
				itSelect = it;
			}
		}

		if (itSelect != equalRange.second && (2 * nMaxMatchLen) > inFeature.horizonLength)
		{
			outValue.push_back(itSelect->second.second);
			return true;
		}
	}
	else
	{
		//1.识别连在一起的字符
		Feature curFeature;
		for (auto it = mKnowledge.begin(); it != mKnowledge.end(); ++it)
		{
			if (it->second.first.MatchAndSplit(inFeature, curFeature))
			{
				std::string tmpOutValue;
				if (Recognize(tmpOutValue, curFeature))
				{
					outValue.push_back(it->second.second);
					outValue.append(tmpOutValue);
					return true;
				}
			}
		}

		//2.LCS识别
		for (auto it = mKnowledge.begin(); it != mKnowledge.end(); ++it)
		{
			if (2 * LCS(it->second.first.vertical, it->second.first.verticalLength, inFeature.vertical, inFeature.verticalLength) > it->second.first.verticalLength)
			{
				if (2 * LCS(it->second.first.horizon, it->second.first.horizonLength, inFeature.horizon, inFeature.horizonLength) > it->second.first.horizonLength)
				{
					outValue.push_back(it->second.second);
					return true;
				}
			}
		}
	}

	return false;
}

bool CRecognizer::Recognize(std::string& outValue, std::string& outFeature, CScreenImage* pImage)
{
	std::vector<Feature> imageFeatures;
	pImage->ScanAndSplit(outFeature, imageFeatures, mSepCount);

	for(auto& itFeature = imageFeatures.begin();
		itFeature != imageFeatures.end();
		++itFeature)
	{
		if (!Recognize(outValue, *itFeature))
		{
			return false;
		}
	}

	return true;
}

bool CRecognizer::RecognizeEx(std::string& outValue, std::string& outFeature, CScreenImage* pImage)
{
	return Recognize(outValue, outFeature, pImage);
}

int CRecognizer::LCS(const char* a, int m, const char* b, int n) const
{
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
            if(a[i] == b[j])
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

