#ifndef _INDEX_SCANNER_H
#define _INDEX_SCANNER_H
#include <map>
#include <vector>
#include <string>
#include "ReCaptcha_types.h"
#include "FontPattern.h"

class FontScanContext;
class IndexScanner
{
public:
    IndexScanner(FontScanContext& context);

    const WindowAreas& Scan(const FontCenterScanIndexRequest& request, CImg<uint8_t>& image, int32_t& guessId);

    void AcceptFeedback(const FontCenterScanIndexFeedbackRequest& feedback);
private:
    double ScanIndex(FontPatterns::const_iterator begin
        , FontPatterns::const_iterator end
        , const WindowArea& area
        , CImg<uint8_t>& image);

    double ScanIndex(FontPattern::Ptr pPattern
        , const WindowArea& area
        , CImg<uint8_t>& image);

    FontPattern::Ptr FindPatternInArea(const WindowArea& area, CImg<uint8_t>& image);

    struct GuessInfo
    {
        typedef GuessInfo* Ptr;
        FontPattern::Ptr pattern;
        int32_t guessCount;
        std::string guessValue;
    };

    FontScanContext& mContext;
    std::map<std::string, FontPatterns> mIndexPatterns;

    std::vector<FontPattern::Ptr> mUnResolvedPatterns;

    int32_t mNextGuessId;
    std::map<int32_t, GuessInfo::Ptr> mGuessInfos;
    std::map<std::string, std::vector<GuessInfo::Ptr> > mGuessValueInfos;
};
#endif
