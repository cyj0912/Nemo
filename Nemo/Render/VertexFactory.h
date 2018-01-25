#pragma once
#include <cstdint>

/*
 * VertexBuffer: just a plain array
 * VertexStreamComponent: Type+Size, bNormalized, Stride, and VertexBuffer
 * VertexStream: Multiple VertexStreamComponents (with index) + Shader parameters
 */

namespace tc
{

enum class EVertexType : uint8_t
{
    Undefined,
    Float1,
    Float2,
    Float3,
    Float4
};

class FVertexBuffer;

class FVertexStreamComponent
{
public:
    const FVertexBuffer* VertexBuffer;
    EVertexType Type;
    uint8_t Stride;
    bool bNormalized;

    FVertexStreamComponent()
        : VertexBuffer(nullptr), Type(EVertexType::Undefined), Stride(0), bNormalized(false)
    {
    }

    FVertexStreamComponent(const FVertexBuffer* vb, EVertexType type, uint8_t stride, bool normalized)
        : VertexBuffer(vb), Type(type), Stride(stride), bNormalized(normalized)
    {
    }
};

class FVertexStream
{
public:
};

}
