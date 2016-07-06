#include "Stdafx.h"
#include <cctype>
#include <string>
#include <functional>
#include "IStrategy.h"

bool IStrategy::Load(std::istream& s)
{
	int emptyLineCount(0);
	std::map<std::string, std::string> configurePairs;
	std::string line;
	while (std::getline(s, line))
	{
		Trim(line);
		if (!line.empty())
		{
			emptyLineCount = 0;
			if ('#' != line[0])
			{
				auto splitPos = line.find_first_of(':');
				if (std::string::npos != splitPos)
				{
					auto key = line.substr(0, splitPos);
					auto value = line.substr(splitPos + 1);

					Trim(key);
					Trim(value);
					configurePairs[key] = value;
				}
			}
		}
		else
		{
			emptyLineCount++;
			if (1 == emptyLineCount)
			{
				break;
			}
		}
	}

	auto itName = configurePairs.find("Name");
	if (itName != configurePairs.end())
	{
		SetName(itName->second);
	}
	else
	{
		return false;
	}

	auto itDescription = configurePairs.find("Description");
	if (itDescription != configurePairs.end())
	{
		SetDescription(itDescription->second);
	}
	else
	{
		return false;
	}

	return DoLoad(configurePairs);
}

void IStrategy::Trim(std::string& s)
{
	if (s.length() > 0)
	{
		auto startPos = 0U;
		while(startPos < s.length() && std::isspace(s[startPos]))
		{
			++startPos;
		}

		auto endPos = s.length() - 1;
		while (endPos > startPos && std::isspace(s[endPos]))
		{
			--endPos;
		}

		s = s.substr(startPos, endPos - startPos + 1);
	}
}

bool IStrategy::DoLoad(const std::map<std::string, std::string>& /*configurePairs*/)
{
	return true;
}
