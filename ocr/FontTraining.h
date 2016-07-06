#pragma once
#include <ostream>
#include "Common.h"
class CScreenImage;
class CFontTraining
{
public:

	CFontTraining(void);
	
	void Initialize(const std::set<char>& charSet);

	void SetSepCount(int sepCount) { mSepCount = sepCount; }
	bool Traning(std::string& outFeature, CScreenImage* pImage, const std::string& realValue);

	KnowledgeT& BuildKnowledge();

	const KnowledgeT& GetKnowledge() const { return mKnowledge; }
	KnowledgeT& GetKnowledge()  { return mKnowledge; }

	int GetSepCount() const { return mSepCount; }
	void Save(const std::string& filePath);
	void Save(std::ostream& oStream);
	~CFontTraining(void);

	void Clear();
private:
	FeatureT mFeatures;
	KnowledgeT mKnowledge;
	int mSepCount;
};

