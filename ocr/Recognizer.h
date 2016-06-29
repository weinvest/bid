#pragma once
#include <vector>
#include "Common.h"

class CScreenImage;
class CRecognizer
{
public:
	CRecognizer(void);
	~CRecognizer(void);

	void Initialize(KnowledgeT& knowledge, int sepCount);

	bool Recognize(CString& outValue, CString& outFeature, CScreenImage* pImage);

	bool RecognizeEx(CString& outValue, CString& outFeature, CScreenImage* pImage);

	bool Recognize(CString& outValue, Feature& inFeature);
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
};

