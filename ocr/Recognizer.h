#pragma once
#include <vector>
#include <array>
#include "Common.h"

class CScreenImage;
class CRecognizer
{
public:
	CRecognizer(void);
	~CRecognizer(void);

	void Initialize(KnowledgeT& knowledge, int sepCount);

	bool Recognize(std::string& outValue, std::string& outFeature, CScreenImage* pImage);

	bool RecognizeEx(std::string& outValue, std::string& outFeature, CScreenImage* pImage);

	bool Recognize(std::string& outValue, Feature& inFeature);
private:
	int GetLCSLength() const { return 128; }
	int& GetLCSValue(int x, int y) const { return mLCSTable[x * GetLCSLength() + y]; };
	int LCS(const char* a, int m, const char* b, int n) const;

	struct StringComparator
	{
		bool operator () (const char* lhs, const char* rhs)
		{
			return strcmp(lhs, rhs) < 0;
		}
	};
	typedef std::multimap<const char*, std::pair<Feature,char>, StringComparator> RKnowledgeT;
	RKnowledgeT mKnowledge;
	int mSepCount;


	mutable int *mLCSTable;
	std::array<std::string, 64> mKeyPool;
	int mNextAviableKey;
};

