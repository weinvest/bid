#include "Color.h"
#include "FontScanner.h"
#include "FontScanContext.h"
#include <tbb/task_scheduler_init.h>
const std::string FontScanJoiner::EMPTY_VALUE;
const double FontScanJoiner::TOLERATE_RATE = 0.2;

FontScanJoiner::FontScanJoiner(const WindowArea& area
    , const CImg<uint8_t>& image
    , FontScanContext& context)
    :mArea(area)
    ,mImage(image)
    ,mContext(context)
    ,mScanResult(nullptr)
{}

FontScanJoiner::FontScanJoiner(FontScanJoiner& o, tbb::split)
    :mArea(o.mArea)
    ,mImage(o.mImage)
    ,mContext(o.mContext)
    ,mScanResult(nullptr)
{}

FontScanJoiner::~FontScanJoiner()
{

}

const auto& FontScanJoiner::GetValue() const
{
    return nullptr == mScanResult ? EMPTY_VALUE : mScanResult->pattern->GetValue();
}

FontScanResult* FontScanJoiner::Scan(int32_t nCW, int32_t nCH, FontPattern::Ptr pPattern)
{
    FontScanResult* pCurrentResult = mContext.CreateFontScanResult();
    pCurrentResult->pattern = pPattern;
    for(auto& sample : pPattern->GetSamples())
    {
        int32_t w = nCW + sample.w;
        int32_t h = nCH + sample.h;

        if(w >= mImage.width() || w < 0 || h >= mImage.height() || h < 0)
        {
            pCurrentResult->unMatched.emplace(w, h);
        }
        else if(Color::IsBackground(mImage(w, h, 0), mImage(w, h, 1), mImage(w, h, 2)))
        {
            pCurrentResult->unMatched.emplace(w, h);
        }
        else
        {
            pCurrentResult->matched.emplace(w, h);
        }
    }

    int32_t tolerateCount = pPattern->GetSampleCount() * TOLERATE_RATE;
    if(pCurrentResult->unMatched.size() <= tolerateCount)
    {
        return pCurrentResult;
    }

    return nullptr;
}

FontScanResult* FontScanJoiner::Compare(FontScanResult* l, FontScanResult* r)
{
    if(nullptr == l)
    {
        return r;
    }

    if(nullptr == r)
    {
        return l;
    }

    if(l->pattern->GetSampleCount() == r->pattern->GetSampleCount())
    {
        if(l->unMatched.size() < r->unMatched.size())
        {
            return l;
        }
        return r;
    }

    if(l->pattern->GetSampleCount() < r->pattern->GetSampleCount())
    {
        auto t = r;
        r = l;
        l = t;
    }

    auto diff = double(l->unMatched.size()) / l->pattern->GetSampleCount()
         - double(r->unMatched.size()) / r->pattern->GetSampleCount();

    if(diff < -1e-7)
    {
        return l;
    }

    auto loseDiff = double(l->unMatched.size() - r->unMatched.size()) / double(l->pattern->GetSampleCount() - r->pattern->GetSampleCount());
    if(loseDiff < 0.2)
    {
        return l;
    }

    return r;
}

void FontScanJoiner::operator()(const tbb::blocked_range<FontPatterns::const_iterator>& b)
{
    FontScanResult* pFontScanResult = nullptr;
    for(auto itPattern = b.begin(); itPattern != b.end(); ++itPattern)
    {
        for(auto nH = mArea.top; nH < mArea.bottom; ++nH)
        {
            for(auto nW = mArea.left; nW < mArea.right; ++nW)
            {
                auto pCurResult = Scan(nW, nH, *itPattern);
                pFontScanResult = Compare(pFontScanResult, pCurResult);
            }
        }
    }//for each pattern

    mScanResult = pFontScanResult;
}

void FontScanJoiner::join(const FontScanJoiner& o)
{
    mScanResult = Compare(mScanResult, o.mScanResult);
}


FontScanner::FontScanner(int32_t nThreads)
{
    //tbb::task_scheduler_init init(nThreads);
    static tbb::task_scheduler_init init(1);
}

void FontScanner::Scan(FontCenterScanResponse& resp
    , WindowAreas::const_iterator areaBegin
    , WindowAreas::const_iterator areaEnd
    , CImg<uint8_t>& image
    , FontScanContext& context)
{
    for(auto itArea = areaBegin; itArea != areaEnd; ++itArea)
    {
        FontScanJoiner joiner(*itArea, image, context);
        auto& fontPatterns = context.GetFontPatterns();
        tbb::parallel_reduce( tbb::blocked_range<FontPatterns::const_iterator>(fontPatterns.begin(), fontPatterns.end()), joiner );
        resp.results.push_back(joiner.GetValue());
    }

    resp.status = 0;
}
