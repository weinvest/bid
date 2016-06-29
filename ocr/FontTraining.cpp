#include "StdAfx.h"
#include <vector>
#include "ScreenImage.h"
#include "FontTraining.h"
#include <fstream>
CFontTraining::CFontTraining(void)
	:mSepCount(1)
{
}


CFontTraining::~CFontTraining(void)
{
}

void CFontTraining::Initialize(const std::set<char>& charSet)
{
	for(std::set<char>::iterator itChar = charSet.begin(); itChar != charSet.end(); ++itChar)
	{
		mFeatures[*itChar] = std::set<Feature>();
	}
}

bool CFontTraining::Traning(CString& outFeature, CScreenImage* pImage, const CString& realValue)
{
	if(nullptr == pImage)
	{
		return false;
	}

	std::vector<Feature> features;
	pImage->ScanAndSplit(outFeature, features, mSepCount);

	if(!features.empty() && realValue.GetLength() == features.size())
	{
		for(int iChar = 0; iChar < realValue.GetLength(); ++iChar)
		{
		    char c = realValue.GetAt(iChar);
			if(mFeatures.find(c) != mFeatures.end())
			{
				mFeatures[c].insert(features[iChar]);
			}
		}
		return true;
	}
	return false;
}

KnowledgeT& CFontTraining::BuildKnowledge()
{
	for(FeatureT::iterator itChar = mFeatures.begin();
		itChar != mFeatures.end();
		++itChar)
	{
		for(auto itFeature = itChar->second.begin();
			itFeature != itChar->second.end();
			++itFeature)
		{
			mKnowledge[*itFeature] = itChar->first;
		}
	}

	return mKnowledge;
}

void CFontTraining::Save(const CString& filePath)
{
	std::ofstream file((LPCTSTR)filePath, std::ios_base::binary | std::ios_base::out);
	Save(file);
}

void CFontTraining::Save(std::ostream& oStream)
{
	oStream << mSepCount << '\n';
	for(FeatureT::iterator itChar = mFeatures.begin();
		itChar != mFeatures.end();
		++itChar)
	{
		for(auto itFeature = itChar->second.begin();
			itFeature != itChar->second.end();
			++itFeature)
		{
			oStream << itFeature->vertical << "+" << itFeature->horizon << ":" <<  itChar->first << '\n';
		}
	}
}


void  CFontTraining::Clear()
{
	for (auto& p : mFeatures)
	{
		p.second.clear();
	}

	mKnowledge.clear();
}