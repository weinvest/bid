#include "FontPattern.h"

std::shared_ptr<FontPattern> FontPattern::Load(const std::string& filePath)
{
    
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
