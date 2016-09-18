#ifndef _INDEX_SCANNER_H
#define _INDEX_SCANNER_H
#include <map>
#include <vector>
#include <string>
#include "ReCaptcha_types.h"
#include "FontScanDataTypes.h"
#include "FontPattern.h"

class FontScanContext;
class IndexScanner
{
public:
    IndexScanner(FontScanContext& context);

    WindowAreas& Scan(FontCenterScanIndexRequest& request, int32_t& guessId);

    void AcceptFeedback(FontCenterScanIndexFeedbackRequest& feedback);
private:
    double ScanIndex(FontPatterns::const_iterator begin
        , FontPatterns::const_iterator end
        , WindowArea& area
        , CImg<uint8_t>& image);

    double ScanIndex(FontPattern::Ptr pPattern
        , WindowArea& area
        , CImg<uint8_t>& image);

    struct GuessInfo
    {
        typedef GuessInfo* Ptr;
        Pattern::Ptr pattern;
        int32_t guessCount;
        std::string guessValue;
    };

    FontScanContext& mContext;
    std::map<std::string, FontPatterns> > mIndexPatterns;
    std::map<int32_t, GuessInfo::Ptr> mGuessInfos;
    std::map<std::string, std::vector<GuessInfo::Ptr> > mGuessValueInfos;
};
#endif
