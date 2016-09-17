#ifndef _FONT_SCAN_DATA_TYPES_H
#define _FONT_SCAN_DATA_TYPES_H

#include <cstdio>
#include <jpeglib.h>
#include <jerror.h>
#define cimg_use_jpeg CIMG_USE_JPEG
#define cimg_plugin "plugins/jpeg_buffer.h"

#include "CImg.h"
#include "proto/ReCaptcha_types.h"
#include "FontPattern.h"
using namespace cimg_library;
typedef std::vector<WindowArea> WindowAreas;
typedef std::vector<FontPattern::Ptr> FontPatterns;

struct FontScanResult
{
    FontSample center;
    FontPattern::Ptr pattern;
    std::set<FontSample> matched;
    std::set<FontSample> unMatched;
};

#endif