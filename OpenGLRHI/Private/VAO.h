#pragma once

#include <UsingStl.h>
#include <functional>

namespace tc
{

static const int MaxInputSlot = 16;

struct FVAOAttribute
{
    GLuint AttribIndex;
    int Size;
    GLenum Type;
    GLuint Offset;
    uint32_t Slot;

    bool operator==(const FVAOAttribute& rhs) const
    {
        return AttribIndex == rhs.AttribIndex && Size == rhs.Size && Type == rhs.Type && Offset == rhs.Offset &&
               Slot == rhs.Slot;
    }

    bool operator!=(const FVAOAttribute& rhs) const { return !operator==(rhs); }

    bool operator<(const FVAOAttribute& rhs) const
    {
        return std::tie(AttribIndex, Size, Type, Offset, Slot) <
               std::tie(rhs.AttribIndex, rhs.Size, rhs.Type, rhs.Offset, rhs.Slot);
    }

    bool operator>(const FVAOAttribute& rhs) const { return rhs < *this; }

    bool operator<=(const FVAOAttribute& rhs) const { return !(*this > rhs); }

    bool operator>=(const FVAOAttribute& rhs) const { return !(*this < rhs); }
};

int InputElementFormatToSize(EInputElementFormat format)
{
    switch (format)
    {
    case IEF_Float:
        return 1;
    case IEF_Float2:
        return 2;
    case IEF_Float3:
        return 3;
    case IEF_Float4:
        return 4;
    }
}

GLenum InputElementFormatToType(EInputElementFormat format)
{
    switch (format)
    {
    case IEF_Float:
    case IEF_Float2:
    case IEF_Float3:
    case IEF_Float4:
        return GL_FLOAT;
    }
}

class FVAOInputLayout : public FRHIInputLayout
{
public:
    void AddAttribute(const FInputElementDesc& desc)
    {
        FVAOAttribute newAttr = {
                desc.SemanticIndex,
                InputElementFormatToSize(desc.Format),
                InputElementFormatToType(desc.Format),
                desc.AlignedByteOffset,
                desc.InputSlot
        };
        Attributes.insert(newAttr);
    }

    bool operator==(const FVAOInputLayout& rhs) const
    {
        return Attributes == rhs.Attributes;
    }

    bool operator!=(const FVAOInputLayout& rhs) const
    {
        return !operator==(rhs);
    }

    friend struct std::hash<FVAOInputLayout>;
    set<FVAOAttribute> Attributes;
};

//TODO: move them to somewhere standard
inline void hash_combine_impl(size_t& seed, size_t value)
{
    seed ^= value + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

template <class T>
inline void hash_combine(size_t& seed, T const& v)
{
    std::hash<T> hasher;
    return hash_combine_impl(seed, hasher(v));
}

template <class T>
struct hash
{
};

template <class T>
inline void tc_hash_combine(size_t& seed, T const& v)
{
    hash<T> hasher;
    return hash_combine_impl(seed, hasher(v));
}

template<>
struct hash<FVAOAttribute>
{
    size_t operator()(const FVAOAttribute& obj) const noexcept
    {
        size_t seed = 0;
        hash_combine(seed, obj.AttribIndex);
        hash_combine(seed, obj.Size);
        hash_combine(seed, obj.Type);
        hash_combine(seed, obj.Offset);
        return seed;
    }
};

template<>
struct hash<FVAOInputLayout>
{
    size_t operator()(const FVAOInputLayout& obj) const noexcept
    {
        size_t seed = 0;
        for (const FVAOAttribute& attr : obj.Attributes)
        {
            tc_hash_combine(seed, attr);
        }
        return seed;
    }
};

} /* namespace tc */
