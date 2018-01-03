#pragma once
#include "Resource.h"
#include <Vector3.h>
#include <PODVector.h>

namespace tc
{

class RESOURCE_API RBaseMesh : public RResource
{
public:
    RBaseMesh();
    size_t CountVertices();
    Vector3 GetVertexPos(size_t index);
    Vector3 GetVertexNormal(size_t index);
};

}
