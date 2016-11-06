#ifndef _SPLITER_H
#define _SPLITER_H
#include "FontScanDataTypes.h"
class Spliter
{
public:
    Spliter(int32_t nWOffset = 2, int32_t nHOffset = 3
        , int32_t nRowThreshold = 5, int32_t nColThreshold = 0
        , int32_t nColorThreshold = 3);

    void Split(std::array<WindowAreas*,4>& areas, CImg<uint8_t>& image, int32_t nLines);
private:
    int32_t mWOffset;
    int32_t mHOffset;

    int32_t mRowThreshold;
    int32_t mColThreshold;
    int32_t mColorThreshold;
};
#endif
