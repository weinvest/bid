#pragma once
#include "Common.h"
class CFontLoader
{
public:
	CFontLoader(void);

	void Load(KnowledgeT& outKnowledge, int& sepCount, const std::string& path);

	~CFontLoader(void);
};

