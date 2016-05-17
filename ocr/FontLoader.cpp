#include "StdAfx.h"
#include <sstream>
#include "FontLoader.h"


CFontLoader::CFontLoader(void)
{
}


CFontLoader::~CFontLoader(void)
{
}

void CFontLoader::Load(KnowledgeT& outKnowledge, int& sepCount, const CString& path)
{
	CFile file(path, CFile::modeRead);

	std::string content;
	content.resize(file.GetLength());

	file.Read((void*)content.data(), content.size());
	file.Close();

	std::string line;
	std::stringstream fontStream(content);
	fontStream >> sepCount;

	while(std::getline(fontStream, line))
	{
		if(!line.empty())
		{
			auto pos = line.find_last_of(':');
			if(pos == line.length() - 1)
			{
				pos--;
			}

			CString feature = line.substr(0, pos).c_str();
			char c = line[pos + 1];

			outKnowledge[feature] = c;
		}
	}
}