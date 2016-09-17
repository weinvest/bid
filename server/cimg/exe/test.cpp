#include <stdint.h>
#include <cstdio>
#include <jpeglib.h>
#include <jerror.h>
#define cimg_use_jpeg CIMG_USE_JPEG
#define cimg_plugin "plugins/jpeg_buffer.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include "CImg.h"

using namespace cimg_library;

int main(int argc, char** argv)
{
    std::ifstream inFile(argv[1]);
    inFile.seekg(0, std::ios::end);    // go to the end
    int32_t length = inFile.tellg();           // report location (this is the length)
    inFile.seekg(0, std::ios::beg);

    JOCTET * buffer = new uint8_t[length];
    memset(buffer, 0, length);
    inFile.read((char*)buffer, length);

    CImg<uint8_t> image;
    image.load_jpeg_buffer(buffer, length / sizeof(JOCTET));

    std::cout << image.width() << "X" << image.height() << std::endl;

    std::cout << (int32_t)image(10, 10, 0)  << std::endl;
    std::cout << (int32_t)image(10, 10, 1)  << std::endl;
    std::cout << (int32_t)image(10, 10, 2)  << std::endl;
    std::cout << (int32_t)image(10, 10, 3)  << std::endl;

    return 0;
}
