#pragma once
#include <EnumClass.h>
#include <cstdint>

namespace tc
{

enum class EPixelFormat
{
    Unknown = 0,

    //Color

    R8,
    R8G8,
    R8G8B8,
    R8G8B8A8,

    //Depth Stencil

    D24S8,

    //Compressed formats

    BC1,
    BC2,
    BC3,
    BC4,
    BC5,

    Count
};

enum EInputElementFormat
{
    IEF_Float,
    IEF_Float2,
    IEF_Float3,
    IEF_Float4,
};

enum class EBufferUsageFlags : uint32_t
{
    None = 0,
    Static = 1 << 0,
    Dynamic = 1 << 1,

    //Add immutable buffer storage and use glBufferStorage
};

DEFINE_ENUM_CLASS_BITWISE_OPERATORS(EBufferUsageFlags)

enum class EBufferLockOptions : uint32_t
{
    ReadOnly,
    WriteOnly,
    ReadWrite,
    WriteDiscard,
    NoOverwrite
};

enum class ETextureUsageFlags : uint32_t
{
    None = 0,
    RenderTarget = 1 << 0,
    DepthStencilTarget = 1 << 1,
    ShaderResource = 1 << 2,
    sRGB = 1 << 3,
    Dynamic = 1 << 4
};

DEFINE_ENUM_CLASS_BITWISE_OPERATORS(ETextureUsageFlags)

enum class EPrimitiveType
{
    TriangleList,
    TriangleStrip,
    LineList,
    QuadList,
    PointList
};

enum ESemanticName
{
    SN_BINORMAL,
    SN_BLENDINDICES,
    SN_BLENDWEIGHT,
    SN_COLOR,
    SN_NORMAL,
    SN_POSITION,
    SN_POSITIONT,
    SN_PSIZE,
    SN_TANGENT,
    SN_TEXCOORD,
};

struct FInputElementDesc
{
    ESemanticName SemanticName;
    uint32_t SemanticIndex;
    EInputElementFormat Format;
    uint32_t InputSlot;
    uint32_t AlignedByteOffset;
    bool IsPerInstance;
    uint32_t InstanceDataStepRate;
};

} /* namespace tc */
