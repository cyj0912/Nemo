#pragma once

#include <Vector3.h>

namespace tc
{

class FUVSphereModelGenerator
{
public:
    void Generate(int segments, int rings, bool generateUV);
};

} /* namespace tc */
