#include <bitset>
#include "Spliter.h"
#include "Color.h"

Spliter::Spliter(int32_t nWOffset, int32_t nHOffset
    ,int32_t nRowThreshold, int32_t nColThreshold
    ,int32_t nColorThreshold)
    :mWOffset(nWOffset)
    ,mHOffset(nHOffset)
    ,mRowThreshold(nRowThreshold)
    ,mColThreshold(nColThreshold)
    ,mColorThreshold(nColThreshold)
{}

void Spliter::Split(std::array<WindowAreas*,4>& areas
    , CImg<uint8_t>& image
    , int32_t nLines)
{
    assert(nLines > 0);

    int32_t rowSum[1024];
    int32_t colSum[512];

    memset(rowSum, sizeof(rowSum), 0);
    memset(colSum, sizeof(colSum), 0);

    auto isBackground = [&](int32_t w, int32_t h)
    {
        return Color::IsBackground(image(w, h, 0), image(w, h, 1), image(w, h, 2)) ? 1 : 0;
    };

    for(int32_t h = 1; h < image.height() - 1; ++h)
    {
        for(int32_t w = 1; w < image.width() - 1; ++w)
        {
            if(Color::IsBackground(image(w, h, 0), image(w, h, 1), image(w, h, 2)))
            {
                auto nearbyCount = isBackground(w-1,h-1) + isBackground(w, h-1)
                    + isBackground(w+1,h-1)
                    + isBackground(w-1, h) + isBackground(w+1,h)
                    + isBackground(w-1, h+1) + isBackground(w, h+1) + isBackground(w+1, h+1);

                if(nearbyCount > mColorThreshold)
                {
                    rowSum[w] += 1;
                    colSum[h] += 1;
                }
            }
        }//for row(h, :)
    }

    int32_t nTop = 0;
    while(colSum[nTop++] <= mColThreshold){}

    int32_t nBottom = image.height() - 1;
    while(colSum[nBottom--] <= mColThreshold){}

    auto toNextLeft = [this, rowSum](int32_t nRight) { while(rowSum[++nRight] <= mRowThreshold); return nRight;};
    auto toNextRight = [this, rowSum](int32_t nLeft) { while(rowSum[++nLeft] > mRowThreshold); return nLeft; };
    int32_t nRight = 0;
    for(auto nLeft = toNextLeft(nRight); nLeft < image.width(); nLeft = toNextLeft(nRight))
    {
        nRight = toNextRight(nLeft);

        auto nCLeft = (nRight + nLeft) / 2 - mWOffset;
        auto nCRight = nCLeft + 2 * mWOffset;
        for(int32_t nLine = 0; nLine < nLines; ++nLine)
        {
            auto nCTop = nTop + ((2 * nLine + 1) * (nBottom - nTop)) / (2 * nLines);
            auto nCBottom = nCTop + mHOffset;
            nCTop -= mHOffset;

            WindowArea area;
            area.left = nCLeft, area.right = nCRight;
            area.top = nCTop, area.bottom = nCBottom;
            areas[nLine]->push_back(area);
        }
    }

}
