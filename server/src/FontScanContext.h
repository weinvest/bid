#ifndef _FONT_CONTEXT_H
#define _FONT_CONTEXT_H
#include "FontScanDataTypes.h"
#include <array>
#include <atomic>
#include <vector>

class FontScanContext
{
public:
    FontScanContext(const std::string& fontRootDirectory);

    FontScanResult* CreateFontScanResult( void );

    const FontPatterns& GetFontPatterns() const { return mFontPatterns; }
    const FontPatterns& GetIndexPatterns() const { return mIndexPatterns; }
private:
    static const int32_t MAX_FONT_SCAN_RESULT_POOL_SIZE = 10000;

    FontPatterns mIndexPatterns;
    FontPatterns mFontPatterns;
    std::array<FontScanResult, MAX_FONT_SCAN_RESULT_POOL_SIZE> mFontScanResults;
    std::atomic_int mCurrentFontScan;
};

#endif
