#include "IndexScanner.h"

IndexScanner::IndexScanner(FontScanContext& context)
    :mContext(context)
{
    for(auto pPattern : context.GetIndexPatterns())
    {
        mIndexPatterns[pPattern->GetValue()].push_back(pPattern);
    }
}

double IndexScanner::ScanIndex(FontPattern::Ptr pPattern
    , WindowArea& area
    , CImg<uint8_t>& image)
{
    double similar = 0.0;
    for(auto nCH = mArea.top; nCH < mArea.bottom; ++nCH)
    {
        for(auto nCW = mArea.left; nCW < mArea.right; ++nCW)
        {
            double loseCount = 0.0;
            double matchCount = 0.0;
            for(auto& sample : pPattern->GetSamples())
            {
                int32_t w = nCW + sample.w;
                int32_t h = nCH + sample.h;

                if(w >= mImage.width() || w < 0 || h >= mImage.height() || h < 0)
                {
                    loseCount += 1.0;
                }
                else if(Color::IsBackground(mImage(w, h, 0), mImage(w, h, 1), mImage(w, h, 2)))
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
    , WindowArea& area
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

WindowAreas& IndexScanner::Scan(FontCenterScanIndexRequest& request, int32_t& guessId)
{
    auto itPattern = mIndexPatterns.find(request.desiredIndexValue);
    if(mIndexPatterns.end() != itPattern)
    {
        auto similar1 = ScanIndex(itPattern->second.begin()
            , itPattern->second.end()
            , request.centerWindows1[0]
            , request.image);

        auto similar2 = ScanIndex(itPattern->second.begin()
            , itPattern->second.end()
            , request.centerWindows2[0]
            , request.image);

        if(similar1 > similar2)
        {
            return request.centerWindows1;
        }

        return return request.centerWindows2;
    }
    else
    {

    }
}

void IndexScanner::AcceptFeedback(FontCenterScanIndexFeedbackRequest& feedback)
{
    if(feedback.success)
    {
        auto itGuessInfo = mGuessInfos.find(feedback.guessId);
        if(itGuessInfo != mGuessInfos.end())
        {
            auto pPattern = itGuessInfo->second.pattern;
            auto& value = itGuessInfo->second.guessValue;
            mGuessValueInfos.erase(value);
            mGuessInfos.erase(itGuessInfo);

            pPattern->SetValue(value);

            mIndexPatterns[value].push_back(pPattern);
        }
    }
}
