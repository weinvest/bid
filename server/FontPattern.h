#ifndef _FONT_PATTERN_H
#define _FONT_PATTERN_H
#include <memory>
#include <vector>
#include <set>

struct FontSample
{
    int32_t w,h;

    friend bool operator < (const FontSample& s1, const FontSample& s2);
}

class FontPattern
{
public:
    static std::shared_ptr<FontPattern> Load(const std::string& filePath);

    FontPattern(const std::string& name);

    int32_t GetMinW() const { return mMinW; }
    int32_t GetMaxW() const { return mMaxW; }

    int32_t GetMinH() const { return mMinH; }
    int32_t GetMaxH() const { return mMaxH; }

    int32_t GetWidth() const { return GetMaxW() - GetMinW() + 1; }
    int32_t GetHeight() const { return GetMaxH() - GetMinW() + 1; }

private:
    void AddSample(FontSample& sample);
    void ComputeSize( void );
    void SetCenter(FontSample& center);
    static void Split(const std::string& line, std::vector<int>& outValues);

    std::string mName;
    int32_t mMinW;
    int32_t mMaxW;
    int32_t mMinH;
    int32_t mMaxH;
    int32_t mWidth;
    int32_t mHeight;
    std::set<FontSample> mSamples;
};
#endif
