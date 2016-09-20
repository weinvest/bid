#include "IndexScanner.h"
#include "FontScanContext.h"
#include "Color.h"

IndexScanner::IndexScanner(FontScanContext& context)
    :mContext(context)
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
        return request.centerWindows2;
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

            pPattern->SetValue(value);

            mIndexPatterns[value].push_back(pPattern);
        }
    }
}
