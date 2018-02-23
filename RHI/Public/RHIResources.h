#pragma once
#include "RHIDefinitions.h"
#include "RHIAPI.h"
#include <Pointer.h>
#include <UsingStl.h>

namespace tc
{

class RHI_API FRHIResource
{
public:
    explicit FRHIResource(bool noDeferDeletion = false)
            : RefCount(1), bDoNotDeferDeletion(noDeferDeletion)
    {
    }

    virtual ~FRHIResource() = default;

    int AddRef() const
    {
        return ++RefCount;
    }

    int Release() const
    {
        const auto c = --RefCount;
        if (c == 0)
        {
            if (bDoNotDeferDeletion)
                delete this;
            else
                PendingDeletionQueue.push(this);
            //TODO: implement deferred deletion for platforms that need it
        }
        return c;
    }

    int GetRefCount() const
    {
        return RefCount;
    }

    static void DeletePending();

private:
    static queue<const FRHIResource*> PendingDeletionQueue;

    mutable int RefCount;
    bool bDoNotDeferDeletion;
};

struct FSubresourceData
{
    //Can be nullptr
    void* BufferDataPtr;
};

//Pipeline states

class FRHIRasterizerState : public FRHIResource
{
};

//Buffers

class FRHIBuffer : public FRHIResource
{
};

class FRHIVertexBuffer : public FRHIBuffer
{
};

class FRHIIndexBuffer : public FRHIBuffer
{
};

//Textures

class FRHITexture2D : public FRHIResource
{
};

class FRHITexture2DArray : public FRHIResource
{
};

class FRHITexture3D : public FRHIResource
{
};

class FRHIViewport : public FRHIResource
{
};

class FRHIInputLayout : public FRHIResource
{
};

class FRHIVertexShader : public FRHIResource
{
};

class FRHIPixelShader : public FRHIResource
{
};

typedef TRefPtr<FRHIVertexBuffer> FRHIVertexBufferRef;
typedef TRefPtr<FRHIIndexBuffer> FRHIIndexBufferRef;

typedef TRefPtr<FRHITexture2D> FRHITexture2DRef;
typedef TRefPtr<FRHITexture2DArray> FRHITexture2DArrayRef;
typedef TRefPtr<FRHITexture3D> FRHITexture3DRef;

typedef TRefPtr<FRHIViewport> FRHIViewportRef;

typedef TRefPtr<FRHIRasterizerState> FRHIRasterizerStateRef;

typedef TRefPtr<FRHIInputLayout> FRHIInputLayoutRef;

typedef TRefPtr<FRHIVertexShader> FRHIVertexShaderRef;
typedef TRefPtr<FRHIPixelShader> FRHIPixelShaderRef;

} /* namespace tc */
