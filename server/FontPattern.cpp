#include <fstream>
#include "FontPattern.h"

std::shared_ptr<FontPattern> FontPattern::Load(const std::string& filePath)
{
    if(filePath.length() < 4 || filePath.substr(filePath.length() - 3) != ".vc")
    {
        return nullptr;
    }


    std::string fileName = filePath.substr(filePath.find_last_of('/') + 1);
    std::string stem = fileName.substr(0, fileName.length() - 3);

    std::ifstream fontFile(filePath, std::ios_base::binary | std::ios_base::in);
}

def __init__(self, name, pattern):
    self.name = name
    self.height, self.width = pattern.shape
    elements = []
    top, left = self.height / 2, self.width / 2
    for h in xrange(0, pattern.shape[0]):
        for w in xrange(0, pattern.shape[1]):
            if 0 != pattern[h, w]:
                ww, hh = w - left, h - top
                elements.append((ww, hh))

    self.elements = elements
