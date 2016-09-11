#ifndef _FONT_PATTERN_H
#define _FONT_PATTERN_H
#include <memory>
struct FontSampleCoor
{
    int32_t w,h;
}

class FontPattern
{
public:
    static std::shared_ptr<FontPattern> Load(const std::string& filePath);

    int32_t GetMinW() const;
    int32_t GetMaxW() const;

    int32_t GetMinH() const;
    int32_t GetMaxH() const;

    int32_t GetWidth() const;
    int32_t GetHeight() const;

private:
    void SetCenter(FontSampleCoor& center);


    std::string mName;
    int32_t mMinW;
    int32_t mMaxW;
    int32_t mMinH;
    int32_t mMaxH;
    std::vector<FontSampleCoor> mSamples;
};
#endif
