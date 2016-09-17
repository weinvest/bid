#include <algorithm>
#include "Color.h"
const uint8_t Color::BACKGROUND_MIN_THRESHOLD = 20;
const uint8_t Color::BACKGROUND_MAX_THRESHOLD = 235;
const uint8_t Color::BACKGROUND_H_V_THRESHOLD = 70 * 2.56;

bool Color::IsBackground(uint8_t r, uint8_t g, uint8_t b)
{
    if(r < BACKGROUND_MIN_THRESHOLD && g < BACKGROUND_MIN_THRESHOLD && b < BACKGROUND_MIN_THRESHOLD)
    {
        return true;
    }

    if(r > BACKGROUND_MAX_THRESHOLD && g > BACKGROUND_MAX_THRESHOLD && b > BACKGROUND_MAX_THRESHOLD)
    {
        return true;
    }

    double h, s, v;
    RGB2HSV(r, g, b, h, s, v);
    return (v - s) > BACKGROUND_H_V_THRESHOLD;
}

void Color::RGB2HSV(uint8_t r, uint8_t g, uint8_t b, double& h, double& s, double& v)
{
    auto maxC = std::max(r, std::max(g, b));
    auto minC = std::min(r, std::min(g, b));
    if(maxC == minC)
    {
        h = 0.0;
        s = 0.0;
        v = maxC;
        return;
    }

    double rng = maxC - minC;
    h = 0.0;
    if(r == maxC)
    {
        h = (g - b) / rng;
    }
    else if(g == maxC)
    {
        h = 2 + (b - r) / rng;
    }
    else
    {
        h = 4 + (r - g) / rng;
    }

    h *= 60.0;
    if(h < -1e-7)
    {
        h += 360;
    }

    s = rng * 256.0 / maxC;
    v = maxC;
}
