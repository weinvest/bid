#ifndef _FONT_PATTERN_H
#define _FONT_PATTERN_H
#include <memory>
class FontPattern
{
public:
    static std::shared_ptr<FontPattern> Load(const std::string& filePath);

private:
    std::vector<> ;
};
#endif
