#ifndef _BID_COMMON_H
#define _BID_COMMON_H
#include <map>
#include <set>
#include <cstring>

struct Feature
{
	static const int MAX_FEATURE_LENGTH = 32;
	char horizon[MAX_FEATURE_LENGTH];
	char vertical[MAX_FEATURE_LENGTH];
	int8_t horizonLength;
	int8_t verticalLength;

	void Trim(char c, const char* pChars, int8_t& len)
	{
		while (len > 0 && pChars[len] == c)
		{
			--len;
		}

		int8_t startIdx = 0;
		while (startIdx < len && pChars[startIdx] == c)
		{
			++startIdx;
		}

		if (startIdx > 0)
		{
			memmove((void*)pChars, pChars + startIdx, len - startIdx);
			len -= startIdx;
		}
	}

	void Append2Vertical(char c)
	{
		vertical[verticalLength++] = c;
	}

	void Append2Horizon(char c)
	{
		horizon[horizonLength++] = c;
	}

	bool MatchAndSplit(const Feature& pattern, Feature& out)
	{
		int idx = 0;
		while (idx < MAX_FEATURE_LENGTH && ('\0' != vertical[idx]) && (vertical[idx] == pattern.vertical[idx]))
		{
			++idx;
		}

		if ('\0' == vertical[idx])
		{
			strncpy(out.vertical, pattern.vertical + idx, MAX_FEATURE_LENGTH - idx);
			idx = 0;
			while (idx < MAX_FEATURE_LENGTH && ('\0' != horizon[idx]))
			{
				out.horizon[idx] = pattern.horizon[idx] - horizon[idx];
				++idx;
			}
			return true;
		}

		return false;
	}

	int Compare(const Feature& o)
	{
		auto result = strncmp(vertical, o.vertical, MAX_FEATURE_LENGTH);
		if (0 != result)
		{
			return result;
		}

		return strncmp(horizon, o.horizon, MAX_FEATURE_LENGTH);
	}

	friend bool operator < (const Feature& lhs, const Feature& rhs)
	{
		return lhs.Compare(rhs) < 0;
	}

	friend bool operator <= (const Feature& lhs, const Feature& rhs)
	{
		return lhs.Compare(rhs) <= 0;
	}

	friend bool operator == (const Feature& lhs, const Feature& rhs)
	{
		return 0 == lhs.Compare(rhs);
	}

	friend bool operator > (const Feature& lhs, const Feature& rhs)
	{
		return 0 < lhs.Compare(rhs);
	}

	friend bool operator >= (const Feature& lhs, const Feature& rhs)
	{
		return 0 <= lhs.Compare(rhs);
	}
};

typedef std::map<char, std::set<Feature> > FeatureT;
typedef std::map<Feature, char> KnowledgeT;

#endif
