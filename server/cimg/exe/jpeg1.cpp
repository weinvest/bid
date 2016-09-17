#define cimg_use_jpeg CIMG_USE_JPEG
#include <stdint.h>
#include "CImg.h"

using namespace cimg_library;

int main(int argc, char** argv)
{
    CImg<uint8_t> image(argv[1]), visu(500, 400, 1, 3, 0);
    uint8_t red[] = {255, 0, 0}, green[] = {0, 255, 0}, blue[] = {0, 0, 255};

    image.blur(2.5);

    CImgDisplay mainDisp(image, "Click a point");
    CImgDisplay drawDisp(visu, "Intensity profile");

    while(!mainDisp.is_closed() && !drawDisp.is_closed())
    {
        mainDisp.wait();

        if(mainDisp.button() && mainDisp.mouse_y() >= 0)
        {
            const int y = mainDisp.mouse_y();
            visu.fill(0).draw_graph(image.get_crop(0, y, 0, 0, image.dimx() - 1, y, 0, 0), red, 0, 255, 0);
            visu.draw_graph(image.get_crop(0, y, 0, 1, image.dimx() - 1, y, 0, 1), green, 0, 255, 0);
            visu.draw_graph(image.get_crop(0, y, 0, 2, image.dimx() - 1, y, 0, 2), blue, 0, 255, 0).display(drawDisp);
        }
    }

    return 0;
}

