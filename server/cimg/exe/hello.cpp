#include "CImg.h"

namespace cimg = cimg_library;

int main()
{
    cimg::CImg<unsigned char> img(640, 400, 1, 3);
    img.fill(0);

    unsigned char purple[] = {255, 0, 255};

    img.draw_text(100, 100, "Hello world", purple);

    img.display("My frist Cimg code");

    return 0;
}

