#include <boost/filesystem.hpp>
#include "FontScanContext.h"
#include "FileHandler.h"

FontScanContext::FontScanContext(const std::string& fontRootDirectory)
    :mCurrentFontScan(0)
{
    PathVec idxPatternPaths;
    std::string idxWild(fontRootDirectory + "/*.ivc");
    auto idxReg = Wildcard2Regex(idxWild);
    FindFiles(fontRootDirectory, idxReg, idxPatternPaths);
    for(auto& p : idxPatternPaths)
    {
        auto pIdxPattern = FontPattern::Load(p.string());
        if(nullptr != pIdxPattern)
        {
            mIndexPatterns.push_back(pIdxPattern);
        }
    }

    PathVec fontPatternPaths;
    auto fontReg = Wildcard2Regex(fontRootDirectory + "/*.vc");
    FindFiles(fontRootDirectory, fontReg, fontPatternPaths);
    for(auto& p : fontPatternPaths)
    {
        auto pFontPattern = FontPattern::Load(p.string());
        if(nullptr != pFontPattern)
        {
            mFontPatterns.push_back(pFontPattern);
        }
    }
}

FontScanResult* FontScanContext::CreateFontScanResult( void )
{
    return &mFontScanResults[mCurrentFontScan.fetch_add(1, std::memory_order_relaxed) % MAX_FONT_SCAN_RESULT_POOL_SIZE];
}
