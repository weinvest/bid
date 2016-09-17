#ifndef _COLOR_H
#define _COLOR_H
#include <stdint.h>
class Color
{
public:
    static const uint8_t BACKGROUND_MIN_THRESHOLD;
    static const uint8_t BACKGROUND_MAX_THRESHOLD;
    static const uint8_t BACKGROUND_H_V_THRESHOLD;

    static bool IsBackground(uint8_t r, uint8_t g, uint8_t b);

    static void RGB2HSV(uint8_t r, uint8_t g, uint8_t b
        , double& h, double& s, double& v);
};
#endif
