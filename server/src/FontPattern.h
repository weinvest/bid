#ifndef _FONT_PATTERN_H
#define _FONT_PATTERN_H
#include <memory>
#include <vector>
#include <set>

struct FontSample
{
    FontSample();
    FontSample(int32_t ww, int32_t hh);

    int32_t w,h;

    friend bool operator < (const FontSample& s1, const FontSample& s2);
};

class FontPattern
{
public:
    typedef std::shared_ptr<FontPattern> Ptr;
    static  Ptr Load(const std::string& filePath);

    FontPattern(const std::string& name);

    int32_t GetMinW() const { return mMinW; }
    int32_t GetMaxW() const { return mMaxW; }

    int32_t GetMinH() const { return mMinH; }
    int32_t GetMaxH() const { return mMaxH; }

    int32_t GetWidth() const { return GetMaxW() - GetMinW() + 1; }
    int32_t GetHeight() const { return GetMaxH() - GetMinW() + 1; }

    int32_t GetSampleCount() const { return mSamples.size(); }

    const auto& GetSamples() const { return mSamples; }

    const auto& GetValue() const { return mValue; }
private:
    void SetValue(const std::string& v) { mValue = v; }
    void AddSample(const FontSample& sample);
    void ComputeSize( void );
    void SetCenter(const FontSample& center);
    static void Split(const std::string& line, std::vector<int>& outValues);

    std::string mName;
    int32_t mMinW;
    int32_t mMaxW;
    int32_t mMinH;
    int32_t mMaxH;
    int32_t mWidth;
    int32_t mHeight;
    std::set<FontSample> mSamples;
    std::string mValue;
};
#endif
