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

private:
	int GetLCSLength() const { return 128; }
	int& GetLCSValue(int x, int y) const { return mLCSTable[x * GetLCSLength() + y]; };
	int LCS(const CString& a, const CString& b) const;

	char LCSRecognize(CString& feature) const;

	KnowledgeT mKnowledge;
	int mSepCount;


	mutable int *mLCSTable;
};

