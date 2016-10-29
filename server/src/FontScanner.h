#ifndef _FONT_SCANNER_H
#define _FONT_SCANNER_H

#include <stdint.h>
#include <atomic>
#include <string>
#include "FontPattern.h"
#include "tbb/parallel_reduce.h"
#include "tbb/blocked_range.h"
#include "FontScanDataTypes.h"

class FontScanContext;
struct FontScanResult;
class FontScanJoiner
{
public:
    FontScanJoiner(const WindowArea& area
        , const CImg<uint8_t>& image
        , FontScanContext& context);

    FontScanJoiner(FontScanJoiner& o, tbb::split);

    ~FontScanJoiner();

    void operator()(const tbb::blocked_range<FontPatterns::const_iterator>& b);

    void join(FontScanJoiner& o);

    const auto& GetValue() const;
private:
    FontScanResult* Scan(int32_t nW, int32_t nH, FontPattern::Ptr pPattern);
    FontScanResult* Compare(FontScanResult* l, FontScanResult* r);

    static const std::string EMPTY_VALUE;
    static const double TOLERATE_RATE;

    const WindowArea& mArea;
    const CImg<uint8_t>& mImage;
    FontScanContext& mContext;

    FontScanResult* mScanResult;
};

class FontScanner
{
public:
    FontScanner(int32_t nThreads);

    void Scan(FontCenterScanResponse& resp
        , WindowAreas::const_iterator areaBegin
        , WindowAreas::const_iterator areaEnd
        , CImg<uint8_t>& image
        , FontScanContext& context);

};
#endif
