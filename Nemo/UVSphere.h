#pragma once

#include <Vector3.h>

namespace tc
{

class FStaticMesh;

class FUVSphereModelGenerator
{
public:
    static FStaticMesh* Generate(int segments, int rings, float radius = 1.0f, bool generateUV = true);
};

} /* namespace tc */
