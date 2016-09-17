#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include "FontPattern.h"
FontSample::FontSample()
    :FontSample(0, 0)
{
}

FontSample::FontSample(int32_t ww, int32_t hh)
    :w(ww)
    ,h(hh)
{
}

bool operator < (const FontSample& s1, const FontSample& s2)
{
    return s1.h == s2.h ? s1.w < s2.w : ((s1.h - s2.h) < 0);
}

FontPattern::FontPattern(const std::string& name)
    :mName(name)
    ,mMinW(0)
    ,mMaxW(0)
    ,mMinH(0)
    ,mMaxH(0)
    ,mWidth(0)
    ,mHeight(0)
{
}

void FontPattern::AddSample(const FontSample& sample)
{
    if(!mSamples.insert(sample).second)
    {
        return;
    }

    if(mMinW > sample.w)
    {
        mMinW = sample.w;
    }

    if(mMaxW <= sample.w)
    {
        mMaxW = sample.w + 1;
    }

    if(mMinH > sample.h)
    {
        mMinH = sample.h;
    }

    if(mMaxH <= sample.h)
    {
        mMaxH = sample.h + 1;
    }
}

void FontPattern::ComputeSize( void )
{
    mWidth = GetMaxW() - GetMinW();
    mHeight = GetMaxH() - GetMinW();
}

std::shared_ptr<FontPattern> FontPattern::Load(const std::string& filePath)
{
    if(filePath.length() < 4
        || (filePath.substr(filePath.length() - 3) != ".vc" && filePath.substr(filePath.length() - 4) != ".ivc"))
    {
        return nullptr;
    }


    std::string fileName = filePath.substr(filePath.find_last_of('/') + 1);
    std::string stem = fileName.substr(0, fileName.find('.'));
    std::string value = stem.substr(stem.find_last_of('-') + 1);
    std::ifstream fontFile(filePath, std::ios_base::binary | std::ios_base::in);
    if(!fontFile.is_open())
    {
        std::cout << "open font file " << filePath << " failed\n";
        return nullptr;
    }

    auto pFontPattern = std::make_shared<FontPattern>(fileName);
    pFontPattern->SetValue(value);
    std::string line;
    std::vector<int> cols;

    int32_t nWidth = -1;
    int32_t nH = 0;
    while(std::getline(fontFile, line))
    {
        Split(line, cols);
        if(-1 == nWidth)
        {
            nWidth = cols.size();
        }
        else if(nWidth != cols.size())
        {
            std::cout << "font file " << filePath << " maybe damaged: line " << (nH + 1) << " should be " << nWidth << '\n' ;
            return nullptr;
        }

        int32_t nW = 0;
        for(auto c : cols)
        {
            if(0 != c)
            {
                pFontPattern->AddSample(FontSample(nW++, nH));
            }
        }
        nH++;
    }

    pFontPattern->SetCenter(FontSample(nWidth / 2, nH / 2));

    return pFontPattern;
}

void FontPattern::Split(const std::string& line, std::vector<int>& outValues)
{
    outValues.clear();
    for(auto c : line)
    {
        if(!std::isspace(c))
        {
            outValues.push_back(c - '0');
        }
    }
}

void FontPattern::SetCenter(const FontSample& center)
{
    for(auto& cSample : mSamples)
    {
        auto& sample = const_cast<FontSample&>(cSample);
        sample.w -= center.w;
        sample.h -= center.h;
    }

    mMinW -= center.w;
    mMaxW -= center.w;

    mMinH -= center.h;
    mMaxH -= center.h;
    ComputeSize();
}
