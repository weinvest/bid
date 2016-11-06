#include "IndexScanner.h"
#include "FontScanContext.h"
#include "Color.h"

IndexScanner::IndexScanner(FontScanContext& context)
    :mContext(context)
    ,mNextGuessId(0)
{
    for(auto pPattern : context.GetIndexPatterns())
    {
        mIndexPatterns[pPattern->GetValue()].push_back(pPattern);
    }
}

double IndexScanner::ScanIndex(FontPattern::Ptr pPattern
    , const WindowArea& area
    , CImg<uint8_t>& image)
{
    double similar = 0.0;
    for(auto nCH = area.top; nCH < area.bottom; ++nCH)
    {
        for(auto nCW = area.left; nCW < area.right; ++nCW)
        {
            double loseCount = 0.0;
            double matchCount = 0.0;
            for(auto& sample : pPattern->GetSamples())
            {
                int32_t w = nCW + sample.w;
                int32_t h = nCH + sample.h;

                if(w >= image.width() || w < 0 || h >= image.height() || h < 0)
                {
                    loseCount += 1.0;
                }
                else if(Color::IsBackground(image(w, h, 0), image(w, h, 1), image(w, h, 2)))
                {
                    loseCount += 1.0;
                }
                else
                {
                    matchCount += 1.0;
                }
            }

            double curSimilar = matchCount / (matchCount + loseCount);
            if(curSimilar > similar)
            {
                similar = curSimilar;
            }
        }
    }

    return similar;
}

double IndexScanner::ScanIndex(FontPatterns::const_iterator begin
    , FontPatterns::const_iterator end
    , const WindowArea& area
    , CImg<uint8_t>& image)
{
    double similar = 0;
    for(auto itPattern = begin; itPattern != end; ++itPattern)
    {
        double similar1 = ScanIndex(*itPattern, area, image);
        if(similar1 > similar)
        {
            similar = similar1;
        }
    }

    return similar;
}

FontPattern::Ptr IndexScanner::FindPatternInArea(const WindowArea& area, CImg<uint8_t>& image)
{
    double maxSimilar = 0.0;
    auto itResultPattern = mUnResolvedPatterns.end();
    for(auto itPattern = mUnResolvedPatterns.begin(); itPattern != mUnResolvedPatterns.end(); ++itPattern)
    {
        double similar = ScanIndex(*itPattern, area, image);
        if(similar > maxSimilar)
        {
            maxSimilar = similar;
            itResultPattern = itPattern;
        }
    }

    if(itResultPattern == mUnResolvedPatterns.end())
    {
        auto pResultPattern = FontPattern::ClipInArea(image, area, "index");
        mUnResolvedPatterns.push_back(pResultPattern);
        return pResultPattern;
    }
    else
    {
        return *itResultPattern;
    }
}

const WindowAreas& IndexScanner::Scan(const FontCenterScanIndexRequest& request, CImg<uint8_t>& image, int32_t& guessId)
{
    auto itPattern = mIndexPatterns.find(request.desiredIndexValue);
    if(mIndexPatterns.end() != itPattern)
    {
        auto similar1 = ScanIndex(itPattern->second.cbegin()
            , itPattern->second.cend()
            , request.centerWindows1[0]
            , image);

        auto similar2 = ScanIndex(itPattern->second.cbegin()
            , itPattern->second.cend()
            , request.centerWindows2[0]
            , image);

        if(similar1 > similar2)
        {
            return request.centerWindows1;
        }

        return request.centerWindows2;
    }
    else
    {
        auto itGuessValueInfo = mGuessValueInfos.find(request.desiredIndexValue);
        auto pPattern1 = FindPatternInArea(request.centerWindows1[0], image);
        auto pPattern2 = FindPatternInArea(request.centerWindows2[0], image);
        GuessInfo::Ptr pGuessInfo = nullptr;
        if(mGuessValueInfos.end() != itGuessValueInfo)
        {
            auto findGuessInfo = [this, itGuessValueInfo](FontPattern::Ptr pPattern, const std::string& value)
            {
                for(auto pGuessInfo : itGuessValueInfo->second)
                {
                    if(pGuessInfo->pattern == pPattern)
                    {
                        return pGuessInfo;
                    }
                }

                auto pGuessInfo = new GuessInfo{pPattern, 0, value};
                itGuessValueInfo->second.push_back(pGuessInfo);
                return pGuessInfo;
            };

            auto pGuessInfo1 = findGuessInfo(pPattern1, request.desiredIndexValue);
            auto pGuessInfo2 = findGuessInfo(pPattern2, request.desiredIndexValue);
            pGuessInfo = pGuessInfo1->guessCount < pGuessInfo2->guessCount ? pGuessInfo1 : pGuessInfo2;
        }
        else
        {
            auto pGuessInfo1 = new GuessInfo{pPattern1, 0, request.desiredIndexValue};
            auto pGuessInfo2 = new GuessInfo{pPattern2, 0, request.desiredIndexValue};

            auto& guessInfos = mGuessValueInfos[request.desiredIndexValue];
            guessInfos.push_back(pGuessInfo1);
            guessInfos.push_back(pGuessInfo2);

            pGuessInfo = pGuessInfo1;
        }

        pGuessInfo->guessCount += 1;
        guessId = mNextGuessId++;
        mGuessInfos[guessId] = pGuessInfo;
        return pGuessInfo->pattern == pPattern1 ? request.centerWindows1 : request.centerWindows2;
    }
}

void IndexScanner::AcceptFeedback(const FontCenterScanIndexFeedbackRequest& feedback)
{
    if(feedback.success)
    {
        auto itGuessInfo = mGuessInfos.find(feedback.guessId);
        if(itGuessInfo != mGuessInfos.end())
        {
            auto pPattern = itGuessInfo->second->pattern;
            auto& value = itGuessInfo->second->guessValue;
            mGuessValueInfos.erase(value);
            mGuessInfos.erase(itGuessInfo);

            auto itUnResolvedPattern = std::find(mUnResolvedPatterns.begin(), mUnResolvedPatterns.end(), pPattern);
            if(itUnResolvedPattern != mUnResolvedPatterns.end())
            {
                mUnResolvedPatterns.erase(itUnResolvedPattern);
            }

            pPattern->SetValue(value);

            mIndexPatterns[value].push_back(pPattern);
        }
    }
}
